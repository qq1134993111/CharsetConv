#pragma once

#include"config.h"

#ifdef CONV_WITH_ICONV

#include <iconv.h>
#include "conv/encoding_errors.h"

namespace conv
{
	namespace impl_iconv
	{
#if defined(__ICONV_F_HIDE_INVALID) && defined(__FreeBSD__)
		extern "C"
		{
			typedef size_t(*const_iconv_ptr_type)(iconv_t d, char const** in, size_t* insize, char** out, size_t* outsize, uint32_t, size_t*);
			typedef size_t(*nonconst_iconv_ptr_type)(iconv_t d, char** in, size_t* insize, char** out, size_t* outsize, uint32_t, size_t*);
		}
		inline size_t do_iconv(const_iconv_ptr_type ptr, iconv_t d, char** in, size_t* insize, char** out, size_t* outsize)
		{
			char const** rin = const_cast<char const**>(in);
			return ptr(d, rin, insize, out, outsize, __ICONV_F_HIDE_INVALID, 0);
		}
		inline size_t do_iconv(nonconst_iconv_ptr_type ptr, iconv_t d, char** in, size_t* insize, char** out, size_t* outsize)
		{
			return ptr(d, in, insize, out, outsize, __ICONV_F_HIDE_INVALID, 0);
		}
		inline size_t call_iconv(iconv_t d, char** in, size_t* insize, char** out, size_t* outsize)
		{
			char const** rin = const_cast<char const**>(in);
			return do_iconv(__iconv, d, in, insize, out, outsize);
		}
#else
		extern "C"
		{
			typedef size_t(*gnu_iconv_ptr_type)(iconv_t d, char const** in, size_t* insize, char** out, size_t* outsize);
			typedef size_t(*posix_iconv_ptr_type)(iconv_t d, char** in, size_t* insize, char** out, size_t* outsize);
		}
		inline size_t do_iconv(gnu_iconv_ptr_type ptr, iconv_t d, char** in, size_t* insize, char** out, size_t* outsize)
		{
			char const** rin = const_cast<char const**>(in);
			return ptr(d, rin, insize, out, outsize);
		}
		inline size_t do_iconv(posix_iconv_ptr_type ptr, iconv_t d, char** in, size_t* insize, char** out, size_t* outsize)
		{
			return ptr(d, in, insize, out, outsize);
		}
		inline size_t call_iconv(iconv_t d, char** in, size_t* insize, char** out, size_t* outsize)
		{
			return do_iconv(iconv, d, in, insize, out, outsize);
		}
#endif

		class iconverter_base
		{
		public:

			iconverter_base() :
				cvt_((iconv_t)(-1))
			{
			}

			~iconverter_base()
			{
				close();
			}

			size_t conv(char const** inbufc, size_t* inchar_left,
				char** outbuf, size_t* outchar_left)
			{
				char** inbuf = const_cast<char**>(inbufc);
				return call_iconv(cvt_, inbuf, inchar_left, outbuf, outchar_left);
			}

			bool do_open(char const* to, char const* from, method_type how)
			{
				close();
				cvt_ = iconv_open(to, from);
				how_ = how;
				return cvt_ != (iconv_t)(-1);
			}

			template<typename OutChar, typename InChar>
			std::basic_string<OutChar> real_convert(InChar const* ubegin, InChar const* uend)
			{
				std::basic_string<OutChar> sresult;

				sresult.reserve(uend - ubegin);

				OutChar result[64];

				char* out_start = reinterpret_cast<char*>(&result[0]);
				char const* begin = reinterpret_cast<char const*>(ubegin);
				char const* end = reinterpret_cast<char const*>(uend);

				enum { normal, unshifting, done } state = normal;

				while (state != done)
				{

					size_t in_left = end - begin;
					size_t out_left = sizeof(result);

					char* out_ptr = out_start;
					size_t res = 0;
					if (in_left == 0)
						state = unshifting;

					if (state == normal)
						res = conv(&begin, &in_left, &out_ptr, &out_left);
					else
						res = conv(0, 0, &out_ptr, &out_left);

					int err = errno;

					size_t output_count = (out_ptr - out_start) / sizeof(OutChar);

					if (res != 0 && res != (size_t)(-1))
					{
						if (how_ == stop)
						{
							throw conversion_error();
						}
					}

					sresult.append(&result[0], output_count);

					if (res == (size_t)(-1))
					{
						if (err == EILSEQ || err == EINVAL)
						{
							if (how_ == stop)
							{
								throw conversion_error();
							}

							if (begin != end)
							{
								begin += sizeof(InChar);
								if (begin >= end)
									break;
							}
							else
							{
								break;
							}
						}
						else if (err == E2BIG)
						{
							continue;
						}
						else
						{
							// We should never get there
							// but if we do
							if (how_ == stop)
								throw conversion_error();
							else
								break;
						}
					}
					if (state == unshifting)
						state = done;
				}
				return sresult;
			}


		private:

			void close()
			{
				if (cvt_ != (iconv_t)(-1))
				{
					iconv_close(cvt_);
					cvt_ = (iconv_t)(-1);
				}
			}

			iconv_t cvt_;

			method_type how_;

		};


	}
}


#endif
#pragma once

#include"config.h"

#ifdef CONV_WITH_WINDOWS

#include <cstring>
#include <string>

#include "converter.h"

#include "windows_conv.h"
#include "encoding_utf.h"
#include "util.h"

namespace conv
{
	class wconv_between : public converter_between
	{
	public:
		wconv_between() :
			how_(skip),
			to_code_page_(-1),
			from_code_page_(-1)
		{
		}
		bool open(char const* to_charset, char const* from_charset, method_type how)
		{
			how_ = how;
			to_code_page_ = encoding_to_windows_codepage(to_charset);
			from_code_page_ = encoding_to_windows_codepage(from_charset);
			if (to_code_page_ == -1 || from_code_page_ == -1)
				return false;
			return true;
		}
		virtual std::string convert(char const* begin, char const* end)
		{
			if (to_code_page_ == 65001 && from_code_page_ == 65001)
				return utf_to_utf<char>(begin, end, how_);

			std::string res;

			std::vector<wchar_t> tmp;   // buffer for mb2w
			std::wstring tmps;          // buffer for utf_to_utf
			wchar_t const* wbegin = 0;
			wchar_t const* wend = 0;

			if (from_code_page_ == 65001)
			{
				tmps = utf_to_utf<wchar_t>(begin, end, how_);
				if (tmps.empty())
					return res;
				wbegin = tmps.c_str();
				wend = wbegin + tmps.size();
			}
			else
			{
				impl_windows::multibyte_to_wide(from_code_page_, begin, end, how_ == skip, tmp);
				if (tmp.empty())
					return res;
				wbegin = &tmp[0];
				wend = wbegin + tmp.size();
			}

			if (to_code_page_ == 65001)
			{
				return utf_to_utf<char>(wbegin, wend, how_);
			}

			std::vector<char> ctmp;
			impl_windows::wide_to_multibyte(to_code_page_, wbegin, wend, how_ == skip, ctmp);
			if (ctmp.empty())
				return res;
			res.assign(&ctmp.front(), ctmp.size());
			return res;
		}
	private:
		method_type how_;
		int to_code_page_;
		int from_code_page_;
	};

	template<typename CharType, int size = sizeof(CharType) >
	class wconv_to_utf;

	template<typename CharType, int size = sizeof(CharType) >
	class wconv_from_utf;

	template<>
	class wconv_to_utf<char, 1> : public  converter_to_utf<char>, public wconv_between
	{
	public:
		virtual bool open(char const* cs, method_type how)
		{
			return wconv_between::open("UTF-8", cs, how);
		}
		virtual std::string convert(char const* begin, char const* end)
		{
			return wconv_between::convert(begin, end);
		}
	};

	template<>
	class wconv_from_utf<char, 1> : public  converter_from_utf<char>, public wconv_between
	{
	public:
		virtual bool open(char const* cs, method_type how)
		{
			return wconv_between::open(cs, "UTF-8", how);
		}
		virtual std::string convert(char const* begin, char const* end)
		{
			return wconv_between::convert(begin, end);
		}
	};

	template<typename CharType>
	class wconv_to_utf<CharType, 2> : public converter_to_utf<CharType>
	{
	public:
		typedef CharType char_type;

		typedef std::basic_string<char_type> string_type;

		wconv_to_utf() :
			how_(skip),
			code_page_(-1)
		{
		}

		virtual bool open(char const* charset, method_type how)
		{
			how_ = how;
			code_page_ = encoding_to_windows_codepage(charset);
			return code_page_ != -1;
		}

		virtual string_type convert(char const* begin, char const* end)
		{
			if (code_page_ == 65001)
			{
				return utf_to_utf<char_type>(begin, end, how_);
			}
			std::vector<wchar_t> tmp;
			impl_windows::multibyte_to_wide(code_page_, begin, end, how_ == skip, tmp);
			string_type res;
			if (!tmp.empty())
				res.assign(reinterpret_cast<char_type*>(&tmp.front()), tmp.size());
			return res;
		}

	private:
		method_type how_;
		int code_page_;
	};

	template<typename CharType>
	class wconv_from_utf<CharType, 2> : public converter_from_utf<CharType>
	{
	public:
		typedef CharType char_type;

		typedef std::basic_string<char_type> string_type;

		wconv_from_utf() :
			how_(skip),
			code_page_(-1)
		{
		}

		virtual bool open(char const* charset, method_type how)
		{
			how_ = how;
			code_page_ = encoding_to_windows_codepage(charset);
			return code_page_ != -1;
		}

		virtual std::string convert(CharType const* begin, CharType const* end)
		{
			if (code_page_ == 65001)
			{
				return utf_to_utf<char>(begin, end, how_);
			}
			wchar_t const* wbegin = 0;
			wchar_t const* wend = 0;
			std::vector<wchar_t> buffer; // if needed
			if (begin == end)
				return std::string();
			if (impl_windows::validate_utf16(begin, end - begin))
			{
				wbegin = reinterpret_cast<wchar_t const*>(begin);
				wend = reinterpret_cast<wchar_t const*>(end);
			}
			else
			{
				if (how_ == stop)
				{
					throw conversion_error();
				}
				else
				{
					impl_windows::clean_invalid_utf16(begin, end - begin, buffer);
					if (!buffer.empty())
					{
						wbegin = &buffer[0];
						wend = wbegin + buffer.size();
					}
				}
			}
			std::string res;
			if (wbegin == wend)
				return res;
			std::vector<char> ctmp;
			impl_windows::wide_to_multibyte(code_page_, wbegin, wend, how_ == skip, ctmp);
			if (ctmp.empty())
				return res;
			res.assign(&ctmp.front(), ctmp.size());
			return res;
		}

	private:
		method_type how_;
		int code_page_;
	};



	template<typename CharType>
	class wconv_to_utf<CharType, 4> : public converter_to_utf<CharType>
	{
	public:
		typedef CharType char_type;

		typedef std::basic_string<char_type> string_type;

		wconv_to_utf() :
			how_(skip),
			code_page_(-1)
		{
		}

		virtual bool open(char const* charset, method_type how)
		{
			how_ = how;
			code_page_ = encoding_to_windows_codepage(charset);
			return code_page_ != -1;
		}

		virtual string_type convert(char const* begin, char const* end)
		{
			if (code_page_ == 65001)
			{
				return utf_to_utf<char_type>(begin, end, how_);
			}
			std::vector<wchar_t> buf;
			impl_windows::multibyte_to_wide(code_page_, begin, end, how_ == skip, buf);

			if (buf.empty())
				return string_type();

			return utf_to_utf<CharType>(&buf[0], &buf[0] + buf.size(), how_);
		}
	private:
		method_type how_;
		int code_page_;
	};

	template<typename CharType>
	class wconv_from_utf<CharType, 4> : public converter_from_utf<CharType>
	{
	public:
		typedef CharType char_type;

		typedef std::basic_string<char_type> string_type;

		wconv_from_utf() :
			how_(skip),
			code_page_(-1)
		{
		}

		virtual bool open(char const* charset, method_type how)
		{
			how_ = how;
			code_page_ = encoding_to_windows_codepage(charset);
			return code_page_ != -1;
		}

		virtual std::string convert(CharType const* begin, CharType const* end)
		{
			if (code_page_ == 65001)
			{
				return utf_to_utf<char>(begin, end, how_);
			}
			std::wstring tmp = utf_to_utf<wchar_t>(begin, end, how_);

			std::vector<char> ctmp;
			impl_windows::wide_to_multibyte(code_page_, tmp.c_str(), tmp.c_str() + tmp.size(), how_ == skip, ctmp);
			std::string res;
			if (ctmp.empty())
				return res;
			res.assign(&ctmp.front(), ctmp.size());
			return res;

		}

	private:
		method_type how_;
		int code_page_;
	};

}



#endif
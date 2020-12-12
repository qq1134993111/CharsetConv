#pragma once

#include"config.h"

#ifdef CONV_WITH_ICONV

#include"converter.h"
#include"iconv_conv.h"

namespace conv
{
	template<typename CharType>
	class iconv_from_utf : public converter_from_utf<CharType>
	{
	public:

		typedef CharType char_type;

		virtual bool open(char const* charset, method_type how)
		{
			return self_.do_open(charset, utf_name<CharType>(), how);
		}

		virtual std::string convert(char_type const* ubegin, char_type const* uend)
		{
			return self_.template real_convert<char, char_type>(ubegin, uend);
		}
		virtual ~iconv_from_utf() {}
	private:
		impl_iconv::iconverter_base self_;
	};

	class iconv_between : public converter_between
	{
	public:
		virtual bool open(char const* to_charset, char const* from_charset, method_type how)
		{
			return self_.do_open(to_charset, from_charset, how);
		}
		virtual std::string convert(char const* begin, char const* end)
		{
			return self_.real_convert<char, char>(begin, end);
		}
		virtual ~iconv_between() {}
	private:
		impl_iconv::iconverter_base self_;

	};

	template<typename CharType>
	class iconv_to_utf : public converter_to_utf<CharType>
	{
	public:

		typedef CharType char_type;
		typedef std::basic_string<char_type> string_type;

		virtual bool open(char const* charset, method_type how)
		{
			return self_.do_open(utf_name<CharType>(), charset, how);
		}

		virtual string_type convert(char const* begin, char const* end)
		{
			return self_.template real_convert<char_type, char>(begin, end);
		}
		virtual ~iconv_to_utf() {}
	private:
		impl_iconv::iconverter_base self_;
	};
}

#endif
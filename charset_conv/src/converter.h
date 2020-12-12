#pragma once
#include"util.h"

namespace conv
{


	class converter_between
	{
	public:
		typedef char char_type;

		typedef std::string string_type;

		virtual bool open(char const* to_charset, char const* from_charset, method_type how) = 0;

		virtual std::string convert(char const* begin, char const* end) = 0;

		virtual ~converter_between()
		{
		}
	};

	template<typename CharType>
	class converter_from_utf
	{
	public:
		typedef CharType char_type;

		typedef std::basic_string<char_type> string_type;

		virtual bool open(char const* charset, method_type how) = 0;

		virtual std::string convert(CharType const* begin, CharType const* end) = 0;

		virtual ~converter_from_utf()
		{
		}
	};

	template<typename CharType>
	class converter_to_utf
	{
	public:
		typedef CharType char_type;

		typedef std::basic_string<char_type> string_type;

		virtual bool open(char const* charset, method_type how) = 0;

		virtual string_type convert(char const* begin, char const* end) = 0;

		virtual ~converter_to_utf()
		{
		}
	};

}
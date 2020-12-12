#pragma once
#include <string>

#include"config.h" 

namespace conv
{
	template<typename CharType>
	char const* utf_name()
	{
		union
		{
			char first;
			uint16_t u16;
			uint32_t u32;
		} v;

		if (sizeof(CharType) == 1)
		{
			return "UTF-8";
		}
		else if (sizeof(CharType) == 2)
		{
			v.u16 = 1;
			if (v.first == 1)
			{
				return "UTF-16LE";
			}
			else
			{
				return "UTF-16BE";
			}
		}
		else if (sizeof(CharType) == 4)
		{
			v.u32 = 1;
			if (v.first == 1)
			{
				return "UTF-32LE";
			}
			else
			{
				return "UTF-32BE";
			}

		}
		else
		{
			return "Unknown Character Encoding";
		}
	}

	inline std::string normalize_encoding(char const* ccharset)
	{
		std::string charset;
		charset.reserve(std::strlen(ccharset));
		while (*ccharset != 0)
		{
			char c = *ccharset++;
			if ('0' <= c && c <= '9')
				charset += c;
			else if ('a' <= c && c <= 'z')
				charset += c;
			else if ('A' <= c && c <= 'Z')
				charset += char(c - 'A' + 'a');
		}
		return charset;
	}

	inline int compare_encodings(char const* l, char const* r)
	{
		return normalize_encoding(l).compare(normalize_encoding(r));
	}


#ifdef  CONV_WITH_WINDOWS
	int encoding_to_windows_codepage(char const* ccharset);
#endif

}


#include "util.h"

#ifdef CONV_WITH_WINDOWS
#include<windows.h> 

namespace conv
{
	struct windows_encoding
	{
		char const* name;
		unsigned codepage;
		unsigned was_tested;
	};

	windows_encoding all_windows_encodings[] =
	{
		{ "big5",       950, 0 },
		{ "cp1250",     1250, 0 },
		{ "cp1251",     1251, 0 },
		{ "cp1252",     1252, 0 },
		{ "cp1253",     1253, 0 },
		{ "cp1254",     1254, 0 },
		{ "cp1255",     1255, 0 },
		{ "cp1256",     1256, 0 },
		{ "cp1257",     1257, 0 },
		{ "cp874",      874, 0 },
		{ "cp932",      932, 0 },
		{ "cp936",      936, 0 },
		{ "eucjp",      20932, 0 },
		{ "euckr",      51949, 0 },
		{ "gb18030",    54936, 0 },
		{ "gb2312",     20936, 0 },
		{ "gbk",        936, 0 },
		{ "iso2022jp",  50220, 0 },
		{ "iso2022kr",  50225, 0 },
		{ "iso88591",   28591, 0 },
		{ "iso885913",  28603, 0 },
		{ "iso885915",  28605, 0 },
		{ "iso88592",   28592, 0 },
		{ "iso88593",   28593, 0 },
		{ "iso88594",   28594, 0 },
		{ "iso88595",   28595, 0 },
		{ "iso88596",   28596, 0 },
		{ "iso88597",   28597, 0 },
		{ "iso88598",   28598, 0 },
		{ "iso88599",   28599, 0 },
		{ "koi8r",      20866, 0 },
		{ "koi8u",      21866, 0 },
		{ "ms936",      936, 0 },
		{ "shiftjis",   932, 0 },
		{ "sjis",       932, 0 },
		{ "usascii",    20127, 0 },
		{ "utf8",       65001, 0 },
		{ "windows1250",        1250, 0 },
		{ "windows1251",        1251, 0 },
		{ "windows1252",        1252, 0 },
		{ "windows1253",        1253, 0 },
		{ "windows1254",        1254, 0 },
		{ "windows1255",        1255, 0 },
		{ "windows1256",        1256, 0 },
		{ "windows1257",        1257, 0 },
		{ "windows874",         874, 0 },
		{ "windows932",         932, 0 },
		{ "windows936",         936, 0 },
	};

	inline bool operator<(windows_encoding const& l, windows_encoding const& r)
	{
		return strcmp(l.name, r.name) < 0;
	}

	int encoding_to_windows_codepage(char const* ccharset)
	{
		std::string charset = normalize_encoding(ccharset);
		windows_encoding ref;
		ref.name = charset.c_str();
		size_t n = sizeof(all_windows_encodings) / sizeof(all_windows_encodings[0]);
		windows_encoding* begin = all_windows_encodings;
		windows_encoding* end = all_windows_encodings + n;
		windows_encoding* ptr = std::lower_bound(begin, end, ref);
		if (ptr != end && strcmp(ptr->name, charset.c_str()) == 0)
		{
			if (ptr->was_tested)
			{
				return ptr->codepage;
			}
			else if (IsValidCodePage(ptr->codepage))
			{
				// the thread safety is not an issue, maximum
				// it would be checked more then once
				ptr->was_tested = 1;
				return ptr->codepage;
			}
			else
			{
				return -1;
			}
		}
		return -1;

	}

}

#endif
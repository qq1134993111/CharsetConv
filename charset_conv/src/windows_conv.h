#pragma once

#include"config.h"

#ifdef CONV_WITH_WINDOWS

#ifndef NOMINMAX
# define NOMINMAX
#include "conv/encoding_errors.h"
#endif

#include <windows.h>

#include <algorithm>
#include <vector>
#include "utf.h"
#include "util.h"

namespace conv
{
	namespace impl_windows
	{

		inline size_t remove_substitutions(std::vector<char>& v)
		{
			if (std::find(v.begin(), v.end(), 0) == v.end())
			{
				return v.size();
			}
			std::vector<char> v2;
			v2.reserve(v.size());
			for (unsigned i = 0; i < v.size(); i++)
			{
				if (v[i] != 0)
					v2.push_back(v[i]);
			}
			v.swap(v2);
			return v.size();
		}

		inline void multibyte_to_wide_one_by_one(int codepage, char const* begin, char const* end, std::vector<wchar_t>& buf)
		{
			buf.reserve(end - begin);
			while (begin != end)
			{
				wchar_t wide_buf[4];
				int n = 0;
				int len = IsDBCSLeadByteEx(codepage, *begin) ? 2 : 1;
				if (len == 2 && begin + 1 == end)
					return;
				n = MultiByteToWideChar(codepage, MB_ERR_INVALID_CHARS, begin, len, wide_buf, 4);
				for (int i = 0; i < n; i++)
					buf.push_back(wide_buf[i]);
				begin += len;
			}
		}


		inline void multibyte_to_wide(int codepage, char const* begin, char const* end, bool do_skip, std::vector<wchar_t>& buf)
		{
			if (begin == end)
				return;
			int n = MultiByteToWideChar(codepage, MB_ERR_INVALID_CHARS, begin, end - begin, 0, 0);
			if (n == 0)
			{
				if (do_skip)
				{
					multibyte_to_wide_one_by_one(codepage, begin, end, buf);
					return;
				}
				throw conversion_error();
			}

			buf.resize(n, 0);
			if (MultiByteToWideChar(codepage, MB_ERR_INVALID_CHARS, begin, end - begin, &buf.front(), buf.size()) == 0)
				throw conversion_error();
		}

		inline void wide_to_multibyte_non_zero(int codepage, wchar_t const* begin, wchar_t const* end, bool do_skip, std::vector<char>& buf)
		{
			if (begin == end)
				return;
			BOOL substitute = FALSE;
			BOOL* substitute_ptr = codepage == 65001 || codepage == 65000 ? 0 : &substitute;
			char subst_char = 0;
			char* subst_char_ptr = codepage == 65001 || codepage == 65000 ? 0 : &subst_char;

			int n = WideCharToMultiByte(codepage, 0, begin, end - begin, 0, 0, subst_char_ptr, substitute_ptr);
			buf.resize(n);

			if (WideCharToMultiByte(codepage, 0, begin, end - begin, &buf[0], n, subst_char_ptr, substitute_ptr) == 0)
				throw conversion_error();
			if (substitute)
			{
				if (do_skip)
					remove_substitutions(buf);
				else
					throw conversion_error();
			}
		}

		inline void wide_to_multibyte(int codepage, wchar_t const* begin, wchar_t const* end, bool do_skip, std::vector<char>& buf)
		{
			if (begin == end)
				return;
			buf.reserve(end - begin);
			wchar_t const* e = std::find(begin, end, L'\0');
			wchar_t const* b = begin;
			for (;;)
			{
				std::vector<char> tmp;
				wide_to_multibyte_non_zero(codepage, b, e, do_skip, tmp);
				size_t osize = buf.size();
				buf.resize(osize + tmp.size());
				std::copy(tmp.begin(), tmp.end(), buf.begin() + osize);
				if (e != end) {
					buf.push_back('\0');
					b = e + 1;
					e = std::find(b, end, L'0');
				}
				else
					break;
			}
		}


		template<typename CharType>
		bool validate_utf16(CharType const* str, unsigned len)
		{
			CharType const* begin = str;
			CharType const* end = str + len;
			while (begin != end)
			{
				utf::code_point c = utf::utf_traits<CharType, 2>::template decode<CharType const*>(begin, end);
				if (c == utf::illegal || c == utf::incomplete)
					return false;
			}
			return true;
		}

		template<typename CharType, typename OutChar>
		void clean_invalid_utf16(CharType const* str, unsigned len, std::vector<OutChar>& out)
		{
			out.reserve(len);
			for (unsigned i = 0; i < len; i++)
			{
				uint16_t c = static_cast<uint16_t>(str[i]);

				if (0xD800 <= c && c <= 0xDBFF)
				{
					i++;
					if (i >= len)
						return;
					uint16_t c2 = static_cast<uint16_t>(str[i]);
					if (0xDC00 <= c2 && c2 <= 0xDFFF)
					{
						out.push_back(static_cast<OutChar>(c));
						out.push_back(static_cast<OutChar>(c2));
					}
				}
				else if (0xDC00 <= c && c <= 0xDFFF)
					continue;
				else
					out.push_back(static_cast<OutChar>(c));
			}
		}

	}

}

#endif


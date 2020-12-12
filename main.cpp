// Conv.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <locale>
#include <assert.h>
#include "conv/encoding.h"
#include "charset_det/CharsetDetector.h"


std::string from_u8string(const std::string& s)
{
	return s;
}
std::string from_u8string(std::string&& s)
{
	return std::move(s);
}

#if defined(__cpp_lib_char8_t)
std::string from_u8string(const std::u8string& s)
{
	return std::string(s.begin(), s.end());
}
#endif


void Detect(char* data, size_t len)
{
	using namespace detect;
	auto p_detector = detect::CreateCharsetDetector(kCompactEncDet);
	assert(p_detector != nullptr);
	auto info = p_detector->Detect(data, len);
	printf("name:%s(%d)\n", info.name.c_str(), info.confidence);
}

void TestConv()
{

	std::string utf_8 = from_u8string(u8"这是utf8字符串,是吗.呜呜呜");
	std::wstring w_str0 = L"这是utf8字符串,是吗.呜呜呜";

	try
	{

		std::cout << "utf_8:" << utf_8 << "\n";
		Detect(utf_8.data(), utf_8.size());

		std::wcout << L"w_str0:" << w_str0 << "\n";
		Detect((char*)w_str0.data(), w_str0.size() * sizeof(w_str0[0]));

		auto gbk = conv::between(utf_8, "GBK", "UTF-8", conv::stop);
		std::cout << "gbk:" << gbk << "\n";
		Detect(gbk.data(), gbk.size());

		std::wstring w_str = conv::to_utf<wchar_t>(gbk, "GBK", conv::stop);
		std::wcout << L"w_str:" << w_str << std::endl;
		Detect((char*)w_str.data(), w_str.size() * sizeof(w_str[0]));


		auto utf8_2 = conv::to_utf<char>(gbk, "GBK", conv::skip);
		std::cout << "utf_8 is same utf_8_2 ? " << (utf8_2 == utf_8) << "\n";

		auto utf8_3 = conv::from_utf(w_str, "UTF-8");
		std::cout << "utf_8 is same utf8_3 ? " << (utf8_3 == utf_8) << "\n";


		auto gbk2 = conv::from_utf(w_str, "GBK");
		std::cout << "gbk is same gbk2 ? " << (gbk2 == gbk) << "\n";


		auto w_str2 = conv::to_utf<wchar_t>(utf8_3, "UTF-8");
		std::cout << "w_str is same w_str2 ? " << (w_str2 == w_str) << "\n";

	}
	catch (std::exception& e)
	{
		std::cout << "conv exception:" << e.what() << "\n";
	}
}

/*
#include "charset_det/IcuCharsetDetector.h"
#include "charset_det/CompactEncDetector.h"
#include "charset_det/UcharDetector.h"

void TestDet()
{
	using namespace detect;

	std::string utf_8 = from_u8string(u8"猜一猜:这是什么编码？");
	std::wstring w_str = L"猜一猜:这是什么编码？";

	std::cout << utf_8 << "\n";
	std::wcout << w_str << L"\n";

	if (1)
	{
		CompactEncDetector compact_detector;
		auto info = compact_detector.Detect(utf_8.data(), utf_8.size());
		std::cout << "charset:" << info.name << "(" << info.lang << ") " << info.confidence << "\n";


		info = compact_detector.Detect((char*)w_str.c_str(), w_str.size() * sizeof(w_str[0]));
		std::cout << "charset:" << info.name << "(" << info.lang << ") " << info.confidence << "\n";
	}

	if (1)
	{
		UcharDetector uchar_detector;
		auto info = uchar_detector.Detect(utf_8.data(), utf_8.size());
		std::cout << "charset:" << info.name << "(" << info.lang << ") " << info.confidence << "\n";


		info = uchar_detector.Detect((char*)w_str.c_str(), w_str.size() * sizeof(w_str[0]));
		std::cout << "charset:" << info.name << "(" << info.lang << ") " << info.confidence << "\n";
	}

	if (1)
	{
		IcuCharsetDetector icu_detector;
		auto v_charset = icu_detector.DetectTextEncoding(utf_8.data(), utf_8.size());

		std::string str_gbk;
		for (auto charset : v_charset)
		{
			std::cout << "charset:" << charset.name << "(" << charset.lang << ") " << charset.confidence << "\n";
			std::cout << "as " << charset.name << " to GBK \n";
			try
			{
				str_gbk = conv::between(utf_8, "GBK", charset.name, conv::method_type::stop);

				std::cout << "as finished :" << str_gbk << "\n";
				Detect(str_gbk.data(), str_gbk.size());
			}
			catch (std::exception& e)
			{
				std::cout << charset.name << " exception:" << e.what() << "\n";
				continue;
			}
		}


		v_charset = icu_detector.DetectTextEncoding((char*)w_str.c_str(), w_str.size() * sizeof(wchar_t));
		for (auto charset : v_charset)
		{
			std::cout << "charset:" << charset.name << "(" << charset.lang << ") " << charset.confidence << "\n";
			std::cout << "as " << charset.name << " to GBK \n";
			try
			{
				str_gbk = conv::between(utf_8, "GBK", charset.name, conv::method_type::skip);

				std::cout << "as finished :" << str_gbk << "\n";
				Detect(str_gbk.data(), str_gbk.size());
			}
			catch (std::exception& e)
			{
				std::cout << charset.name << " exception:" << e.what() << "\n";
				continue;
			}
		}
	}
}
*/

int main()
{
	std::locale lc;
	std::cout << lc.name().c_str() << "\n";

	std::cout << "-----------------------------------------------------------------------------\n";
	TestConv();
	std::cout << "-----------------------------------------------------------------------------\n";
	//TestDet();
	std::cout << "Hello World!\n";
}

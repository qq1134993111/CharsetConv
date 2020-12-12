#pragma once
#include"DetectConfig.h"

#ifdef DETECT_WITH_UCHARSET

#include "CharsetDetector.h"

namespace detect
{
	class UcharDet
	{
	public:
		UcharDet();
		~UcharDet();
		void Reset();
		int HandleData(const char* data, size_t size);
		void HandleEnd();
		std::string GetCharsetName();
		std::string Detect(const std::string& data);
		std::string Detect(const char* data, size_t size);
	private:

		void* handle_ = nullptr;
	};

	//uchardet ±àÂë¼ì²â
	class UcharDetector :public ICharsetDetector
	{
	public:
		virtual DetectInfo Detect(const char* text, size_t size);
	};
}

#endif


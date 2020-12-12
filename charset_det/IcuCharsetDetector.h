#pragma once
#include"DetectConfig.h"
#ifdef DETECT_WITH_ICU

#include "CharsetDetector.h"

namespace detect
{

	//icu ±àÂë¼ì²â
	class  IcuCharsetDetector :public ICharsetDetector
	{
	public:
		virtual DetectInfo Detect(const char* text, size_t size);

		std::vector<DetectInfo> DetectTextEncoding(const char* text, size_t size);
	};

}

#endif


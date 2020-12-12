#pragma once
#include"DetectConfig.h"

#ifdef DETECT_WITH_COMPACT_ENC

#include <tuple>
#include "CharsetDetector.h"

namespace detect
{

	//compact_enc_det ±àÂë¼ì²â
	class CompactEncDetector :public ICharsetDetector
	{
	public:
		virtual DetectInfo Detect(const char* text, size_t size);

		DetectInfo DetectTextEncoding(const char* text, size_t size);

		std::tuple <bool, std::string, std::string> DetectEncoding(const char* text, size_t size);
	};

}

#endif


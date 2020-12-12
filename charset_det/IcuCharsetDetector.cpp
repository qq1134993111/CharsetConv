#include "IcuCharsetDetector.h"

#ifdef DETECT_WITH_ICU

#include <iostream>
#include "unicode/ucsdet.h"

namespace detect
{
	std::vector<DetectInfo> IcuCharsetDetector::DetectTextEncoding(const char* text, size_t size)
	{
		std::vector<DetectInfo> v_charset_info;

		UCharsetDetector* csd = nullptr;
		const UCharsetMatch** csm = nullptr;
		int32_t match = 0, matchCount = 0;

		UErrorCode status = U_ZERO_ERROR;

		csd = ucsdet_open(&status);
		if (status != U_ZERO_ERROR)
		{
			std::cerr << "ucsdet_open error:" << u_errorName(status) << "\n";
			return v_charset_info;
		}

		ucsdet_setText(csd, text, size, &status);
		if (status != U_ZERO_ERROR)
		{
			std::cerr << "ucsdet_setText error:" << u_errorName(status) << "\n";
			return v_charset_info;
		}

		csm = ucsdet_detectAll(csd, &matchCount, &status);
		if (status != U_ZERO_ERROR)
		{
			std::cerr << "ucsdet_detectAll error:" << u_errorName(status) << "\n";
			return v_charset_info;
		}

		//打印出探测的可能的编码
		for (match = 0; match < matchCount; match += 1)
		{
			const char* name = ucsdet_getName(csm[match], &status);
			const char* lang = ucsdet_getLanguage(csm[match], &status);
			int32_t confidence = ucsdet_getConfidence(csm[match], &status);

			if (name == nullptr || strlen(name) == 0)
				name = "";

			if (lang == nullptr || strlen(lang) == 0)
				lang = "";

			//printf("match charset: %s (%s) %d\n", name, lang, confidence);

			v_charset_info.push_back({ name,lang,confidence });
		}

		ucsdet_close(csd);
		return v_charset_info;
	}

	DetectInfo IcuCharsetDetector::Detect(const char* text, size_t size)
	{
		auto v_charset = DetectTextEncoding(text, size);
		if (!v_charset.empty())
			return v_charset[0];

		return DetectInfo();
	}

}


#endif


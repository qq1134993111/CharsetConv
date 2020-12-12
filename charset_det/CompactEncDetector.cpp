

#include "CompactEncDetector.h"
#ifdef DETECT_WITH_COMPACT_ENC

#include "compact_enc_det/compact_enc_det.h"

namespace detect
{
	DetectInfo CompactEncDetector::DetectTextEncoding(const char* text, size_t size)
	{

		auto [is_reliable, mime, name] = DetectEncoding(text, size);

		DetectInfo info;

		info.name = mime;
		if (!info.name.empty())
		{
			info.confidence = is_reliable ? 100 : 30;
		}

		return info;
	}

	std::tuple <bool, std::string, std::string> CompactEncDetector::DetectEncoding(const char* text, size_t size)
	{
		bool is_reliable = false;
		int bytes_consumed = 0;

		Encoding encoding = CompactEncDet::DetectEncoding(text, size,
			nullptr, nullptr, nullptr,
			UNKNOWN_ENCODING, UNKNOWN_LANGUAGE/*CHINESE*/,
			CompactEncDet::WEB_CORPUS/*CompactEncDet::QUERY_CORPUS*/,
			false, &bytes_consumed, &is_reliable);

		if (IsValidEncoding(encoding))
		{
			const char* mime_name = MimeEncodingName(encoding);
			const char* name = EncodingName(encoding);

			return std::make_tuple(is_reliable, mime_name ? mime_name : "", name ? name : "");
		}

		return std::make_tuple(is_reliable, "", "");
	}

	DetectInfo CompactEncDetector::Detect(const char* text, size_t size)
	{
		return DetectTextEncoding(text, size);
	}

}

#endif
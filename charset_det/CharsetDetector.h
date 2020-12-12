#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace detect
{
	struct DetectInfo
	{
		std::string name;
		std::string lang;
		int32_t  confidence = 0;
	};

	class ICharsetDetector
	{
	public:
		virtual DetectInfo Detect(const char* text, size_t size) = 0;
		virtual ~ICharsetDetector() {}
	};

	enum  DetectorType
	{
		kIcu,
		kCompactEncDet,
		kUcharDet,
		kDefult = kIcu
	};

	std::shared_ptr<ICharsetDetector> CreateCharsetDetector(DetectorType type = DetectorType::kDefult);
}

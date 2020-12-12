#include "CharsetDetector.h"
#include "iostream"

#include "IcuCharsetDetector.h"
#include "CompactEncDetector.h"
#include "UcharDetector.h"

namespace detect
{
	std::shared_ptr<ICharsetDetector> CreateCharsetDetector(DetectorType type /*= DetectorType::kDefult*/)
	{
		switch (type)
		{
		case  kIcu:
#ifdef DETECT_WITH_ICU
			return std::make_shared<IcuCharsetDetector>();
#else
			std::cerr << "Not Support DETECT_WITH_ICU\n";
#endif
			break;
		case  kCompactEncDet:
#ifdef DETECT_WITH_COMPACT_ENC
			return std::make_shared<CompactEncDetector>();
#else
			std::cerr << "Not Support DETECT_WITH_COMPACT_ENC\n";
#endif
			break;
		case  kUcharDet:
#ifdef DETECT_WITH_UCHARSET
			return std::make_shared<UcharDetector>();
#else
			std::cerr << "Not Support DETECT_WITH_UCHARSET\n";
#endif
			break;
		default:
			std::cerr << "Not Support type " << type << "\n";
			return nullptr;
			break;
		}

		return nullptr;
	}

}

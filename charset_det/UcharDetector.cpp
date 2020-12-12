#include "UcharDetector.h"

#ifdef DETECT_WITH_UCHARSET

#include "uchardet/uchardet.h"

namespace detect
{
	UcharDet::UcharDet()
	{
		handle_ = uchardet_new();
	}

	void UcharDet::Reset()
	{
		uchardet_reset((uchardet_t)handle_);
	}

	UcharDet::~UcharDet()
	{
		uchardet_delete((uchardet_t)handle_);
	}

	int UcharDet::HandleData(const char* data, size_t size)
	{
		return uchardet_handle_data((uchardet_t)handle_, data, size);
	}

	void UcharDet::HandleEnd()
	{
		uchardet_data_end((uchardet_t)handle_);
	}

	std::string UcharDet::GetCharsetName()
	{
		const char* name = uchardet_get_charset((uchardet_t)handle_);
		return name != nullptr ? name : "";
	}

	std::string UcharDet::Detect(const std::string& data)
	{
		return Detect(data.data(), data.size());
	}

	std::string UcharDet::Detect(const char* data, size_t size)
	{
		if (HandleData(data, size) != 0)
			return "";
		HandleEnd();
		return GetCharsetName();
	}

	DetectInfo UcharDetector::Detect(const char* text, size_t size)
	{
		UcharDet det;
		DetectInfo info;
		info.name = det.Detect(text, size);
		info.confidence = info.name.empty() ? 0 : 20;
		return info;
	}

}

#endif


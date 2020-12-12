#pragma once
#include <string>

namespace conv
{
	class locale_data
	{
	public:
		locale_data() :
			language("C"),
			encoding("us-ascii"),
			utf8(false)
		{
		}

		std::string language;
		std::string country;
		std::string variant;
		std::string encoding;
		bool utf8;

		void parse(std::string const& locale_name);

	private:

		void parse_from_lang(std::string const& locale_name);
		void parse_from_country(std::string const& locale_name);
		void parse_from_encoding(std::string const& locale_name);
		void parse_from_variant(std::string const& locale_name);
	};

}

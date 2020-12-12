#pragma once
#include <stdexcept>
namespace conv
{
	///
	/// \addtogroup codepage 
	///
	/// @{

	///
	/// \brief The excepton that is thrown in case of conversion error
	///
	class  conversion_error : public std::runtime_error
	{
	public:
		conversion_error() : std::runtime_error("Conversion failed") {}
	};

	///
	/// \brief This exception is thrown in case of use of unsupported
	/// or invalid character set
	///
	class  invalid_charset_error : public std::runtime_error
	{
	public:

		/// Create an error for charset \a charset
		invalid_charset_error(std::string charset) :
			std::runtime_error("Invalid or unsupported charset:" + charset)
		{
		}
	};


	///
	/// enum that defines conversion policy
	///
	typedef enum
	{
		skip = 0,    ///< Skip illegal/unconvertable characters
		stop = 1,    ///< Stop conversion and throw conversion_error
		default_method = skip  ///< Default method - skip
	} method_type;


	/// @}

} // conv
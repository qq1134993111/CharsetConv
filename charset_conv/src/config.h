#pragma once

//#define  CONV_WITH_ICU
//#define  CONV_WITH_ICONV
//#define  CONV_WITH_WINDOWS

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#ifndef  CONV_WITH_WINDOWS
#define  CONV_WITH_WINDOWS
#endif
#endif


#if defined(__linux__) || defined(linux) || defined(__linux) || defined(__gnu_linux__)|| defined(__GNU__) || defined(__GLIBC__)||defined(__unix__) || defined(__unix)
#ifndef  CONV_WITH_ICONV
#defined CONV_WITH_ICONV
#endif
#endif




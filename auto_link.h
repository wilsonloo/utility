/*
	this header file used to auto load library as .DLL in windows or .so in unix-like platform
	this header file without include safty wrapper, include correctly

	macro RTQ_LIBRARY_NAME needed specifying the library , and will be undef at the tail of file

*/

// check whethre RTQ_LIBRARY_NAME set before include this file
#ifndef RTQ_LIBRARY_NAME
#error RTQ_LIBRARY_NAME macro not set
#endif // !RTQ_LIBRARY_NAME

#if defined(_MT) && defined(_DLL)
#  define EVL_LIB_RTL_OPT "dynamic-rtl"
#else
#	if defined(_MT)
#		define EVL_LIB_RTL_OPT "static-rtl"
#	else
#		define EVL_LIB_RTL_OPT "static-rtl"
#	endif
#endif

#if _MSC_VER == 1700
    #define PLARFORM_TOOLSET "v110"
#elif _MSC_VER == 1800
	#define PLARFORM_TOOLSET "v120"
#elif _MSC_VER == 1900
	#define PLARFORM_TOOLSET "v140"
#elif _PF_LINUX_
    #define PLARFORM_TOOLSET "v120"
#else
    #pragma message("unsuported playerform toolset, asume is Linux")
#endif

// set EVL_LIB_DEBUG_OPT
#ifdef _DEBUG
#  define EVL_LIB_DEBUG_OPT "debug"
#else
#  define EVL_LIB_DEBUG_OPT "release"
#endif

#ifdef _PF_WINDOWS_

#ifdef _UNICODE
#define RTQ_LIBRARY_NAME_FULLNAME RTQ_LIBRARY_NAME "_" EVL_LIB_RTL_OPT "_" PLARFORM_TOOLSET "_" EVL_LIB_DEBUG_OPT "_ud"
#else // _UNICODE
#define RTQ_LIBRARY_NAME_FULLNAME RTQ_LIBRARY_NAME "_" EVL_LIB_RTL_OPT "_" PLARFORM_TOOLSET "_" EVL_LIB_DEBUG_OPT
#endif // _UNICODE

#pragma comment(lib, RTQ_LIBRARY_NAME_FULLNAME".lib")

#elif _PF_LINUX_

#define RTQ_LIBRARY_NAME_FULLNAME RTQ_LIBRARY_NAME "_" EVL_LIB_RTL_OPT "_" PLARFORM_TOOLSET "_" EVL_LIB_DEBUG_OPT "_ud"

#else
#error not implemented yet for this unknown platform.
#endif

#undef RTQ_LIBRARY_NAME
#undef EVL_LIB_RTL_OPT
#undef EVL_LIB_DEBUG_OPT
#undef RTQ_LIBRARY_NAME_FULLNAME

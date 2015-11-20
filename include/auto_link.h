/*
	this header file used to auto load library as .DLL in windows or .so in unix-like platform
	this header file without include safty wrapper, include correctly

	macro RTQ_LIBRARY_NAME needed specifying the library , and will be undef at the tail of file

*/

// check whethre RTQ_LIBRARY_NAME set before include this file
#ifndef RTQ_LIBRARY_NAME
#error RTQ_LIBRARY_NAME macro not set
#endif // !RTQ_LIBRARY_NAME

#ifdef _PF_WINDOWS_

#pragma comment(lib, RTQ_LIBRARY_NAME".lib")

#else
#error not implemented yet.
#endif

#undef RTQ_LIBRARY_NAME

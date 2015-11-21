#ifndef EVL_UTILITY_MPOOL_SETUP_H_
#define EVL_UTILITY_MPOOL_SETUP_H_


// ÷ª π”√æ≤Ã¨ø‚
#if defined(_DLL)
#define _DLL_DEFINED
#undef _DLL
#endif

#define RTQ_LIBRARY_NAME "evl_utility"
#include "auto_link.h"

// ª÷∏¥
#if defined(_DLL_DEFINED)
#undef _DLL_DEFINED
#define _DLL
#endif

#endif // EVL_UTILITY_MPOOL_SETUP_H_
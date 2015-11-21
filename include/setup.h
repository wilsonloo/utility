#ifndef EVL_UTILITY_MPOOL_SETUP_H_
#define EVL_UTILITY_MPOOL_SETUP_H_

#if EVL_UTILITY_MPOOL_RELEASE

#define RTQ_LIBRARY_NAME "evl_utility_release"
#include "auto_link.h"

#else

#define RTQ_LIBRARY_NAME "evl_utility_debug"
#include "auto_link.h"

#endif // DEBUG_DEVELOP_EVL_NET

#endif // EVL_UTILITY_MPOOL_SETUP_H_
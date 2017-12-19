#ifndef EVL_UTILITY_FLAGS_UTIL
#define EVL_UTILITY_FLAGS_UTIL

#define INSERT_FLAG(source, flag) \
	((source) = (unsigned int)(source) | (unsigned int)(flag))

#define REMOVE_FLAG(source, flag) \
	((source) = (unsigned int)(source) & (~(unsigned int)(flag)))

#define HAS_FLAG(source, flag) \
	(((source) & (unsigned int)(flag)) != 0)

#endif	//EVL_UTILITY_FLAGS_UTIL 

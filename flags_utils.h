#ifndef EVL_UTILITY_FLAGS_UTIL
#define EVL_UTILITY_FLAGS_UTIL

#ifndef INSERT_FLAG
#define INSERT_FLAG(source, flag) \
	((source) = (unsigned int)(source) | (unsigned int)(flag))

#define REMOVE_FLAG(source, flag) \
	((source) = (unsigned int)(source) & (~(unsigned int)(flag)))

#define HAS_FLAG(source, flag) \
	(((source) & (unsigned int)(flag)) != 0)

#endif // 

#endif	//EVL_UTILITY_FLAGS_UTIL 

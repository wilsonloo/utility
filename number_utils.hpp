#ifndef EVL_NUMBER_UTILS_UTIL
#define EVL_NUMBER_UTILS_UTIL

namespace evl
{
    namespace utility
    {
        namespace number_utils
        {
            template<typename T>
            inline T fix_between_in_range(const T minValue, const T maxValue, /*OUT*/T& value)
            {
                if (value < minValue)
                    value = minValue;

                if (value > maxValue)
                    value = maxValue;

                return value;
            }
        }
    }
}

#endif	//EVL_NUMBER_UTILS_UTIL 

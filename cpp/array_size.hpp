#ifndef EVL_UTILITY_ARRAY_SIZE_HPP_
#define EVL_UTILITY_ARRAY_SIZE_HPP_

namespace evl
{
	namespace utility
	{
        template <typename T, int N>                                                                                                                                                                  
        unsigned int ArraySize (T (&arr)[N]) 
        { //此处是数组的引用，在C语言中不支持                                                                                                                                                                
            return N;                                                                                                                                                                                 
        }
    }
}    

#endif // EVL_UTILITY_ARRAY_SIZE_HPP_
/********************************************************************
 *     Author  : Wensheng.Luo
 *       Mail  : samule21@163.com
 * CreateTime  : 2017-11-25 14:36
 * Description : 
*********************************************************************/


#ifndef EVL_UTILITY_SCOPED_PTR_HPP_
#define EVL_UTILITY_SCOPED_PTR_HPP_

#include <stdarg.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>

namespace evl
{
    namespace utility
    {
        class scoped_ptr
        {
        private:
            // 析构时调用
            std::function<void ()> onExitHandler;

        public:
            explicit scoped_ptr(std::function<void()> onExitHandler) {
                this->onExitHandler = onExitHandler;
            }

            virtual ~scoped_ptr()
            {
                if (onExitHandler != nullptr)
                    onExitHandler();
            }
        };
    }
}

#endif  // EVL_UTILITY_SCOPED_PTR_HPP_

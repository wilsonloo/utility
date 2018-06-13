/********************************************************************
 *     Author  : Wensheng.Luo
 *       Mail  : samule21@163.com
 * CreateTime  : 2017-11-25 14:36
 * Description : 
*********************************************************************/


#ifndef EVL_UTILITY_TICK_CALLER_HPP_
#define EVL_UTILITY_TICK_CALLER_HPP_

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
        class TickCaller 
        {
        public:
            typedef std::function<void()> CallBackType;

        private:
            CallBackType callbackHandler_;
            int next_;
            int now_;

        public:
            explicit TickCaller(CallBackType onTimeoutHandler, int interval = 1)
                : next_(interval)
                , now_(0)
                , callbackHandler_(onTimeoutHandler)
            {}

            void tick(int eclapsed = 1)
            {
                now_ += eclapsed;

                // todo 
                if (now_ >= next_)
                {
                    now_ = 0;

                    callbackHandler_();
                }
            }
        };
    }
}

#endif  // EVL_UTILITY_TICK_CALLER_HPP_

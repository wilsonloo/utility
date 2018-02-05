#ifndef EVL_TIME_UTILS_UTIL
#define EVL_TIME_UTILS_UTIL

namespace evl
{
    namespace utility
    {
        namespace time_utls
        {
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined(_WINDOWS) || defined(WIN) || defined(_WIN64) || defined( __WIN64__ )
            static time_t str_to_unixtime(const char* strTime)
            {
                tm tm_;
                int year, month, day, hour, minute, second;
                sscanf(strTime, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
                tm_.tm_year = year - 1900;
                tm_.tm_mon = month - 1;
                tm_.tm_mday = day;
                tm_.tm_hour = hour;
                tm_.tm_min = minute;
                tm_.tm_sec = second;
                tm_.tm_isdst = 0;

                time_t t_ = mktime(&tm_); //已经减了8个时区  
                return t_; //秒时间  
            }

#else
            static time_t str_to_unixtime(const char* strTime)
            {
                tm tm_;
                time_t t_;

                strptime(strTime, "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间  
                tm_.tm_isdst = 0;
                t_ = mktime(&tm_); //将tm时间转换为秒时间  

                return t_;
            }

#endif

            // 获取下月开始的unixtime（单位：秒，从1970年开始的秒数）
            static unsigned int getNextMonthBeginUnixtime()
            {
                time_t now = time(nullptr);
                struct tm* p_tm = localtime(&now); /*获取本地时区时间*/
                p_tm->tm_mon += 1; // 增加一个月
                p_tm->tm_mday = 1; // 每月第一天
                p_tm->tm_hour = 0;
                p_tm->tm_min = 0;
                p_tm->tm_sec = 0;
                p_tm->tm_isdst = 0;

                time_t t_ = mktime(p_tm); //已经减了8个时区  

                return t_;
            }

            // 获取本月开始的unixtime（单位：秒，从1970年开始的秒数）
            static unsigned int getMonthBeginUnixtime()
            {
                time_t now = time(nullptr);
                struct tm* p_tm = localtime(&now); /*获取本地时区时间*/
                p_tm->tm_mday = 1; // 每月第一天
                p_tm->tm_hour = 0;
                p_tm->tm_min = 0;
                p_tm->tm_sec = 0;
                p_tm->tm_isdst = 0;

                time_t t_ = mktime(p_tm); //已经减了8个时区  

                return t_;
            }

            // 获取本周开始的unixtime（单位：秒，从1970年开始的秒数）(星期天是开始日期)
            static unsigned int getWeekBeginUnixtime()
            {
                time_t now = time(nullptr);
                struct tm* p_tm = localtime(&now); /*获取本地时区时间*/
                p_tm->tm_wday = 0;
                p_tm->tm_hour = 0;
                p_tm->tm_min = 0;
                p_tm->tm_sec = 0;
                p_tm->tm_isdst = 0;

                time_t t_ = mktime(p_tm); //已经减了8个时区  

                return t_;
            }

            // 当前开始时的 unixtime（单位：秒，从1970年开始的秒数）
            static unsigned int getTodayBeginUnixtime()
            {
                time_t now = time(nullptr);
                struct tm* p_tm = localtime(&now); /*获取本地时区时间*/
                p_tm->tm_hour = 0;
                p_tm->tm_min = 0;
                p_tm->tm_sec = 0;
                p_tm->tm_isdst = 0;

                time_t t_ = mktime(p_tm); //已经减了8个时区  

                return t_;
            }
        }
    }
}

#endif	//EVL_TIME_UTILS_UTIL 

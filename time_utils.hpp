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

                time_t t_ = mktime(&tm_); //�Ѿ�����8��ʱ��  
                return t_; //��ʱ��  
            }

#else
            static time_t str_to_unixtime(const char* strTime)
            {
                tm tm_;
                time_t t_;

                strptime(strTime, "%Y-%m-%d %H:%M:%S", &tm_); //���ַ���ת��Ϊtmʱ��  
                tm_.tm_isdst = 0;
                t_ = mktime(&tm_); //��tmʱ��ת��Ϊ��ʱ��  

                return t_;
            }

#endif

            // ��ȡ���¿�ʼ��unixtime����λ���룬��1970�꿪ʼ��������
            static unsigned int getNextMonthBeginUnixtime()
            {
                time_t now = time(nullptr);
                struct tm* p_tm = localtime(&now); /*��ȡ����ʱ��ʱ��*/
                p_tm->tm_mon += 1; // ����һ����
                p_tm->tm_mday = 1; // ÿ�µ�һ��
                p_tm->tm_hour = 0;
                p_tm->tm_min = 0;
                p_tm->tm_sec = 0;
                p_tm->tm_isdst = 0;

                time_t t_ = mktime(p_tm); //�Ѿ�����8��ʱ��  

                return t_;
            }

            // ��ȡ���¿�ʼ��unixtime����λ���룬��1970�꿪ʼ��������
            static unsigned int getMonthBeginUnixtime()
            {
                time_t now = time(nullptr);
                struct tm* p_tm = localtime(&now); /*��ȡ����ʱ��ʱ��*/
                p_tm->tm_mday = 1; // ÿ�µ�һ��
                p_tm->tm_hour = 0;
                p_tm->tm_min = 0;
                p_tm->tm_sec = 0;
                p_tm->tm_isdst = 0;

                time_t t_ = mktime(p_tm); //�Ѿ�����8��ʱ��  

                return t_;
            }

            // ��ȡ���ܿ�ʼ��unixtime����λ���룬��1970�꿪ʼ��������(�������ǿ�ʼ����)
            static unsigned int getWeekBeginUnixtime()
            {
                time_t now = time(nullptr);
                struct tm* p_tm = localtime(&now); /*��ȡ����ʱ��ʱ��*/
                p_tm->tm_wday = 0;
                p_tm->tm_hour = 0;
                p_tm->tm_min = 0;
                p_tm->tm_sec = 0;
                p_tm->tm_isdst = 0;

                time_t t_ = mktime(p_tm); //�Ѿ�����8��ʱ��  

                return t_;
            }

            // ��ǰ��ʼʱ�� unixtime����λ���룬��1970�꿪ʼ��������
            static unsigned int getTodayBeginUnixtime()
            {
                time_t now = time(nullptr);
                struct tm* p_tm = localtime(&now); /*��ȡ����ʱ��ʱ��*/
                p_tm->tm_hour = 0;
                p_tm->tm_min = 0;
                p_tm->tm_sec = 0;
                p_tm->tm_isdst = 0;

                time_t t_ = mktime(p_tm); //�Ѿ�����8��ʱ��  

                return t_;
            }
        }
    }
}

#endif	//EVL_TIME_UTILS_UTIL 

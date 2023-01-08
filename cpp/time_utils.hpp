#ifndef EVL_TIME_UTILS_UTIL
#define EVL_TIME_UTILS_UTIL

#include <iomanip>
#include <ctime>

#if defined( __WIN32__ ) || defined( _WIN32 ) || defined(_WINDOWS) || defined(WIN) || defined(_WIN64) || defined( __WIN64__ )
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#endif

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

			static time_t str_to_unixtime2(const char* strTime)
			{
				tm tm_;
				int year, month, day;
				sscanf(strTime, "%d-%d-%d", &year, &month, &day);
				tm_.tm_year = year - 1900;
				tm_.tm_mon = month - 1;
				tm_.tm_mday = day;
				tm_.tm_hour = 0;
				tm_.tm_min = 0;
				tm_.tm_sec = 0;
				tm_.tm_isdst = 0;

				time_t t_ = mktime(&tm_); //�Ѿ�����8��ʱ��  
				return t_; //��ʱ��  
			}

            static std::string unixtime_to_str(const time_t& what_time, const char* format = "%Y-%m-%d %H:%M:%S") {
                std::stringstream ss;
                ss << std::put_time(std::localtime(&what_time), format);
                return ss.str();
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

			static time_t str_to_unixtime2(const char* strTime)
			{
				tm tm_;
				time_t t_;

				strptime(strTime, "%Y-%m-%d", &tm_); //���ַ���ת��Ϊtmʱ��
				tm_.tm_hour = 0;
				tm_.tm_min = 0;
				tm_.tm_sec = 0;
				tm_.tm_isdst = 0;
				t_ = mktime(&tm_); //��tmʱ��ת��Ϊ��ʱ��  

				return t_;
			}

            static std::string unixtime_to_str(const time_t& what_time, const char* format = "%Y-%m-%d %H:%M:%S") {

                std::tm * ptm = std::localtime(&what_time);
                char buffer[128] = { 0, };
                std::strftime(buffer, sizeof(buffer), format, ptm);

                return std::string(buffer);
            }

#endif
			// ��ȡ����ָ���µ�unixtime����λ���룬��1970�꿪ʼ��������
			static unsigned int getNextYearAssignMonthUnixtime(int when)
			{
				time_t when_time(when);
				struct tm* when_tm = localtime(&when_time); /*��ȡ����ʱ��ʱ��*/
				int when_mon = when_tm->tm_mon;
				int when_mday = when_tm->tm_mday;
				int when_hour = when_tm->tm_hour;
				int when_min = when_tm->tm_min;
				int when_sec = when_tm->tm_sec;

				time_t now = time(nullptr);
				struct tm* p_tm = localtime(&now); /*��ȡ����ʱ��ʱ��*/
				p_tm->tm_year += 1;	// ����һ����
				p_tm->tm_mon = when_mon;
				p_tm->tm_mday = when_mday;
				p_tm->tm_hour = when_hour;
				p_tm->tm_min = when_min;
				p_tm->tm_sec = when_sec;
				p_tm->tm_isdst = 0;

				time_t t_ = mktime(p_tm); //�Ѿ�����8��ʱ�� 

				return t_;
			}

			// ��ȡ���꿪ʼ��unixtime����λ���룬��1970�꿪ʼ��������
			static unsigned int getNextYearBeginUnixtime()
			{
				time_t now = time(nullptr);
				struct tm* p_tm = localtime(&now); /*��ȡ����ʱ��ʱ��*/
				p_tm->tm_year += 1;	// ����һ����
				p_tm->tm_mon = 1;	// ÿ���һ��
				p_tm->tm_mday = 1;	// ÿ�µ�һ��
				p_tm->tm_hour = 0;
				p_tm->tm_min = 0;
				p_tm->tm_sec = 0;
				p_tm->tm_isdst = 0;

				time_t t_ = mktime(p_tm); //�Ѿ�����8��ʱ��  

				return t_;
			}

			// ��ȡ����ָ�����unixtime����λ���룬��1970�꿪ʼ��������
			static unsigned int getNextMonthAssignDayUnixtime(int when)
			{
				time_t when_time(when);
				struct tm* when_tm = localtime(&when_time); /*��ȡ����ʱ��ʱ��*/
				int when_mday = when_tm->tm_mday;
				int when_hour = when_tm->tm_hour;
				int when_min = when_tm->tm_min;
				int when_sec = when_tm->tm_sec;

				time_t now = time(nullptr);
				struct tm* p_tm = localtime(&now); /*��ȡ����ʱ��ʱ��*/
				p_tm->tm_mon += 1; // ����һ����
				p_tm->tm_mday = when_mday;
				p_tm->tm_hour = when_hour;
				p_tm->tm_min = when_min;
				p_tm->tm_sec = when_sec;
				p_tm->tm_isdst = 0;

				time_t t_ = mktime(p_tm); //�Ѿ�����8��ʱ�� 

				return t_;
			}

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

            // ���쿪ʼʱ�� unixtime����λ���룬��1970�꿪ʼ��������
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


            // ���쿪ʼʱ�� unixtime����λ���룬��1970�꿪ʼ��������
            static unsigned int getYesterdayBeginUnixtime()
            {
                auto today = getTodayBeginUnixtime();
                return today - 24 * 3600;
            }

            // ָ��ʱ��ÿ�쿪ʼʱ�� unixtime����λ���룬��1970�꿪ʼ��������
            static unsigned int getDailyBeginUnixtime(int when)
            {
                time_t when_time(when);
                struct tm* p_tm = localtime(&when_time); /*��ȡ����ʱ��ʱ��*/
                p_tm->tm_hour = 0;
                p_tm->tm_min = 0;
                p_tm->tm_sec = 0;
                p_tm->tm_isdst = 0;

                time_t t_ = mktime(p_tm); //�Ѿ�����8��ʱ��  

                return t_;
            }

			// �õ�ÿ���huor���ʱ����ʱ��� unixtime����λ���룬��1970�꿪ʼ��������
			static unsigned int getSpecifyUnixtime(int when,const int &huor)
			{
				time_t when_time(when);
				struct tm* p_tm = localtime(&when_time); /*��ȡ����ʱ��ʱ��*/
				p_tm->tm_hour = huor;
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

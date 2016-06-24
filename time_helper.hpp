#ifndef EVL_UTILITY_TIME_HELPER_HPP_
#define EVL_UTILITY_TIME_HELPER_HPP_

#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace evl
{
	namespace utility
	{
		// ʱ����㣨2009��1��1��0ʱ��
		static boost::posix_time::ptime get_epoch_timestamp()
		{
			static boost::posix_time::ptime epoch_timestamp(boost::posix_time::time_from_string("2009-01-01 00:00:00"));
			return epoch_timestamp;
		}

		// ��ȡ��2009��1��1��0ʱ �������ܼ�������
		static boost::uint64_t get_epoch_nano_seconds()
		{
			boost::posix_time::ptime now(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration duration = now - get_epoch_timestamp();
			return duration.total_nanoseconds();
		}

		// ��ȡ��2009��1��1��0ʱ �������ܼ�΢��
		static boost::uint64_t get_epoch_micro_seconds()
		{
			boost::posix_time::ptime now(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration duration = now - get_epoch_timestamp();
			return duration.total_microseconds();
		}

		// ��ȡ��2009��1��1��0ʱ �������ܼƺ�����
		static boost::uint64_t get_epoch_mil_seconds()
		{
			boost::posix_time::ptime now(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration duration = now - get_epoch_timestamp();
			return duration.total_milliseconds();
		}

		// ��ȡ��2009��1��1��0ʱ �������ܼ�����
		static boost::uint32_t get_epoch_seconds()
		{
			boost::posix_time::ptime now(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration duration = now - get_epoch_timestamp();
			return duration.total_seconds();
		}

		// ��ȡ����Ŀ�ʼʱ�䣨��2009�꿪ʼ��������
		static boost::uint32_t get_today_seconds_from_epoch()
		{
			boost::uint32_t now_seconds = get_epoch_seconds();

			// ȥ��������ѹ�ȥ����
			return now_seconds - now_seconds % (24*3600);
		}

		// ͨ������ʱ�任�����ʵʱ��
		static void EpochMilliSecond2RealTime(/*IN*/boost::uint64_t ms, /*OUT*/boost::posix_time::ptime& real_time)
		{
			real_time = get_epoch_timestamp() + boost::posix_time::microseconds(ms);
		}
	}
}

#endif // EVL_UTILITY_TIME_HELPER_HPP_

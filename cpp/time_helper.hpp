#ifndef EVL_UTILITY_TIME_HELPER_HPP_
#define EVL_UTILITY_TIME_HELPER_HPP_

#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace evl
{
	namespace utility
	{
		// 时间起点（2009年1月1日0时）
		static boost::posix_time::ptime get_epoch_timestamp()
		{
			static boost::posix_time::ptime epoch_timestamp(boost::posix_time::time_from_string("2009-01-01 00:00:00"));
			return epoch_timestamp;
		}

		// 获取从2009年1月1日0时 到现在总计纳秒数
		static boost::uint64_t get_epoch_nano_seconds()
		{
			boost::posix_time::ptime now(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration duration = now - get_epoch_timestamp();
			return duration.total_nanoseconds();
		}

		// 获取从2009年1月1日0时 到现在总计微秒
		static boost::uint64_t get_epoch_micro_seconds()
		{
			boost::posix_time::ptime now(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration duration = now - get_epoch_timestamp();
			return duration.total_microseconds();
		}

		// 获取从2009年1月1日0时 到现在总计毫秒数
		static boost::uint64_t get_epoch_mil_seconds()
		{
			boost::posix_time::ptime now(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration duration = now - get_epoch_timestamp();
			return duration.total_milliseconds();
		}

		// 获取从2009年1月1日0时 到现在总计秒数
		static boost::uint32_t get_epoch_seconds()
		{
			boost::posix_time::ptime now(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration duration = now - get_epoch_timestamp();
			return duration.total_seconds();
		}

		// 获取当天的开始时间（从2009年开始的秒数）
		static boost::uint32_t get_today_seconds_from_epoch()
		{
			boost::uint32_t now_seconds = get_epoch_seconds();

			// 去除当天的已过去秒数
			return now_seconds - now_seconds % (24*3600);
		}

		// 通过毫米时间换算成真实时间
		static void EpochMilliSecond2RealTime(/*IN*/boost::uint64_t ms, /*OUT*/boost::posix_time::ptime& real_time)
		{
			real_time = get_epoch_timestamp() + boost::posix_time::microseconds(ms);
		}
	}
}

#endif // EVL_UTILITY_TIME_HELPER_HPP_

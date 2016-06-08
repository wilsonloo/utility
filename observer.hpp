#ifndef EVL_UTILITY_OBSERVER_HPP_
#define EVL_UTILITY_OBSERVER_HPP_

#include <set>
#include <boost/signals2/signal.hpp>
#include <boost/signals2/trackable.hpp>
#include <boost/signals2/connection.hpp>
#include <boost/function.hpp>

namespace evl
{
	namespace utility
	{
		//
		// 可观察者（可以观察其他对象）
		// 
		class TrackableObservable : public boost::signals2::trackable
		{
		public:
			typedef boost::function<boost::int32_t(const void*, boost::uint32_t)> ObserveredEventHandlerType;
			typedef std::set<boost::signals2::connection> ObservableListType;

			TrackableObservable(ObserveredEventHandlerType event_handler)
				:event_handler_(event_handler)
			{}

			~TrackableObservable()
			{
				observable_conn_.clear();
			}

			inline bool insert_connection(const boost::signals2::connection& conn)
			{
				return observable_conn_.insert(conn).second;
			}

			// 返回连接数
			inline boost::uint32_t num_conns()
			{
				// 清理废弃连接的操作
				// 由于observable_conn_被动被boost::signal断开时没法获得通知
				// 此列表>=boost::signals::trackable中的连接列表
				ObservableListType::iterator it = observable_conn_.begin();
				for (; it!=observable_conn_.end();)
				{
					if (!(*it).connected())
						observable_conn_.erase(it++);
					else
						++it;
				}

				return observable_conn_.size();
			}

			inline boost::int32_t notify_event(const void* data_ptr, boost::uint32_t data_len)
			{
				return event_handler_(data_ptr, data_len);
			}

			// 断开所有(主动断开)
			inline void disconnect_all()
			{
				ObservableListType::iterator it = observable_conn_.begin();
				for (; it!=observable_conn_.end(); ++it)
				{
					if ((*it).connected())
						(*it).disconnect();
				}
				observable_conn_.clear();
			}

		private:
			ObserveredEventHandlerType event_handler_;
			// 此列表为了处理主动断开
			ObservableListType observable_conn_;
		};

		//
		// 观察者列表
		//
		typedef boost::signals2::signal<boost::int32_t(const void*, boost::uint32_t)> ObserverListType;
	}
}

#endif // EVL_UTILITY_OBSERVER_HPP_
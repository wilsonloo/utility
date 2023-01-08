/********************************************************************
*     Author  : Wensheng.Luo
*       Mail  : samule21@163.com
* CreateTime  : 2016-08-18 11:49
* Description : 观察者模式

	该模式的实现主要有两个类：Observer（观察者） 和 Observable（被观察者, 由ObjservableSubject 封装）
	Observer 与 Observable 之间通过 Connection 进行连接，具体的操作已由 ObservableSubject 进行了封装
	Observer 通过虚函数 on_event() 提供了被观察者事件的回调封装，需要由子类进行覆盖实现。
	以下是例子：

	// 观察者主体
	class Master : public evl::utility::Observer
	{
		// 重写观察者事件处理
		virtual int on_event(const void* data, int param) override {
		std::cout << "received : " << param << std::endl;

		return 0;
		}
	};

	具体使用：
	{
		// 被观察者对象
		auto subject = new evl::utility::ObservableSubject();

		// 观察者
		auto master = new Master();

		// master 向 subject 进行观察
		subject->add_observer(master);

		// subject 触发事件
		subject->notify(nullptr, 22);

		// 测试运行结果
		subject->dump();
		master->dump();
	}
 *
*********************************************************************/

#ifndef EVL_UTILITY_OBSERVER_HPP_
#define EVL_UTILITY_OBSERVER_HPP_

#include <set>
#include <list>
#include <map>
#include <functional>
#include <memory>
#include <iostream>

namespace evl
{
	namespace utility
	{
		class Observable;
		class Observer;

		//
		// 自连接器
		// 作用：当被观察者、观察者断开时，自动释放连接；是全双工机制，不会出现单方向连接
		//
		class Connection
		{
			friend class Observable;
			friend class Observer;

		private:
			// 断开连接后的销毁器
			typedef std::function<void(Connection*)> ConnectionDisposerType;
			typedef std::list<ConnectionDisposerType> DisposerCollectionType;
			DisposerCollectionType disposers_;

			// 断开连接
			void disconnect()
			{
				for (auto dispose : disposers_) {
					dispose(this);
				}

				disposers_.clear();
			}

			// 添加断开连接的销毁器
			void add_dispose_handler(ConnectionDisposerType disposer) {
				disposers_.push_back(disposer);
			}
		};
		typedef std::shared_ptr<Connection> ConnectionPtr;

		//
		// 被观察者
		//
		class Observable
		{
		public:
			//
			// 事件处理器
			//
			typedef std::function<int(const void*, int)> EventHandlerType;

			//
			// 每个连接对应一个事件处理器
			//
			typedef std::map<ConnectionPtr, EventHandlerType> ConnectionCollectionType;

		private:
			// 连接列表
			ConnectionCollectionType conns_;

		private:
			//////////////////////////////////////////////////////////////////////////
			// 禁止所有拷贝构造函数、赋值函数，以免造成connection 的引用计数错误
			explicit Observable(const Observable& other);
			explicit Observable(const Observable* other);
			Observable operator=(const Observable& other);
			Observable operator=(const Observable* other);

		public :
			Observable() {

			}

			virtual ~Observable() {
				disconnect_all();
			}

			// 全双工断开了全部链接
			void disconnect_all() {
				if (!conns_.empty()) {
					for (ConnectionCollectionType::iterator iter = conns_.begin(); iter != conns_.end(); ) {
						ConnectionPtr conn = iter->first;
						conns_.erase(iter++);

						conn->disconnect();
					}
				}
			}

			// 向全部观察者发送事件
			virtual void notify_all(const void* data, int param) {
				if (conns_.empty())
					return;
				
				for (ConnectionCollectionType::iterator iter = conns_.begin(); iter != conns_.end(); ++iter) {
					EventHandlerType handler = iter->second;
					handler(data, param);
				}
			}

			ConnectionPtr create_connection(EventHandlerType observer) {
				auto conn_ptr = std::shared_ptr<Connection>(new Connection());

				// 绑定连接销毁器
				conn_ptr->add_dispose_handler(std::bind(&Observable::ConnectionDisposer, this, std::placeholders::_1));

				// conn 绑定处理器
				conns_.insert(std::make_pair(conn_ptr, observer));

				return conn_ptr;
			}

			void ConnectionDisposer(Connection* disposing_conn) {
				if (disposing_conn == nullptr)
					return;

				// 从列表里移除 key 为 disposing_conn 的元素
				for (ConnectionCollectionType::iterator iter = conns_.begin(); iter != conns_.end(); ) {
					ConnectionPtr conn = iter->first;
					if (conn.get() == disposing_conn) {
						iter = conns_.erase(iter);
					}
					else {
						++iter;
					}
				}
			}

			void dump()
			{
				std::cout << "connection count : " << conns_.size() << std::endl;
			}
		};

		//
		// 可观察者（可以观察其他对象）
		// 
		class Observer
		{
		private:
			// 此列表为了处理主动断开
			typedef std::list<ConnectionPtr> ObservingTargetsCollectionType;
			ObservingTargetsCollectionType observing_targets_;

		private:
			//////////////////////////////////////////////////////////////////////////
			// 禁止所有拷贝构造函数、赋值函数，以免造成connection 的引用计数错误
			explicit Observer(const Observer& other);
			explicit Observer(const Observer* other);
			Observer operator=(const Observer& other);
			Observer operator=(const Observer* other);

		public:

			Observer()
			{

			}

			virtual ~Observer()
			{
				disconnect_all();
			}

			// 断开所有(主动断开)
			inline void disconnect_all()
			{
				for(ObservingTargetsCollectionType::iterator it = observing_targets_.begin(); it != observing_targets_.end(); )
				{
					ConnectionPtr conn_ptr = *it;
					observing_targets_.erase(it++);

					conn_ptr->disconnect();
				}

				observing_targets_.clear();
			}

			// 事件处理
			virtual int on_event(const void*, int) {
				// todo 
				return 0;
			}

			inline void insert_connection(ConnectionPtr conn_ptr)
			{
				// 绑定连接销毁器
				conn_ptr->add_dispose_handler(std::bind(&Observer::ConnectionDisposer, this, std::placeholders::_1));

				observing_targets_.push_back(conn_ptr);

			}

			void ConnectionDisposer(Connection* disposing_conn) {
				if (disposing_conn == nullptr)
					return;

				// 从列表里移除 key 为 disposing_conn 的元素
				for (ObservingTargetsCollectionType::iterator iter = observing_targets_.begin(); iter != observing_targets_.end(); ) {
					ConnectionPtr conn = *iter;
					if (conn.get() == disposing_conn) {
						observing_targets_.erase(iter++);
					}
					else {
						++iter;
					}
				}
			}
			
			void dump()
			{
				std::cout << "observing target count : " << observing_targets_.size() << std::endl;
			}
		};

		class ObservableSubject : public Observable
		{
		public:
			void add_observer(evl::utility::Observer* observer)
			{
				auto conn = create_connection(std::bind<int>(&evl::utility::Observer::on_event, observer, std::placeholders::_1, std::placeholders::_2));
				observer->insert_connection(conn);
			}

			virtual void notify(const void* data, int param) {
				notify_all(data, param);
			}

            virtual void notify(const void* data) {
                notify(data, 0);
            }
		};
	}
}

#endif // EVL_UTILITY_OBSERVER_HPP_
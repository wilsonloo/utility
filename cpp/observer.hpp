/********************************************************************
*     Author  : Wensheng.Luo
*       Mail  : samule21@163.com
* CreateTime  : 2016-08-18 11:49
* Description : �۲���ģʽ

	��ģʽ��ʵ����Ҫ�������ࣺObserver���۲��ߣ� �� Observable�����۲���, ��ObjservableSubject ��װ��
	Observer �� Observable ֮��ͨ�� Connection �������ӣ�����Ĳ������� ObservableSubject �����˷�װ
	Observer ͨ���麯�� on_event() �ṩ�˱��۲����¼��Ļص���װ����Ҫ��������и���ʵ�֡�
	���������ӣ�

	// �۲�������
	class Master : public evl::utility::Observer
	{
		// ��д�۲����¼�����
		virtual int on_event(const void* data, int param) override {
		std::cout << "received : " << param << std::endl;

		return 0;
		}
	};

	����ʹ�ã�
	{
		// ���۲��߶���
		auto subject = new evl::utility::ObservableSubject();

		// �۲���
		auto master = new Master();

		// master �� subject ���й۲�
		subject->add_observer(master);

		// subject �����¼�
		subject->notify(nullptr, 22);

		// �������н��
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
		// ��������
		// ���ã������۲��ߡ��۲��߶Ͽ�ʱ���Զ��ͷ����ӣ���ȫ˫�����ƣ�������ֵ���������
		//
		class Connection
		{
			friend class Observable;
			friend class Observer;

		private:
			// �Ͽ����Ӻ��������
			typedef std::function<void(Connection*)> ConnectionDisposerType;
			typedef std::list<ConnectionDisposerType> DisposerCollectionType;
			DisposerCollectionType disposers_;

			// �Ͽ�����
			void disconnect()
			{
				for (auto dispose : disposers_) {
					dispose(this);
				}

				disposers_.clear();
			}

			// ��ӶϿ����ӵ�������
			void add_dispose_handler(ConnectionDisposerType disposer) {
				disposers_.push_back(disposer);
			}
		};
		typedef std::shared_ptr<Connection> ConnectionPtr;

		//
		// ���۲���
		//
		class Observable
		{
		public:
			//
			// �¼�������
			//
			typedef std::function<int(const void*, int)> EventHandlerType;

			//
			// ÿ�����Ӷ�Ӧһ���¼�������
			//
			typedef std::map<ConnectionPtr, EventHandlerType> ConnectionCollectionType;

		private:
			// �����б�
			ConnectionCollectionType conns_;

		private:
			//////////////////////////////////////////////////////////////////////////
			// ��ֹ���п������캯������ֵ�������������connection �����ü�������
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

			// ȫ˫���Ͽ���ȫ������
			void disconnect_all() {
				if (!conns_.empty()) {
					for (ConnectionCollectionType::iterator iter = conns_.begin(); iter != conns_.end(); ) {
						ConnectionPtr conn = iter->first;
						conns_.erase(iter++);

						conn->disconnect();
					}
				}
			}

			// ��ȫ���۲��߷����¼�
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

				// ������������
				conn_ptr->add_dispose_handler(std::bind(&Observable::ConnectionDisposer, this, std::placeholders::_1));

				// conn �󶨴�����
				conns_.insert(std::make_pair(conn_ptr, observer));

				return conn_ptr;
			}

			void ConnectionDisposer(Connection* disposing_conn) {
				if (disposing_conn == nullptr)
					return;

				// ���б����Ƴ� key Ϊ disposing_conn ��Ԫ��
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
		// �ɹ۲��ߣ����Թ۲���������
		// 
		class Observer
		{
		private:
			// ���б�Ϊ�˴��������Ͽ�
			typedef std::list<ConnectionPtr> ObservingTargetsCollectionType;
			ObservingTargetsCollectionType observing_targets_;

		private:
			//////////////////////////////////////////////////////////////////////////
			// ��ֹ���п������캯������ֵ�������������connection �����ü�������
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

			// �Ͽ�����(�����Ͽ�)
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

			// �¼�����
			virtual int on_event(const void*, int) {
				// todo 
				return 0;
			}

			inline void insert_connection(ConnectionPtr conn_ptr)
			{
				// ������������
				conn_ptr->add_dispose_handler(std::bind(&Observer::ConnectionDisposer, this, std::placeholders::_1));

				observing_targets_.push_back(conn_ptr);

			}

			void ConnectionDisposer(Connection* disposing_conn) {
				if (disposing_conn == nullptr)
					return;

				// ���б����Ƴ� key Ϊ disposing_conn ��Ԫ��
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
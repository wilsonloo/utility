#ifndef EVL_UTILITY_RING_BUFFER_H_
#define EVL_UTILITY_RING_BUFFER_H_

#include <list>
#include <mutex>
#include <condition_variable>

namespace evl
{
    namespace utility
    {
        namespace double_queue
        {
            // 信号量
            class Semaphore {
            public:
                Semaphore(int value = 1) : count{ value } {}

                void wait() {
                    std::unique_lock<std::mutex> lock{ mutex };
                    if (--count < 0) // count is not enough ?
                        condition.wait(lock); // suspend and wait...
                }
                void signal() {
                    std::lock_guard<std::mutex> lock{ mutex };
                    if (++count <= 0) // have some thread suspended ?
                        condition.notify_one(); // notify one !
                }

            private:
                int count;
                std::mutex mutex;
                std::condition_variable condition;
            };

            template<typename E, typename C = std::list<E> >
            class Container
            {
                typedef typename C::iterator iterator;

            public:
                Container()
                    : mCount(0)
                {}

                virtual ~Container()
                {}

            public:
                void push_back(const E& e) {
                    mRawContainer.push_back(e); 
                    ++mCount;
                }

                const E& front() { return mRawContainer.front(); }

                void pop_front() {
                    mRawContainer.pop_front();
                    if (mCount > 0)
                        --mCount;
                }

                bool empty()const { return mRawContainer.empty();}

                iterator begin() { return mRawContainer.begin(); }
                iterator end() { return mRawContainer.end();  }

                int count() { return mCount; }

            private:
                C mRawContainer;

                // 当前大小
                int mCount;
            };

            template<typename E, typename QueueType = Container<E> >
            class DoubleQueue
            {
            public:
                DoubleQueue()
                    : mWriteQueueIndex(0)
                {}
                
                // 获取写队列
                QueueType& GetWriteQueue() {
                    std::lock_guard<std::mutex> lockGuard(mLock);

                    return mQueues[get_write_queue_index()];
                }

                // 获取读队列
                QueueType& GetReadQueue() {
                    std::lock_guard<std::mutex> lockGuard(mLock);

                    return mQueues[get_read_queue_index()];
                }

                // 交换读写队列
                void SwapReadWriteQueue() {
                    std::lock_guard<std::mutex> lockGuard(mLock);

                    mWriteQueueIndex = (mWriteQueueIndex + 1) % 2;
                }

            private:
                inline int get_write_queue_index()const { return mWriteQueueIndex; }
                inline int get_read_queue_index()const { return (get_write_queue_index() + 1) % 2; }

            private:
                // 读写互斥所
                std::mutex mLock;

                // 写队列索引
                int mWriteQueueIndex;

                // 双缓冲队列，分别负责读、写
                QueueType mQueues[2];

            };
        }
    }
}

#endif	//EVL_UTILITY_RING_BUFFER_H_ 

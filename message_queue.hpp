#ifndef __EVL_MESSAGE_QUEUE_H__
#define __EVL_MESSAGE_QUEUE_H__

/************************************************************************/
/* 基于 MessageBlock 的消息队列                                         */
/************************************************************************/

#include <mutex>
#include <utility/semaphore.h>
#include "message_block.hpp"

namespace evl
{
	namespace utility
	{
		class MessageQueue
		{
		public:
			enum
			{
				INVALID_VALUE = 0xFFFFfff,
				DEFAULT_TASK_QUEUE_SIZE = 500000,
			};

		public:
			MessageQueue()
				: maxSize_m(DEFAULT_TASK_QUEUE_SIZE),
				curSize_m(0),
				header_m(NULL),
				tail_m(NULL)
			{
				
			}

			virtual ~MessageQueue()
			{
				(void)cleanup();
			}

			// 阻塞式插入单元
			int putQueue(MessageBlock *&node)
			{
				if (0 != pushQueue(node))
				{
					return INVALID_VALUE;
				}

				(void)sem_m.post();

				return 0;
			}

			// 基于信号量的阻塞式获取元素
			int getQueue(MessageBlock *&node)
			{
				(void)sem_m.wait();

				return popQueue(node);
			}

			// 超时获取元素
			int getQueueTimeout(MessageBlock *&node, int interval)
			{
				int ret = sem_m.timedWait(interval);
				if (Semaphore::SEM_WAIT_TIMEOUT == ret)
				{
					// Timeout
					return 0;
				}

				if (Semaphore::INVALID_VALUE == ret)
				{
					// Error
					return INVALID_VALUE;
				}

				// Signaled
				return popQueue(node);
			}

			// 阻塞式插入单元
			int pushQueue(MessageBlock *&node)
			{
				std::lock_guard<std::mutex> auto_lock{ mutex_m };

				if (curSize_m > maxSize_m)
					return INVALID_VALUE;

				(void)_put_queue_non_lock(node);

				return 0;
			}

			// 阻塞式获取指定单元
			int popQueue(MessageBlock *&node)
			{
				std::lock_guard<std::mutex> auto_lock{ mutex_m };

				if (NULL == header_m || NULL == tail_m)
					return 0;

				(void)_get_queue_non_lock(node);

				return 0;
			}

			inline unsigned int getSize() const { return curSize_m; }
			inline unsigned int getMaxSize() const { return maxSize_m; }

			inline void setHigherWaterLevel(unsigned int maxSize){ maxSize_m = maxSize; }
			inline void setLowerWaterLevel(unsigned int minSize) { minSize_m = minSize; }

		private:

			// 添加单元（注：函数为内部函数，不能继续使用锁，否则可能导致重入问题）
			void _put_queue_non_lock(MessageBlock *&node)
			{
				if (NULL == header_m)
				{
					header_m = tail_m = node;
					curSize_m = 1;
				}
				else
				{
					node->setNext(header_m);
					header_m->setPrev(node);
					header_m = node;
					curSize_m++;
				}
			}

			// 获取单元（注：函数为内部函数，不能继续使用锁，否则可能导致重入问题）
			void _get_queue_non_lock(MessageBlock *&node)
			{
				node = tail_m;
				tail_m = tail_m->getPrev();
				if (NULL == tail_m)
				{
					header_m = tail_m;
				}

				if (curSize_m > 0)
				{
					curSize_m--;
				}
			}

			void cleanup()
			{
				std::lock_guard<std::mutex> auto_lock{ mutex_m };

				if (NULL != header_m || NULL != tail_m)
				{
					while (header_m != tail_m)
					{
						MessageBlock *node = header_m;
						header_m = header_m->getNext();
						
						if (node != NULL)
						{
							delete node;
							node = NULL;
						}
					}

					if (NULL != header_m)
					{
						if (header_m != NULL)
						{
							delete header_m;
							header_m = NULL;
						}
					}

					sem_m.post(curSize_m);
					curSize_m = 0;
					tail_m = NULL;
				}
			}


		private:
			// 不可拷贝
			MessageQueue(const MessageQueue&);
			MessageQueue& operator=(const MessageQueue&);

		public:

			// The maximum queue size.
			unsigned int maxSize_m;

			// The minimum queue size.
			unsigned int minSize_m;

			// Current queue size.
			unsigned int curSize_m;

			// Tail of queue.
			MessageBlock *tail_m;
			// Header of queue.
			MessageBlock *header_m;
			
			// mutex of queue.
			std::mutex mutex_m;
			
			// Signal condition of queue.
			//pthread_cond_t cond_m;
			Semaphore sem_m;
		};
	} // namespace utility
} // namespace evl

#endif // __EVL_MESSAGE_QUEUE_H__

#ifndef __EVL_UTILITY_TASK_H__
#define __EVL_UTILITY_TASK_H__

/************************************************************************/
/* 任务处理器（维护任务队列）                                           */
/* 需要由外部线程、或逻辑驱动，内部不进行线程线管处理*/
/************************************************************************/

#include <cassert>
#include "message_queue.hpp"
#include "message_block.hpp"

namespace evl
{
	namespace utility
	{
		//////////////////////////////////////////////////////////////////////////
		// 任务基类
		class TaskBase
		{
		public:
			// 任务状态
			enum TaskState
			{
				TASK_STATE_BEGIN = 0,
				TASK_STATE_START,
				TASK_STATE_RUN,
				TASK_STATE_STOPPING,
				TASK_STATE_STOP,
				TASK_STATE_END
			};

		public:
			TaskBase()
			{
				state_m = TASK_STATE_BEGIN;
			}

			virtual ~TaskBase(void)
			{}

			virtual int suspend(void)
			{
				assert(false);

				return 0;
			}

			virtual int resume(void)
			{
				assert(false);

				return 0;
			}

			virtual int wait(void)
			{
				assert(false);

				return 0;
			}

			virtual int putq(MessageBlock *&node)
			{
				return messageQueue_m.putQueue(node);
			}

			virtual int getq(MessageBlock *&node)
			{
				return messageQueue_m.getQueue(node);
			}

			virtual int getq(MessageBlock *&node, int interval)
			{
				return messageQueue_m.getQueueTimeout(node, interval);
			}

			virtual int pushq(MessageBlock *&node)
			{
				return messageQueue_m.pushQueue(node);
			}

			virtual int popq(MessageBlock *&node)
			{
				return messageQueue_m.popQueue(node);
			}

			inline int getState() const { return state_m; }
			
			inline bool isRunning() const{ return (TASK_STATE_RUN == state_m); }

			inline void setState(int state)
			{
				if (TASK_STATE_END <= state)
				{
					return;
				}

				state_m = state;
			}

		private:
			void cleanup()
			{}

		protected:
			MessageQueue messageQueue_m;

			int threadIntId_m;
			int state_m;

		private:
			// 不允许拷贝
			TaskBase &operator=(const TaskBase &);
			TaskBase(const TaskBase &);
		};
	}
}

#endif // __EVL_UTILITY_TASK_H__

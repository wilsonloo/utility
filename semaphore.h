#ifndef __EVL_UTILITY_SEMAPHORE_H__
#define __EVL_UTILITY_SEMAPHORE_H__

/************************************************************************/
/* 信号量                                                               */
/************************************************************************/

#if _PF_WINDOWS_
#include "winsock_wrapper.h"
#elif _PF_LINUX_
#include<semaphore.h>
#include <sys/time.h>
#endif 

namespace evl
{
	namespace utility
	{
		class Semaphore
		{
		public:
			enum
			{
				SEM_WAIT_SIGNALED = 1,
				SEM_WAIT_TIMEOUT = 2,
				INVALID_VALUE = 0xFFFFffff,
			};

		public:
			explicit Semaphore(int initVal = 0)
			{
#if _PF_WINDOWS_
				sem_m = ::CreateSemaphore(NULL, initVal, LONG_MAX, NULL);
#elif _PF_LINUX_
				sem_init(&sem_m, 0, initVal);
#else
				static int val = 1;
				char tmpVal[20] = { 0 };
				memset(tmpVal, 0, sizeof(tmpVal));
				uint32 curTime = (uint32)time(NULL) + val;
				val++;
				sprintf(tmpVal, "%d", curTime);
				sem_m = sem_open(tmpVal, O_CREAT | O_EXCL, 0644, 0);
#endif
			}

			virtual ~Semaphore()
			{
#if _PF_WINDOWS_
				::CloseHandle(sem_m);
				sem_m = INVALID_HANDLE_VALUE;
#elif _PF_LINUX_
				sem_destroy(&sem_m);
#else
				sem_destroy(sem_m);
#endif
			}

			void wait()
			{
#if _PF_WINDOWS_
				::WaitForSingleObject(sem_m, INFINITE);
#else
				pthread_testcancel();
#if _PF_LINUX_
				while (sem_wait(&sem_m) != 0 && errno == EINTR);
#else
				while (sem_wait(sem_m) != 0 && errno == EINTR);
#endif
#endif
			}

			bool tryWait()
			{
#if _PF_WINDOWS_
				if (::WaitForSingleObject(sem_m, 0) == WAIT_OBJECT_0)
				{
					return true;
				}

				return false;
#else
				pthread_testcancel();
				int waitRet = 0;
#if _PF_LINUX_
				while ((waitRet = sem_trywait(&sem_m)) != 0 && errno == EINTR);
#else
				while ((waitRet = sem_trywait(sem_m)) != 0 && errno == EINTR);
#endif
				return waitRet == 0;
#endif
			}

			int timedWait(int milliSeconds)
			{
#if _PF_WINDOWS_
				DWORD waitRet = 0;
				if ((waitRet = ::WaitForSingleObject(sem_m, milliSeconds)) == WAIT_OBJECT_0)
				{
					return SEM_WAIT_SIGNALED;
				}
				else if (waitRet == WAIT_TIMEOUT)
				{
					return SEM_WAIT_TIMEOUT;
				}

				return INVALID_VALUE;
#else
#if _PF_LINUX_
				struct timeval tvStart, tvEnd;
				struct timespec ts;

				::gettimeofday(&tvStart, NULL);
				tvEnd = tvStart;
				tvEnd.tv_sec += milliSeconds / 1000;
				tvEnd.tv_usec += (milliSeconds % 1000) * 1000;
				tvEnd.tv_sec += tvEnd.tv_usec / (1000 * 1000);
				tvEnd.tv_usec = tvEnd.tv_usec % (1000 * 1000);

				TIMEVAL_TO_TIMESPEC(&tvEnd, &ts);
				int waitRet = 0;
				while ((waitRet = sem_timedwait(&sem_m, &ts)) != 0 && errno == EINTR);
				if (waitRet == 0)
				{
					//logUtil_g.writeLog(LOG_TYPE_DEBUG, "sem_timedwait  ======== SEM_WAIT_SIGNALED!");
					return SEM_WAIT_SIGNALED;
				}

				if (errno == ETIMEDOUT)
				{
					//logUtil_g.writeLog(LOG_TYPE_DEBUG, "sem_timedwait  ======== SEM_WAIT_TIMEOUT!");
					return SEM_WAIT_TIMEOUT;
				}

				return INVALID_VALUE;
#else
				if (milliSeconds <= 0)
				{
					if (this->tryWait())
					{
						return SEM_WAIT_SIGNALED;
					}
					else
					{
						return SEM_WAIT_TIMEOUT;
					}
				}

				uint32 curTime = time(NULL);
				uint32 expireTime = curTime + milliSeconds;
				while (true)
				{
					if (this->tryWait())
					{
						return SEM_WAIT_SIGNALED;
					}

					usleep(10000);
					if (this->tryWait())
					{
						return SEM_WAIT_SIGNALED;
					}

					curTime = time(NULL);
					if (curTime < expireTime)
					{
						break;
					}
				}

				return SEM_WAIT_TIMEOUT;
#endif
#endif
			}

			void post(int count = 1)
			{
				if (count <= 0)
				{
					count = 1;
				}

#if _PF_WINDOWS_
				::ReleaseSemaphore(sem_m, count, NULL);
#else
				for (int i = 0; i < count; i++)
				{
#if _PF_LINUX_
					sem_post(&sem_m);
#else
					sem_post(sem_m);
#endif
				}

				pthread_testcancel();
#endif
			}


		private:

		#if _PF_WINDOWS_
			HANDLE sem_m;

#elif _PF_LINUX_
			sem_t sem_m;
		#else
			sem_t *sem_m;
		#endif

		};


	} // namespace utility
} // namespace evl

#endif // __EVL_UTILITY_SEMAPHORE_H__

#ifndef __EVL_THIRDPARTY_REF_PTR_H__
#define __EVL_THIRDPARTY_REF_PTR_H__

/************************************************************************/
/* 引用计数器                                                           */
/************************************************************************/

namespace evl
{
	namespace utility
	{
		class RefPtr
		{
		public:
			// 引用析构器
			typedef void(*RefPtrFreeFunc)(void *ptr);

		public:

			void *_data;
			int _refCount;
			RefPtrFreeFunc _freeFunc;

		public:
			RefPtr(void *ptr, RefPtrFreeFunc freefun)
			{
				_data = ptr;
				_refCount = 1;
				_freeFunc = freefun;
			}

			~RefPtr()
			{
				if (_data) {
					_freeFunc(_data);
				}
			}

			void *operator->()
			{
				return _data;
			}

			void release()
			{
				_refCount--;
				if (_refCount <= 0)
				{
					delete this;
				}
			}

			void retain()
			{
				_refCount++;
			}

			void *getData()
			{
				return _data;
			}
		}; // class RefPtr
	} // namespace utility
} // namespace evl

#endif // __EVL_THIRDPARTY_REF_PTR_H__

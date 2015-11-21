#include <../include/mpool/mpool.h>
#include <stdlib.h>

namespace evl
{
	namespace utility
	{
		namespace mpool
		{
			using namespace detail;

			/**********************************************************************************/
			/**
			* 初始化 Alloctor， 使用系统内置的 malloc/free
			*/
			static void* malloc_salloc(struct AlloctorInfo* sa, size_t size)
			{
				return malloc(size);
			}

			static void malloc_sfree(struct AlloctorInfo* sa, void* block, size_t size)
			{
				free(block);
			}

			static void* malloc_srealloc(struct AlloctorInfo* sa, void* block, size_t old_size, size_t new_size)
			{
				return realloc(block, new_size);
			}
			/**********************************************************************************/
		}
	}
}

// 构建一个 全局Alloctor实例，用以封装基本分配、释放、重分配机制
evl::utility::mpool::detail::AlloctorInfo sg_alloctor_info =
{
	&evl::utility::mpool::malloc_salloc,
	&evl::utility::mpool::malloc_srealloc,
	&evl::utility::mpool::malloc_sfree
};
#ifndef EVL_UTILITY_MPOOL_COMMON_H_
#define EVL_UTILITY_MPOOL_COMMON_H_

#include <assert.h>
#include <third_party\utility\include\mpool\mpool.h>

namespace evl
{
	namespace utility
	{
		namespace mpool
		{
			using namespace detail;

			static void* default_realloc(struct AlloctorInfo* sa, void* ptr, size_t old_size, size_t new_size)
			{
				assert(old_size > 0);
				assert(new_size > 0);

				void* q = sa->alloc(sa, new_size);
				if (NULL == q)
					return NULL;

				memcpy(q, ptr, old_size < new_size ? old_size : new_size);
				sa->free(sa, ptr, old_size);
				return q;
			}

			// ≥ı ºªØ mpool_cell 
			static void chunk_init(struct MPoolCell* cell, size_t cell_count, size_t cell_size)
			{
				struct MPoolCell* p = cell;
				for (size_t i = 0; i < cell_count-1; ++i)
				{
					p = p->next = (struct MPoolCell*)((char*)p + cell_size);
				}

				p->next = NULL;
			}
		}
	}
}

#endif // EVL_UTILITY_MPOOL_COMMON_H_
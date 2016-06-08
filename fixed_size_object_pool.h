#ifndef EVL_FIX_OBJ_POOL_H_
#define EVL_FIX_OBJ_POOL_H_

#include <cstdio>
#include "utility_def.h"

namespace evl
{
	namespace utility
	{
		class FixedSizeObjectPool
		{
#ifdef DEBUG_FIXED_SIZE_OBJECT_POOL
#define LOG_FIXED_SIZE_OBJECT_POOL(MSG) \
			printf("[Debuging fixed size object pool] : "); \
			printf(MSG); \
			printf('\n');
#else 
#define LOG_FIXED_SIZE_OBJECT_POOL(MSG) 
#endif 
			enum
			{
				DEFAULT_OBJ_COUNTS = 128,
				INVALID_IDX = -1,
			};

			// block infos
			struct BlockInfoType{
				unsigned int valid_head; // the head of valid memory blocks can be alloced for 
				unsigned int valid_tail; // the tail( the last one) of valid memory blocks can be alloced for
				unsigned int valid_frees;
				size_t obj_size;
				unsigned int obj_count;

				union{
					mem_align_min_t m_objs[1]; //beginning for objs to be allocked for
					unsigned int next_valid_idx; // index of next valid memory block
				};
			};

		public:
			FixedSizeObjectPool()
				: block_info_(NULL)
			{

			}

			virtual ~FixedSizeObjectPool()
			{
				if (block_info_ != NULL)
				{
					free(block_info_);
					block_info_ = NULL;
				}
			}

			bool Init(size_t obj_size, size_t max_obj_count = DEFAULT_OBJ_COUNTS)
			{
				size_t poolSize = sizeof(struct block_t) + obj_size * DEFAULT_OBJ_COUNTS - sizeof(unsigned int);
				block_info_ = (struct BlockInfoType*)malloc(poolSize);
				
				if (block_info_ == NULL)
				{
					assert(block_info_ != NULL && "cannot alloc memory in calling function block_create()");
					return false;
				}

				memset(block_info_, 0, poolSize);

				_init_block_(block_info_, obj_size, max_obj_count);

				return true;
			}

			void* Alloc()
			{
				void *valid_found = NULL;

				if (block_info_ == NULL)
				{
					LOG_FIXED_SIZE_OBJECT_POOL("not initialized.");
					return NULL;
				}

				if (block->valid_head == INVALID_IDX)
				{
					LOG_FIXED_SIZE_OBJECT_POOL("no more memory.");
					return NULL;
				}

				int found = block_info_->valid_head;
				block_info_->valid_head = *(unsigned int*)block_get(block_info_, block_info_->valid_head);
				if (block_info_->valid_head == INVALID_IDX)
					block_info_->valid_tail = INVALID_IDX;

				valid_found = block_get(INVALID_IDX, found);
				*(unsigned int*)valid_found = INVALID_IDX;

				return valid_found;
			}

			void* block_get(BlockInfoType *block, unsigned int idx);
			int block_get_idx(BlockInfoType *block, void *obj);

			
			int block_dealloc(BlockInfoType *block, void *obj);

			void block_dump(BlockInfoType *block);


		private:
			void _init_block_(BlockInfoType *block, size_t obj_size, size_t max_obj_count)
			{
				block->valid_head = 0;
				block->valid_tail = 0;
				block->obj_size = obj_size;
				block->obj_count = max_obj_count;

				//define all the ready objects' next valid index
				// this a sequenced list index a chain
				for (unsigned int idx = 0; idx < block->obj_count - 1; ++idx)
				{
					block->m_objs[block->obj_size * idx] = (char)(idx + 1);
				}

				block->m_objs[block->obj_size * (block->obj_count - 1)] = INVALID_IDX;
			}

		private:
			BlockInfoType* block_info_;
		};
	}
}

#endif

#ifndef EVL_UTILITY_MPOOL_H_
#define EVL_UTILITY_MPOOL_H_

namespace evl
{
	namespace utility
	{
		namespace mpool
		{
			namespace detail
			{
				// 前置声明：分配管理器信息
				struct AlloctorInfo;

				// 分配、重分配、释放接口类型定义
				typedef void* (*AllocFunctionType)(struct AlloctorInfo* self, size_t size);
				typedef void* (*ReallocFunctionType)(struct AlloctorInfo* self, void* block, size_t old_size, size_t new_size);
				typedef void(*FreeFunctionType)(struct AlloctorInfo* self, void* block, size_t size);

				struct AlloctorInfo
				{
					AllocFunctionType alloc;
					ReallocFunctionType realloc;
					FreeFunctionType free;
				};

				struct mpool_chunk;
				struct MPoolCell;
			}

			struct FixedMPoolInfo
			{
				struct detail::AlloctorInfo* alloctor_info;
				struct detail::mpool_chunk* pChunks;
				struct detail::MPoolCell*  head;
				size_t iNextChunk;
				size_t nChunks;
				size_t cell_size;
				size_t chunk_size;
				size_t used_cells;
			};

			namespace detail
			{
				struct sfixed_mpool
				{
					//------------------------------------------------------------------------------------------
					/// export a allocator interface
					AllocFunctionType alloc;
					ReallocFunctionType realloc;
					FreeFunctionType free;
					//------------------------------------------------------------------------------------------
					struct FixedMPoolInfo fmp;
				};

				// 是否允许 mpool 分配超出 max_cell_size 的内存块
				//#define EVL_UTILITY_MPOOL_ALLOW_BIG_BLOCK
#ifdef EVL_UTILITY_MPOOL_ALLOW_BIG_BLOCK
				struct big_block_header
				{
					struct big_block_header *next, *prev;
				};
#endif // EVL_UTILITY_MPOOL_ALLOW_BIG_BLOCK

				enum
				{
					/* must be power of 2 and greater than sizeof(void*) */
					MPOOL_MIN_CELL = 8,
					MPOOL_MIN_CHUNK = 256,
				};

				struct MPoolCell
				{
					struct MPoolCell* next;
				};

				struct mpool_chunk
				{
					struct MPoolCell* cell; // cell array
					size_t size; // size in bytes;
				};


				/***********************************************************************/
				/**
				* sfixed_mpool_{salloc|sfree} should only called by sallocator interface
				* sfixed_mpool_srealloc is an assert only hook.
				*/
				void sfixed_mpool_init(struct sfixed_mpool* mp);
				void sfixed_mpool_destroy(struct sfixed_mpool* mp);
				/***********************************************************************/
			}

			struct MPoolInfo
			{
				//------------------------------------------------------------------------------------------
				/// export a sallocator interface
				detail::AllocFunctionType alloc;
				detail::ReallocFunctionType realloc;
				detail::FreeFunctionType free;
				//------------------------------------------------------------------------------------------

				/// sallocator for this mpool self
				struct detail::AlloctorInfo* sa;

				struct FixedMPoolInfo* fixed;
				size_t max_cell_size;
				size_t chunk_size;
#ifdef EVL_UTILITY_MPOOL_ALLOW_BIG_BLOCK
				size_t big_blocks; // size > max_cell_size, use malloc, this is rare case
				struct big_block_header big_list;
#endif // EVL_UTILITY_MPOOL_ALLOW_BIG_BLOCK
			};

			/***********************************************************************/
			void fixed_mpool_init(struct FixedMPoolInfo* mpf);
			void fixed_mpool_destroy(struct FixedMPoolInfo* mpf);

			void* fixed_mpool_alloc(struct FixedMPoolInfo* mpf);
			void  fixed_mpool_free(struct FixedMPoolInfo* mpf, void* ptr);
			/***********************************************************************/

			/***********************************************************************/
			/**
			 * mpool_{salloc|sfree} may called by function name, or by interface
			 */
			void mpool_init(struct MPoolInfo* mp);
			void mpool_destroy(struct MPoolInfo* mp);

			void* mpool_alloc(struct MPoolInfo* mp, size_t size);
			void  mpool_free(struct MPoolInfo* mp, void* ptr, size_t size);

			size_t mpool_used_cells(const struct MPoolInfo* mp);
			size_t mpool_used_bytes(const struct MPoolInfo* mp);
			/***********************************************************************/


			/***********************************************************************/
			struct MPoolInfo* mpool_get_global(void);

			void* gsalloc(size_t size);
			void  gsfree(void* ptr, size_t size);
			/***********************************************************************/


		}
	}
}

#endif // EVL_UTILITY_MPOOL_H_
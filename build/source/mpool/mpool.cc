#include <third_party\utility\include\mpool\mpool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"

// 在 mpool_alloctor.cc 定义
extern evl::utility::mpool::detail::AlloctorInfo sg_alloctor_info;

namespace evl
{
	namespace utility
	{
		namespace mpool
		{
			using namespace detail;
			/**********************************************************************************/
			void* sfixed_mpool_salloc(struct sfixed_mpool* fmp, size_t size);
			void* sfixed_mpool_srealloc(struct sfixed_mpool* fmp, size_t old_size, size_t new_size);
			void  sfixed_mpool_sfree(struct sfixed_mpool* fmp, void* ptr, size_t size);

			/**
			* require initialized fields:
			*	 cell_size
			*	 size
			*	 sa [0 OR initialized]
			*/
			void fixed_mpool_init(struct FixedMPoolInfo* fmp)
			{
				assert(fmp != NULL);
				assert(fmp->chunk_size > 0);
				assert(fmp->cell_size > 0);
				assert(fmp->cell_size < fmp->chunk_size);

				struct AlloctorInfo* al;

				// 设置分配器
				if (NULL == fmp->alloctor_info)
				{
					// 使用默认分配器
					al = fmp->alloctor_info = &sg_alloctor_info;
				}
				else
				{
					al = fmp->alloctor_info;
					assert(NULL != al->alloc);
					assert(NULL != al->free);
					if (NULL == al->realloc)
						al->realloc = &default_realloc;
				}

				fmp->cell_size = (fmp->cell_size + MPOOL_MIN_CELL - 1) / MPOOL_MIN_CELL * MPOOL_MIN_CELL;
				fmp->chunk_size = (fmp->chunk_size + MPOOL_MIN_CHUNK - 1) / MPOOL_MIN_CHUNK * MPOOL_MIN_CHUNK;

				if (fmp->nChunks < MPOOL_MIN_CHUNK/sizeof(struct mpool_chunk))
					fmp->nChunks = MPOOL_MIN_CHUNK/sizeof(struct mpool_chunk);

				fmp->iNextChunk = 0;
				fmp->pChunks = (struct mpool_chunk*)
					al->alloc(al, sizeof(struct mpool_chunk) * fmp->nChunks);
				if (NULL == fmp->pChunks)
				{
					fprintf(stderr
						, "fatal: febird.fixed_mpool_init failed, chunk[size=%zd, count=%zd]\n"
						, fmp->chunk_size, fmp->nChunks);
					abort();
				}

				fmp->head = NULL;
				fmp->used_cells = 0;
			}

			void sfixed_mpool_init(struct sfixed_mpool* sfmp)
			{
				fixed_mpool_init(&sfmp->fmp);
				sfmp->alloc   = (AllocFunctionType)&sfixed_mpool_salloc;
				sfmp->free    = (FreeFunctionType)&sfixed_mpool_sfree;
				sfmp->realloc = (ReallocFunctionType)&sfixed_mpool_srealloc;
			}

			void fixed_mpool_destroy(struct FixedMPoolInfo* fmp)
			{
				struct AlloctorInfo* sa = fmp->alloctor_info;
				long i;
				for (i = fmp->iNextChunk - 1; i >= 0; --i)
					sa->free(sa, fmp->pChunks[i].cell, fmp->pChunks[i].size);

				sa->free(sa, fmp->pChunks, sizeof(struct mpool_chunk) * fmp->nChunks);
			}

			void sfixed_mpool_destroy(struct sfixed_mpool* sfmp)
			{
				fixed_mpool_destroy(&sfmp->fmp);
			}

			struct MPoolCell* fixed_mpool_add_chunk(struct FixedMPoolInfo* fmp)
			{
				struct MPoolCell* cell;

				assert(fmp->pChunks != NULL);

				if (fmp->iNextChunk == fmp->nChunks)
				{
					size_t old_size = sizeof(struct mpool_chunk) * fmp->nChunks;
					size_t new_size = 2 * old_size;

					// allocate new chunk array
					struct mpool_chunk* c = (struct mpool_chunk*)
						fmp->alloctor_info->realloc(fmp->alloctor_info, fmp->pChunks, old_size, new_size);

					if (NULL == c) return NULL;
					fmp->pChunks = c;
					fmp->nChunks *= 2;     // chunk array expanded 2
					fmp->chunk_size *= 2;  // chunk_size  expanded 2 also
				}

				// allocate a new cell array
				cell = (struct MPoolCell*)fmp->alloctor_info->alloc(fmp->alloctor_info, fmp->chunk_size);
				if (NULL == cell) return NULL;
				fmp->pChunks[fmp->iNextChunk].cell = cell;
				fmp->pChunks[fmp->iNextChunk].size = fmp->chunk_size;
				fmp->iNextChunk++;
				chunk_init(cell, fmp->chunk_size / fmp->cell_size, fmp->cell_size);

				/* alloc cell */
				fmp->used_cells++;
				fmp->head = cell->next;

				return cell;
			}

			/***************************************************************/

#define FIXED_MPOOL_IMPL_ALLOC(fmp) 	\
{										\
	struct MPoolCell* cell = fmp->head;\
	if (cell) {							\
		fmp->used_cells++;				\
		fmp->head = cell->next;			\
		return cell;					\
	}									\
	return fixed_mpool_add_chunk(fmp);	\
}
			/***************************************************************/

#define FIXED_MPOOL_IMPL_FREE(fmp, ptr)	\
{										\
	struct MPoolCell* cell = (struct MPoolCell*)ptr; \
	cell->next = fmp->head;	\
	fmp->used_cells--;		\
	fmp->head = cell;		\
}

			/***************************************************************/

			void* fixed_mpool_alloc(struct FixedMPoolInfo* fmp)
				FIXED_MPOOL_IMPL_ALLOC(fmp)

			void* sfixed_mpool_salloc(struct sfixed_mpool* sfmp, size_t size)
			{
				if (size != sfmp->fmp.cell_size)
				{
					fprintf(stderr, "fatal: sfixed_mpool_salloc:[cell_size=%zd, request_size=%zd]\n", sfmp->fmp.cell_size, size);
					abort();
				}
				FIXED_MPOOL_IMPL_ALLOC((&sfmp->fmp))
			}

			void fixed_mpool_free(struct FixedMPoolInfo* fmp, void* ptr)
				FIXED_MPOOL_IMPL_FREE(fmp, ptr)

			void sfixed_mpool_sfree(struct sfixed_mpool* sfmp, void* ptr, size_t size)
			{
				if (size != sfmp->fmp.cell_size)
				{
					fprintf(stderr, "fatal: sfixed_mpool_salloc:[cell_size=%zd, request_size=%zd]\n", sfmp->fmp.cell_size, size);
					abort();
				}
				FIXED_MPOOL_IMPL_FREE((&sfmp->fmp), ptr)
			}

			static void* sfixed_mpool_srealloc(struct sfixed_mpool* fmp, size_t old_size, size_t new_size)
			{
				fprintf(stderr, "fatal: sfixed_mpool_srealloc: this function shuold not be called\n");
				abort();
				return NULL; // avoid warning
			}

			/**********************************************************************************/
			/**
			* require initialized fields:
			*   max_cell_size
			*	 size
			*	 sa [0 OR initialized]
			*/
			void mpool_init(struct MPoolInfo* mp)
			{
				int i, nFixed;
				struct AlloctorInfo* al;

				assert(mp->max_cell_size < mp->chunk_size);

				if (NULL == mp->alloc)
					al = mp->sa = &sg_alloctor_info;
				else
				{
					al = mp->sa;
					assert(NULL != al->alloc);
					assert(NULL != al->free);
					if (NULL == al->realloc)
						al->realloc = &default_realloc;
				}
				mp->alloc = (AllocFunctionType)&mpool_alloc;
				mp->free = (FreeFunctionType)&mpool_free;
				mp->realloc = (ReallocFunctionType)&default_realloc;

				mp->max_cell_size = (mp->max_cell_size + MPOOL_MIN_CELL - 1) / MPOOL_MIN_CELL * MPOOL_MIN_CELL;
				mp->chunk_size = (mp->chunk_size + MPOOL_MIN_CHUNK - 1) / MPOOL_MIN_CHUNK * MPOOL_MIN_CHUNK;
				nFixed = mp->max_cell_size / MPOOL_MIN_CELL;

				mp->fixed = (struct FixedMPoolInfo*)al->alloc(al, sizeof(struct FixedMPoolInfo) * nFixed);
				if (NULL == mp->fixed)
				{
					fprintf(stderr, "fatal: febird.mpool_init[max_cell_size=%zd, size=%zd]\n"
						, mp->max_cell_size, mp->chunk_size);
					abort();
				}

				for (i = 0; i < nFixed; ++i)
				{
					mp->fixed[i].cell_size = (i + 1) * MPOOL_MIN_CELL;
					mp->fixed[i].chunk_size = mp->chunk_size;
					mp->fixed[i].nChunks = 16;
					mp->fixed[i].alloctor_info = mp->sa;
					fixed_mpool_init(&mp->fixed[i]);
				}
#ifdef FEBIRD_MPOOL_ALLOW_BIG_BLOCK
				mp->big_blocks = 0;
				mp->big_list.prev = mp->big_list.next = &mp->big_list;
#endif
			}

			void mpool_destroy(struct MPoolInfo* mp)
			{
				size_t i, nFixed;
#ifdef FEBIRD_MPOOL_ALLOW_BIG_BLOCK
				struct big_block_header *p;
				for (i = 0, p = mp->big_list.next; p != &mp->big_list; ++i)
				{
					struct big_block_header *q;
					if (i > mp->big_blocks)
					{
						fprintf(stderr, "fatal: febird.mpool_destroy\n");
						abort();
					}
					q = p->next;
					free(p);
					p = q;
				}
				assert(i == mp->big_blocks);
#endif
				nFixed = mp->max_cell_size / MPOOL_MIN_CELL;
				for (i = 0; i < nFixed; ++i)
				{
					fixed_mpool_destroy(&mp->fixed[i]);
				}
				mp->sa->free(mp->sa, mp->fixed, sizeof(struct FixedMPoolInfo) * nFixed);
			}

			void* mpool_alloc(struct MPoolInfo* mp, size_t size)
			{
				size_t idx;
				struct FixedMPoolInfo* fmp;

#ifdef FEBIRD_MPOOL_ALLOW_BIG_BLOCK
				if (size > mp->max_cell_size)
				{
					// this is rare case
					struct big_block_header *p, *h;
					p = (struct big_block_header*)malloc(sizeof(struct big_block_header) + size);
					if (p)
					{
						h = &mp->big_list;
						p->prev = h;
						p->next = h->next;
						h->next->prev = p;
						h->next = p;
						mp->big_blocks++;
						return (p + 1);
					}
					else
						return NULL;
				}
#else
				if (size > mp->max_cell_size)
				{
					fprintf(stderr, "fatal: mpool_salloc: [size=%zd, max_cell_size=%zd]\n", size, mp->max_cell_size);
					abort();
				}
#endif
				assert(size > 0);

				idx = (size - 1) / MPOOL_MIN_CELL;
				fmp = &mp->fixed[idx];

				FIXED_MPOOL_IMPL_ALLOC(fmp)
			}

			void mpool_free(struct MPoolInfo* mp, void* ptr, size_t size)
			{
				size_t idx;
				struct FixedMPoolInfo* fmp;

#ifdef FEBIRD_MPOOL_ALLOW_BIG_BLOCK
				if (size > mp->max_cell_size)
				{
					// this is rare case
					struct big_block_header* bbh = (struct big_block_header*)ptr - 1;
					bbh->prev->next = bbh->next;
					bbh->next->prev = bbh->prev;
					free(bbh);
					mp->big_blocks--;
					return;
				}
#else
				assert(size <= mp->max_cell_size);
#endif
				assert(size > 0);

				idx = (size - 1) / MPOOL_MIN_CELL;
				fmp = &mp->fixed[idx];

				FIXED_MPOOL_IMPL_FREE(fmp, ptr)
			}

			size_t mpool_used_cells(const struct MPoolInfo* mp)
			{
				size_t i, n = mp->max_cell_size / MPOOL_MIN_CELL;
				size_t used = 0;
				for (i = 0; i < n; ++i)
					used += mp->fixed[i].used_cells;
				return used;
			}

			size_t mpool_used_bytes(const struct MPoolInfo* mp)
			{
				size_t i, n = mp->max_cell_size / MPOOL_MIN_CELL;
				size_t used = 0;
				for (i = 0; i < n; ++i)
					used += mp->fixed[i].cell_size * mp->fixed[i].used_cells;
				return used;
			}

			static struct MPoolInfo sg_mpool = { 0 };
			static void destroy_global_mpool(void)
			{
				if (sg_mpool.sa)
					mpool_destroy(&sg_mpool);
			}

			struct MPoolInfo* mpool_get_global(void)
			{
				if (NULL == sg_mpool.sa)
				{
					sg_mpool.chunk_size = 4096;
					sg_mpool.max_cell_size = 256;
					sg_mpool.sa = &sg_alloctor_info;
					mpool_init(&sg_mpool);
					atexit(&destroy_global_mpool);
				}
				return &sg_mpool;
			}

			void* gsalloc(size_t size)
			{
				return mpool_alloc(&sg_mpool, size);
			}
			void  gsfree(void* ptr, size_t size)
			{
				mpool_free(&sg_mpool, ptr, size);
			}
		}
	}
}

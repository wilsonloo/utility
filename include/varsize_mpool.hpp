#ifndef EVL_UTILITY_VARSIZE_MPOOL_H_
#define EVL_UTILITY_VARSIZE_MPOOL_H_

/*
	usage:
		turn on EVL_UTILITY_DEBUG_VARSIZE_MPOOL can debug invalid freeing operation
*/

#include <iostream>
#include <vector>
#include <boost/pool/pool.hpp>
#include <boost/assert.hpp>

#include "setup.h"
#include "mpool/mpool.h"

namespace evl
{
	namespace utility
	{
		class VarsizeMPool
		{
			enum
			{
				// min cell size and the same increase size of per level
				MIN_CELL_SIZE = 8, 
				MAX_TRUNCK_SIZE = 256,
			};

		public:
			VarsizeMPool()
				: min_cell_size_(MIN_CELL_SIZE)
				, max_cell_size_(MIN_CELL_SIZE)
			{

			}

			virtual ~VarsizeMPool()
			{
				for (ChunkVectorType::iterator it = chunks_.begin();
					it != chunks_.end(); ++it)
				{
					if (*it != NULL)
					{
						delete *it;
					}
				}
			
				chunks_.clear();
			}

			virtual bool Init(boost::uint32_t max_cell_size, boost::uint32_t min_cell_size = MIN_CELL_SIZE)
			{
				if (min_cell_size > max_cell_size)
				{
					std::cerr << "min_cell_size larger than max_cell_size." << std::endl;
					BOOST_ASSERT(false);
					return false;
				}

				// upgrade to suitable cell_size
				min_cell_size_ = (min_cell_size + MIN_CELL_SIZE - 1) / MIN_CELL_SIZE * MIN_CELL_SIZE;
				max_cell_size_ = (max_cell_size + MIN_CELL_SIZE - 1) / MIN_CELL_SIZE * MIN_CELL_SIZE;

				boost::uint32_t chunk_count = max_cell_size_ / min_cell_size_ + 1;
				InitChunks(chunk_count);

				return true;
			}

			virtual void* Alloc(boost::uint32_t cell_size)
			{
				if (cell_size > max_cell_size_)
				{
					std::cerr << "allocing too large size than limit." << std::endl;
					BOOST_ASSERT(false);
					return NULL;
				}

				boost::uint32_t fixed_cell_size = cell_size < min_cell_size_ ? min_cell_size_ : cell_size;
				boost::uint32_t chunk_idx = (fixed_cell_size + min_cell_size_ - 1) / min_cell_size_ - 1;
				
				return chunks_.at(chunk_idx)->malloc();
			}

			virtual void Free(void* cell, boost::uint32_t cell_size)
			{
				if (cell_size > max_cell_size_)
				{
					std::cerr << "allocing too large size than limit." << std::endl;
					BOOST_ASSERT(false);
					return;
				}

				boost::uint32_t fixed_cell_size = cell_size < min_cell_size_ ? min_cell_size_ : cell_size;
				boost::uint32_t chunk_idx = (fixed_cell_size + min_cell_size_ - 1) / min_cell_size_ - 1;

#ifdef EVL_UTILITY_DEBUG_VARSIZE_MPOOL

				if(!chunks_.at(chunk_idx)->is_from(cell))
				{
					std::cerr << "freeing cell not matching its size." << std::endl;
					BOOST_ASSERT(false);
					return;
				}

#endif // EVL_UTILITY_DEBUG_VARSIZE_MPOOL

				chunks_.at(chunk_idx)->free(cell);
			}

		private:
			void InitChunks(boost::uint32_t chunk_count)
			{
				for (boost::uint32_t chunk_idx = 0; chunk_idx < chunk_count; ++chunk_idx)
				{
					boost::uint32_t chunk_size = min_cell_size_ * (chunk_idx + 1);
					chunks_.push_back(new boost::pool<>(chunk_size));
				}
			}

		private:
			boost::uint32_t min_cell_size_;
			boost::uint32_t max_cell_size_;
			typedef std::vector<boost::pool<>* > ChunkVectorType;
			ChunkVectorType	chunks_;
		};
	}
}

#endif // EVL_UTILITY_VARSIZE_MPOOL_H_
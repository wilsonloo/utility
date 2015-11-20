#ifndef ENGINE_UTILITY_READ_WRITE_ARRAY_HPP_
#define ENGINE_UTILITY_READ_WRITE_ARRAY_HPP_

//
// 读写字符数组
// 特性：	1.(尾部不断裂)
//			2.write_pos == read_pos == 0: 数组为空
//			3.write_pos == 0xFFFFffff	: 数组满
//			4.read_pos < write_pos		: 有效数据范围为 [read_pos, write_pos)
//			5.write_pos < read_pos		: 有效数据范围为 [read_pos, 尾部]， [头部, write_pos) 
// 
namespace evl
{
	namespace utility
	{
		template<size_t MAX_BUFF_SIZE>
		class ReadWriteArray
		{
			enum
			{
				// 非法位置
				INVALID_WRITE_POS = 0xFFFFffff,
			};
		public:
			ReadWriteArray()
				: read_pos(0)
				, write_pos(0)
				, un_used_pos(INVALID_WRITE_POS)
				, locked_block_for_write_(NULL)
			{
				memset(buffer_, 0, sizeof(buffer_));
			}

			virtual ~ReadWriteArray()
			{

			}

			void* GetAndLockWritableBlock(size_t data_length)
			{
				if(locked_block_for_write_ != NULL)
				{
					BOOST_ASSERT(false && "already lock for writting!");
					return NULL;
				}

				//
				// 获取可写数据位置
				size_t can_write_pos = INVALID_WRITE_POS;
				if(!get_free_space_for(data_length, can_write_pos))
					return NULL;

				locked_block_for_write_ = &buffer_[0] + can_write_pos;
				return locked_block_for_write_;
			}

			void ReleaseWritableBlock(void* block)
			{
				BOOST_ASSERT(locked_block_for_write_ == block);
				if(locked_block_for_write_ == NULL)
				{
					locked_block_for_write_ = NULL;
				}
			}
			
			void SubmitWritableBlock(void* block, size_t block_size)
			{
				if(locked_block_for_write_ == NULL)
				{
					BOOST_ASSERT(false && "no writable block locked!");
					return;
				}

				if(block != locked_block_for_write_)
				{
					BOOST_ASSERT(false && "submit unknown write block!");
					return;
				}

				SeekNextWritablePos(block_size);
				locked_block_for_write_ = NULL;
			}

			void SeekNextWritablePos(size_t data_length)
			{
				//
				// 获取可写数据位置
				size_t can_write_pos = INVALID_WRITE_POS;
				if(!get_free_space_for(data_length, can_write_pos))
				{
					// 不可能来到这里，同一函数get_free_space_for() 保证相同可写位置
					BOOST_ASSERT(false);
					return;
				}

				if(can_write_pos != write_pos)
				{
					// 不是原本可写数据位置，物理尾部有空余空间
					un_used_pos = write_pos;
				}
				else
				{
					un_used_pos = INVALID_WRITE_POS;
				}

				// 
				// 移动下一个可写位置
				size_t next_write_pos = can_write_pos + data_length;
				if(next_write_pos == read_pos)
				{
					// 数组已满
					write_pos = INVALID_WRITE_POS;
				}
				else if(next_write_pos > read_pos)
				{
					if(next_write_pos == sizeof(buffer_))
					{
						// 尾部没有空间, 可能头部有空间
						if(read_pos > 0)
						{
							// 头部有空闲空间
							write_pos = 0;
						}
						else
						{
							write_pos = INVALID_WRITE_POS;
						}
					}
					else if(next_write_pos > sizeof(buffer_))
					{
						// 不会出现这种情况， 因为要确保物理最后的数据块，不会‘断裂’
						BOOST_ASSERT(false);
						write_pos = INVALID_WRITE_POS;
					}
					else // write_pos < sizeof(buffer_)
					{
						// 尾部还有剩余可写空间
						write_pos = next_write_pos;
					}
				}
				else // next_write_pos < read_pos
				{
					write_pos = next_write_pos;
				}
			}

			bool AddData(const void* data, size_t data_length)
			{
				if(locked_block_for_write_ != NULL)
				{
					BOOST_ASSERT(false && "already lock for writting!");
					return NULL;
				}

				//
				// 获取可写数据位置
				size_t can_write_pos = INVALID_WRITE_POS;
				if(!get_free_space_for(data_length, can_write_pos))
					return false;
				
				locked_block_for_write_ = &buffer_[0] + can_write_pos;

				//
				// 拷贝数据 
				memcpy(locked_block_for_write_, data, data_length);				
				SeekNextWritablePos(data_length);

				locked_block_for_write_ = NULL;
				return true;
			}

			void RemoveData(size_t read_done_data_length)
			{
				// 检测是否能够移除指定长度的数据
				{
					if(is_empty())
					{
						BOOST_ASSERT(false);
						return;
					}

					if(write_pos > read_pos && read_done_data_length > (write_pos - read_pos))
					{
						BOOST_ASSERT(false);
						return;
					}

					if(write_pos < read_pos && read_done_data_length > (sizeof(buffer_) - read_pos))
					{
						BOOST_ASSERT(false);
						return;
					}
				}

				size_t next_read_pos = read_pos + read_done_data_length;

				if(next_read_pos == un_used_pos || next_read_pos == sizeof(buffer_))
				{
					// 该物理（非逻辑）尾部位置不可读，没有有效数据
					// 尝试读取头部位置
					next_read_pos = 0;
				}

				if(next_read_pos == read_pos)
				{
					// 当前只有一个数据块， 并且刚刚被读完
					// 数组变成空
					read_pos = 0;
					write_pos = 0;
					un_used_pos = INVALID_WRITE_POS;
					return;
				}

				if(next_read_pos == write_pos)
				{
					// 当前只有一个数据块， 并且刚刚被读完
					// 数组变成空
					read_pos = 0;
					write_pos = 0;
					un_used_pos = INVALID_WRITE_POS;
					return;
				}

				if(next_read_pos > read_pos)
				{
					// 继续尾部数据
					if(write_pos == INVALID_WRITE_POS)
					{
						write_pos = read_pos;
					}

					read_pos = next_read_pos;
				}
				else // next_read_pos < read_pos
				{
					// 继续头部数据
					if(write_pos == INVALID_WRITE_POS)
					{
						write_pos = read_pos;
					}

					read_pos = next_read_pos;
				}
			}

			bool get_read_data(/*OUT*/const char*& reading_data, size_t& reading_data_length)
			{
				if(is_empty())
					return false;

				reading_data = (&buffer_[0]) + read_pos;
				if(read_pos < write_pos)
				{
					reading_data_length = write_pos - read_pos;
					return true;
				}
				else if(write_pos < read_pos)
				{
					if(un_used_pos == INVALID_WRITE_POS)
					{
						reading_data_length = sizeof(buffer_) - read_pos;
						return true;
					}
					else
					{
						reading_data_length = un_used_pos - read_pos;
						return true;
					}
				}
				
				return false;
			}

			inline bool is_full(){ return write_pos == INVALID_WRITE_POS; }
			inline bool is_empty(){ return write_pos == read_pos && read_pos == 0; }
			
		private:
			bool get_free_space_for(size_t expecting_space, /*OUT*/size_t& valid_write_pos)
			{
				if(is_full())
					return false;

				if(is_empty())
				{
					if(sizeof(buffer_) >= expecting_space)
					{
						valid_write_pos = 0;
						return true;
					}

					// 及时空数组都不能容纳新数据
					return false;
				}

				if(read_pos < write_pos)
				{
					if(sizeof(buffer_) - write_pos >= expecting_space)
					{
						// 尾部有足够空闲位置
						valid_write_pos = write_pos;
						return true;
					}
				}

				if(write_pos < read_pos)
				{
					if(read_pos >= expecting_space)
					{
						valid_write_pos = write_pos;
						return true;
					}
				}

				return false;
			}

		private:
			// 读写位置
			size_t read_pos;
			size_t write_pos;
			// 物理（非逻辑）尾部未被使用的开始位置
			size_t un_used_pos;
			// 锁定的写区
			void* locked_block_for_write_;

			// 数据
			char buffer_[MAX_BUFF_SIZE];
		};
	} // namespace utility
} // namespace evl

#endif // ENGINE_UTILITY_READ_WRITE_ARRAY_HPP_
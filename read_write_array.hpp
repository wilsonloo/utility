#ifndef ENGINE_UTILITY_READ_WRITE_ARRAY_HPP_
#define ENGINE_UTILITY_READ_WRITE_ARRAY_HPP_

//
// ��д�ַ�����
// ���ԣ�	1.(β��������)
//			2.write_pos == read_pos == 0: ����Ϊ��
//			3.write_pos == 0xFFFFffff	: ������
//			4.read_pos < write_pos		: ��Ч���ݷ�ΧΪ [read_pos, write_pos)
//			5.write_pos < read_pos		: ��Ч���ݷ�ΧΪ [read_pos, β��]�� [ͷ��, write_pos) 
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
				// �Ƿ�λ��
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
				// ��ȡ��д����λ��
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
				// ��ȡ��д����λ��
				size_t can_write_pos = INVALID_WRITE_POS;
				if(!get_free_space_for(data_length, can_write_pos))
				{
					// �������������ͬһ����get_free_space_for() ��֤��ͬ��дλ��
					BOOST_ASSERT(false);
					return;
				}

				if(can_write_pos != write_pos)
				{
					// ����ԭ����д����λ�ã�����β���п���ռ�
					un_used_pos = write_pos;
				}
				else
				{
					un_used_pos = INVALID_WRITE_POS;
				}

				// 
				// �ƶ���һ����дλ��
				size_t next_write_pos = can_write_pos + data_length;
				if(next_write_pos == read_pos)
				{
					// ��������
					write_pos = INVALID_WRITE_POS;
				}
				else if(next_write_pos > read_pos)
				{
					if(next_write_pos == sizeof(buffer_))
					{
						// β��û�пռ�, ����ͷ���пռ�
						if(read_pos > 0)
						{
							// ͷ���п��пռ�
							write_pos = 0;
						}
						else
						{
							write_pos = INVALID_WRITE_POS;
						}
					}
					else if(next_write_pos > sizeof(buffer_))
					{
						// ���������������� ��ΪҪȷ�������������ݿ飬���ᡮ���ѡ�
						BOOST_ASSERT(false);
						write_pos = INVALID_WRITE_POS;
					}
					else // write_pos < sizeof(buffer_)
					{
						// β������ʣ���д�ռ�
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
				// ��ȡ��д����λ��
				size_t can_write_pos = INVALID_WRITE_POS;
				if(!get_free_space_for(data_length, can_write_pos))
					return false;
				
				locked_block_for_write_ = &buffer_[0] + can_write_pos;

				//
				// �������� 
				memcpy(locked_block_for_write_, data, data_length);				
				SeekNextWritablePos(data_length);

				locked_block_for_write_ = NULL;
				return true;
			}

			void RemoveData(size_t read_done_data_length)
			{
				// ����Ƿ��ܹ��Ƴ�ָ�����ȵ�����
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
					// ���������߼���β��λ�ò��ɶ���û����Ч����
					// ���Զ�ȡͷ��λ��
					next_read_pos = 0;
				}

				if(next_read_pos == read_pos)
				{
					// ��ǰֻ��һ�����ݿ飬 ���Ҹոձ�����
					// �����ɿ�
					read_pos = 0;
					write_pos = 0;
					un_used_pos = INVALID_WRITE_POS;
					return;
				}

				if(next_read_pos == write_pos)
				{
					// ��ǰֻ��һ�����ݿ飬 ���Ҹոձ�����
					// �����ɿ�
					read_pos = 0;
					write_pos = 0;
					un_used_pos = INVALID_WRITE_POS;
					return;
				}

				if(next_read_pos > read_pos)
				{
					// ����β������
					if(write_pos == INVALID_WRITE_POS)
					{
						write_pos = read_pos;
					}

					read_pos = next_read_pos;
				}
				else // next_read_pos < read_pos
				{
					// ����ͷ������
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

					// ��ʱ�����鶼��������������
					return false;
				}

				if(read_pos < write_pos)
				{
					if(sizeof(buffer_) - write_pos >= expecting_space)
					{
						// β�����㹻����λ��
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
			// ��дλ��
			size_t read_pos;
			size_t write_pos;
			// �������߼���β��δ��ʹ�õĿ�ʼλ��
			size_t un_used_pos;
			// ������д��
			void* locked_block_for_write_;

			// ����
			char buffer_[MAX_BUFF_SIZE];
		};
	} // namespace utility
} // namespace evl

#endif // ENGINE_UTILITY_READ_WRITE_ARRAY_HPP_
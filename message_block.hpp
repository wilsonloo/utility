#ifndef __EVL_MESSAGE_BLOCK_H__
#define __EVL_MESSAGE_BLOCK_H__

/************************************************************************/
/* 消息块(只维护一个数据块，体统后置前置指针，供更高层应用）            */
/************************************************************************/

#include <cstring>

namespace evl
{
	namespace utility
	{

#pragma pack(push, 1)

		enum
		{
			MESSAGE_BLOCK_LEN_OFFSET = 20, // The Internal message header length is 20
			DEFAULT_MESSAGE_DATA_SIZE = 8 * 1024, 
		};

		class MessageBlock
		{
		public:
			MessageBlock()
				: data_m(NULL),
				length_m(0),
				currentPos_m(0),
				type_m(0),
				param_m(0),
				isAttached_m(true),
				next_m(NULL),
				prev_m(NULL)
			{
			}

			MessageBlock(int length)
				: data_m(NULL),
				length_m(length),
				currentPos_m(0),
				type_m(0),
				param_m(0),
				isAttached_m(false),
				next_m(NULL),
				prev_m(NULL)
			{
				// The maximum of message block is 8192, in case of any exception
				// From GateServer to LogicServer, length may exceed 
				//if (length_m > (DEFAULT_MESSAGE_DATA_SIZE + MESSAGE_BLOCK_LEN_OFFSET))
				//{
				//	length_m = (DEFAULT_MESSAGE_DATA_SIZE + MESSAGE_BLOCK_LEN_OFFSET);
				//}

				data_m = new char[length_m + 1];
				memset(data_m, 0, length_m + 1);
			}

			MessageBlock(char *data, int length, bool isAttached = true)
				: data_m(NULL),
				length_m(length),
				currentPos_m(0),
				type_m(0),
				param_m(0),
				isAttached_m(isAttached),
				next_m(NULL),
				prev_m(NULL)
			{
				// The maximum of message block is 8192, in case of any exception
				if (length_m > (DEFAULT_MESSAGE_DATA_SIZE + MESSAGE_BLOCK_LEN_OFFSET))
				{
					length_m = (DEFAULT_MESSAGE_DATA_SIZE + MESSAGE_BLOCK_LEN_OFFSET);
				}

				if (isAttached_m)
				{
					data_m = data;
				}
				else
				{
					data_m = new char[length_m + 1];
					memset(data_m, 0, length_m + 1);
					(void)copy(data, length_m);
				}
			}

			void setNext(MessageBlock *next)
			{
				next_m = next;
			}

			MessageBlock *getNext() const
			{
				return next_m;
			}

			void setPrev(MessageBlock *prev)
			{
				prev_m = prev;
			}

			MessageBlock *getPrev() const
			{
				return prev_m;
			}

			void copy(const char *data, int length)
			{
				int lengthCopied = 0;
				if (length <= (length_m - currentPos_m))
				{
					lengthCopied = length;
				}
				else
				{
					lengthCopied = (length_m - currentPos_m);
				}

				memcpy(data_m + currentPos_m, data, lengthCopied);
				currentPos_m += lengthCopied;
			}

			char *getData()
			{
				return data_m;
			}

			void release()
			{
				if (!isAttached_m && NULL != data_m)
				{
					delete[] data_m;
				}
			}

		// no copyable
		private: 
			MessageBlock &operator=(const MessageBlock &other);
			MessageBlock(const MessageBlock &other);

		public:
			char *data_m;
			int length_m;
			int currentPos_m;
			int type_m;
			int param_m;
			bool isAttached_m;
			MessageBlock *next_m;
			MessageBlock *prev_m;
		};

#pragma pack(pop)
	} // namespace utility
} // namespace evl


#endif //__EVL_MESSAGE_BLOCK_H__

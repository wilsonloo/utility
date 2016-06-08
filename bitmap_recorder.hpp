#ifndef EVL_UTILITY_BIT_MAP_RECORDER_HPP_
#define EVL_UTILITY_BIT_MAP_RECORDER_HPP_

////////////////////////////////////////////////////////////////////////////
// USAGE:
// subscript should > 0
//////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <boost/cstdint.hpp>

namespace evl
{
	namespace utility
	{
		class BitMapRecorder
		{
			enum
			{
				MIN_ALIGN_SIZE = 8, // expecting 8 bit of char
			};

		public:
			explicit BitMapRecorder(boost::uint32_t capacity) 
				: m_capacity(capacity)
				, m_data(NULL)
			{
				unsigned long charsNeeded = m_capacity / MIN_ALIGN_SIZE + 1;

				m_data = new char[charsNeeded];
				memset(m_data, 0, charsNeeded);
			}

			virtual ~BitMapRecorder( void)
			{
				if (m_data != NULL)
				{
					delete m_data;
					m_data = NULL;
				}
			}

			bool IsSet(boost::uint32_t what)
			{
				assert(what > 0 && what <= m_capacity && "out of bitmap range.");

				char* whichByte = &(m_data[what / MIN_ALIGN_SIZE]);
				return (whichByte[0] & (char)(1 << (what % MIN_ALIGN_SIZE))) != 0;
			}

			void Set(boost::uint32_t what)
			{
				assert(what > 0 && what <= m_capacity && "out of bitmap range.");

				char* whichByte = &(m_data[what / MIN_ALIGN_SIZE]);
				whichByte[0] = whichByte[0] | (char)(1 << (what % MIN_ALIGN_SIZE));
			}

			void UnSet(boost::uint32_t what)
			{
				assert(what > 0 && what <= m_capacity && "out of bitmap range.");

				char* whichByte = &(m_data[what / MIN_ALIGN_SIZE]);
				whichByte[0] = whichByte[0] & (char)(~(1 << (what % MIN_ALIGN_SIZE)));
			}

			bool GetNextDirtyIdx(boost::uint32_t& cur_idx)
			{
				for (; cur_idx < m_capacity - 1; ++cur_idx)
				{
					if (IsSet(cur_idx + 1))
					{
						++cur_idx;
						return true;
					}
				}
				
				return false;
			}
    
			inline boost::uint32_t get_capacity()const { return m_capacity; }

		private:
			BitMapRecorder( void);
			BitMapRecorder( const BitMapRecorder& other);
			BitMapRecorder& operator = ( const BitMapRecorder& other);

			char* m_data;
			boost::uint32_t m_capacity;
		};
	}
}

#endif // EVL_UTILITY_BIT_MAP_RECORDER_HPP_
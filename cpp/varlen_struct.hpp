#ifndef __EVL_COMMON_VARLEN_STRUCT_HPP__
#define __EVL_COMMON_VARLEN_STRUCT_HPP__

#include <boost/function.hpp>
#include <boost/cstdint.hpp>

#if defined(__GNUC__) && ((__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ < 5)))
// gcc 4.4及以下版本的存在template对齐bug问题，参考gcc bug: 7046
#  define TWP_ALIGNMENT_TEMPLATE_PACKET __attribute__((packed))
#else
#  define TWP_ALIGNMENT_TEMPLATE_PACKET
#endif

namespace evl {
	namespace utility {
#pragma pack(push, 1)

		//
		// 元素定长的变长结构
		//
		struct NullStruct{};

#ifdef _MSC_VER
#pragma  warning(disable: 4311)
#endif

		template<class TBase, class TElemType, boost::uint32_t t_maxElemAmount>
		struct VarLenStruct : public TBase
		{
		private:
			typedef VarLenStruct<TBase, TElemType, t_maxElemAmount> Self;

		public:
			typedef TElemType		ElemType;
			enum{ MAX_ELEM_AMOUNT = t_maxElemAmount, };

			boost::uint16_t	amount;
			ElemType		elems[MAX_ELEM_AMOUNT];
			int				last_reserve;

			VarLenStruct()
				:amount(0), last_reserve(0)
			{
			}

			template<class TAddElemType>
			bool Add(const TAddElemType& elem)
			{
				assert(sizeof(TAddElemType) <= sizeof(ElemType));

				if (amount >= MAX_ELEM_AMOUNT)
					return false;

				::memcpy(&elems[amount], &elem, sizeof(TAddElemType));
				++amount;
				return true;
			}

			ElemType* GetNextElemBuffer()
			{
				if (amount >= MAX_ELEM_AMOUNT)
					return NULL;

				return Get(amount++);
			}

			ElemType* ReserveElemBuffer(boost::uint32_t n)
			{
				if (last_reserve > 0)
					return NULL;
				if (amount + n > MAX_ELEM_AMOUNT)
					return NULL;
				ElemType* from = &elems[amount];

				amount += n;
				last_reserve = n;
				return from;
			}

			void ReleaseAndFixElemBuffer(boost::uint32_t nUsed)
			{
				if (nUsed > last_reserve)
					nUsed = last_reserve;

				amount -= last_reserve - nUsed;
				last_reserve = 0;
			}

			const ElemType* Get(boost::uint32_t index)const
			{
				if (index >= amount)
					return NULL;
				return &elems[index];
			}

			ElemType* Get(boost::uint32_t index)
			{
				if (index >= amount)
					return NULL;
				return &elems[index];
			}

			boost::uint32_t GetAmount()const
			{
				return amount;
			}

			static boost::uint32_t GetMaxAmount()
			{
				return MAX_ELEM_AMOUNT;
			}


			boost::uint32_t GetSize()const
			{
				//return (boost::uint32_t)(((Self*)0)->elems) + (amount * sizeof(ElemType));
				return (sizeof(*this) - sizeof(elems) - sizeof(last_reserve) + (amount * sizeof(ElemType)));
			}


			static boost::uint32_t GetMaxSize()
			{
				return sizeof(VarLenStruct<TBase, TElemType, t_maxElemAmount>);
			}

			void Clear()
			{
				memset(elems, 0, sizeof(elems));
				amount = 0;
				last_reserve = 0;
			}
		}TWP_ALIGNMENT_TEMPLATE_PACKET;

		template<typename VarLenStructType>
		class VarLenStructSender
		{
			typedef boost::function<void(const void*, boost::uint32_t)> SenderHandleType;
			typedef typename VarLenStructType::ElemType ElemType;
		public:
			VarLenStructSender(const SenderHandleType& sender)
				:sender_(sender),send_count_(0)
			{
			}

			~VarLenStructSender()
			{
				if(var_len_struct_.GetAmount() > 0)
				{
					sender_(&var_len_struct_, var_len_struct_.GetSize());
					var_len_struct_.Clear();
				}
			}

			VarLenStructType& get_var_len_struct() { return var_len_struct_; }

			void send()
			{
				sender_(&var_len_struct_, var_len_struct_.GetSize());
				++send_count_;
				var_len_struct_.Clear();
			}

			template<class TAddElemType>
			bool add(const TAddElemType& elem)
			{
				if(var_len_struct_.GetAmount() >= var_len_struct_.GetMaxAmount())
				{
					send();
				}
				return var_len_struct_.Add(elem);
			}

			ElemType* get_next_elem_buffer()
			{
				if(var_len_struct_.GetAmount() >= var_len_struct_.GetMaxAmount())
				{
					send();
				}
				return var_len_struct_.GetNextElemBuffer();
			}

			ElemType* reserve_elem_buffer(boost::uint32_t n)
			{
				return var_len_struct_.ReserveElemBuffer(n);
			}

			void release_and_fix_elem_buffer(boost::uint32_t used)
			{
				return var_len_struct_.ReleaseAndFixElemBuffer(used);
			}

			boost::uint32_t get_amount()const
			{
				return var_len_struct_.GetAmount();
			}

			boost::uint32_t get_send_count()const
			{
				return send_count_;
			}
		private:
			SenderHandleType sender_;
			VarLenStructType var_len_struct_;
			// send counting
			boost::uint32_t send_count_;
		};

#ifdef _MSC_VER
#pragma warning(default: 4311)
#endif


#pragma pack(pop)

    } // namespace utility
} // namespace evl


#endif // __EVL_COMMON_VARLEN_STRUCT_HPP__

#ifndef RTQ_UTILITY_UUID_GENERATOR_HPP_
#define RTQ_UTILITY_UUID_GENERATOR_HPP_

#include <list>
#include <set>
#include <cassert>

namespace evl
{
	namespace utility
	{
		//
		// 唯一ID生成器基类
		//
		template<typename T>
		class UUIDGenerator
		{
		public:
			UUIDGenerator(){}
			virtual ~UUIDGenerator(){}

			// 生成唯一ID
			// return invalid_id 表示生成失败
			virtual T generate() = 0;
			virtual void recycle(T id) = 0;

			virtual T invalid_id() = 0;

		private:
			UUIDGenerator(const UUIDGenerator& other);
			UUIDGenerator& operator=(const UUIDGenerator& other);
		};

		//
		// 非线程安全的唯一ID生成器
		// 注：该生成器采用回收机制， 会造成ID的回收累积， 需要确保ID不会太多
		template<typename T>
		class UnsafeUUIDGenerator : public UUIDGenerator<T>
		{
		public:
			UnsafeUUIDGenerator(const T& min_id, const T& max_id, const T& invalid_id)
				: MIN_ID_(min_id)
				, MAX_ID_(max_id)
				, INVALUE_ID_(invalid_id)
				, current_valid_id_(min_id)
			{
				assert(min_id < max_id);
				assert(invalid_id < min_id && max_id < invalid_id)
			}

			virtual ~UnsafeUUIDGenerator()
			{

			}

			// 生成唯一ID
			// return invalid_value 表示生成失败
			virtual T generate()
			{
				if(MIN_ID_ <= current_valid_id_ && current_valid_id_ < MAX_ID_)
					return current_valid_id_ ++;

				if(recycled_ids_.empty())
					return INVALUE_ID_;
				
				T valid_id = *recycled_ids_.front();
				recycled_ids_.pop_front();
			}

			virtual void recycle(T id)
			{
				assert(MIN_ID_ <= id && id <= MAX_ID_);

				recycled_ids_.push_back(id);
			}
			
			virtual T invalid_id()
			{
				return INVALUE_ID_;
			}

		private:
			// 静态值
			const T MIN_ID_;
			const T MAX_ID_;
			const T INVALUE_ID_;

			// 当前有效值
			T current_valid_id_;
			// 回收的ID
			typedef std::list<T> RecycledIDListType;
			RecycledIDListType recycled_ids_;
		};

		//
		// 非线程安全的唯一ID生成器
		// 
		template<typename T>
		class SequenceUUIDGenerator : public UUIDGenerator<T>
		{
		public:
			SequenceUUIDGenerator(const T& min_id, const T& max_id, const T& invalid_id)
				: MIN_ID_(min_id)
				, MAX_ID_(max_id)
				, INVALUE_ID_(invalid_id)
				, current_valid_id_(min_id)
			{
				assert(min_id < max_id);
				assert(invalid_id < min_id || max_id < invalid_id);
			}

			virtual ~SequenceUUIDGenerator()
			{

			}

			// 生成唯一ID
			// return invalid_value 表示生成失败
			virtual T generate()
			{
				if(MIN_ID_ <= current_valid_id_ && current_valid_id_ < MAX_ID_)
					return current_valid_id_++;

				if(current_valid_id_ == INVALUE_ID_)
					return INVALUE_ID_;

				if(current_valid_id_ == MAX_ID_)
				{
					current_valid_id_ = INVALUE_ID_;
					return MAX_ID_;
				}

				return INVALUE_ID_;
			}

			virtual void recycle(T id)
			{
				assert(MIN_ID_ <= id && id <= MAX_ID_);
			}

			virtual T invalid_id()
			{
				return INVALUE_ID_;
			}

		private:
			// 静态值
			const T MIN_ID_;
			const T MAX_ID_;
			const T INVALUE_ID_;

			// 当前有效值
			T current_valid_id_;
		};
	} // namespace utility
} // namespace evl

#endif // RTQ_UTILITY_UUID_GENERATOR_HPP_
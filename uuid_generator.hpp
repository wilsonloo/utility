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
		// ΨһID����������
		//
		template<typename T>
		class UUIDGenerator
		{
		public:
			UUIDGenerator(){}
			virtual ~UUIDGenerator(){}

			// ����ΨһID
			// return invalid_id ��ʾ����ʧ��
			virtual T generate() = 0;
			virtual void recycle(T id) = 0;

			virtual T invalid_id() = 0;

		private:
			UUIDGenerator(const UUIDGenerator& other);
			UUIDGenerator& operator=(const UUIDGenerator& other);
		};

		//
		// ���̰߳�ȫ��ΨһID������
		// ע�������������û��ջ��ƣ� �����ID�Ļ����ۻ��� ��Ҫȷ��ID����̫��
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

			// ����ΨһID
			// return invalid_value ��ʾ����ʧ��
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
			// ��ֵ̬
			const T MIN_ID_;
			const T MAX_ID_;
			const T INVALUE_ID_;

			// ��ǰ��Чֵ
			T current_valid_id_;
			// ���յ�ID
			typedef std::list<T> RecycledIDListType;
			RecycledIDListType recycled_ids_;
		};

		//
		// ���̰߳�ȫ��ΨһID������
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

			// ����ΨһID
			// return invalid_value ��ʾ����ʧ��
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
			// ��ֵ̬
			const T MIN_ID_;
			const T MAX_ID_;
			const T INVALUE_ID_;

			// ��ǰ��Чֵ
			T current_valid_id_;
		};
	} // namespace utility
} // namespace evl

#endif // RTQ_UTILITY_UUID_GENERATOR_HPP_
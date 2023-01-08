#ifndef EVL_UTILITY_SINGLETON_HPP_
#define EVL_UTILITY_SINGLETON_HPP_

namespace evl
{
	namespace utility
	{
		template<typename T>
		class Singleton
		{
		public:
			static T& get_mutable_instance();

			static T* get_mutable_instance_ptr();

			Singleton();
			virtual ~Singleton();

		private:
			static T* instance_;
		};

		template<typename T>
		T* Singleton<T>::instance_ = (T*)0;

		template<typename T>
		T& Singleton<T>::get_mutable_instance()
		{
			if (instance_ == (T*)0)
			{
				instance_ = new T;
			}

			return *instance_;
		}

		template<typename T>
		T* Singleton<T>::get_mutable_instance_ptr()
		{
			if (instance_ == (T*)0)
			{
				instance_ = new T;
			}

			return instance_;
		}

		template<typename T>
		Singleton<T>::Singleton()
		{}

		template<typename T>
		Singleton<T>::~Singleton()
		{
			if (instance_ != (T*)0)
			{
				delete instance_;
				instance_ = (T*)0;
			}
		}
	}
} // namespace evl

#endif // EVL_UTILITY_SINGLETON_HPP_
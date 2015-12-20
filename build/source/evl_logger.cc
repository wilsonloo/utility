#include <../include\evl_logger.h>
#include <iostream>
#include <boost/assert.hpp>

namespace evl
{
	namespace utility
	{
		EvlLogger::EvlLogger()
			: cur_color_type_(CONSOLE_COLOR_TYPE_NONE)
		{
			evl::utility::init_console_color();
			log4cplus::initialize();
		}

		bool EvlLogger::Init(const char* name)
		{
#ifdef _DEBUG
			if (log4cplus::Logger::exists(name))
			{
				std::cerr << "logger name " << name << " already exists." << std::endl;
				return false;
			}
#endif
			impl_ = log4cplus::Logger::getInstance(name);
			return true;
		}

		log4cplus::Logger& EvlLogger::GetImpl(evl::utility::ConsoleColorType color_type)
		{
			if (color_type == CONSOLE_COLOR_TYPE_NONE)
				return impl_;

			if (color_type != cur_color_type_)
			{
				evl::utility::set_text_color(color_type);
				cur_color_type_ = color_type;
			}

			return impl_;
		}

		log4cplus::Logger& EvlLogger::GetImpl()
		{
			return impl_;
		}

		void EvlLogger::SetBackColor(evl::utility::ConsoleColorType color_type)
		{
			evl::utility::set_back_color(color_type);
		}

		void EvlLogger::SetTextColor(evl::utility::ConsoleColorType color_type)
		{
			evl::utility::set_text_color(color_type);

			cur_color_type_ = color_type;
		}
		
	} // namespace utility
} // namejspace dps

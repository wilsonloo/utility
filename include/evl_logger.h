#ifndef EVL_UTILITY_EVL_LOGGER_H_
#define EVL_UTILITY_EVL_LOGGER_H_

#include <string>
#include <fstream>
#include <log4cplus/log4cplus.hpp>
#include "win_console_color.h"

namespace evl
{ 
	namespace utility
	{
		enum LogLevelType
		{
			LOG_LEVEL_TYPE_INVALID = log4cplus::NOT_SET_LOG_LEVEL,
			LOG_LEVEL_TYPE_DEBUG = log4cplus::DEBUG_LOG_LEVEL,
			LOG_LEVEL_TYPE_ERROR = log4cplus::ERROR_LOG_LEVEL,
			LOG_LEVEL_TYPE_WARN = log4cplus::WARN_LOG_LEVEL,
			LOG_LEVEL_TYPE_INFO = log4cplus::INFO_LOG_LEVEL,
		};

		class EvlLogger
		{
		public:
			EvlLogger();

			void SetBackColor(evl::utility::ConsoleColorType color_type);
			void SetTextColor(evl::utility::ConsoleColorType color_type);

			bool Init(const char* name);
				
			inline evl::utility::ConsoleColorType get_cur_text_color()const{return cur_color_type_;}
			inline void set_log_level(LogLevelType lvl){ impl_.setLogLevel(lvl); }

			log4cplus::Logger& GetImpl(evl::utility::ConsoleColorType color_type);
			log4cplus::Logger& GetImpl();

		private:
			log4cplus::Logger impl_;
			evl::utility::ConsoleColorType cur_color_type_;
		}; // class Logger

#ifdef _UNICODE

#define DPS_LOG_ERROR(MSG) \
	std::wcout << __FUNCTION__ << " @ " << __LINE__ << " : " <<  MSG << std::endl;

#define DPS_LOG_INFO(MSG) \
	std::wcout << __FUNCTION__ << " @ " << __LINE__ << " : " << MSG << std::endl;

#define DPS_LOG_DEBUG(MSG) \
	std::wcout << __FUNCTION__ << " @ " << __LINE__ << " : " << MSG << std::endl;

#define EVL_LOG_ERROR(logger_obj, MSG) \
	LOG4CPLUS_ERROR( logger_obj.GetImpl(evl::utility::CONSOLE_COLOR_TYPE_RED), MSG);

#define EVL_LOG_WARN(logger_obj, MSG) \
	LOG4CPLUS_WARN( logger_obj.GetImpl(evl::utility::CONSOLE_COLOR_TYPE_YELLOW), MSG);

#define EVL_LOG_DEBUG(logger_obj, MSG) \
	LOG4CPLUS_DEBUG( logger_obj.GetImpl(evl::utility::CONSOLE_COLOR_TYPE_MAGENTA), MSG);

#define EVL_LOG_INFO(logger_obj, MSG) \
	LOG4CPLUS_INFO( logger_obj.GetImpl(evl::utility::CONSOLE_COLOR_TYPE_WHITE), MSG);

#define EVL_LOG_ERROR_FUNCLINE(logger_obj, MSG) \
	EVL_LOG_ERROR(logger_obj, "(" << __FUNCTION__ << ":" << __LINE__ << ") " << MSG)

#define EVL_LOG_WARN_FUNCLINE(logger_obj, MSG) \
	EVL_LOG_WARN(logger_obj, "(" << __FUNCTION__ << ":" << __LINE__ << ") " << MSG)

#define EVL_LOG_DEBUG_FUNCLINE(logger_obj, MSG) \
	EVL_LOG_DEBUG(logger_obj, "(" << __FUNCTION__ << ":" << __LINE__ << ") " << MSG)

#define EVL_LOG_INFO_FUNCLINE(logger_obj, MSG) \
	EVL_LOG_INFO(logger_obj, "(" << __FUNCTION__ << ":" << __LINE__ << ") " << MSG)

#else 


#define EVL_LOG_FATAL(logger_obj, MSG) \
	LOG4CPLUS_FATAL( logger_obj.GetImpl(evl::utility::CONSOLE_COLOR_TYPE_GREEN), MSG);

#define EVL_LOG_ERROR(logger_obj, MSG) \
	LOG4CPLUS_ERROR( logger_obj.GetImpl(evl::utility::CONSOLE_COLOR_TYPE_RED), MSG);

#define EVL_LOG_WARN(logger_obj, MSG) \
	LOG4CPLUS_WARN( logger_obj.GetImpl(evl::utility::CONSOLE_COLOR_TYPE_YELLOW), MSG);

#define EVL_LOG_DEBUG(logger_obj, MSG) \
	LOG4CPLUS_DEBUG( logger_obj.GetImpl(evl::utility::CONSOLE_COLOR_TYPE_MAGENTA), MSG);

#define EVL_LOG_INFO(logger_obj, MSG) \
	LOG4CPLUS_INFO( logger_obj.GetImpl(evl::utility::CONSOLE_COLOR_TYPE_WHITE), MSG);

#define EVL_LOG_FATAL_FUNCLINE(logger_obj, MSG) \
	EVL_LOG_FATAL(logger_obj, "(" << __FUNCTION__ << ":" << __LINE__ << ") " << MSG)

#define EVL_LOG_ERROR_FUNCLINE(logger_obj, MSG) \
	EVL_LOG_ERROR(logger_obj, "(" << __FUNCTION__ << ":" << __LINE__ << ") " << MSG)

#define EVL_LOG_WARN_FUNCLINE(logger_obj, MSG) \
	EVL_LOG_WARN(logger_obj, "(" << __FUNCTION__ << ":" << __LINE__ << ") " << MSG)

#define EVL_LOG_DEBUG_FUNCLINE(logger_obj, MSG) \
	EVL_LOG_DEBUG(logger_obj, "(" << __FUNCTION__ << ":" << __LINE__ << ") " << MSG)

#define EVL_LOG_INFO_FUNCLINE(logger_obj, MSG) \
	EVL_LOG_INFO(logger_obj, "(" << __FUNCTION__ << ":" << __LINE__ << ") " << MSG)

#endif 

	} // namespace utility
} // namespace dps

#endif // EVL_UTILITY_EVL_LOGGER_H_
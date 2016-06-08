#ifndef EVL_UTILITY_WIN_CONSOLE_COLOR_HPP_
#define EVL_UTILITY_WIN_CONSOLE_COLOR_HPP_

/*Header file to color text and background in windows console applications
Global variables - textcol,backcol,deftextcol,defbackcol,colorprotect*/

#if __LINUX__

#elif _WIN32__
    #include <windows.h>
#endif

#include <iosfwd>
#include "setup.h"

namespace evl
{
	namespace utility
	{
		enum ConsoleColorType
		{
			CONSOLE_COLOR_TYPE_NONE			= 0xFFFFffff,
			CONSOLE_COLOR_TYPE_BLACK		= 0,
			CONSOLE_COLOR_TYPE_DARK_BLUE	= 1,
			CONSOLE_COLOR_TYPE_DARK_GREEN	= 2,

			CONSOLE_COLOR_TYPE_DARK_AQUA	= 3, 
			CONSOLE_COLOR_TYPEDARK_CYAN		= 3,

			CONSOLE_COLOR_TYPE_DARK_RED		= 4,

			CONSOLE_COLOR_TYPE_DARK_PURPLE	= 5, 
			CONSOLE_COLOR_TYPE_DARK_PINK	= 5, 
			CONSOLE_COLOR_TYPE_DARK_MAGENTA = 5,

			CONSOLE_COLOR_TYPE_DARK_YELLOW	= 6,
			CONSOLE_COLOR_TYPE_DARK_WHITE	= 7,
			CONSOLE_COLOR_TYPE_GRAY			= 8,
			CONSOLE_COLOR_TYPE_BLUE			= 9,
			CONSOLE_COLOR_TYPE_GREEN		= 10,

			CONSOLE_COLOR_TYPE_AQUA			= 11, 
			CONSOLE_COLOR_TYPE_CYAN			= 11,

			CONSOLE_COLOR_TYPE_RED			= 12,

			CONSOLE_COLOR_TYPE_PURPLE		= 13, 
			CONSOLE_COLOR_TYPE_PINK			= 13, 
			CONSOLE_COLOR_TYPE_MAGENTA		= 13,

			CONSOLE_COLOR_TYPE_YELLOW		= 14,
			CONSOLE_COLOR_TYPE_WHITE		= 15,
		};

		void update_colors();

		void set_color(ConsoleColorType textcolor, ConsoleColorType backcolor);

		void set_text_color(ConsoleColorType textcolor);

		void set_back_color(ConsoleColorType backcolor);

		void init_console_color();

		template<class elem, class traits>
		inline std::basic_ostream<elem, traits>& operator<<(std::basic_ostream<elem, traits>& os, ConsoleColorType col)
		{
			os.flush(); 
			set_text_color(col); 
			return os;
		}

		template<class elem, class traits>
		inline std::basic_istream<elem, traits>& operator>>(std::basic_istream<elem, traits>& is, ConsoleColorType col)
		{
			std::basic_ostream<elem, traits>* p = is.tie();
			if (p != NULL)
				p->flush();

			set_text_color(col);
			return is;
		}
	}
}	//end of namespace evl

#endif	//EVL_UTILITY_WIN_CONSOLE_COLOR_HPP_ 

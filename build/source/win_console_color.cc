#include <third_party\utility\include\win_console_color.h>

namespace evl
{
	namespace utility
	{
		//Standard Output Handle
		static HANDLE std_con_out;

		//If colorprotect is true, background and text colors will never be the same
		static bool colorprotect = false;

		/*
		textcol - current text color
		backcol - current back color
		deftextcol - original text color
		defbackcol - original back color
		*/
		static ConsoleColorType textcol, backcol, deftextcol, defbackcol;

		void update_colors()
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(std_con_out, &csbi);
			textcol = ConsoleColorType(csbi.wAttributes & 15);
			backcol = ConsoleColorType((csbi.wAttributes & 0xf0) >> 4);
		}

		void set_color(ConsoleColorType textcolor, ConsoleColorType backcolor)
		{
			if (colorprotect && textcolor == backcolor)
				return;

			textcol = textcolor;
			backcol = backcolor;
			unsigned short wAttributes = ((unsigned int)backcol << 4) | (unsigned int)textcol;
			SetConsoleTextAttribute(std_con_out, wAttributes);
		}

		void set_text_color(ConsoleColorType textcolor)
		{
			if (colorprotect && textcolor == backcol)
				return;

			textcol = textcolor;
			unsigned short wAttributes = ((unsigned int)backcol << 4) | (unsigned int)textcol;
			SetConsoleTextAttribute(std_con_out, wAttributes);
		}

		void set_back_color(ConsoleColorType backcolor)
		{
			if (colorprotect && textcol == backcolor)
				return;

			backcol = backcolor;
			unsigned short wAttributes = ((unsigned int)backcol << 4) | (unsigned int)textcol;
			SetConsoleTextAttribute(std_con_out, wAttributes);
		}

		void init_console_color()
		{
			std_con_out = GetStdHandle(STD_OUTPUT_HANDLE);
			update_colors();
			deftextcol = textcol; defbackcol = backcol;
		}

	}
}	//end of namespace evl
#ifndef EVL_UTILITY_CONSOLE_HPP_
#define EVL_UTILITY_CONSOLE_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <boost/function.hpp>
#include <boost/tokenizer.hpp>

namespace evl
{
	namespace utility
	{
		class Console
		{
		public:
			enum 
			{
				LIMIT_COMMAND_TEXT_LENGTH = 1024,
			};

			typedef boost::function<void(const std::vector<std::string>&/*arg_list*/)> HandlerType;

		public:
			explicit Console(HandlerType command_handler)
				: command_handler_(command_handler)
			{}

			virtual ~Console()
			{}

			void run_routine()
			{
				std::vector<std::string> arg_list;
				char command_text[LIMIT_COMMAND_TEXT_LENGTH];
				std::string command_string;
				while (std::cin.getline(command_text, sizeof(command_text) + 1))
				{
					command_string.assign(command_text);
					arg_list.clear();

					boost::tokenizer<> tok(command_string);
					for (boost::tokenizer<>::iterator iter = tok.begin();
						iter != tok.end(); ++iter)
					{
						arg_list.push_back(*iter);
					}

					if (command_handler_ != 0)
						command_handler_(arg_list);
				}
			}
			
		private:
			HandlerType command_handler_;
		}; // class Console
	} // namespace server_common
} // namespace rtq

#endif // EVL_UTILITY_CONSOLE_HPP_
/********************************************************************
 *     Author  : Wensheng.Luo
 *       Mail  : samule21@163.com
 * CreateTime  : 2017-11-1 20:26
 * Description : �ַ������˸�����
*********************************************************************/

#ifndef EVL_UTILITY_STRING_FILTERS_HPP_
#define EVL_UTILITY_STRING_FILTERS_HPP_

#include <stdarg.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <regex>
#include "string_utils.hpp"

namespace evl
{
    namespace utility
    {
        namespace string_filters
        {
            // �Ƿ����ָ��ֵ�ַ�
            static bool contains_chars(const std::string& source, const std::string chars)
            {
                int charLength = chars.length();
                for (int k = 0; k < charLength; ++k) {
                    if (source.find(chars.at(k) == std::string::npos))
                        return true;
                }

                return false;
            }

            static bool contains_special_chars(const std::string& source, const std::string special_chars = "/\\<>%#'();\r\n\t ")
            {
                return contains_chars(source, special_chars);
            }
            

            static bool is_viewable_ascii_char(unsigned long unicode)
            {
                return unicode >= 0x20 && unicode <= 0x7E;
            }

            static bool is_chinise_char(unsigned long unicode)
            {
                return unicode >= 0x4e00 && unicode <= 0x9fa5;
            }

            static bool is_japanise_char(unsigned long unicode)
            {
                return unicode >= 0x0800 && unicode <= 0x4e00;
            }

            static bool is_korean_char(unsigned long unicode)
            {
                return unicode >= 0xAC00 && unicode <= 0xD7AF;
            }
            
            //************************************
            // Description : �Ƿ���emoji�ַ�
            // @Parameter  : unicode ��Ӧ���ַ�unicodeֵ
            // @Returns    : bool 
            //************************************
            static bool is_emoji_char(unsigned long unicode)
            {
                if (unicode >= 0x1F600 && unicode <= 0x1F65F)
                    return true;

                return false;
            }

            //************************************
            // Description : �Ƿ���Dingbats�ַ�
            // @Parameter  : unicode ��Ӧ���ַ�unicodeֵ
            // @Returns    : bool 
            //************************************
            static bool is_dingbats_char(unsigned long unicode)
            {
                return unicode >= 0x2702 && unicode <= 0x27B0;
            }

            //************************************
            // Description : �Ƿ��ǽ�ͨ��ͼ�ź��ַ�
            // @Parameter  : unicode ��Ӧ���ַ�unicodeֵ
            // @Returns    : bool 
            //************************************
            static bool is_transport_and_map_char(unsigned long unicode)
            {
                return unicode >= 0x1F680 && unicode <= 0x1F6C5;
            }

            //************************************
            // Description : �Ƿ���enclosed�ַ�
            // @Parameter  : unicode ��Ӧ���ַ�unicodeֵ
            // @Returns    : bool 
            //************************************
            static bool is_enclosed_char(unsigned long unicode)
            {
                return unicode >= 0x24C2 && unicode <= 0x1F251;
            }

            //************************************
            // Description : �Ƿ���δ������ַ�
            // @Parameter  : unicode ��Ӧ���ַ�unicodeֵ
            // @Returns    : bool 
            //************************************
            static bool is_uncategorized_char(unsigned long unicode)
            {
                if (unicode >= 0x1F30D && unicode <= 0x1F567)
                    return true;

                if (unicode == 0xA9 ||
                    unicode == 0xAE ||
                    unicode == 0x203C ||
                    unicode == 0x2049)
                {
                    return true;
                }

                if (unicode >= 0x2122 && unicode <= 0x1F5FF)
                    return true;

                return false;
            }

            //************************************
            // Description : �ַ������Ƿ����emoji�ַ�
            // @Parameter  : source 
            // @Parameter  : is_contains �Ƿ����emoji
            // @Returns    : �����ɹ����
            // 
            //************************************
            static bool contains_emoji(const std::string& source, bool& is_contains)
            {
                is_contains = false;

                std::vector<unsigned long> unicode_values;
                if (!evl::utility::string_utls::enc_utf8_to_unicodes(reinterpret_cast<const unsigned char*>(source.c_str()), source.length(), unicode_values)) {
                    return false;
                }

                for (auto elem : unicode_values) {
                    if (is_emoji_char(elem)) {
                        is_contains = true;
                        return true;
                    }
                }
            
                return true;
            }

        }// namespace string_filters
      } // namespace utility
} // namespace evl

#endif // EVL_UTILITY_STRING_FILTERS_HPP_

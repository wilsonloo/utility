/********************************************************************
 *     Author  : Wensheng.Luo
 *       Mail  : samule21@163.com
 * CreateTime  : 2017-11-1 20:26
 * Description : 字符串相关辅助类
*********************************************************************/

#ifndef EVL_UTILITY_STRING_UTILS_HPP_
#define EVL_UTILITY_STRING_UTILS_HPP_

#include <stdarg.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>

namespace evl
{
    namespace utility
    {
        namespace string_utls
        {
            //////////////////////////////////////////////////////////////////////////
            // #pragma mark - Position
            /** Returns index of first occurrence of substring, otherwise returns -1. */
            inline int indexOf(const std::string& s, const std::string& sub) {
                if (s == "" || sub == "" || sub.length() > s.length()) return -1;
                auto pos = s.find(sub);
                return (pos != std::string::npos) ? static_cast<int>(pos) : -1;
            }

            /** Returns index of last occurrence of substring, otherwise returns -1. */
            inline int lastIndexOf(const std::string &s, const std::string &sub) {
                if (s == "" || sub == "" || sub.length() > s.length()) return -1;
                auto it = std::find_end(std::begin(s), std::end(s), std::begin(sub), std::end(sub));
                return (it != s.end()) ? static_cast<int>(it - std::begin(s)) : -1;
            }

            /** Returns whether a string begins with a substring. */
            inline bool startsWith(const std::string &s, const std::string &pre) { return s.find(pre) == 0; }
            
            /** Returns whether a string ends with a substring. */
            inline bool endsWith(const std::string &s, const std::string &suf) {
                if (s.length() < suf.length()) return false;
                return s.compare(s.length() - suf.length(), suf.length(), suf) == 0;
            }

            /** Returns whether a string contains a certain substring. */
            inline bool includes(const std::string &s, const std::string &sub) {
                return indexOf(s, sub) != -1;
            }

            //////////////////////////////////////////////////////////////////////////
            // #pragma mark - Casing
            /** Converts a string to lowercase. */
            inline std::string toLowerCase(const std::string& str) {
                std::string s(str);
                std::transform(std::begin(str), std::end(str), std::begin(s), ::tolower);

                return s;
            }
            /** Convers a string to uppercase. */
            inline std::string toUpperCase(const std::string& str) {
                std::string s(str);
                std::transform(std::begin(str), std::end(str), std::begin(s), ::toupper);

                return s;
            }

            //////////////////////////////////////////////////////////////////////////
            // #pragma mark - Trimming
            /** Trims whitespace at the beginning of a string. */
            inline std::string trimLeft(const std::string& str) {
                std::string s(str);

                s.erase(std::begin(s),
                    std::find_if(std::begin(s),
                        std::end(s),
                        std::not1(std::ptr_fun<int, int>(std::isspace))));
                return s;
            }

            /** Trims whitespace at the end of a string. */
            inline std::string trimRight(const std::string& str) {
                std::string s(str);

                s.erase(std::find_if(s.rbegin(),
                    s.rend(),
                    std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
                    std::end(s));
                return s;
            }

            /** Trims whitespace at the end of a string, starting from a delimiter. */
            inline std::string trimRight(const std::string& s, const std::string &delim) {
                return s.substr(0, s.find(delim));
            }

            /** Trims whitespace to the beginning and end of a string. */
            inline std::string trim(const std::string& s) { return trimRight(trimLeft(s)); }

            
            /** Removes whitespace from a string. */
            /* inline std::string compact(std::string s) {
                s.erase(std::remove_if(std::begin(s),
                    std::end(s),
                    std::bind(std::isspace<char>, std::placeholders::_1, std::locale::classic())),
                    std::end(s));
                return s;
            }
            */

            //////////////////////////////////////////////////////////////////////////
            // #pragma mark - Splitting
            /** Splits strings based on max line length. */
            inline std::vector<std::string> split(const std::string &s, const size_t maxLen) {
                std::vector<std::string> strs;
                for (size_t i = 0; i < s.length(); i += maxLen) {
                    strs.push_back(s.substr(i, maxLen));
                }
                return strs;
            }

            /** Splits strings based on a delimiter. */
            inline std::vector<std::string> split(const std::string &s, const std::string &delim = "\n") {
                std::vector<std::string> strs;
                auto start = 0U;
                auto end = s.find(delim);
                while (end != std::string::npos) {
                    strs.push_back(s.substr(start, end - start));
                    start = static_cast<unsigned int>(end + delim.length());
                    end = s.find(delim, start);
                }
                strs.push_back(s.substr(start, end));
                return strs;
            }

            //////////////////////////////////////////////////////////////////////////
            // #pragma mark - Replacement
            /** Replaces all occurrences of a search with a replacement substring. */
            inline std::string replace(const std::string& str, const std::string &search, const std::string &r) {
                std::string s(str);

                std::size_t pos = 0;
                while ((pos = s.find(search, pos)) != std::string::npos) {
                    s.replace(pos, search.length(), r);
                    pos += r.length();
                }
                return s;
            }

            /** Replaces all occurrences of multiple searches with a replacement substring. */
            inline std::string replace(const std::string& str, std::initializer_list<std::string> searches, const std::string &r) {
                std::string s(str);

                for (auto &i : searches) { s = replace(s, i, r); }
                return s;
            }

            /** Replaces everything after a maxLen in a string with a replacement substring (default: ""). */
            inline std::string trunc(const std::string& str, const size_t maxLen, const std::string &r = "") {
                std::string s(str);

                if (s.length() > maxLen && maxLen > 0) s = s.substr(0, maxLen) + r;
                return s;
            }

            //////////////////////////////////////////////////////////////////////////
            // #param mark - Formating
            inline std::string format(const char* format, ...)
            {
                // get the data-length after formated
                va_list args;
                va_start(args, format);
                int count = vsnprintf(nullptr, 0, format, args);
                va_end(args);

                va_start(args, format);
                int length = count * sizeof(char) + 1;
                char* buf = (char*)malloc(length);
                vsnprintf(buf, length, format, args);
                buf[length - 1] = '\0';
                va_end(args);

                std::string str(buf);
                free(buf);
                buf = nullptr;

                return str;
            }

            inline std::string concat(const std::vector<std::string>& array, const std::string& connector)
            {
                std::string ret;
                
                int array_size = array.size();
                for (int k = 0; k < array_size; ++k) {
                    if (k + 1 < array_size)
                    {
                        ret += array.at(k) + connector;
                    }
                    else {
                        ret += array.at(k);
                    }

                }

                return ret;
            }

            typedef std::function<std::string(const std::string& connector)> ConcatElemModifierType;

            inline std::string concat(const std::vector<std::string>& array, const std::string& connector, ConcatElemModifierType elemModifier)
            {
                std::string ret;

                int array_size = array.size();
                for (int k = 0; k < array_size; ++k) {
                    auto elem = elemModifier(array.at(k));

                    if (k + 1 < array_size)
                    {
                        ret += elem + connector;
                    }
                    else {
                        ret += elem;
                    }

                }

                return ret;
            }

            template<typename ElemType>
            struct ConcatElemModifierTraits
            {
                typedef std::function<std::string(const ElemType& connector)> type;
            };

            template<typename ElemType>
            inline std::string concat_ex(const std::vector<ElemType>& array, const std::string& connector, typename ConcatElemModifierTraits<ElemType>::type elemModifier)
            {
                std::string ret;

                int array_size = array.size();
                for (int k = 0; k < array_size; ++k) {
                    auto elem = elemModifier(array.at(k));

                    if (k + 1 < array_size)
                    {
                        ret += elem + connector;
                    }
                    else {
                        ret += elem;
                    }
                }

                return ret;
            }

            /*****************************************************************************
            * 返回一个utf编码的字符表示的 字节长度
            * 参数：
            *       c   utf编码的开始字符
            *
            * 返回值：
            *       成功则返回 >0 的字节长度；失败则返回0
            *****************************************************************************/
            static int enc_get_utf8_size(const unsigned char c)
            {
                int n = 0;

                if ((c & 0x80) == 0)         n = 1;
                else if ((c & 0xE0) == 0xC0) n = 2;
                else if ((c & 0xF0) == 0xE0) n = 3;
                else if ((c & 0xF8) == 0xF0) n = 4;
                else if ((c & 0xFC) == 0xF8) n = 5;
                else if ((c & 0xFE) == 0xFC) n = 6;
                else n = 0; 

                return n;
            }

            /*****************************************************************************
            * 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
            *
            * 参数:
            *    pInput      指向输入缓冲区, 以UTF-8编码
            *    Unic        指向输出缓冲区, 其保存的数据即是Unicode编码值,
            *                类型为unsigned long .
            *
            * 返回值:
            *    成功则返回该字符的UTF8编码所占用的字节数; 失败则返回0.
            *
            * 注意:
            *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
            *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
            *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
            ****************************************************************************/
            static int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned long *Unic)
            {
                assert(pInput != NULL && Unic != NULL);

                // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...  
                unsigned char b1, b2, b3, b4, b5, b6;

                // 把 *Unic 初始化为全零  
                *Unic = 0x0;

                // 该utf8 标记的字节长度
                int utfbytes = enc_get_utf8_size(*pInput);

                // 进行 Unic 写入准备
                unsigned char *pOutput = (unsigned char *)Unic;

                switch (utfbytes)
                {
                case 1:
                    *pOutput = *pInput;
                    break;
                case 2:
                    b1 = *pInput;
                    b2 = *(pInput + 1);
                    if ((b2 & 0xC0) != 0x80)
                        return 0;
                    *pOutput = (b1 << 6) + (b2 & 0x3F);
                    *(pOutput + 1) = (b1 >> 2) & 0x07;
                    break;
                case 3:
                    b1 = *pInput;
                    b2 = *(pInput + 1);
                    b3 = *(pInput + 2);
                    if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80))
                        return 0;
                    *pOutput = (b2 << 6) + (b3 & 0x3F);
                    *(pOutput + 1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
                    break;
                case 4:
                    b1 = *pInput;
                    b2 = *(pInput + 1);
                    b3 = *(pInput + 2);
                    b4 = *(pInput + 3);
                    if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                        || ((b4 & 0xC0) != 0x80))
                        return 0;
                    *pOutput = (b3 << 6) + (b4 & 0x3F);
                    *(pOutput + 1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
                    *(pOutput + 2) = ((b1 << 2) & 0x1C) + ((b2 >> 4) & 0x03);
                    break;
                case 5:
                    b1 = *pInput;
                    b2 = *(pInput + 1);
                    b3 = *(pInput + 2);
                    b4 = *(pInput + 3);
                    b5 = *(pInput + 4);
                    if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                        || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80))
                        return 0;
                    *pOutput = (b4 << 6) + (b5 & 0x3F);
                    *(pOutput + 1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
                    *(pOutput + 2) = (b2 << 2) + ((b3 >> 4) & 0x03);
                    *(pOutput + 3) = (b1 << 6);
                    break;
                case 6:
                    b1 = *pInput;
                    b2 = *(pInput + 1);
                    b3 = *(pInput + 2);
                    b4 = *(pInput + 3);
                    b5 = *(pInput + 4);
                    b6 = *(pInput + 5);
                    if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                        || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
                        || ((b6 & 0xC0) != 0x80))
                        return 0;
                    *pOutput = (b5 << 6) + (b6 & 0x3F);
                    *(pOutput + 1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
                    *(pOutput + 2) = (b3 << 2) + ((b4 >> 4) & 0x03);
                    *(pOutput + 3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
                    break;
                default:
                    return 0;
                    break;
                }

                return utfbytes;
            }

            static bool enc_utf8_to_unicodes(const unsigned char* source, unsigned int length, std::vector<unsigned long>& unicode_values)
            {
                if (source == nullptr)
                    return false;

                // 清空值
                unicode_values.clear();

                unsigned int offset = 0;
                while (offset < length)
                {
                    unsigned long unicode = 0;
                    int bytes = enc_utf8_to_unicode_one(source + offset, &unicode);
                    if (bytes <= 0) {
                        // 解析失败
                        return false;
                    }

                    unicode_values.push_back(unicode);
                    offset += static_cast<unsigned int>(bytes);
                }

                return true;
            }

        }// namespace string_utils
      } // namespace utility
} // namespace evl

#endif // EVL_UTILITY_STRING_UTILS_HPP_

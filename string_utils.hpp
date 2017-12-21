/********************************************************************
 *     Author  : Wensheng.Luo
 *       Mail  : samule21@163.com
 * CreateTime  : 2017-11-1 20:26
 * Description : ×Ö·û´®Ïà¹Ø¸¨ÖúÀà
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

        }// namespace string_utils
      } // namespace utility
} // namespace evl

#endif // EVL_UTILITY_STRING_UTILS_HPP_

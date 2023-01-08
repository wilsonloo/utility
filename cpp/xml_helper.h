/********************************************************************
 *     Author  : Wensheng.Luo
 *       Mail  : samule21@163.com
 * CreateTime  : 2018-3-9 20:35
 * Description : xml 解析辅助类
*********************************************************************/

#ifndef ___NFXMLHELPER_H__
#define ___NFXMLHELPER_H__

#include <string>
#include <exception>
#include "common/lexical_cast.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"
#include "NFComm/Utility/string_utils.hpp"

namespace pt
{
    namespace xml_utils {
        class NullPointerException : public std::runtime_error
        {
        private:
            const std::string mWhere;

        public:
            explicit NullPointerException(char const* _Message, const char* where)
                : std::runtime_error(_Message)
                , mWhere(where)
            {}

            explicit NullPointerException(const std::string& _Message, const std::string& where)
                : std::runtime_error(_Message)
                , mWhere(where)
            {}

            inline const std::string& where()const { return mWhere; }
        };

        // 生成 where 信息
        static std::string generate_where_info(rapidxml::xml_node<>* xml_node)
        {
            std::string whereInfo = evl::utility::string_utls::format("<%s ", xml_node->name());

            auto pAttri = xml_node->first_attribute();
            while (pAttri != nullptr) {
                whereInfo = whereInfo.append(evl::utility::string_utls::format("%s=\"%s\" ", pAttri->name(), pAttri->value()));
                pAttri = pAttri->next_attribute();
            }
        
            whereInfo = whereInfo.append(" />");

            return whereInfo;
        }

        template<typename RETURN_TYPE>
        inline RETURN_TYPE parse_first_attribute(rapidxml::xml_node<>* xml_node, const char* attribute_name)
        {
            auto attribute = xml_node->first_attribute(attribute_name);
            if (attribute == nullptr) {

                // 生成 where 信息
                std::string whereInfo;
#ifndef DISABLE_PT_XML_UTILS_GENERATE_WHERE_INFO
                whereInfo = generate_where_info(xml_node);
#endif // DISABLE_PT_XML_UTILS_GENERATE_WHERE_INFO

                throw NullPointerException(std::string("attribute [").append(attribute_name).append("] is NULL or NOT-SET"), whereInfo);
            }

            return lexical_cast<RETURN_TYPE>(attribute->value());
        }

        template<>
        inline const char* parse_first_attribute<const char*>(rapidxml::xml_node<>* xml_node, const char* attribute_name)
        {
            auto attribute = xml_node->first_attribute(attribute_name);
            if (attribute == nullptr) {

                // 生成 where 信息
                std::string whereInfo;
#ifndef DISABLE_PT_XML_UTILS_GENERATE_WHERE_INFO
                whereInfo = generate_where_info(xml_node);
#endif // DISABLE_PT_XML_UTILS_GENERATE_WHERE_INFO

                throw NullPointerException(std::string("attribute [").append(attribute_name).append("] is NULL or NOT-SET"), whereInfo);
            }

            return attribute->value();
        }

        static int parse_first_attribute_int(rapidxml::xml_node<>* xml_node, const char* attribute_name)
        {
            return parse_first_attribute<int>(xml_node, attribute_name);
        }

        static const char* parse_first_attribute_string(rapidxml::xml_node<>* xml_node, const char* attribute_name)
        {
            return parse_first_attribute<const char*>(xml_node, attribute_name);
        }
    }
}


#endif  // ___NFXMLHELPER_H__

/********************************************************************
 *     Author  : Wensheng.Luo
 *       Mail  : samule21@163.com
 * CreateTime  : 2018-1-19 18:01
 * Description : 内存分配跟踪器 
*********************************************************************/

#ifndef ___MEMORY_MALLOC_TRACKER_H__
#define ___MEMORY_MALLOC_TRACKER_H__

#include <iostream>

namespace evl
{
    namespace utility
    {
        //////////////////////////////////////////////////////////////////////////
        // 内存块信息
        class MemInfo
        {
        public:
            MemInfo()
                : ptr(nullptr)
                , file(nullptr)
                , line(0)
                , next(nullptr)
            {}

        private:
            void* ptr;
            const char* file;
            unsigned int line;
            MemInfo* next;
            friend class MemStack;
        };

        //////////////////////////////////////////////////////////////////////////
        // 内存记录栈
        class MemStack
        {
        private:
            MemInfo* head;

        public:
            MemStack()
                : head(nullptr)
            {

            }

            virtual ~MemStack()
            {
                MemInfo* temp = head;
                while (temp != nullptr) {

                    // 释放被泄露的内存
                    if (temp->ptr != nullptr)
                    {
                        free(temp->ptr);
                        temp->ptr = nullptr;

                        MemInfo* deleting = temp;
                        temp = temp->next;

                        free(deleting);
                        deleting = nullptr;
                    }

                }
            }

            void Insert(void* ptr, const char* file, unsigned int line)
            {
                MemInfo* node = (MemInfo*)malloc(sizeof(MemInfo));
                node->ptr = ptr;
                node->file = file;
                node->line = line;

                node->next = head;
                head = node;
            }

            void Delete(void* ptr) {
                MemInfo* node = head;
                MemInfo* pre = nullptr;

                // 查找 ptr 指定的节点
                while (node != nullptr && node->ptr != ptr) {
                    pre = node;
                    node = node->next;
                }

                if (node == nullptr) {
                    std::cout << "deleting an un-malloced memory." << std::endl;
                }
                else {
                    // 删除的是head
                    if (pre == nullptr)
                    {
                        head = node->next;
                    }
                    else {
                        pre->next = node->next;
                    }

                    free(node);
                    node = nullptr;
                }
            }

            void Print() {
                if (head == nullptr) {
                    std::cout << "memory is empty" << std::endl;
                    return;
                }

                std::cout << "memory leak occured" << std::endl;
                MemInfo* node = head;
                while (node != nullptr) {
                    std::cout << "filename: " << node->file << " line: " << node->line << std::endl;
                    std::cout << "address: " << node->ptr << std::endl;

                    node = node->next;
                }
            }
        };
    }
}

#endif  // ___MEMORY_MALLOC_TRACKER_H__

#ifndef STL_ALLOC_H
#define STL_ALLOC_H

#include <cstddef>

namespace ltx
{
    
    // 一级配置器
    class base_alloc
    {
    public:
        static void * allocate(size_t n)
        {
            void * result = ::operator new(n);
            return result;
        }
        static void deallocate(void * p, size_t)
        {
            ::operator delete(p);
        }

    };

    // 二级配置器
    enum {__ALIGN = 8};     // 小区快上调边界
    enum {__MAX_BYTES = 128};       // 小区快上限
    enum {__NFREELISTS = __MAX_BYTES/__ALIGN};      // free-list 个数

    class default_alloc
    {
    private:
        // 将bytes上调值8的倍数
        static size_t ROUND_UP(size_t bytes)
        {
            return ( (bytes+__ALIGN-1) & (~(__ALIGN-1)) );
        }

    private:
        union obj
        {
            union obj * free_list_link;
            char client_data[1];
        };

    private:   
        static obj * volatile free_list[__NFREELISTS];

        // 根据区块大小决定使用哪个free-list
        static size_t FREELIST_INDEX(size_t bytes)
        {
            return ((bytes+__ALIGN-1)/__ALIGN - 1);
        }

    // 内存池
    private:
        static char *start_free;
        static char *end_free;
        static size_t heap_size; // 作用见chunk_allo函数
        
        // 配置一大块空间, 可容纳nobjs个大小为size的区块
        // 如果空间不够但足够供应一个及一个以上的区块, 那就只供应能供应的数量, 并将nobj的值改变为供应数量
        static char *chunk_alloc(size_t size, int &nobjs)
        {
            char * result;
            size_t total_bytes = size*nobjs;
            size_t bytes_left = end_free - start_free;

            if(bytes_left >= total_bytes)
            {
                result = start_free;
                start_free += total_bytes;
                return result;
            }
            else if(bytes_left >= size)
            {
                nobjs = bytes_left/size;
                total_bytes = size*nobjs;
                result = start_free;
                start_free += total_bytes;
                return result;
            }
            else 
            {
            //一个区块都无法提供
                // 需要新配置空间的大小=2倍请求的空间+一个数值
                // 其中ROUND_UP(heap_size>>4)这一项的目的是让每次配置空间都比之前大一些
                size_t bytes_to_get = 2*total_bytes + ROUND_UP(heap_size>>4);

                // 让内存池中的空间利用起来(放到free_list中)
                if(bytes_left > 0)
                {
                    // volatile 锁住 *my_free_list, 也就是free_list数组中的个元素
                    // 网上说是为了多线程情况
                    // 但volatile并不能保证多线程下不被别的线程改变, 可能是个历史遗留问题
                    obj * volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
                    ((obj*)start_free)->free_list_link = *my_free_list;
                    *my_free_list = (obj*)start_free;
                }

                // 从堆中获取内存
                try
                {
                    start_free = (char*) ::operator new(bytes_to_get);
                }
                // catch(std::bad_alloc)
                catch(std::bad_alloc e)
                {
                    int i;
                    obj *volatile *my_free_list, *p;
                    for(i=size; i<=__MAX_BYTES; i+=__ALIGN)
                    {
                        my_free_list = free_list + FREELIST_INDEX(i);
                        p = *my_free_list;
                        if(p != nullptr)
                        {
                            *my_free_list = p->free_list_link;
                            start_free = (char*)p;
                            end_free = start_free+i;
                            return chunk_alloc(size, nobjs);
                        }
                    }
                    end_free = 0;
                    throw e;
                }
                heap_size += bytes_to_get;
                end_free = start_free+bytes_to_get;
                return chunk_alloc(size, nobjs);

            }
        }

    };
    // 定义和初始化类中的静态变量
    char* default_alloc::start_free = 0;
    char* default_alloc::end_free = 0;
    size_t default_alloc::heap_size = 0;

    default_alloc::obj* volatile default_alloc::free_list[__NFREELISTS] = {0};
}
#endif
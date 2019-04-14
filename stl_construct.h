#ifndef STL_CONSTRUCT_H
#define STL_CONSTRUCT_H

#include <type_traits>
#include <iostream>
using namespace std;

// 定义全局函数, 用于对象的构造和析构
namespace ltx
{
    template <typename T1, typename T2>
    inline void _construct(T1* p, const T2& value)
    {
        new (p) T1(value);
    }

    template <typename T>
    inline void _destroy(T* pointer)
    {
        pointer->~T();
    }

    // destroy第二版本, 析构一个区间的对象
    template <typename ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last)
    {
        bool is_trivial = std::is_trivially_destructible<decltype(*first)>::value;
        if(is_trivial) {}
        else 
        {
            for(; first<last; ++first) destroy(&*first);
        }
    }


}

#endif
#ifndef STL_ALGOBASE_H
#define STL_ALGOBASE_H

#include "stl_iterator.h"
namespace ltx
{
        template <typename ForwardIterator1, typename ForwardIterator2, typename T>
    inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*)
    {
        T tmp = *a;
        *a = *b;
        *b = tmp;
    }
    template <typename ForwardIterator1, typename ForwardIterator2>
    inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
    {
        __iter_swap(a, b, value_type(a));
    }

    
} // namespace ltx


#endif
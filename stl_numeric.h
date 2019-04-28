#ifndef STL_NUMERIC_H
#define STL_NUMERIC_H

namespace ltx
{
    // 将所有元素相加并加上初始值返回
    template <typename InputIterator, typename T>
    T accumulate(InputIterator first, InputIterator last, T init)
    {
        for(; first != last; ++first)
            init += *first;
        return init;
    }

    // 对init和所有元素执行二元操作并返回
    template <typename InputIterator, typename T, typename BinaryOperation>
    T accumulate(InputIterator first, InputIterator last, T init, 
                BinaryOperation binary_op)
    {
        for(; first != last; ++first)
            init = binary_op(init, *first);
        return init;
    }
}

#endif
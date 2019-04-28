#ifndef STL_ALGO_H
#define STL_ALGO_H

#include "stl_iterator.h"
#include "stl_algobase.h"
#include "heap.h"

namespace ltx   
{
    // 求第一个大于等于value的迭代器, 使用operator<比较
    template <typename ForwardIterator, typename T>
    inline ForwardIterator lower_bound(ForwardIterator first,
                                        ForwardIterator last,
                                        const T& value)
    {
        return __lower_bound(first, last, value, distance_type(first),
                            iterator_category(first));
    }

    // ForwardIterator版
    template <typename ForwardIterator, typename T, typename Distance>
    ForwardIterator __lower_bound(ForwardIterator first, 
                                  ForwardIterator last,
                                  const T& value,
                                  Distance*,
                                  forward_iterator_tag)
    {
        Distance len = 0;
        len = distance(first, last);   
        Distance half;
        ForwardIterator middle;

        while(len > 0)
        {
            half = len >> 1;
            middle = first;
            advance(middle, half);
            if(*middle < value) // 取右半区间, 不包括middle
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else len =  half;
        }

        return first;
    }

    // RandomAccessIterator版
    template <typename RandomAccessIterator, typename T, typename Distance>
    RandomAccessIterator __lower_bound(RandomAccessIterator first,
                                       RandomAccessIterator last,
                                       const T& value,
                                       Distance *,
                                       random_access_iterator_tag)
    {
        Distance len = last - first;
        Distance half;
        RandomAccessIterator middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first + half;
            if(*middle < value)
            {
                first = middle+1;
                len = len - half - 1;
            }
            else len = half;
        }
        return first;
    }

    // 查找第一个大于等于v的迭代器, 使用仿函数比较
    template <typename ForwardIterator, typename T, typename Compare>
    inline ForwardIterator lower_bound(ForwardIterator first,
                                        ForwardIterator last,
                                        const T& value,
                                        Compare comp)
    {
        return __lower_bound(first, last, value, comp, distance_type(first),
                            iterator_category(first));
    }

    // ForwardIterator版
    template <typename ForwardIterator, typename T, typename Compare, typename Distance>
    ForwardIterator __lower_bound(ForwardIterator first, 
                                  ForwardIterator last,
                                  const T& value,
                                  Compare comp,
                                  Distance*,
                                  forward_iterator_tag)
    {
        Distance len = 0;
        len = distance(first, last);   
        Distance half;
        ForwardIterator middle;

        while(len > 0)
        {
            half = len >> 1;
            middle = first;
            advance(middle, half);
            if(comp(*middle, value)) // 取右半区间, 不包括middle
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else len =  half;
        }

        return first;
    }

    // RandomAccessIterator版
    template <typename RandomAccessIterator, typename T, typename Compare, typename Distance>
    RandomAccessIterator __lower_bound(RandomAccessIterator first,
                                       RandomAccessIterator last,
                                       const T& value,
                                       Compare comp,
                                       Distance *,
                                       random_access_iterator_tag)
    {
        Distance len = last - first;
        Distance half;
        RandomAccessIterator middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first + half;
            if(comp(*middle, value))
            {
                first = middle+1;
                len = len - half - 1;
            }
            else len = half;
        }
        return first;
    }

    // 第一个大于v的迭代器, 使用operator<比较
    template <typename ForwardIterator, typename T>
    inline ForwardIterator upper_bound(ForwardIterator first,
                                       ForwardIterator last,
                                       const T& value)
    {
        return __upper_bound(first, last, value, distance_type(first),
                            iterator_category(first));
    }

    template <typename ForwardIterator, typename T, typename Distance>
    ForwardIterator __upper_bound(ForwardIterator first, 
                                  ForwardIterator last, 
                                  const T& value, 
                                  Distance*,
                                  forward_iterator_tag)
    {
        Distance len = 0;
        len = distance(first, last);   
        Distance half;
        ForwardIterator middle;

        while(len > 0)
        {
            half = len >> 1;
            middle = first;
            advance(middle, half);
            if(value < *middle) len =  half;
            else    // 取右半区间, 不包括middle
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
        }

        return first;
    }

    // RandomAccessIterator版
    template <typename RandomAccessIterator, typename T, typename Distance>
    RandomAccessIterator __upper_bound(RandomAccessIterator first,
                                       RandomAccessIterator last,
                                       const T& value,
                                       Distance *,
                                       random_access_iterator_tag)
    {
        Distance len = last - first;
        Distance half;
        RandomAccessIterator middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first + half;
            if(value < *middle) len = half;
            else 
            {
                first = middle+1;
                len = len - half - 1;
            }
        }
        return first;
    }

    // 查找第一个大于v的迭代器, 使用仿函数比较
    template <typename ForwardIterator, typename T, typename Compare>
    inline ForwardIterator upper_bound(ForwardIterator first,
                                        ForwardIterator last,
                                        const T& value,
                                        Compare comp)
    {
        return __upper_bound(first, last, value, comp, distance_type(first),
                            iterator_category(first));
    }

    // ForwardIterator版
    template <typename ForwardIterator, typename T, typename Compare, typename Distance>
    ForwardIterator __upper_bound(ForwardIterator first, 
                                  ForwardIterator last,
                                  const T& value,
                                  Compare comp,
                                  Distance*,
                                  forward_iterator_tag)
    {
        Distance len = 0;
        len = distance(first, last);   
        Distance half;
        ForwardIterator middle;

        while(len > 0)
        {
            half = len >> 1;
            middle = first;
            advance(middle, half);
            if(comp(value, *middle)) len = half;
            else 
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
        }

        return first;
    }

    // RandomAccessIterator版
    template <typename RandomAccessIterator, typename T, typename Compare, typename Distance>
    RandomAccessIterator __upper_bound(RandomAccessIterator first,
                                       RandomAccessIterator last,
                                       const T& value,
                                       Compare comp,
                                       Distance *,
                                       random_access_iterator_tag)
    {
        Distance len = last - first;
        Distance half;
        RandomAccessIterator middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first + half;
            if(comp(value, *middle)) len = half;
            else 
            {
                first = middle+1;
                len = len - half - 1;
            }
        }
        return first;
    }

    // 判断有序数组中是否存在value
    template <typename ForwardIterator, typename T>
    bool binary_search(ForwardIterator first, ForwardIterator last,
                        const T& value)
    {
        ForwardIterator i = lower_bound(first, last, value);
        return i!=last && !(value < *i);
    }

    template <typename ForwardIterator, typename T, typename Compare>
    bool binary_search(ForwardIterator first, ForwardIterator last, const T& value, Compare comp)
    {
        ForwardIterator i = lower_bound(first, last, value, comp);
        return i!=last && !comp(value, *i);
    }

    template <class BidirectionalIterator>
    void __reverse(BidirectionalIterator first, BidirectionalIterator last, 
                bidirectional_iterator_tag) 
    {
        while (true)
            if (first == last || first == --last) return;
            else iter_swap(first++, last);
    }

    template <class RandomAccessIterator>
    void __reverse(RandomAccessIterator first, RandomAccessIterator last,
                random_access_iterator_tag) 
    {
        while (first < last)
            iter_swap(first++, --last);
    }

    template <class BidirectionalIterator>
    inline void reverse(BidirectionalIterator first, BidirectionalIterator last) 
    {
        __reverse(first, last, iterator_category(first));
    }



    // 将[first, last)变为下一个排列组合(第一个字典序大于当前的排列), 如果没有返回false
    template <typename BidirectionalIterator>
    bool next_permutation(BidirectionalIterator first,
                          BidirectionalIterator last)
    {
        // 长度小于等于1的区间直接返回false
        if(first == last) return false; 
        BidirectionalIterator i = first;
        ++i;
        if(i==last) return false;

        // 令i指向尾端
        i = last;
        --i;

        for(;;)
        {
            BidirectionalIterator ii = i;
            --i;
            // 找到一对相邻元素i,ii, 且*i<*ii
            if(*i < *ii)
            {
                BidirectionalIterator j = last;
                // 从后往前找到第一个j, *j>*i 
                while(!(*i < *--j));
                // 交换i j所指元素
                iter_swap(i, j);
                // 将ii之后所有元素颠倒
                reverse(ii, last);
                return true;
            }
            if(i == first) // 进行到最前面饿了
            {
                reverse(first, last);
                return false;
            }
        }
    }

    // 求前一个排列
    template <typename BidirectionalIterator>
    bool prev_permutation(BidirectionalIterator first,
                          BidirectionalIterator last)
    {
        // 长度小于等于1的区间直接返回false
        if(first == last) return false; 
        BidirectionalIterator i = first;
        ++i;
        if(i==last) return false;

        // 令i指向尾端
        i = last;
        --i;

        for(;;)
        {
            BidirectionalIterator ii = i;
            --i;
            // 找到一对相邻元素i,ii, *ii < *i
            if(*ii < *i)
            {
                BidirectionalIterator j = last;
                // 从后往前找到第一个j, *j>*i 
                while(!(*--j < *i));
                // 交换i j所指元素
                iter_swap(i, j);
                // 将ii之后所有元素颠倒
                reverse(ii, last);
                return true;
            }
            if(i == first) // 进行到最前面饿了
            {
                reverse(first, last);
                return false;
            }
        }
    }

    // 排序
    // 将value插入前面的有序数组, 不进行边界检查, 因为上一个函数已经保证*first<=value了
    template <typename RandomAccessIterator, typename T>
    void __unguarded_linear_insert(RandomAccessIterator last, T value)
    {
        RandomAccessIterator next = last;
        --next;
        while(value < *next)
        {
            *last = *next;
            last = next;
            --next;
        }
        *last = value;
    }
    // 将last插入前面的有序数组中
    template <typename RandomAccessIterator, typename T>
    inline void __linear_insert(RandomAccessIterator first, 
                                RandomAccessIterator last,
                                T*)
    {
        T value = *last;
        if(value < *first)
        {
            copy_backward(first, last, last+1);
            *first = value;
        }
        else __unguarded_linear_insert(last, value);
    }


    // insertion sort
    template <typename RandomAccessIterator>
    void __insertion_sort(RandomAccessIterator first, 
                         RandomAccessIterator last)
    {
        if(first == last) return ;
        for(RandomAccessIterator i = first+1; i!=last; ++i)
            __linear_insert(first, i, value_type(first));
    }

    template <typename RandomAccessIterator, typename T>
    void __unguarded_insertion_sort_aux(RandomAccessIterator first,
                                        RandomAccessIterator last, 
                                        T*)
    {
        for(RandomAccessIterator i = first; i != last; ++i)
            __unguarded_linear_insert(i, T(*i));
    }

    template <typename RandomAccessIterator>
    inline void __unguarded_insertion_sort(RandomAccessIterator first,
                                           RandomAccessIterator last)
    {
        __unguarded_insertion_sort_aux(first, last, value_type(first));
    }


    // 返回a, b, c居中者
    template <typename T>
    inline const T& __median(const T& a, const T& b, const T& c)
    {
        if(a < b)
        {
            if(b<c) return b; // a b c
            else if(a < c) return c; // a<b b>=c a<c
            else return a;
        }
        else if(a < c) return a; // a>=b a<c
        else if(b < c) return c; // a>=b a>=c b < c;
        else return b;
    }

    // 分割函数, 返回分割后右段第一个位置
    template <typename RandomAccessIterator, typename T>
    RandomAccessIterator __unguarded_partition(
                                RandomAccessIterator first,
                                RandomAccessIterator last,
                                T pivot)
    {
        while(true)
        {
            while(*first < pivot) ++first;
            --last;
            while(pivot < * last) --last;
            if(!(first < last)) return first;
            iter_swap(first, last);
            ++first;
        }
    }

    // 返回2^k <= n的最大值k
    template <typename Size>
    inline Size __lg(Size n)
    {
        Size k;
        for(k = 0; n > 1; n>>=1) ++k;
        return k;
    }

    
    template <typename RandomAccessIterator, typename T>
    void __partial_sort(RandomAccessIterator first,
                        RandomAccessIterator middle, 
                        RandomAccessIterator last,
                        T*)
    {
        make_heap(first, middle);
        for(RandomAccessIterator i = middle; i<last; ++i)
            if(*i < *first)
                __pop_heap(first, middle, i, T(*i), distance_type(first));
        sort_heap(first, middle);
    }


    template <typename RandomAccessIterator>
    inline void partial_sort(RandomAccessIterator first,
                             RandomAccessIterator middle,
                             RandomAccessIterator last)
    {
        __partial_sort(first, middle, last, value_type(first));
    }


    const int __stl_threshold = 16;

    

    // introsort, 限定递归层数
    template <typename RandomAccessIterator, typename T, typename Size>
    void __introsort_loop(RandomAccessIterator first, 
                          RandomAccessIterator last,
                          T*,
                          Size depth_limit)
    {
        while(last-first > __stl_threshold) // 排序到长度为16为止
        {
            if(depth_limit == 0) // 分隔恶化, 使用heapsort
            {
                partial_sort(first, last, last);
                return ;
            }

            --depth_limit;

            RandomAccessIterator cut = __unguarded_partition(
                first, last, T(__median(
                                        *first,
                                        *(first+(last-first)/2),
                                        *(last-1)))
            );
            // 对右半部分递归
            __introsort_loop(cut, last, value_type(first), depth_limit);
            // 对左半部分继续循环处理
            last = cut;
        }   
    }

    // 进行插入排序
    template <typename RandomAccessIterator>
    void __final_insertion_sort(RandomAccessIterator first,
                                RandomAccessIterator last)
    {
        if(last - first > __stl_threshold)
        {
            __insertion_sort(first, first+__stl_threshold);
            __unguarded_insertion_sort(first+__stl_threshold, last);
        }
        else 
            __insertion_sort(first, last);
    }

    // 排序
    template <typename RandomAccessIterator>
    inline void sort(RandomAccessIterator first, 
                     RandomAccessIterator last)
    {
        if(first != last)
        {
            __introsort_loop(first, last, value_type(first), __lg(last-first)*2);
            __final_insertion_sort(first, last);
        }
    }

    
} // namespace ltx


#endif
#ifndef HEAP_H
#define HEAP_H
#include "stl_iterator.h"

namespace ltx
{
    template <typename RandomAccessIterator, typename Distance, typename T>
    void __push_heap(RandomAccessIterator first, 
    Distance holeIndex, Distance topIndex, T value)
    {
        Distance parent = (holeIndex - 1) / 2;
        // 当尚未达到顶端, 且父节点小于新值
        while(holeIndex>topIndex && *(first+parent)<value)
        {
            *(first+holeIndex) = *(first+parent); // 把其父亲放下来
            holeIndex = parent; // 继续向上调整父亲
            parent = (holeIndex-1)/2;
        }
        *(first+holeIndex) = value;
    }

    template <typename RandomAccessIterator, typename Distance, typename T>
    inline void __push_heap_aux(RandomAccessIterator first, 
    RandomAccessIterator last, Distance*, T*)
    {
        __push_heap(first, Distance((last-first)-1), Distance(0), T(*(last-1)));
    }

    // 此函数被调用时, 新元素应已置于容器最尾端
    template <typename RandomAccessIterator>
    inline void push_heap(RandomAccessIterator first,
                            RandomAccessIterator last)
    {
        __push_heap_aux(first, last, distance_type(first), value_type(first));
    }


    template <typename RandomAccessIterator, typename Distance, typename T>
    void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
    {
        Distance topIndex = holeIndex;
        Distance secondChild = 2*holeIndex+2;// 右儿子
        while(secondChild < len)
        {
            // second为值大的那个儿子
            if(*(first+secondChild) < *(first+(secondChild-1)))
                secondChild--;
            *(first+holeIndex) = *(first+secondChild);
            holeIndex = secondChild;
            secondChild = 2*(secondChild+1);
        }
        if(secondChild == len)
        {
            *(first + holeIndex) = *(first + (secondChild-1));
            holeIndex = secondChild-1;
        }
        __push_heap(first, holeIndex, topIndex, value);
    }

    template <typename RandomAccessIterator, typename T, typename Distance>
    inline void __pop_heap
    (RandomAccessIterator first, 
    RandomAccessIterator last,
    RandomAccessIterator result,
    T value, 
    Distance *)
    {
        *result = *first; // 设定尾值为首值
        __adjust_heap(first, Distance(0), Distance(last-first), value);
    }

    template <typename RandomAccessIterator, typename T>
    inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
    {
        __pop_heap(first, last-1, last-1, T(*(last-1)), distance_type(first));
    }


    // 将堆顶元素移除并维持堆的性质, 将堆顶元素放到数组末尾
    template <typename RandomAccessIterator>
    inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        __pop_heap_aux(first, last, value_type(first));
    }


    // 将heap转换成递增序列
    template <typename RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, 
    RandomAccessIterator last)
    {
        while(last-first > 1)
            pop_heap(first, last--);
    }

    template <typename RandomAccessIterator, typename T, typename Distance>
    void __make_heap(RandomAccessIterator first, 
    RandomAccessIterator last, T*, Distance*)
    {
        if(last - first < 2) return ;
        Distance len = last - first;

        Distance holeIndex = (len-2)/2; // 最后一个节点的父亲, 也就是第一个需要调整的节点
        while (true)
        {
            __adjust_heap(first, holeIndex, len, T(*(first+holeIndex)));
            if(holeIndex == 0) return ;
            holeIndex--;    
        }
    }

    template <typename RandomAccessIterator>
    inline void make_heap(RandomAccessIterator first, 
    RandomAccessIterator last)
    {
        __make_heap(first, last, value_type(first), distance_type(first));
    }


    // 使用比较器版本的函数

    

    template <typename RandomAccessIterator, typename Distance, typename T, 
            typename Compare>
    void
    __push_heap(RandomAccessIterator first, Distance holeIndex,
                Distance topIndex, T value, Compare comp)
    {
        Distance parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && comp(*(first + parent), value)) 
        {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    template <typename RandomAccessIterator, typename Compare,
            typename Distance, typename T>
    inline void 
    __push_heap_aux(RandomAccessIterator first,
                    RandomAccessIterator last, Compare comp,
                    Distance*, T*) 
    {
        __push_heap(first, Distance((last - first) - 1), Distance(0), 
                T(*(last - 1)), comp);
    }

    template <typename RandomAccessIterator, typename Compare>
    inline void 
    push_heap(RandomAccessIterator first, RandomAccessIterator last,
            Compare comp)
    {
        __push_heap_aux(first, last, comp,
                        distance_type(first), value_type(first));
    }


    
    template <typename RandomAccessIterator, typename Distance,
            typename T, typename Compare>
    void
    __adjust_heap(RandomAccessIterator first, Distance holeIndex,
                Distance len, T value, Compare comp)
    {
        Distance topIndex = holeIndex;
        Distance secondChild = 2 * holeIndex + 2;
        while (secondChild < len) 
        {
            if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
            *(first + holeIndex) = *(first + secondChild);
            holeIndex = secondChild;
            secondChild = 2 * (secondChild + 1);
        }
        if (secondChild == len) {
            *(first + holeIndex) = *(first + (secondChild - 1));
            holeIndex = secondChild - 1;
        }
        __push_heap(first, holeIndex, topIndex, value, comp);
    }

    template <typename RandomAccessIterator, typename T, typename Compare, 
            typename Distance>
    inline void 
    __pop_heap(RandomAccessIterator first, RandomAccessIterator last,
            RandomAccessIterator result, T value, Compare comp,
            Distance*)
    {
        *result = *first;
        __adjust_heap(first, Distance(0), Distance(last - first), 
                        value, comp);
    }

    template <typename RandomAccessIterator, typename T, typename Compare>
    inline void 
    __pop_heap_aux(RandomAccessIterator first,
                RandomAccessIterator last, T*, Compare comp)
    {
        __pop_heap(first, last - 1, last - 1, T(*(last - 1)), comp,
                    distance_type(first));
    }

    template <typename RandomAccessIterator, typename Compare>
    inline void 
    pop_heap(RandomAccessIterator first,
            RandomAccessIterator last, Compare comp)
    {
        __pop_heap_aux(first, last, value_type(first), comp);
    }


        
    template <class RandomAccessIterator, class Compare,
            class T, class Distance>
    void
    __make_heap(RandomAccessIterator first, RandomAccessIterator last,
                Compare comp, T*, Distance*)
    {
        if (last - first < 2) return;
        Distance len = last - first;
        Distance parent = (len - 2)/2;
            
        while (true) 
        {
            __adjust_heap(first, parent, len, T(*(first + parent)),
                        comp);
            if (parent == 0) return;
            parent--;
        }
    }

    template <class RandomAccessIterator, class Compare>
    inline void 
    make_heap(RandomAccessIterator first, 
            RandomAccessIterator last, Compare comp)
    {
        __make_heap(first, last, comp,
                value_type(first), distance_type(first));
    }


    template <class RandomAccessIterator, class Compare>
    void 
    sort_heap(RandomAccessIterator first,
            RandomAccessIterator last, Compare comp)
    {
        while (last - first > 1)
            pop_heap(first, last--, comp);
    }


}

#endif
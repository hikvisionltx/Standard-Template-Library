`heap`不是一个容器, 而是一系列堆相关的函数, 是实现`priority_queue`的基础

## heap表示方式

在`STL`的实现中, `heap`使用拥有`RandomAccessIterator`的序列容器(数组, `vector`等等)作为堆的底层容器, 由于堆是一棵完全二叉树, 如果我们将0位置作为树根, 那么对于节点`i`, `(i-1)/2`是它的父亲, `i*2+1`是它的左儿子, `i*2+2`是它的右儿子

## push_heap

`push_heap`将一个元素加入到堆中, 并维持堆的性质

需要将新加入的元素放到数组末尾的位置, 然后函数将最后这个位置作为`holeIndex`, 意为新值要放入的位置, 然后使用新值跟`holeIndex`的父亲比较, 如果父亲比新值小, 则将父亲的值填入`holeIndex`, 然后将父亲设置为新的`holeIndex`, 然后继续向上处理

```cpp
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
```

## pop_heap

`pop_heap`将堆顶值移除, 并维护堆的性质, 完成后原来堆顶的值将会被放在数组的末端

将数组最后一个元素取出, 填充去除堆顶后留下的空白, 将堆顶设置为`holeIndex`, 然后不断将`holeIndex`儿子中较大的那个填到`holeIndex`中, 然后将较大的儿子所在的位置设置为新的`holeIndex`, 然后继续向下

```cpp

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
        if(secondChild == len) // 没有右儿子
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
```

## sort_heap

将一个堆转换成一个有序数组

利用`pop_heap`函数, 不断将最大值放到末尾, 直到堆为空

```cpp
    // 将heap转换成递增序列
    template <typename RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, 
    RandomAccessIterator last)
    {
        while(last-first > 1)
            pop_heap(first, last--);
    }
```

## make_heap

将数组转换为heap

因为叶子节点都是一个合法的堆, 所以不需要调整叶子节点, 找到最后一个节点的父亲, 从它开始, 利用`__adjust_heap`函数, 将其调整为合法的堆, 不断向上, 直到整个堆合法

```cpp
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
```


STL的SGI实现中的排序算法是Intro排序, 它综合了快速排序, 插入排序, 堆排序三种算法, 得到了一个非常优秀的效率, 代码也非常巧妙

首先, Intro排序主要还是使用快速排序, 但是有两个限制:

1. 递归层数不超过$\log_2^n+1$, 超过这个层数后不在使用快速快速排序递归下去, 而是改用堆排序
2. 但快速排序递归到区间长度小于等于16时, 使用不在进行排序, 而是等到最后使用插入排序

并且在代码实现中, 使用了许多巧妙的优化方法, 减少了比较次数和函数调用次数, 虽然一次比较或者函数调用耗费的资源非常少, 但对于数据量比较大的时候, 能节省的资源还是比较可观的



# 插入排序

首先是插入排序, 插入排序对于区间中每个元素, 调用`__linear_insert`函数, 将该元素插入到前面的有序区间中去

```cpp
// insertion sort
template <typename RandomAccessIterator>
    void __insertion_sort(RandomAccessIterator first, 
                          RandomAccessIterator last)
{
    if(first == last) return ;
    for(RandomAccessIterator i = first+1; i!=last; ++i)
        __linear_insert(first, i, value_type(first));
}
```

`__linear_insert`首先进行一次判断, 如果该元素比第一个元素还要小, 那么就直接将它插入到区间首, 而不是从后往前进行判断

```cpp
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
```

否则, 调用`__unguarded_linear_insert`, 由于之间检查了`value < *first`, 所以可以保证前面至少一个元素小于要插入的元素, 这样就不需要进行边界检查了, 因为在到达边界之前一定能找到插入的位置, 之后所有以`unguarded`开头的函数都是不进行边界检查而是依赖调用它的函数保证了不会越过边界.

```cpp
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

```

下面的`__unguarded_insertion_sort`函数就是如此, 不进行边界检查, 而是不断将元素插入到前面的有序区间中

```cpp
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
```

# 堆排序

接下来是堆排序, 堆排序使用了之前`heap.h`中的一系列堆相关的函数

`__partial_sort`函数, 利用堆, 找出`[first, last)`中前`middle-first`小的元素, 并将他们有序放到`[first, middle)`中

原理是在`[first, middle)`建立一个大小为`middle-first`的堆, 然后不断将后面的元素与堆顶元素比较, 如果后面的元素小于堆顶元素, 后面的元素和堆顶互换位置, 并调整堆; 最后再将`[first, middle)`使用`sort_heap`转换成有序

```cpp
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
```

堆相关的函数介绍见[heap与priority_queue](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/heap与priority_queue.md), 下面列出使用到的相关函数

```cpp
// 将value填入holeindex所在位置, 并调整堆使其符合性质
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

// 将[first, last)堆最大元素变为value, 并将原来的堆顶元素放入result中
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


// 将heap转换成递增序列
template <typename RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, 
                   RandomAccessIterator last)
{
    while(last-first > 1)
        pop_heap(first, last--);
}

```

# 快速排序

接下来就是快速排序了, 这里的快速排序排序大致有序(区间小于等于16)后便停止, 如果递归深度过大(大于$\log_2^n+1$), 就改用堆排序

除此之外, 还使用了一个优化方法, 减少了函数调用次数; 

一般的快速排序写法, 是对区间调用分分割函数, 将区间分为L和R两个区间, 然后分别递归对L和R进行排序;

而这里的写法, 使用了一个循环, 当区间长度大于16时, 对区间使用分割函数分成L和R, 对R递归排序, 而对于L, 则将其作为新的区间继续循环, 这样减少了一半的函数调用

对于分割函数, 这里使用了区间头, 区间中间, 区间末尾三个元素的中位数作为枢纽, 进行分割

```cpp
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
```

# 最终的排序

首先调用快速排序, 将其排至大致有序, 然后调用插入排序`__final_insertion_sort`, 将其完全排序

```cpp
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
```



```cpp
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
```

# 测试

对比没有任何优化的快速排序

```cpp
int* partition(int *l, int *r)
{
    int pivot = *(r-1);
    while(true)
    {
        while(*l < pivot) ++l;
        --r;
        while(pivot < *r) --r;
        if(!(l<r)) return l;
        swap(*l, *r);
        ++l;
    }   
}
void quick_sort(int *l, int *r)
{
    if(l < r)
    {
        int * m = partition(l, r);
        quick_sort(l, m);
        quick_sort(m+1, r);
    }
}
```

对于随机的1e8的数据

```cpp
for(int i=0; i<len; ++i) a1[i] = a2[i] = rand();
int begin_time = clock();
ltx::sort(a1, a1+len);
cout << clock() - begin_time << endl;
begin_time = clock();
quick_sort(a2, a2+len);
cout << clock() - begin_time << endl;
```

耗时分别是42738和48736, 相差无几有点尴尬

但对于已经有序的数组, intro排序效率提升了一半左右, 而朴素的快速排序, 由于枢纽选取的原因, 每次都是将长度为n的区间分割为长度为n-1和1的区间, 直接退化成了n^2

如果将快排的枢纽选取加入随机性

```cpp
int* partition(int *l, int *r)
{
    int x = rand()%(r-l);
    swap(*r, *(l+x));
    int pivot = *(r-1);
    while(true)
    {
        while(*l < pivot) ++l;
        --r;
        while(pivot < *r) --r;
        if(!(l<r)) return l;
        swap(*l, *r);
        ++l;
    }   
}
```

在进行比较, Intro排序和新的快排比较, 对于随机数组, Intro快一倍左右, 而对于已经有序的数组, Intro快了40倍
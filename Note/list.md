# list

SGI的`list`实现太漂亮了(特别是链表的排序), 单独一篇来记录

STL的`list`是一个双向链表, 并且定义了一个`node`节点, 作为一个特殊的节点,, `node->next`是链表第一个节点, 链表最后一个节点指向`node`, 所以它还是一个循环链表, 这样的实现简化了代码

`list`有一个`transfer(postion, first, last)`方法, 用于将一节链表从它所属的`list`中取出, 并接到一个链表(可以是同一个也可以是不同一个链表)的指定位置去, 之后的`splice`, `merge`, `reverse`都利用了这个方法来简化代码

## transfer

代码注释的很详细了, 实现也很简单, 就是指针有点烦

```cpp
// 将[first, last) 移动到position之前
void transfer(iterator position, iterator first, iterator last)
{
    if(position != last)
    {
        // ...prefist->first->nextfirst->...->prelast->last...
        // ...prep->p->nextp...

        // 区间最后一个元素(prelast)的next为position
        (link_type((last.node)->prev))->next = position.node;

        // 区间第一个元素的前一个元素(prefirst)的next为last元素
        (link_type((first.node)->prev))->next = last.node;

        // position的前一个元素(prep)的下一个为first
        (link_type((position.node)->prev))->next = first.node;


        //至此所有next指针配置完成
        // tmp 为prep
        link_type tmp = link_type((position.node)->prev);

        // p的prev为prelast
        (position.node)->prev = (last.node)->prev;

        // last的prev为first的prev
        (last.node)->prev = (first.node)->prev;

        // first的prev为prep
        (first.node)->prev = tmp;
    }
}
```

## splice

用于将一个链表, 或者链表中的一段, 或者链表的一个节点接合本链表的指定位置

```cpp
// 将x接和于position之前, x必须不同于*this
void splice(iterator position, list & x)
{
    if(!x.empty())
    {
        transfer(position, x.begin(), x.end());
    }
}

//将i所指元素接和于postion之前
void splice(iterator position, list&, iterator i)
{
    iterator j = i;
    ++j;
    if(position == i || position == j) return ;
    transfer(position, i, j);
}

// 将[first, last), 接和position之前
void splice(iterator position, list&, iterator first, iterator last)
{
    if(first != last)
    {
        transfer(position, first, last);
    }
}
```

## merge

合并两个有序链表, 实现很简单

```cpp
// 合并两个有序递增的链表
void merge(list<T, Alloc>& x)
{
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end();

    iterator next;
    while(first1!=last1 && first2!=last2)
    {
        if(*first2 < *first1)
        {
            next = first2;
            transfer(first1, first2, ++next);
            first2 = next;
        }
        else ++first1;
    }
    if(first2 != last2) transfer(last1, first2, last2);
}
```

## reverse

将链表翻转, 和我平时写的不太一样, 实现很优雅

平时我一般就是找第一个和最后一个元素, 不断交换然后向中间移动

这个的实现是, 从链表第二个节点开始一直遍历到最后一个节点, 不断将当前节点移动到链表的最开头

```cpp
// 将链表翻转
void reverse()
{
    if(node->next == node || link_type(node->next)->next==node) return ;
    iterator first = begin();
    ++first;
    // 从第二个节点开始, 不断将节点移动到链表最前端
    while(first != end())
    {
        iterator old = first;
        ++first;
        transfer(begin(), old, first);
    }
}
```

## sort

排序函数, 实现的很有趣

`carry`是一个中间变量, 用来临时存储一段链表

`counter[i]`最多存储$2^i$长度的链表, 当`counter[i]`中的链表达到$2^i$时, 就利用`carry`将`counter[i]` `merge`到`counter[i+1]`里面去

具体实现看代码

```cpp
// 排序
void sort()
{
    if(node->next == node || link_type(node->next)->next==node) return ;

    list<T, Alloc> carry; // 中间变量
    list<T, Alloc> counter[64]; 
    // counter[i]容量为2^(i+1)个元素, 达到2^(i+1)个元素后
    // 将counter[i] merge到counter[i+1]
    // 由于每次从count[i-1]merge来的元素数量为2^(i), 为counter[i]容量的一半
    // 所以每次counter[i-1]每merge两次, counter[i]向上merge一次

    // fill 为第一个没有使用的counter
    int fill = 0;
    while(!empty())
    {
        // 将链表第一个元素放到carry中
        carry.splice(carry.begin(), *this, begin());

        // 如果i已到达没有用过的节点
        // 或则counter[i]为空, 直接由循环后面的swap语句将carry放到相应的位置
        int i = 0;
        while(i<fill && !counter[i].empty())
        {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
            // counter[i]不为空(由循环进入条件保证)
            // 又merge了一个carry, 所以需要向counter[i+1]merge
            // 如果counter[i+1]为空, 将由循环后面的语句将carry放到counter[i+1]处
            // 否则接着在循环中不断向上merge
        }
        carry.swap(counter[i]);
        if(i == fill) ++fill;
    }
    // 外层循环最多循环n次, 内层最多循环log(n)次
    // 内层循环merge复杂度最多为2^i
    // 复杂度大概是nlog(n)吧

    for (int i=1; i<fill; ++i)
        counter[i].merge(counter[i-1]);
    swap(counter[fill-1]);
}
```

## 其它方法

都是些简单的链表操作, 略
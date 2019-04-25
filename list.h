#ifndef LIST_H
#define LIST_H

#include "stl_iterator.h"
#include "memory.h"

namespace ltx
{
    template <typename T>
    struct __list_node
    {
        typedef void * void_pointer;
        void_pointer prev;
        void_pointer next;
        T data;
    };

    // list迭代器
    template <typename T, typename Ref, typename Ptr>
    struct __list_iterator
    {
        typedef __list_iterator<T, T&, T*> iterator;
        typedef __list_iterator<T, Ref, Ptr> self;

        typedef bidirectional_iterator_tag iterator_category;

        typedef T                   value_type;
        typedef Ptr                 pointer;
        typedef Ref                 reference;
        typedef __list_node<T>*     link_type;
        typedef size_t              size_type;
        typedef ptrdiff_t           difference_type;


        link_type node;

        __list_iterator(link_type x) : node(x) {}
        __list_iterator() {}
        __list_iterator(const iterator &x) : node(x.node) {}
        

        bool operator== (const self& x) const { return node == x.node; }
        bool operator!= (const self& x) const { return node != x.node; }

        reference operator* () const { return (*node).data; }
        pointer operator-> () const { return &(operator*()); }
        
        self& operator++()
        {
            node = (link_type)((*node).next);
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }

        self& operator--()
        {
            node = (link_type)((*node).prev);
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            --*this;
            return tmp;
        }
    };
    
    template <typename T, typename Alloc = alloc>
    class list
    {
    protected:
        typedef __list_node<T>      list_node;
    public:
        typedef list_node*          link_type;

        typedef T                   value_type;
        typedef value_type*         pointer;
        typedef value_type&         reference;
        typedef size_t              size_type;
        typedef ptrdiff_t           difference_type;

        
        typedef const value_type&         const_reference;

        typedef __list_iterator<T, T&, T*>  iterator;

    protected:
        typedef simple_alloc<list_node, Alloc> list_node_allocator;

    protected:
        link_type node;

    protected:
        // 配置一个节点
        link_type get_node() { return list_node_allocator::allocate(); }
        // 释放一个节点
        void put_node(link_type p) { list_node_allocator::deallocate(p); }

        // 配置并构造一个节点
        link_type create_node(const T&x)
        {
            link_type p = get_node();
            _construct(&p->data, x);
            return p;
        }
        // 销毁一个节点
        void destroy_node(link_type p)
        {
            _destroy(&p->data);
            put_node(p);
        }


    protected:
        // 空链表初始化
        void empty_initialize()
        {
            node = get_node();
            node->next = node;
            node->prev = node;
        }


    public:
        list() { empty_initialize(); }


    public:
        iterator begin() { return (link_type)((*node).next); }
        iterator end() { return node; }

        bool empty() const { return node->next == node; }

        size_type size() const 
        {
            size_type result = 0;
            distance(begin(), end(), result);
            return result;
        }
        
        // 返回第一个元素值的引用
        reference front() { return *begin(); }

        // 返回最后一个元素值的引用
        reference back() { return *(--end()); }

        // 插入一个元素到position后面
        iterator insert(iterator position, const T& x)
        {
            link_type tmp = create_node(x);
            tmp->next = position.node;
            tmp->prev = position.node->prev;
            (link_type(position.node->prev))->next = tmp;
            position.node->prev = tmp;
            return tmp;
        }

        void push_front(const T&x)
        {
            insert(begin(), x); 
        }
        void push_back(const T&x)
        {
            insert(end(), x);
        }

        // 删除一个元素并返回这个元素后一个元素的迭代器
        iterator erase(iterator position)
        {
            link_type next_node = link_type(position.node->next);
            link_type prev_node = link_type(position.node->prev);
            prev_node->next = next_node;
            next_node->prev = prev_node;
            _destroy(position.node);
            return iterator(next_node);
        }

        void pop_front() { erase(begin()); }
        void pop_back()
        {
            iterator tmp = end();
            erase(--tmp);
        }


        void clear()
        {
            link_type cur = (link_type) node->next;
            while(cur != node)
            {
                link_type tmp = cur;
                cur = (link_type) cur->next;
                _destroy(tmp);
            }
            node->prev = node;
            node->next = node;
        }

        // 清除数为value的元素
        void remove(const T& value)
        {
            iterator first = begin();
            iterator last = end();
            while(first != last)
            {
                iterator next = first;
                ++next;
                if(*first == value) erase(first);
                first = next;
            }
        }

        // 移除连续相同的元素
        void unique()
        {
            iterator first = begin();
            iterator last = end();
            if(first == last) return ;
            iterator next = first;
            while(++next != last)
            {
                if(*first == *next) erase(next);
                else first = next;
                next = first;
            }
        }

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

        // 交换两个链表
        void swap(list<T, Alloc>& x)
        {
            link_type tmp = node;
            node = x.node;
            x.node = tmp;
        }

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

    };

}

#endif
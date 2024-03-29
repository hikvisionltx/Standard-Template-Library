#ifndef RB_TREE_H
#define RB_TREE_H

#include "memory.h"
#include "stl_iterator.h"
#include <cstddef>

namespace ltx
{

    typedef bool __rb_tree_color_type;
    const __rb_tree_color_type __rb_tree_red = false;
    const __rb_tree_color_type __rb_tree_black = true;

    // 黑色为true, 红色false
    struct __rb_tree_node_base
    {
        typedef __rb_tree_color_type color_type;
        typedef __rb_tree_node_base* base_ptr;

        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr right;

        static base_ptr minimum(base_ptr x)
        {
            while(x->left != nullptr) x = x->left;
            return x;
        }
        static base_ptr maximum(base_ptr x)
        {
            while(x->right != nullptr) x = x->right;
            return x;
        }
    };

    template <typename Value>
    struct __rb_tree_node :public __rb_tree_node_base
    {
        typedef __rb_tree_node<Value>* link_type;
        Value value_field;
    };


    struct __rb_tree_base_iterator
    {
        typedef __rb_tree_node_base::base_ptr base_ptr;
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;

        base_ptr node;



        void increment()
        {
            if(node->right != nullptr)
            {
                // 有右子节点, 下一个节点为右子树最左节点
                node = node->right;
                while(node->left != nullptr) 
                    node = node->left;
            }
            else 
            {
                // 没有右子节点, 下一节点为向上走第一个不为右儿子的节点的父亲
                base_ptr y = node->parent;
                while(node == y->right)
                {
                    node = y;
                    y = y->parent;
                }
                if(node->right != y)    // 参见stl剖析P218, 下一节点为第一个不为右儿子的节点的父亲,
                    node = y;           // 如果当前节点为最后一个节点, 下一节点为header(end())
                // 当node->right == y时, 此时的情况是:当前节点为根节点, 且根节点没有右子节点(header->right==root)
                // 此时node==header, y==root
            }
        }

        void decrement()
        {
            // 如果此时节点为header(end()), 则下一节点为最大节点
            if(node->color == __rb_tree_red && node->parent->parent == node)
                node = node->right;
            else if(node->left != nullptr)
            {
                // 有右节点, 上一个节点为左树最大节点
                base_ptr y = node->left;
                while(y->right != nullptr)
                    y = y->right;
                node = y;
            }
            else 
            {
                // 没有左树, 向上找到第一个不为左儿子的节点, 它的父亲就是上一节点
                // 如果当前为最小节点, 则下一个为header(end())
                base_ptr y = node->parent;
                while(node == y->left)
                {
                    node = y;
                    y = y->parent;
                }
                node = y;
            }
        }
    };

    template <typename Value, typename Ref, typename Ptr>
    struct __rb_tree_iterator :public __rb_tree_base_iterator
    {
        typedef Value value_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
        typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
        typedef __rb_tree_iterator<Value, Ref, Ptr> self;
        typedef __rb_tree_node<Value> * link_type;

        __rb_tree_iterator() {}
        __rb_tree_iterator(link_type x) { node = x; }
        __rb_tree_iterator(const iterator & ite) { node = ite.node; }

        reference operator* () const { return link_type(node)->value_field; }
        pointer operator-> () const { return &(operator*()); }

        self& operator++() { increment(); return *this; }
        self operator++(int) 
        {
            self tmp = *this;
            increment();
            return tmp;
        }

        self& operator--() { decrement(); return *this; }
        self operator--(int)
        {
            self tmp = *this;
            decrement();
            return tmp;
        }

        
        inline bool operator==(const __rb_tree_base_iterator& y) const 
        {
            return this->node == y.node;
        }
        inline bool operator!=(const __rb_tree_base_iterator& y) const 
        {
            return this->node != y.node;
        }
    };

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, 
    typename Alloc = alloc>
    class rb_tree
    {
    protected:
        typedef void* void_pointer;
        typedef __rb_tree_node_base* base_ptr;
        typedef __rb_tree_node<Value> rb_tree_node;

        typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;

        typedef __rb_tree_color_type color_type;
    
    public:
        typedef Key keyvalue;
        typedef Value value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef __rb_tree_node<Value>* link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
    
    protected:
        link_type get_node()
        {
            return rb_tree_node_allocator::allocate();
        }
        void put_node(link_type p)
        {
            rb_tree_node_allocator::deallocate(p);
        }

        link_type create_node(const value_type & x)
        {
            link_type tmp = get_node();
            try
            {
                _construct(&tmp->value_field, x);
            }
            catch(...)
            {
                put_node(tmp);
            }
            return tmp;
        }

        link_type clone_type(link_type x)
        {
            link_type tmp = create_node(x->value_field);
            tmp->color = x->color;
            tmp->left = nullptr;
            tmp->right = nullptr;
            return tmp;
        }

        void destroy_node(link_type p)
        {
            _destroy(&p->value_field);
            put_node(p);
        }

        size_type node_count;
        link_type header;
        Compare key_compare;

        link_type& root() const { return (link_type&) header->parent; }
        link_type& leftmost() const { return (link_type&) header->left; }
        link_type& rightmost() const { return (link_type&) header->right; }

        static link_type& left(link_type x)
        {
            return (link_type&)(x->left);
        }
        static link_type& right(link_type x)
        {
            return (link_type&)(x->right);
        }
        static link_type& parent(link_type x)
        {
            return (link_type&)(x->parent);
        }
        static reference value(link_type x)
        {
            return x->value_field;
        }
        static const Key& key(link_type x)
        {
            return KeyOfValue()(value(x));
        }
        static color_type& color(link_type x)
        {
            return (color_type&)(x->color);
        }

        // baseptr
        static link_type& left(base_ptr x)
        {
            return (link_type&)(x->left);
        }
        static link_type& right(base_ptr x)
        {
            return (link_type&)(x->right);
        }
        static link_type& parent(base_ptr x)
        {
            return (link_type&)(x->parent);
        }
        static reference value(base_ptr x)
        {
            return ((link_type)x)->value_field;
        }
        static const Key& key(base_ptr x)
        {
            return KeyOfValue()(value(link_type(x)));
        }
        static color_type& color(base_ptr x)
        {
            return (color_type&)(x->color);
        }

        static link_type minimum(link_type x)
        {
            return (link_type) __rb_tree_node_base::minimum(x);
        }
        static link_type maximum(link_type x)
        {
            return (link_type) __rb_tree_node_base::maximum(x);
        }
        
    public:
        typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
        typedef __rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

    private:
        // x为新值插入点, y为插入点的父节点, v为新值
        iterator __insert(base_ptr x_, base_ptr y_, const value_type& v)
        {
            link_type x = (link_type) x_;
            link_type y = (link_type) y_;
            link_type z;

            if(y == header || x != nullptr || key_compare(KeyOfValue()(v), key(y)))
            {
                z = create_node(v);
                left(y) = z;
                if(y == header)
                {
                    root() = z;
                    rightmost() = z;
                }
                else if(y == leftmost())
                {
                    leftmost() = z;
                }
            }
            else 
            {
                z = create_node(v);
                right(y) = z;
                if(y == rightmost()) rightmost() = z;
            }
            parent(z) = y;
            left(z) = nullptr;
            right(z) = nullptr;

            __rb_tree_rebalance(z, header->parent);
            ++node_count;
            return iterator(z);
        }

        link_type __copy(link_type x, link_type p);

        // 删除而不重新平衡
        void __earse(link_type x)
        {
            while(x != nullptr)
            {
                __earse(right(x));
                link_type y = left(x);
                destroy_node(x);
                x = y;
            }
        }


        void init()
        {
            header = get_node();
            color(header) = __rb_tree_red;
            root() = 0;
            leftmost() = header;
            rightmost() = header;
        }

    public:
        rb_tree(const Compare& comp = Compare())
            :node_count(0), key_compare(comp) { init(); }
        
        // clear 定义
        ~rb_tree()
        {
            // clear();
            put_node(header);
        }

        void clear()
        {
            if(node_count != 0)
            {
                __earse(root());
                leftmost() = header;
                root() = nullptr;
                rightmost = header;
                node_count = 0;
            }
        }

        rb_tree<Key,Value, KeyOfValue, Compare, Alloc>& 
        operator=(const rb_tree<Key,Value, KeyOfValue, Compare, Alloc>& x)
        {
            if (this != &x) {
                                            // Note that Key may be a constant type.
                clear();
                node_count = 0;
                key_compare = x.key_compare;        
                if (x.root() == nullptr) 
                {
                    root() = nullptr;
                    leftmost() = header;
                    rightmost() = header;
                }
                else 
                {
                    root() = copy(x.root(), header);
                    leftmost() = minimum(root());
                    rightmost() = maximum(root());
                    node_count = x.node_count;
                }
            }
            return *this;
        }

        // void swap(_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& t) 
        // {
        //     swap(header, t.header);
        //     swap(node_count, t.node_count);
        //     swap(key_compare, t.key_compare);
        // }
        

    public:
        Compare key_comp() const { return key_compare; }
        iterator begin() { return leftmost(); }
        iterator end() { return header; }
        bool empty() { return node_count == 0; }
        size_type size() const { return node_count; }
        size_type max_size() const { return size_type(-1); }

    public:
        // 插入新值, 不允许重复, 返回新节点迭代器和插入成功与否的bool值
        pair<iterator, bool> insert_unique(const value_type& v)
        {
            link_type y = header;
            link_type x = root();
            bool comp = true;
            while (x != nullptr)
            {
                y = x;
                comp = key_compare(KeyOfValue()(v), key(x));
                x = comp ? left(x) : right(x);
            }

            iterator j = iterator(y);
            if(comp) // v小于y节点
            {
                if(j == begin()) // 如果y是最小的了, 直接插入y左边
                    return pair<iterator, bool>(__insert(x, y, v), true);
                else // 否则还要看比y小一个的那个节点是否和v相等
                    --j;
            }
            if(key_compare(key(j.node), KeyOfValue()(v))) // j小于v
                return pair<iterator, bool>(__insert(x, y, v), true);
            return pair<iterator, bool>(j, false);
        }

        // 插入新值, 允许重复, 返回新节点迭代器
        iterator insert_equal(const value_type& v)
        {
            link_type y = header;
            link_type x = root();
            while(x != nullptr)
            {
                y = x;
                x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
            }
            return __insert(x, y, v);
        }

        iterator insert_unique(const value_type* first, const value_type* last)
        {
            for(; first!=last; ++first)
                insert_unique(*first);
        }
        iterator insert_equal(const value_type* first, const value_type* last)
        {
            for(; first=last; ++first)
                insert_equal(*first);
        }
        
        // 删除指定的键值
        inline size_type erase(const Key& x)
        {
            pair<iterator, iterator> p = equal_range(x);
            size_type n = distance(p.first, p.second);
            erase(p.first, p.second);
            return n;
        }

        // 删除迭代器指向的节点
        inline void erase(iterator position)
        {
            link_type y = (link_type) __rb_tree_rebalance_for_erase(
                position.node,
                header->parent,
                header->left,
                header->right
            );
            destroy_node(y);
            --node_count;
        }

        // 删除迭代器区间
        inline void erase(iterator first, iterator last)
        {
            if(first == begin() && last == end()) clear();
            else 
                while(first != last) erase(first++);
        }

        // 删除键值数组中所有键值
        void erase(const Key* first, const Key* last)
        {
            while(first!=last) erase(*first++);
        }

        // 查找是否有键值为k的节点, 并返回迭代器
        iterator find(const Key&k)
        {
            link_type y = header;
            link_type x = root();

            while(x != nullptr)
            {
                if(!key_compare(key(x), k)) 
                {
                    // x大于等于k
                    y = x;
                    x = left(x);
                }
                else x = right(x);
            }
            iterator j = iterator(y);
            return (j==end() || key_compare(k, key(j.node))) ? end() : j;
        }

                
        size_type count(const Key& k) 
        {
            pair<iterator, iterator> p = equal_range(k);
            size_type n = distance(p.first, p.second);
            return n;
        }


        // 第一个大于等于k的迭代器
        iterator lower_bound(const Key & k)
        {
            link_type y = header;
            link_type x = root();

            while(x != 0)
            {
                if(!key_compare(key(x), k))
                {
                    y = x;
                    x = left(x);
                }
                else x = right(x);
            }
            return iterator(y);
        }

        // 第一个大于k的迭代器
        iterator upper_bound(const Key& k)
        {
            link_type y = header;
            link_type x = root();

            while(x != 0)
            {
                if(key_compare(k, key(x))) // x大于k
                {
                    y = x;
                    x = left(x);
                }
                else x = right(x);
            }
            return iterator(y);
            
        }

        // 返回迭代器对, 表示等于k的左开右闭区间
        pair<iterator, iterator> equal_range(const Key& k)
        {
            return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
        }
    };


    // 左旋
    inline void 
    __rb_tree_rotate_left(  __rb_tree_node_base* x, 
                            __rb_tree_node_base* &root)
    {
        // y为x右节点
        __rb_tree_node_base* y = x->right;
        // x右儿子为y左儿子
        x->right = y->left;
        if(y->left != nullptr)
        {
            y->left->parent = x;
        }
        // y的par为x的par
        y->parent = x->parent;
        if(x == root) root = y;
        else if(x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;

        // x的par为y
        y->left = x;
        x->parent = y;
    }

    inline void 
    __rb_tree_rotate_right( __rb_tree_node_base* x, 
                            __rb_tree_node_base*& root)
    {
        __rb_tree_node_base* y = x->left;
        x->left = y->right;
        if(y->right != nullptr)
        {
            y->right->parent = x;
        }

        y->parent = x->parent;
        if(x == root) root = y;
        else if(x == x->parent->right)
            x->parent->right = y;
        else x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    inline void 
    __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {
        x->color = __rb_tree_red;
        // 父节点为红色
        while(x != root && x->parent->color == __rb_tree_red)
        {
            // 父节点为祖父的左儿子
            if(x->parent == x->parent->parent->left)
            {
                __rb_tree_node_base* y = x->parent->parent->right; // 伯父
                if(y!=nullptr && y->color == __rb_tree_red) // 伯父红色, 情况1
                {
                    // 父亲伯父变黑
                    x->parent->color = __rb_tree_black;
                    y->color = __rb_tree_black;
                    // 祖父变红
                    x->parent->parent->color = __rb_tree_red;
                    // 当前节点变成祖父, 继续迭代
                    x = x->parent->parent;
                }
                else // 无伯父或者伯父为黑色 情况2, 3
                {
                    if(x == x->parent->right) // 新节点为右儿子, 情况2
                    {
                        // 以x父亲为左旋点左旋
                        x = x->parent;
                        __rb_tree_rotate_left(x, root);
                    }
                    // 改变父亲和祖父颜色
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    // 祖父右旋
                    __rb_tree_rotate_right(x->parent->parent, root);
                }
            }
            else // 父亲为右节点, 和上面对称
            {
                __rb_tree_node_base* y = x->parent->parent->right; // 伯父
                if(y!=nullptr && y->color == __rb_tree_red) // 伯父红色, 情况1
                {
                    // 父亲伯父变黑
                    x->parent->color = __rb_tree_black;
                    y->color = __rb_tree_black;
                    // 祖父变红
                    x->parent->parent->color = __rb_tree_red;
                    // 当前节点变成祖父, 继续迭代
                    x = x->parent->parent;
                }
                else // 无伯父或者伯父为黑色 情况2, 3
                {
                    if(x == x->parent->left) // 新节点为右儿子, 情况2
                    {
                        // 以x父亲为左旋点左旋
                        x = x->parent;
                        __rb_tree_rotate_right(x, root);
                    }
                    // 改变父亲和祖父颜色
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    // 祖父右旋
                    __rb_tree_rotate_left(x->parent->parent, root);
                }
            }
        }
        // 循环结束
        root->color = __rb_tree_black;
    }

    inline __rb_tree_node_base*
    __rb_tree_rebalance_for_erase(__rb_tree_node_base* z,
                                  __rb_tree_node_base*& root,
                                  __rb_tree_node_base*& leftmost,
                                  __rb_tree_node_base*& rightmost)
    {
        // y是实际要删掉的节点, x是y被删掉后接替y的节点
        __rb_tree_node_base* y = z,  *x = nullptr, *x_parent = nullptr;
        if(y->left == nullptr) // 如果y左为空, 右儿子接替y(两个都为空的话nullptr接替它)
        {
            x = y->right;
        }
        else if(y->right == nullptr) // 左儿子不为空, 右儿子为空
        {
            x = y->left;
        }
        else // 两儿子都不为空, 将z的后继节点赋给z, 然后删除z的后继节点
        {
            // 找到z的后继节点
            y = y->right;
            while(y->left != nullptr)
            {
                y = y->left;
            }
            // 此时y为z后继节点, 也就是我们要删除的节点
            // 这种情况y一定是左儿子为空
            x = y->right;
        }
        // 上面三种情形, y都一定至少有一个儿子为空, 记住这个

        // 要删除的y是z的后继节点
        // 用y替换z, 然后让y指向原来的z
        if(y != z)
        {
            // z的左儿子作为y的左儿子(y肯定没有左儿子)
            z->left->parent = y;
            y->left = z->left;

            // x接替要被删除的节点的位置
            // if(y != z->left)
            if(y != z->right)
            {
                x_parent = y->parent;
                if(x != nullptr) x->parent = y->parent;
                y->parent->left = x;
                y->right = z->right;
                z->right->parent = y;
            }
            else x_parent = y; // 如果y是z左儿子, 
                    // 由于之后会用y指向的节点代替z, 然后y指向z, 再删除y指向的节点(原来的z)
                    // 所以x最后的父亲依旧是现在的y, 不需要重新链接

            if(root == z) root = y;
            else if(z->parent->left == z) z->parent->left = y;
            else z->parent->right = y;

            y->parent = z->parent;
            std::swap(y->color, z->color);
            y = z;
        }
        else // y==z, 要删除的就是z
        {
            x_parent = y->parent;
            if(x!=nullptr) x->parent = y->parent;
            if(root == z)
                root = x;
            else if(z->parent->left == z) z->parent->left = x;
            else z->parent->right = x;

            if(leftmost == z) 
            {
                if(z->right == nullptr) leftmost = z->parent;
                else leftmost = __rb_tree_node_base::minimum(x);
            }
            if(rightmost == z)
            {
                if(z->left == nullptr) rightmost = z->parent;
                else rightmost = __rb_tree_node_base::maximum(x);
            }
        }

        // 至此y指向要删除的节点, 已经脱离了树, 将y返回交由上一层函数进行析构释放处理
        // 而x接替了y的位置
        // 如果y的颜色是黑色, 删除y后违背了rbtree的性质
        if(y->color != __rb_tree_red)
        {
            // 我们假设x有两种颜色, 被删除节点原来的颜色+x的颜色(黑色)
            // 这样就不会违背黑色节点数目相同的原则
            while(x!=root && (x==nullptr || x->color == __rb_tree_black))
            {
                if(x == x_parent->left)
                {
                    // x的兄弟节点
                    __rb_tree_node_base* w = x_parent->right;

                    // x的兄弟为红色, 情况1
                    // 将w设置为黑色, x->par设置为红色
                    // x->par 左旋
                    // 重新设置w为x的兄弟
                    // 这样就将情况一转换成了兄弟节点为黑色的情况, 回到循环继续处理
                    if(w->color == __rb_tree_red)
                    {
                        w->color == __rb_tree_black;
                        x_parent->color = __rb_tree_red;
                        __rb_tree_rotate_left(x_parent, root);
                        w = x_parent->right;
                    }

                    // 兄弟节点为黑色, 并且兄弟节点两个子节点都为黑色(或者为空也行)
                    // 情况2, 把x和w的一层黑色提到父亲上面, 这样父亲就变成了两种颜色
                    // 而x变成了黑色, w变成了红色
                    // 然后下一轮继续处理父亲
                    if((w->left == nullptr || w->left->color == __rb_tree_black)
                    && (w->right == nullptr || w->right->color == __rb_tree_black)
                    )
                    {
                        w->color = __rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else 
                    {
                        // w右儿子为黑色, 左儿子为红色(没有进入上面的if而是来到了这个else, 一定不可能是两个黑色, 所以左儿子为红色)
                        // 将w变红色, w左儿子变黑色
                        // 再对w左旋, 重新设置w为x兄弟转换为情况4
                        if(w->right == 0 || w->right->color == __rb_tree_black)
                        {
                             if(w->left != nullptr) w->left->color = __rb_tree_black;
                             w->color = __rb_tree_red;
                             __rb_tree_rotate_right(w, root);
                             w = x_parent->right;
                        }

                        // 情况4, w右儿子为红色, 左儿子随意
                        // 将w变成父亲节点颜色, 父亲节点变成黑色, w右儿子变成黑色
                        // 然后对父亲左旋, 这时x这边多了一个黑色节点, 刚好弥补缺少的一个黑色
                        // 所有条件满足, 退出循环
                        w->color = x_parent->color;
                        x_parent->color = __rb_tree_black;

                        if(w->right != nullptr) w->right->color = __rb_tree_black;
                        
                        __rb_tree_rotate_left(x_parent, root);
                        break;
                    }
                }
                else // 和if一样, left和right交换
                {
                    __rb_tree_node_base* w = x_parent->left;
                    
                    if(w->color == __rb_tree_red)
                    {
                        w->color == __rb_tree_black;
                        x_parent->color = __rb_tree_red;
                        __rb_tree_rotate_right(x_parent, root);
                        w = x_parent->left;
                    }
                    
                    if((w->right == nullptr || w->right->color == __rb_tree_black)
                    && (w->left == nullptr || w->left->color == __rb_tree_black)
                    )
                    {
                        w->color = __rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else 
                    {
                        if(w->left == 0 || w->left->color == __rb_tree_black)
                        {
                             if(w->right != nullptr) w->right->color = __rb_tree_black;
                             w->color = __rb_tree_red;
                             __rb_tree_rotate_left(w, root);
                             w = x_parent->left;
                        }
                        w->color = x_parent->color;
                        x_parent->color = __rb_tree_black;

                        if(w->left != nullptr) w->left->color = __rb_tree_black;
                        
                        __rb_tree_rotate_right(x_parent, root);
                        break;
                    }
                }
            }
            if(x!=nullptr) x->color = __rb_tree_black;
        }
        return y;
    }
}


#endif
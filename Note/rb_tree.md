完整代码见: [Standard-Template-Library/rb_tree.h](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/rb_tree.h)

# 红黑树的基本性质

红黑树是SGI实现里面`set`, `map`的底层, 本质是一个二叉搜索树, 有以下5个性质

1. 每个结点要么是红的，要么是黑的
2. 根结点是黑的
3. 每个叶结点（叶结点即指树尾端NIL指针或NULL结点）是黑的
4. 如果一个结点是红的，那么它的俩个儿子都是黑的
5. 对于任一结点而言，其到叶结点树尾端NIL指针的每一条路径都包含相同数目的黑结点



# 红黑树的节点

```cpp
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
```

一个节点包含三个指针, 分别指向父亲, 左儿子, 右儿子, 和一个颜色变量, 还有他的值变量



# 迭代器

```cpp
    struct __rb_tree_base_iterator
    {
        typedef __rb_tree_node_base::base_ptr base_ptr;
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;

        base_ptr node;
        void increment();

        void decrement();
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

        __rb_tree_iterator() ;
        __rb_tree_iterator(link_type x) ;
        __rb_tree_iterator(const iterator & ite);

        reference operator* () const ;
        pointer operator-> () const ;

        self& operator++() ;
        self operator++(int) ;

        self& operator--();
        self operator--(int);

        
        inline bool operator==(const __rb_tree_base_iterator& y) const;
        inline bool operator!=(const __rb_tree_base_iterator& y) const;
    };
```

迭代器这里比较简单, 使用一个`base_ptr`(`__rb_tree_node_base::base_ptr`: rb_tree节点基类指针)关联到一个红黑树节点

其中`increment`用于找到当前迭代器在红黑树中的下一各节点, `decremetn`相反, 

```cpp
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
        node = node->left;
        while(node->right != nullptr)
            node = node->right;
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
```

# rb_tree类

`rb_tree`内使用一个`header`指针, 它的父亲是红黑树的树根`root`, 颜色为红色, 而`root`的父亲是`header`

`header`的左儿子代表红黑树中第一个节点(`begin()`), 右儿子代表最后一个节点`--end()`

模版参数中, `Key`为键类型, `Value`代表值类型, `KeyOfValue`是一个仿函数, 用于获取`Value`的键 `Compare`用于键之间的比较

```cpp
template <typename Key, typename Value, typename KeyOfValue, typename Compare, 
typename Alloc = alloc>
class rb_tree
{
    size_type node_count;
    link_type header;
    Compare key_compare;
};
```



# 红黑树中的操作

## 左旋右旋

![](https://raw.githubusercontent.com/LiTianxiong/Pictures/master/20190508135649.png)

```cpp
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
```



## 插入

根据二叉搜索树的性质, 找到要插入的位置, 将节点插入

```cpp
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
```

插入之后会违背红黑树的性质, 需要进行调整, 这里就比较复杂了

记插入节点为z

对于z, 我们将他的颜色设置为红色

如果z的父亲为黑色, 或则z为根节点, 则无需调整就符合条件

否则, 需要进行调整

分三种情况(一下情况都是z的父亲节点为祖父的左儿子时的情况, 为右儿子时情况与下面的对称)

1. z的叔节点y为红色

   将z的爷爷变红色, z的父亲和叔叔边黑色, 如下图, 这样c节点往下就全部符合了

   如果c节点的父亲为黑色, 则符合条件, 否则如果c的父亲为红色, 则违背`红色节点儿子为黑色`的条件, 需要将C作为当前节点, 继续调整

   ![](https://raw.githubusercontent.com/LiTianxiong/Pictures/master/20190508135722.png)

2. z的叔叔为黑色, 且z为右儿子

3. z的叔叔为黑色, 且z为左儿子

   情况2, 3如下图所示

   ![](https://raw.githubusercontent.com/LiTianxiong/Pictures/master/20190508135739.png)

   对于情况2, 对z父亲左旋将其转换成情况3

   对于情况3, 将z改变父亲和祖父颜色, 对祖父进行右旋, 至此红黑树性质全部满足

   

```cpp
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
            //....
        }
    }
    // 循环结束
    root->color = __rb_tree_black;
}
```

## 删除

如果要删除的节点z没有儿子, 则将z删除, 用空指针接替它

如果z有一个儿子, 则将z删除, 用那一个儿子接替它

如果z有两个儿子, 则找到z的后继节点, 把用后继节点的内容赋给z, 然后删除后继节点

如果被删除的节点是黑色, 则会导致红黑树性质违背, 需要进行调整

假设顶替*被删除节点y*位置的节点为x, y可能是z(没有儿子或者只有一个儿子), y也可能是z的后继节点(有两个儿子)

如果y的颜色是黑色, 删除y后违背红黑树性质, 那么我们可以给x赋予两种颜色, 一种是x本身的颜色, 一种是y的颜色, 这样就可以保证`每条路径黑色节点数量一样`这条性质不被违背, 但违背了`每个节点只能是红色或者黑色`

分三种情况:

1. x为红+黑, 直接将x设置为黑色, 恢复
2. x为黑+黑, x为树根, 这时候减去一个黑色相当于所有路径都减去一个黑色, 恢复
3. x为黑+黑色, 且x不为树根, 这是就比较复杂了, 分4中情况

情况3中的4中情况(一下都是x为左儿子时的情况, 为右儿子时情况对称):

1. x的兄弟节点w为红色

2. ![](https://raw.githubusercontent.com/LiTianxiong/Pictures/master/20190508135800.png)

   如图, 将x的父亲和x的兄弟改变颜色, 然后对父亲左旋, 将情况转换成了兄弟结点颜色为黑的情况(情况2, 3, 4)

3. x的兄弟节点w为黑色, 且w两个儿子都为黑色

   ![](https://raw.githubusercontent.com/LiTianxiong/Pictures/master/20190508135817.png)

   如图, 这时我们将x和w的一层黑色提到父亲身上, 这时父亲变成了有两种颜色的节点, 然后以父亲节点为但当前节点继续处理

4. x兄弟节点w为黑色, w左儿子红色, 右儿子黑色

   ![](https://raw.githubusercontent.com/LiTianxiong/Pictures/master/20190508135841.png)

   这时候, 我们改变w和它做孩子的颜色, 并对w右旋, 变为情况4

5. x兄弟节点w为黑色, w右儿子为红色, 左儿子随意

   ![](https://raw.githubusercontent.com/LiTianxiong/Pictures/master/20190508135857.png)

   如图, 这是我们将w变成父亲节点的颜色, 将父亲节点和w的右儿子变成黑色, 再对父亲节点进行左旋, 此时可以发现x节点这条路径多了一个黑色, 填补了删除y少去的那个黑色, 至此所有性质满足

   

```cpp
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
                    //...
                }
            }
            if(x!=nullptr) x->color = __rb_tree_black;
        }
        return y;
    }
```

## 查找

查找比较简单, 利用二叉搜索树的性质即可

```cpp
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
```

# 参考

<https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/03.01.md>

https://blog.csdn.net/kangroger/article/details/38587785

《算法导论》：

《STL源码剖析》
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
    };
}


#endif
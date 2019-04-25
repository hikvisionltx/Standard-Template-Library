#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "vector.h"
#include "heap.h"

namespace ltx
{
    template <typename T, typename Sequence = vector<T>,
        typename Compare = less<typename Sequence::value_type>>
    class priority_queue
    {
    public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef typename Sequence::const_reference const_reference;

    protected:
        Sequence c;
        Compare comp;
    public:
        priority_queue() : c() {}
        explicit priority_queue(const Compare& x) : c(), comp(x) {}

        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }
        const_reference top() const { return c.front(); }
        void push(const value_type& x)
        {
            try
            {
                c.push_back(x);
                push_heap(c.begin(), c.end(), comp);
            }
            catch(...)
            {
                c.clear();
                throw;
            }
        }
        void pop()
        {
            try
            {
                pop_heap(c.begin(), c.end(), comp);
                c.pop_back();
            }
            catch(...)
            {
                c.clear();
                throw;
            }
            
        }

    };
}

#endif
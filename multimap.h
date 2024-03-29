#ifndef MULTIMAP_H
#define MULTIMAP_H

#include "rb_tree.h"

namespace ltx
{
    template <typename Key, typename T, 
            typename Compare = less<Key>,
            typename Alloc = alloc>
    class multimap
    {
    public:
        typedef Key key_type;
        typedef T data_type;
        typedef T mapped_type;
        typedef pair<const Key, T> value_type;
        typedef Compare key_compare;

        class value_compare :  public binary_function<value_type, value_type, bool>
        {
            friend class multimap<Key, T, Compare, Alloc>;
        protected:
            Compare comp;
            value_compare(Compare c) :comp(c) {}
        public:
            bool operator()(const value_type& x, const value_type& y)const
            {
                return comp(x.first, y.first);
            }
        };

    private:
        template <class _Pair>
        struct _Select1st : public unary_function<_Pair,
                               typename _Pair::first_type>
        {
            typename _Pair::first_type&
            operator()(_Pair& __x) const
            { return __x.first; }

            const typename _Pair::first_type&
            operator()(const _Pair& __x) const
            { return __x.first; }
        };
        typedef rb_tree<key_type, value_type, 
                    _Select1st<value_type>, key_compare, Alloc> rep_type;
        rep_type t;

    public:
        typedef typename rep_type::pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::reference reference;
        typedef typename rep_type::const_reference const_reference;
        typedef typename rep_type::iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;

        multimap() :t(Compare()) {}
        explicit multimap(const Compare& comp) :t(comp) {}

        template <typename InputIterator>
        multimap(InputIterator first, InputIterator last) 
            :t(Compare())
        { 
            t.insert_equal(first, last);
        }

        template <typename InputIterator>
        multimap(InputIterator first, InputIterator last, const Compare& comp) 
            :t(comp)
        { 
            t.insert_equal(first, last);
        }

        multimap(const multimap<Key, T, Compare, Alloc>& x) :t(x) {}
        multimap<Key, T, Compare, Alloc>& operator==(const multimap<Key, T, Compare, Alloc>& x)
        {
            t = x.t;
            return *this;
        }

        key_compare key_comp() const { return t.key_comp(); }
        value_compare value_comp() const { return value_compare(t.key_comp()); }

        iterator begin() { return t.begin(); }
        const_iterator begin() const { return t.begin(); }
        iterator end() { return t.end(); }
        const_iterator end() const { return t.end(); }

        bool empty() const { return t.empty(); }
        size_type size() const { return t.size(); }
        size_type max_size() const { return t.max_size(); }
        // void swap(set<Key, Compare, Alloc>& x) { t.swap(x.t); }

        iterator insert(const value_type& x)
        {
            return t.insert_equal(x);
        }
        template <typename InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            t.insert_equal(first, last);
        }

        void erase(iterator position) { t.erase(position); }
        size_type erase(const key_type& x) { return t.erase(x); }
        void erase(iterator first, iterator last) { t.erase(first, last); }
        void erase() { t.clear(); }

        iterator find(const key_type& x) { return t.find(x); }
        const_iterator find(const key_type& x) const {return t.find(x); }
        size_type count(const key_type& x) const { return t.count(x); }
        iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
        iterator upper_bound(const key_type& x) { return t.upper_bound(x); }

        pair<iterator, iterator> equal_range(const key_type& x) const 
        {
            return t.equal_range(x);
        }


    };
}

#endif

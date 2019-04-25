#ifndef STL_UNINITIALIZED_H
#define STL_UNINITIALIZED_H

#include <type_traits>
#include "stl_construct.h"
namespace ltx
{
    template <typename InputIterator, typename ForwardIterator>
    ForwardIterator
    uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
    {
        bool is_pod = std::is_pod<decltype(*first)>::value;
        if(is_pod)
        {
            return copy(first, last, result);
        }
        else 
        {
            ForwardIterator cur = result;
            for(; first!=last; ++first, ++cur)
            {
                _construct(&*cur, *first);
            }
            return cur;
        }
    }

    template <typename InputIterator, typename ForwardIterator, typename T>
    inline void uninitialized_copy_fill(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2,
                            const T& x)
    {
        ForwardIterator mid2 = uninitialized_copy(first1, last1, first2);
        try
        {
            uninitialized_fill(mid2, last2, x);
        }
        catch(...)
        {
            _destroy(first2, mid2);
            throw;
        }
    }

    template <class ForwardIterator, class T, class InputIterator>
    inline ForwardIterator 
    uninitialized_fill_copy(ForwardIterator result, ForwardIterator mid,
                                const T& x,
                                InputIterator first, InputIterator last)
    {
        uninitialized_fill(result, mid, x);
        try
        {
            return uninitialized_copy(first, last, mid);
        }
        catch(...)
        {
            _destroy(result, mid);
            throw;
        }
    }


    inline char * uninitialized_copy(const char * first, const char * last, char * result)
    {
        memmove(result, first, last-first);
        return result+(last-first);
    }
    inline wchar_t * uninitialized_copy(const wchar_t * first, const wchar_t * last, wchar_t * result)
    {
        memmove(result, first, last-first);
        return result+(last-first);
    }

    template <typename ForwardIterator, typename T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T&x)
    {
        bool is_pod = std::is_pod<decltype(*first)>::value;
        if(is_pod)
        {
            fill(first, last, x);
        }
        else 
        {
            ForwardIterator cur = first;
            for(; cur!=last; ++cur)
            {
                _construct(&*cur, x);
            }
        }
    }

    template <typename ForwardIterator, typename Size, typename T>
    ForwardIterator
    uninitialized_fill_n(ForwardIterator first, Size n, const T&x)
    {
        bool is_pod = std::is_pod<decltype(*first)>::value;
        if(is_pod)
        {
            return fill_n(first, n, x);
        }
        else 
        {
            ForwardIterator cur = first;
            for(; n>0; --n, ++cur)
            {
                _construct(&*cur, x);
            }
            return cur;
        }
    }



}


#endif
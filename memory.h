#ifndef MEMORY_H
#define MEMORY_H

#include "stl_construct.h"
#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_uninitialized.h"


namespace ltx
{
    typedef default_alloc alloc;
    template<class T>
    class Allocator
    {
    public:
        typedef T               value_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef T&              reference;
        typedef const T&        const_reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;

        template<class U>
        struct rebind
        {
            typedef Allocator<U> other;
        };

        pointer allocate(size_type n, const void* hint=0)
        {
            return (pointer) default_alloc::allocate((size_t)n*sizeof(value_type));
        }

        void deallocate(pointer p, size_type n)
        {
            default_alloc::deallocate(p, n);
        }

        void construct(pointer p, const T& value) 
        {
            _construct(p, value);
        }

        void destroy(pointer p) 
        {
            _destroy(p);
        }

        pointer address(reference x) 
        {
            return (pointer)&x;
        }

        const_pointer address(const_reference x) 
        {
            return (const_pointer)&x;
        }

        size_type max_size() const 
        {
            return size_type(UINT_MAX/sizeof(T));
        }
    };

    template <typename T, typename Alloc>
    class simple_alloc 
    {
    public:
        static T * allocate(size_t n)
        {
            return 0==n ? nullptr : (T*)Alloc::allocate(n*sizeof(T));
        }
        static T * allocate(void)
        {
            return (T*) Alloc::allocate(sizeof(T));
        }
        static void deallocate(T*p, size_t n)
        {
            if(0!=n) Alloc::deallocate(p, n*sizeof(T));
        }
        static void deallocate(T *p)
        {
            Alloc::deallocate(p, sizeof(T));
        }
    };
}

#endif
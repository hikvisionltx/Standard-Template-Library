#include "stl_construct.h"
#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_uninitialized.h"


namespace ltx
{
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
}

/*
* todo
* 添加 simple_alloc
*/
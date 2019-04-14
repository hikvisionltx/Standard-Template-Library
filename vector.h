#include "memory.h"

#include <cstddef>

#include <iostream>
using namespace std;
/*
* todo
* 将Alloc 改为使用simplealloc
*/
namespace ltx
{
    template <typename T, typename Alloc=default_alloc>
    class vector
    {
    public:
        typedef T               value_type;
        typedef value_type*     pointer;
        typedef value_type*     iterator;
        typedef value_type&     reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;
    public:
        void display()
        {
            for(int i=0; i<size(); ++i)
            {
                cout << (*this)[i] << " ";
            }
            cout << "\t\t----" << capacity() << endl;
        }

    protected:
        iterator start;
        iterator finish;
        iterator end_of_storage;
        

        void insert_aux(iterator position, const T&x)
        {
            // 还有空间
            if(finish != end_of_storage)
            {
                _construct(finish, *(finish-1));
                ++finish;
                T x_copy = x;
                copy_backward(position, finish-2, finish-1);
                *position = x_copy;
            }
            else  //没有空间
            {
                const size_type old_size = size();
                const size_type len = old_size!=0 ? 2*old_size : 1;
                
                iterator new_start = (iterator) Alloc::allocate(len*sizeof(T));
                cout << "new_start" << new_start << endl;
                iterator new_finish = new_start;
                try
                {
                    new_finish = uninitialized_copy(start, position, new_start);
                    _construct(new_finish, x);
                    ++new_finish;
                    for(auto x = new_start; x!=new_finish; ++x)
                    {
                        cout << *x << "-";
                    }
                    cout << endl;
                    cout << start <<" " << position << " " << new_start << endl;
                    new_finish = uninitialized_copy(position, finish, new_finish);
                    cout << new_start << " try " << new_finish-new_start << endl;
                                        for(auto x = new_start; x!=new_finish; ++x)
                    {
                        cout << *x << "-";
                    }
                    cout << endl;
                }
                catch(...)
                {
                    _destroy(new_start, new_finish);
                    Alloc::deallocate(new_start, len);
                    throw;
                }
                for(auto x=new_start; x!=new_finish; ++x)
                {
                    cout << *x << "++";
                }
                cout << endl;
                cout << new_start << " " << new_finish << " " <<
                begin() << " " << end() << endl;
                _destroy(begin(), end());
                deallocate();
                
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;   

                for(auto x=start; x!=finish; ++x)
                {
                    cout << *x << "+";
                }
                cout << endl;
            }
        }
        void deallocate()
        {
            if(start != nullptr) Alloc::deallocate(start, end_of_storage-start);
        }

        void fill_initialize(size_type n, const T& value)
        {
            start  = allocate_and_fill(n, value);
            finish = start + n;
            end_of_storage = finish;
        }

    public:
        iterator begin() { return start; }
        iterator end() { return finish; }
        size_type size() const { return size_type(finish-start); }
        size_type capacity() const { return size_type(end_of_storage - start); }
        bool empty() const { return start==finish; }
        reference operator[](size_type n) { return *(begin()+n); }

        vector() :start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
        vector(size_type n, const T&value) { fill_initialize(n, value); }
        vector(int n, const T& value) { fill_initialize(n, value); }
        vector(long n, const T& value) { fill_initialize(n, value); }
        explicit vector(size_type n) { fill_initialize(n, T()); }

        ~vector()
        {
            _destroy(start, finish);
            
            deallocate();
        }

        reference front() { return *begin(); }
        reference back() { return *(end()-1); }
        void push_back(const T&x)
        {
            if(finish != end_of_storage)
            {
                _construct(finish, x);
                ++finish;
            }
            else insert_aux(end(), x);
        }
        void pop_back()
        {
            --finish;
            _destroy(finish);
        }
        iterator erase(iterator position)
        {
            if(position+1 != end())
                copy(position+1, finish, position);
            --finish;
            _destroy(finish);
            return position;
        }   
        void resize(size_type new_size, const T&x)
        {
            if(new_size < size())
            {
                erase(begin()+new_size, end());
            }
            else insert(end(), new_size-size(), x);
        }
        void resize(size_type new_size) { resize(new_size, T()); }
        void clear() { erase(begin(), end()); }

    protected:
        iterator allocate_and_fill(size_type n, const T&x)
        {
            iterator result = (iterator) Alloc::allocate(n);
            uninitialized_fill_n(result, n, x);
            return result;
        }
    };
}
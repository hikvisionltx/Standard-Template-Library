#ifndef VECTOR_H
#define VECTOR_H

#include "memory.h"

#include <cstddef>

namespace ltx
{
    template <typename T, typename Alloc=alloc>
    class vector
    {
    public:
        typedef T               value_type;
        typedef value_type*     pointer;
        typedef value_type*     iterator;
        typedef value_type&     reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;

        
        typedef const value_type&     const_reference;

    protected:
        typedef simple_alloc<value_type, Alloc> data_allocator;

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
                
                iterator new_start = data_allocator::allocate(len);
                iterator new_finish = new_start;
                try
                {
                    new_finish = uninitialized_copy(start, position, new_start);
                    _construct(new_finish, x);
                    ++new_finish;
                    new_finish = uninitialized_copy(position, finish, new_finish);

                }
                catch(...)
                {
                    _destroy(new_start, new_finish);
                    Alloc::deallocate(new_start, len);
                    throw;
                }

                _destroy(begin(), end());
                deallocate();
                
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;   


            }
        }

        void deallocate()
        {
            if(start != nullptr) data_allocator::deallocate(start, end_of_storage-start);
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

        void insert(iterator position, size_type n, const T& x)
        {
            if(n!=0)
            {
                // 备用空间足够
                if(size_type(end_of_storage-finish) >= n)
                {
                    T x_copy = x;
                    const size_type elems_after = finish - position;
                    iterator old_finish = finish;
                    // 插入点后的元素数量大于新增元素个数, 将原有元素中的后n个使用uninitial copy后移
                    // 剩余需要后移的元素使用copy, 之后直接使用fill填充新元素
                    if(elems_after > n)
                    {
                        uninitialized_copy(finish-n, finish, finish);
                        finish += n;
                        copy_backward(position, old_finish-n, old_finish);
                        fill(position, position+n, x_copy);
                    }
                    else  
                    {
                        // 插入点后的元素小于等于新增元素个数
                        // 也是一样, 已经构造过得位置直接使用copy和fill
                        // 没有构造过的位置使用uninitial copy 和fill
                        uninitialized_fill_n(finish, n-elems_after, x_copy);
                        finish += n-elems_after;
                        uninitialized_copy(position, old_finish, finish);
                        finish += elems_after;
                        fill(position, old_finish, x_copy);
                    }
                }
                else 
                {
                    // 备用空间不足
                    const size_type old_size = size();
                    const size_t len = old_size + old_size>n ? old_size : n;
                    // 新长度等于旧长度*2 或者 旧长度+n
                    iterator new_start = data_allocator::allocate(len);
                    iterator new_finish = new_start;
                    try
                    {
                        new_finish = uninitialized_copy(start, position, new_start);
                        new_finish = uninitialized_fill_n(new_finish, n, x);
                        new_finish = uninitialized_copy(position, finish, new_finish);
                    }
                    catch(...)
                    {
                        _destroy(new_start, new_finish);
                        data_allocator::deallocate(new_start, len);
                        throw;
                    }

                    _destroy(start, finish);
                    deallocate();

                    start = new_start;
                    finish = new_finish;
                    end_of_storage = new_start + len;
                    
                }
            }
        }

    protected:
        iterator allocate_and_fill(size_type n, const T&x)
        {
            iterator result = (iterator) Alloc::allocate(n);
            uninitialized_fill_n(result, n, x);
            return result;
        }
    };
}

#endif
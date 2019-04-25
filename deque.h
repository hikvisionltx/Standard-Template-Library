#include "memory.h"
#include "stl_iterator.h"

namespace ltx
{
    // n不为0, 使用用户自定义的值, 否则如果元素大小大于等于512, 为1, 否则为512/sizeof(T)
    inline size_t __dequeue_buf_size(size_t n, size_t sz)
    {
        return n!=0 ? n : (sz<512 ? size_t(512/sz) : size_t(1));
    }

    template <typename T, typename Ref, typename Ptr, size_t BufSiz>
    struct __deque_iterator
    {
        typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
        typedef __deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
        static size_t buffer_size() 
        {
            return __dequeue_buf_size(BufSiz, sizeof(T)); // 全局函数
        }

        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef ptrdiff_t difference_type;

        typedef size_t size_type;
        typedef T** map_pointer;

        typedef __deque_iterator self;

        T* cur;
        T* first;
        T* last;
        map_pointer node;

        // 重新设置node
        void set_node(map_pointer new_node)
        {
            node = new_node;
            first = *new_node;
            last = first + difference_type(this->buffer_size());
        }

        __deque_iterator(T* x, map_pointer y)
            : cur(x), first(*y), last(*y + buffer_size()), node(y) {}
        __deque_iterator() : cur(0), first(0), last(0), node(0) {}
        __deque_iterator(const iterator& x)
            : cur(x.cur), first(x.first), last(x.last), node(x.node) {}
    
        reference operator*() const { return *cur; }

        difference_type operator-(const self& x) const
        {
            return difference_type(buffer_size()) * (node - x.node - 1) +
                (cur - first) + (x.last - x.cur);
        }
    
        self& operator++()
        {
            ++cur;
            if (cur == last) // 到达缓冲区尾部, 切换到下一个缓冲区
            {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }
    
        self& operator--()
        {
            if (cur == first)
            {
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }
    
        self operator--(int)
        {
            self tmp = *this;
            --*this;
            return tmp;
        }
    
        self& operator+=(difference_type n)
        {
            difference_type offset = n + (cur - first);
            // 同一缓冲区
            if (0<= offset && offset < difference_type(buffer_size()))
                cur += n;
            else // 不同缓冲区
            {
                difference_type node_offset =
                    offset > 0 ? offset / difference_type(buffer_size())
                    : -difference_type((-offset - 1) / buffer_size()) - 1;
                set_node(node + node_offset);
                cur = first + (offset - node_offset * difference_type(buffer_size()));
            }
            return *this;
        }
    
        self operator+(difference_type n) const
        {
            self tmp = *this;
            return tmp += n;
        }
    
        self& operator-=(difference_type n) { return *this += -n; }
    
        self operator-(difference_type n) const
        {
            self tmp = *this;
            return tmp -= n;
        }
    
        reference operator[](difference_type n) const { return *(*this + n); }
    
        bool operator==(const self& x) const { return cur == x.cur; }
        bool operator!=(const self& x) const { return !(*this == x); }
        bool operator<(const self& x) const
        {
            return (node == x.node) ? (cur < x.cur) : (node < x.node);
        }
        bool operator>(const self& x) const  { return x < *this; }
        bool operator<=(const self& x) const { return !(x < *this); }
        bool operator>=(const self& x) const { return !(*this < x); }
    };

    template <typename T, typename Alloc=alloc, size_t BufSiz=0>
    class deque
    {
    public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
    
    public:
        typedef __deque_iterator<T, T&, T*, BufSiz>       iterator;
    
    protected: 
    
        typedef pointer* map_pointer;

        typedef simple_alloc<value_type, Alloc> data_allocator;
        typedef simple_alloc<pointer, Alloc> map_allocator;
    
        // 缓冲区大小
        static size_type buffer_size()
        {
            return __dequeue_buf_size(BufSiz, sizeof(value_type));
            
        }
    
        static size_type initial_map_size() { return 8; }
    
    protected:
        iterator start;
        iterator finish;
    
        map_pointer map;
        size_type map_size;
    
    public:
        iterator begin() { return start; }
        iterator end() { return finish; }
        
        reference operator[](size_type n) { return start[difference_type(n)]; }
    
        reference front() { return *start; }
        reference back()
        {
            iterator tmp = finish;
            --tmp;
            return *tmp;
        }
    
    
        size_type size() const { return finish - start; }
        size_type max_size() const { return size_type(-1); }
        
        bool empty() const { return finish == start; }
    
    public:
        deque() : start(), finish(), map(0), map_size(0)
        {
            create_map_and_nodes(0);
        }

        ~deque()
        {
            _destroy(start, finish);
            destroy_map_and_nodes();
        }

    
    protected:
    // 元素构造析构分配释放系列

        // 配置deque结构
        void fill_initialize(size_type n, const value_type& value)
        {
            create_map_and_nodes(n);
            map_pointer cur;
            try 
            {
                // 为每个缓冲区设定初值
                for (cur = start.node; cur < finish.node; ++cur)
                    uninitialized_fill(*cur, *cur + buffer_size(), value);
                // 最后一个节点后面的备用空间不需要设置初值
                uninitialized_fill(finish.first, finish.cur, value);
            }
            catch(...) 
            {
                for (map_pointer n = start.node; n < cur; ++n)
                _destroy(*n, *n + buffer_size());
                destroy_map_and_nodes();
                throw;
            }
        }

        map_pointer allocate_map(size_type n) 
        {
            return map_allocator::allocate(n); 
        }
        void deallocate_map(map_pointer p, size_type n) 
        {
            map_allocator::deallocate(p, n); 
        }


        void create_map_and_nodes(size_type num_elements)
        {
            size_type num_nodes = num_elements / buffer_size() + 1;
            map_size = max(initial_map_size(), num_nodes + 2);
            
            map = map_allocator::allocate(map_size);
            
            // 令nstart和nfinish指向map中间区段
            map_pointer nstart = map + (map_size - num_nodes) / 2;
            map_pointer nfinish = nstart + num_nodes - 1;
            
            // 分配结点空间
            map_pointer cur;
            try
            {
                for (cur = nstart; cur <= nfinish; ++cur)
                *cur = allocate_node();
            }
            catch(...) 
            {
                for (map_pointer n = nstart; n < cur; ++n)
                    deallocate_node(*n);
                map_allocator::deallocate(map, map_size);
                throw;
            }
            
            start.set_node(nstart);
            finish.set_node(nfinish);
            start.cur = start.first;
            finish.cur = finish.first + num_elements % buffer_size();
        }


        void destroy_map_and_nodes()
        {
            for (map_pointer cur = start.node; cur <= finish.node; ++cur)
                deallocate_node(*cur);
            map_allocator::deallocate(map, map_size);
        }

        value_type* allocate_node()
        {
            return data_allocator::allocate(buffer_size());
        }

        void deallocate_node(pointer n)
        {
            data_allocator::deallocate(n, buffer_size());
        }

        void destroy_nodes(map_pointer nstart, map_pointer nfinish)
        {
            for (map_pointer n = nstart; n < nfinish; ++n)
                deallocate_node(*n);
        }

    public:
    // 接口

        void push_back(const value_type & t)
        {
            // 如果最后一个缓冲区还有空间
            if(finish.cur != finish.last-1)
            {
                _construct(finish.cur, t);
                ++finish.cur;
            }
            else
                push_back_aux(t);
        }

        void push_front(const value_type& t)
        {
            if (start.cur != start.first) 
            {
                _construct(start.cur - 1, t);
                --start.cur;
            } 
            else
                push_front_aux(t);
        }
        
        void pop_back()
        {
            if (finish.cur != finish.first) 
            {
                --finish.cur;
                _destroy(finish.cur);
            }
            else
                pop_back_aux();
        }
        
        void pop_front() 
        {
            if (start.cur != start.last - 1)
            {
                _destroy(start.cur);
                ++start.cur;
            }
            else
                pop_front_aux();
        }


        iterator insert(iterator position, const value_type& x)
        {
            // 如果是在deque的最前端插入, 那么直接push_front()即可
            if (position.cur == start.cur) 
            {
                push_front(x);
                return start;
            }
            // 如果是在deque的末尾插入, 直接调用push_back()
            else if (position.cur == finish.cur) 
            {
                push_back(x);
                iterator tmp = finish;
                --tmp;
                return tmp;
            }
            else 
            {
                return insert_aux(position, x);
            }
        }
        
        iterator insert(iterator position) 
        { 
            return insert(position, value_type()); 
        }
        
        
        void insert(iterator pos, size_type n, const value_type& x)
        {
            if (pos.cur == start.cur) {
                iterator new_start = reserve_elements_at_front(n);
                uninitialized_fill(new_start, start, x);
                start = new_start;
            }
            else if (pos.cur == finish.cur) {
                iterator new_finish = reserve_elements_at_back(n);
                uninitialized_fill(finish, new_finish, x);
                finish = new_finish;
            }
            else
                insert_aux(pos, n, x);
        }
        
        void insert(iterator pos, int n, const value_type& x)
        {
            insert(pos, (size_type) n, x);
        }
        void insert(iterator pos, long n, const value_type& x)
        {
            insert(pos, (size_type) n, x);
        }
        
        
        void resize(size_type new_size, const value_type& x)
        {
            const size_type len = size();
            if (new_size < len)
            erase(start + new_size, finish);
            else
            insert(finish, new_size - len, x);
        }
        
        void resize(size_type new_size) 
        { 
            resize(new_size, value_type()); 
        }
 
        iterator erase(iterator pos)
        {
            iterator next = pos;
            ++next;

            difference_type index = pos - start;
        
            if (index < (size() >> 1))
            {
                copy_backward(start, pos, next); 
                pop_front();
            }
            else 
            {
                copy(next, finish, pos);
                pop_back();
            }
            return start + index;
        }
        
        iterator erase(iterator first, iterator last)
        {
            if (first == start && last == finish) 
            {
                clear();
                return finish;
            }
            else 
            {
                difference_type n = last - first; //清除区间长度
                difference_type elems_before = first - start; //清楚区间前方的元素
            
                //判断清除区间前后哪个元素少
                if (elems_before < (size() - n) / 2) { //如果前方元素少，则将前方元素
                copy_backward(start, first, last);   //后移（覆盖清除区间）
                iterator new_start = start + n;  //标记deque新起点
                _destroy(start, new_start); //移动完毕，将冗余元素析构
                //将冗余缓冲区释放
                for (map_pointer cur = start.node; cur < new_start.node; ++cur)
                    data_allocator::deallocate(*cur, buffer_size());
                start = new_start;  //设定deque新起点
                }
                else { //清除区间后方元素少，向前移动后方元素（覆盖清除区）
                copy(last, finish, first);
                iterator new_finish = finish - n;
                _destroy(new_finish, finish);
                for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
                    data_allocator::deallocate(*cur, buffer_size());
                finish = new_finish;
                }
                return start + elems_before;
            }
        }
        
        
        void clear()
        {
            for (map_pointer node = start.node + 1; 
                        node < finish.node; ++node) 
            {
                _destroy(*node, *node + buffer_size());
                data_allocator::deallocate(*node, buffer_size());
            }
            
            if (start.node != finish.node) 
            {
                _destroy(start.cur, start.last); 
                _destroy(finish.first, finish.cur); 
                data_allocator::deallocate(finish.first, buffer_size());
            }
            else
                _destroy(start.cur, finish.cur);
            finish = start; 
        }

        
    protected:
    // 辅助函数
    
        void push_back_aux(const value_type& t)
        {
            value_type t_copy = t;
            reserve_map_at_back();
            *(finish.node + 1) = allocate_node();
            try
            {
                _construct(finish.cur, t_copy);
                finish.set_node(finish.node + 1); 
                finish.cur = finish.first; 
            }
            catch(...)
            {
                deallocate_node(*(finish.node + 1));
            }
        }
        
        void push_front_aux(const value_type& t)
        {
            value_type t_copy = t;
            reserve_map_at_front();
            *(start.node - 1) = allocate_node(); 
            try
            {
                start.set_node(start.node - 1); 
                start.cur = start.last - 1;
                _construct(start.cur, t_copy);
            }
            catch(...) 
            {
                start.set_node(start.node + 1);
                start.cur = start.first;
                deallocate_node(*(start.node - 1));
                throw;
            }
        }
        
        void pop_back_aux()
        {
            deallocate_node(finish.first); 
            finish.set_node(finish.node - 1);
            finish.cur = finish.last - 1;
            _destroy(finish.cur);
        }
        
        void pop_front_aux()
        {
            _destroy(start.cur);
            deallocate_node(start.first);
            start.set_node(start.node + 1);
            start.cur = start.first;
        }
        
        
        iterator insert_aux(iterator pos, const value_type& x)
        {
            difference_type index = pos - start; //插入点之前的元素个数
            value_type x_copy = x;
            
            if (index < size() / 2) {   //如果插入点前元素个数少
                push_front(front()); //在最前端加入与第一个元素同值的元素
                iterator front1 = start;
                ++front1;
                iterator front2 = front1;
                ++front2;
                pos = start + index;
                iterator pos1 = pos;
                ++pos1;
                copy(front2, pos1, front1); //元素移动
            }
            else { //插入点后的元素个数较少
                push_back(back());  //在尾部插入一个与最后一个元素同值的元素
                iterator back1 = finish;
                --back1;
                iterator back2 = back1;
                --back2;
                pos = start + index;
                copy_backward(pos, back2, back1); //元素移动
            }
            *pos = x_copy; //在插入点上设定新值
            return pos;
        }

        
        void insert_aux(iterator pos, size_type n, const value_type& x)
        {
            const difference_type elems_before = pos - start;
            size_type length = this->size();
            value_type x_copy = x;
            if (elems_before < difference_type(length / 2)) 
            {
                iterator new_start = reserve_elements_at_front(n);
                iterator old_start = start;
                pos = start + elems_before;
                try 
                {
                    if (elems_before >= difference_type(n))
                    {
                        iterator start_n = start + difference_type(n);
                        uninitialized_copy(start, start_n, new_start);
                        start = new_start;
                        copy(start_n, pos, old_start);
                        fill(pos - difference_type(n), pos, x_copy);
                    }
                    else 
                    {
                        uninitialized_copy_fill(start, pos, new_start, 
                                                start, x_copy);
                        start = new_start;
                        fill(old_start, pos, x_copy);
                    }
                }
                catch(...)
                {
                    destroy_nodes(new_start.node, start.node);
                }
            }
            else 
            {
                iterator new_finish = reserve_elements_at_back(n);
                iterator old_finish = finish;
                const difference_type elems_after = 
                difference_type(length) - elems_before;
                pos = finish - elems_after;
                try 
                {
                    if (elems_after > difference_type(n)) 
                    {
                        iterator finish_n = finish - difference_type(n);
                        uninitialized_copy(finish_n, finish, finish);
                        finish = new_finish;
                        copy_backward(pos, finish_n, old_finish);
                        fill(pos, pos + difference_type(n), x_copy);
                    }
                    else 
                    {
                        uninitialized_fill_copy(finish, pos + difference_type(n),
                                                x_copy, pos, finish);
                        finish = new_finish;
                        fill(pos, old_finish, x_copy);
                    }
                }
                catch(...)
                {
                    destroy_nodes(finish.node + 1, new_finish.node + 1);
                }
            }
        }



        void reserve_map_at_back (size_type nodes_to_add = 1)
        {
            if (nodes_to_add + 1 > map_size - (finish.node - map))
            reallocate_map(nodes_to_add, false);
        }
        
        void reserve_map_at_front (size_type nodes_to_add = 1)
        {
            if (nodes_to_add > start.node - map)
            reallocate_map(nodes_to_add, true);
        }

        void reallocate_map(size_type nodes_to_add, bool add_at_front)
        {
            size_type old_num_nodes = finish.node - start.node + 1;
            size_type new_num_nodes = old_num_nodes + nodes_to_add;

            map_pointer new_nstart;
            if (map_size > 2 * new_num_nodes) 
            {
                new_nstart = map + (map_size - new_num_nodes) / 2 
                                + (add_at_front ? nodes_to_add : 0);
                if (new_nstart < start.node)
                    copy(start.node, finish.node + 1, new_nstart);
                else
                    copy_backward(start.node, finish.node + 1, 
                                new_nstart + old_num_nodes);
            }
            else 
            {
                size_type new_map_size = 
                map_size + max(map_size, nodes_to_add) + 2;

                map_pointer new_map = allocate_map(new_map_size);
                new_nstart = new_map + (new_map_size - new_num_nodes) / 2
                                    + (add_at_front ? nodes_to_add : 0);
                copy(start.node, finish.node + 1, new_nstart);
                deallocate_map(map, map_size);

                map = new_map;
                map_size = new_map_size;
            }

            start.set_node(new_nstart);
            finish.set_node(new_nstart + old_num_nodes - 1);
        }


        iterator reserve_elements_at_front(size_type n)
        {
            size_type vacancies = start.cur - start.first;
            if (n > vacancies)
            new_elements_at_front(n - vacancies);
            return start - difference_type(n);
        }
        
        iterator reserve_elements_at_back(size_type n)
        {
            size_type vacancies = (finish.last - finish.cur) - 1;
            if (n > vacancies)
            new_elements_at_back(n - vacancies);
            return finish + difference_type(n);
        }

        
        void new_elements_at_front(size_type new_elems)
        {
            size_type new_nodes
                = (new_elems + buffer_size() - 1) / buffer_size();
            reserve_map_at_front(new_nodes);
            size_type i;
            try
            {
                for (i = 1; i <= new_nodes; ++i)
                *(start.node - i) = allocate_node();
            }
            catch(...) 
            {
                for (size_type j = 1; j < i; ++j)
                deallocate_node(*(start.node - j));      
                throw;
            }
        }


        void new_elements_at_back(size_type new_elems)
        {
            size_type new_nodes
                = (new_elems + buffer_size() - 1) / buffer_size();
            reserve_map_at_back(new_nodes);
            size_type i;
            try
            {
                for (i = 1; i <= new_nodes; ++i)
                *(finish.node + i) = allocate_node();
            }
            catch(...) 
            {
                for (size_type j = 1; j < i; ++j)
                deallocate_node(*(finish.node + j));      
                throw;
            }
        }
    };

}
`stack`和`queue`都是对`deque`的封装, 并且提供了一个模版参数`Sequence`可以指定底层容器, 默认为`deque`, 但也可以显示指定为`list`或者实现了相应接口的`vector`



```cpp
  template <typename T, typename Sequence = deque<T>>
    class stack
    {
    public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef typename Sequence::const_reference const_reference;
    protected:
        Sequence c;
    public:
        bool empty() { return c.empty(); }
        size_type size() const { return c.size(); }
        reference top() { return c.back(); }
        const_reference top() const { return c.back(); }
        void push(const value_type& x) { c.push_back(x); }
        void pop() { c.pop_back(); }
    };

template <typename T, typename Sequence = deque<T>>
    class queue
    {
        public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef typename Sequence::const_reference const_reference;
        protected:
        Sequence c;
        public:
        bool empty() { return c.empty(); }
        size_type size() const { return c.size(); }
        reference front() { return c.front(); }
        const_reference front() const { return c.front(); }
        reference back() { return c.back(); }
        const_reference back() const { return c.back(); }
        void push(const value_type& x) { c.push_back(x); }
        void pop() { c.pop_front(); }
    };

```

使用`list'和`vector`作为底层容器:

```cpp
ltx::stack<int, ltx::vector<int>> s;
ltx::queue<int, ltx::list<int>> q;
```


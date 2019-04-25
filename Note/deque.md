完整代码见: [Standard-Template-Library/dqueu.h](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/deque.h)

SGI的`deque`的实现比较麻烦, 使用了一个`map`中控数组, `map`中每个元素指向一块连续的缓冲区, 所以实现比较麻烦

(可以将`list`封装成`deque`, 但是这样的话频繁的`push`和`pop`由于每次都要配置空间, 会比较低效; 也可以将`vector`封装成`deque`, 但是头部插入效率非常低; `deque`取了折中的实现方式)

![](https://i.loli.net/2019/04/25/5cc154ee12f7f.png)



具体的实现没有什么好讲的, 虽然麻烦但是挺简单的, 和`vector` 差不多, 只是`deque`是通过多个缓冲区达到的伪连续空间

代码注释不多, 因为都是些简单的逻辑, 可以结合《源码剖析》食用



## 参考

[SGI-STL/stl_deque.h](https://github.com/steveLauwh/SGI-STL/blob/master/SGI-STL V3.3/stl_deque.h)

<https://blog.csdn.net/u013074465/article/details/44617629>
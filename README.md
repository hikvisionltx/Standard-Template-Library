# STL源码剖析

参照*STL源码剖析*和SGI STL的源码, 将常用的一些STL的内容较为完整的实现了一遍, 能在C++11下正常编译运行

根目录下带的代码是我自己实现的STL, 测试代码放在了[Test](https://github.com/LiTianxiong/Standard-Template-Library/tree/master/Test)目录下, [SGI-STL V3.3](https://github.com/LiTianxiong/Standard-Template-Library/tree/master/SGI-STL%20V3.3)目录中是STI-STL的源码, [Note](https://github.com/LiTianxiong/Standard-Template-Library/tree/master/Note)目录中是学习过程中的笔记(下面列出了所有笔记)

1. **[配置器](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/配置器.md)**: 负责空间的配置和管理
2. [迭代器与traits编程方式](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/迭代器.md): 泛型指针, 沟通算法和容器
3. 容器: 各种数据结构, 用来存放数据
   1. 序列式容器
      1. **[list](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/list.md)**
      2. **[vector](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/vector.md)**
      3. [deque](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/deque.md)
         1. [stack和queue](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/stack和queue.md)
      4. [heap与priority_queue](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/heap与priority_queue.md)
   2. 关联式容器
      1. **[rb_tree](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/rb_tree.md)**
         1. [set, map, multiset, multimap](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/set,map,multiset,multimap.md)
4. 算法: 各种常用算法
   1. **[sort](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/sort.md)**
5. 仿函数: 行为类似函数, 可作为算法的某种策略
6. **[配接器](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/配接器.md)**: 用来修饰容器, 仿函数或迭代器接口
   1. **[迭代器配接器](https://github.com/LiTianxiong/Standard-Template-Library/blob/master/Note/迭代器配接器.md)**





![](https://raw.githubusercontent.com/LiTianxiong/Pictures/master/20190429151826.png)


`set`, `map`, `multiset`, `multimap`都是对`rb_tree`的封装

`set`和`map`不同的地方在于, `set`的`KeyOfValue`是返回元素本身的值, 而`map`的元素是`pair`, 其`KeyOfValue`返回`pair`的第一个元素

`multi*`和非`multi`的区别在于非multi使用`insert_unique`进行插入, 不能有重复值, 而`multi`使用`insert_equal`, 可以有重复值


所有内容均基于SGI STL.该项目仅作为本人学习C++STL库,数据结构与算法使用.
##  内存置配器
容器使用内存置配器来进行内存空间的分配和释放.
为了适配STL标准,在实际使用的分配器实现一个上层接口类simple_alloc.该类默认使用第二级分配器

### 第一级分配器 malloc_alloc_template
该分配器在内部直接使用operator new/delete进行内存管理
### 第二级分配器 default_alloc_template
该分配器以小型内存池进行内存管理,减轻分配内存时的cookie占用的内存.
#### 成员变量
维护一个free_list数组,每一个free_list是一个存放未分配内存的单向链表(的头指针).用来管理释放或未分配的内存.
另外维护两个所有二级分配器共用的静态指针start_free和end_free,分别指向空闲内存(空闲且未链入free_list)的首尾.
还有一个size_t类型的heap_size成员,在重新向操作系统malloc内存时会用到其来计算需要申请的空间大小,但不太懂其存在的意义.
#### 实现
##### 内存分配
当容器使用alloc分配空间时,若内存超过128bytes(MAX_BYTES)使用第一级分配器进行内存分配.反之使用第二级.

首先检查对应大小的free_list是否存在未分配空间.若有就直接取出.
如果没有,调用refill函数用空闲内存区域先填充free_list(内存充足的情况下每次链入20块),再取出一块完成分配.
refill函数其实只负责将从空闲内存区域取出的空间链入free_list,实际的取出工作由chunk_alloc函数完成.

chunk_alloc函数先检查空闲区域是否足够取出20个对应大小的堆块,若够则能取多少个取多少.如果一个的空间都不够,就先将空闲区域整个链入free_list对应大小链表中.然后调用malloc函数向系统申请2\*20\*nbytes(一个元素的大小)字节的空间.

##### 内存释放
若内存超过128bytes,使用第一级分配器释放内存.
否则使用第二级分配器直接链入对应free_list中.

值得注意的时,第二级分配器并没有对free_list以及start_free~end_free之间区域的释放操作,这一操作最后将由操作系统完成?
[STL allocator何时释放内存？](https://www.zhihu.com/question/38524347)
## 迭代器
迭代器用来粘合算法和容器.
#### Traits技法
##### template参数推导机制
在算法中使用迭代器时,可能会使用到迭代器所指之物的型别,该型别可以利用template参数推导机制完成.
```cpp
template <class I>
void func(I iter)
{
	func2(iter,*iter);//func2可通过参数推导机制获得iter所指型别
}
```
但如果func函数需要返回一个iter指向的对象,template推导机制就不起作用了
```cpp
//下列写法无法通过编译
template <class I>
(*I) func(I iter)
{
	func2(iter,*iter);
}
```
##### 内嵌型别声明
我们可以使用内嵌型别声明的方法解决该问题.
```cpp
template <class T>
struct Iterator{
typedef T value_type;
......
}

template <class I>
typename I::value_type func(I iter)
{
	func2(iter,*iter);
}
```

对于泛型算法,不仅需要接受迭代器,还需要能够接受一个原生指针作为参数,而原生指针没有,我们也无法为其内嵌一个名为value_type的变量
于是需要用到traits技法
##### Traits
使用中间层traits,利用template参数推导机制进行偏特化,将原生指针和迭代器统一.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202304061931218.webp)
```cpp
template <class T>
struct iterator_traits {
    typedef typename T::value_type value_type;
};
// 偏特化1
template <class T>
struct iterator_traits<T*> {
    typedef T value_type;
};
// 偏特化2
template <class T>
struct iterator_traits<const T*> {
    typedef T value_type;
};
```
#### 分类
根据迭代器操作容器元素的方式,分为InputIterator和OutputIterator
根据迭代器遍历容器的方式,分为ForwardIterator,BidirectionalIterator,RandomAccessIterator等.

为了使函数能够获取迭代器的类型,将类型标签定义为一个新类型,再以traits方式型别声明为category,使用时进行萃取.
```cpp
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectional_iterator_tag:public forward_iterator_tag{};
struct random_access_iterator_tag:public bidirectional_iterator_tag{};
```

## 容器
### vector
单端开口的连续线性空间容器
#### 迭代器
由于该容器的空间是连续线性的,故其迭代器为原生指针.迭代器在扩容后失效
#### 成员变量
维护了三个指针(即vector的迭代器类型),start指向vector已存储区域的开头,finish指向vector中已存储区域的末尾,end_of_storage指向vector总存储空间的末尾.
#### 实现
加入新元素时,若存储空间充足就直接在finish构造元素,若存储空间不足便申请二倍存储空间,将原空间内容拷贝到新申请的空间,再释放掉原有空间.
由于扩容操作会使vector中所有元素迁移,故迭代器失效.
#### 安全问题
由于迭代器失效,若程序有对迭代器的保存动作,可能造成uaf,double free等安全问题.

### list
两端开口的双向环状链表
#### 迭代器
Bidirectional迭代器.内嵌一个链表结点指针.以链表结点的指针完成迭代.迭代器不会失效
#### 成员变量
仅维护一个环状链表中的空结点指针
#### 实现
和普通双向环状链表区别不大.
注意一下5个基本操作,有助于对list空间结构的想象.
```c
	iterator begin() { return (link_type)(node->next); }
	iterator end() { return (link_type)node; }
	bool empty() { return node->next == node; }
	reference front() { return *begin(); }
	reference back() { return *(--end()); }
```

### deque
两端开口的连续线性空间.(连续线性空间实则是伪造的)
#### 迭代器
内含四个指针,分别为value_type\*类型的cur,first,last分别指向所在缓冲区的当前元素,头部,尾部.另一个map_pointer指向deque的管控中心map(不是STL中的容器map)

deque的连续线性空间伪造主要依靠的便是迭代器,迭代器在到达一片缓冲区的末尾时跳跃到下一缓冲区或到达开头时跳转到上一个缓冲区,伪造出连续的空间.

#### 成员变量
指向map的指针和map_size.迭代器start,finish分别关联第一个和最后一个缓冲区
#### 实现
主要是map的扩容.
### RB-tree
具有排序的平衡二叉树,用于快速查找.
#### 迭代器
内含一个指向树结点的指针,increment和decrement操作使迭代器指向比当前结点值大的最小元素.(对迭代器而言并没有"大小"的概念,例如increment只是简单的移动到右节点,然后持续移动到左叶子结点."大小的概念由容器插入节点的树成长过程体现")

#### 成员变量
一个header指针,一个node_count,一个用于元素比较的仿函数.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202309141728705.png)

## 算法
### copy
主要学习函数dispatch
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202304091803715.png)

### heap
push_heap:新加入的元素放在最后一个结点,循环与父节点比较判断是否交换.
pop_heap:将根节点的值与最后一个结点的值交换(交换只是一种说法,实际实现可以不同),让最后一个结点进行下溯交换,直到回到根节点当前位置的前一个.
sort_heap:反复进行pop_heap,每次将finish-1即可排序.
make_heap:使用pop_heap类似的下溯操作,从最后一个父节点开始依次进行下溯后再使用push_heap操作完成实质的父子结点比较排序过程.

要注意一点,下溯操作仅是下放某个结点,并不涉及排序.所以《STL源码剖析》一书中侯捷的注释是错误的.按书上更改后,pop_heap调用时若左叶子结点的值小于右叶子节点的值发生错误,make_heap调用时更是完全错误.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202309111124768.png)


## 配接器
### stack
单端开口先进先出.
Stack内部以deque为底层容器进行封装,不存在迭代器.也可以以list为底层容器.
### queue
单端开口先进后出
Queue内部以deque为底层容器进行封装,不存在迭代器.也可以以list为底层容器.
### priority_queue
单端开口,出队顺序由排序方式决定.
以vector为底层容器进行封装,算法主要由heap算法实现.
### set map
默认均以rb_tree为底层容器,区别是前者键值相同,后者键值类型不一定相同.
# 思考题
## 思考题1 
> 在写容器的时候 容器里有很多的成员函数或者非成员函数 。有的容器都有对元素的增删操作 ，那么这些操作在如何不恰当使用的情况下，会造成安全问题请构造出poc来 。

### 漏洞1
这个其实不能算是容器的增删操作导致的,而是所有类中指针的通病.
在没有重载复制(构造)函数情况下,默认的复制(构造)函数会直接复制指针的值(浅拷贝),当其中一个对象销毁时释放掉指针内存而另一个对象还保留着已释放内存的指针.
如图,vec和vec2使用同一块内存,若vec被销毁,vec2中仍存有该内存指针造成uaf.
(由于STL二级空间置配器并未真正释放内存给操作系统,所以asan并未检测到double free)
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202304162312107.png)
改用一级空间置配器检测到double free.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202304162322079.png)

重载拷贝构造函数后正常:
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202309141856874.png)

## 思考题2
> 为什么实现了uninitialized_xxx和copy/fill这样两组不同的函数

-   `uninitialized_xx`函数，其内部会执行结构体的构造函数。该函数面向于`尚未进行初始化`的内存。
-   而没有“`uninitialized_`”前缀的函数，面向的是`已经初始化过`的内存。所以不需要再执行构造函数。

对已包含元素的内存中调用uninitialized_copy,可能导致对象未析构,或者出现问题1同样的情况,如copy,因重载的赋值运算符未被调用而导致浅拷贝的发生.
![](https://obsidian-1314737433.cos.ap-beijing.myqcloud.com/202309141803372.png)

## 思考题3

> 理解每个容器的内存模型。

-   vector的内存模型是一个数组。
-   list的内存模型是一个带空结点的双向循环链表.
-  deque的内存模型是多个buffer缓冲区和管理buffer的map数组.
- set,map的内存模型是平衡二叉树(红黑树)
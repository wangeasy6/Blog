---
title: C++ STL 基础
data: 2025-03-01 21:14:00
toc: true
categories:
  - tech
tags:
  - C++
---

STL（Standard Template Library，标准模板库）是 C++ 标准库的一部分，由 ISO/IEC C++ 标准定义。它提供了一系列通用的、可复用的算法和数据结构。

本篇包含了 STL 6大组件的基本使用方法，按照组件在源码中的占比从大到小排序约为：

1. 容器（Containers）
2. 算法（Algorithms）
3. 迭代器（Iterators）
4. 函数对象（Function Objects）
5. 适配器（Adapter）
6. 分配器（Allocator）

开发环境：

* Compiler：[Cygwin](https://www.cygwin.com/) GCC 12.4.0
* C++ standard：`-std=c++17`，以下内容都基于此标准

<!--more-->

<br/>

## 组件间交互关系

Container 通过 Allocator 取得存储空间，Algorithm 通过 Iterator 存取 Container 内容，Functor 可以协助 Algorithm 完成不同的策略变化，Adapter 可以修饰或套接 Functor。

<br/>



## 容器（Containers）

### 速览

#### 序列容器

序列容器实现了可按顺序访问的数据结构。

| 容器                                                         | 描述                 | 底层数据结构 | 时间复杂度                                                | 有无序 | 可不可重复 | 其他                                                        |
| ------------------------------------------------------------ | -------------------- | ------------ | --------------------------------------------------------- | ------ | ---------- | ----------------------------------------------------------- |
| [array](/tech/Cpp-STL-Basics/#array)                       | 固定大小的连续数组   | 数组         | 随机读改 O(1)                                             | 无序   | 可重复     | 支持随机访问                                                |
| [vector](/tech/Cpp-STL-Basics/#vector) | 可调整大小的连续数组 | 数组         | 随机读改、尾部插入、尾部删除 O(1) 头部插入、头部删除 O(n) | 无序   | 可重复     | 支持随机访问                                                |
| [deque](/tech/Cpp-STL-Basics/#deque) | 双端队列             | 双端队列     | 头尾插入、头尾删除 O(1)                                   | 无序   | 可重复     | 一个中央控制器 + 多个缓冲区，支持首尾快速增删，支持随机访问 |
| [forward_list](/tech/Cpp-STL-Basics/#forward_list) | 单向链表             | 单向链表     | 插入、删除 O(1)                                           | 无序   | 可重复     | 不支持随机访问                                              |
| [list](/tech/Cpp-STL-Basics/#list) | 双向链表             | 双向链表     | 插入、删除 O(1)                                           | 无序   | 可重复     | 不支持随机访问                                              |

#### 容器适配器

容器适配器为序列容器提供了不同的接口。

| 容器                                                     | 描述                                                         | 底层数据结构      | 时间复杂度              | 有无序 | 可不可重复 | 其他                                                         |
| -------------------------------------------------------- | ------------------------------------------------------------ | ----------------- | ----------------------- | ------ | ---------- | ------------------------------------------------------------ |
| [stack](/tech/Cpp-STL-Basics/#stack)                   | 栈（后进先出）                                               | deque / list      | 顶部插入、顶部删除 O(1) | 无序   | 可重复     | deque 或 list 封闭头端开口，不用 vector 的原因应该是容量大小有限制，扩容耗时 |
| [queue](/tech/Cpp-STL-Basics/#queue)                   | 队列（先进先出）                                             | deque / list      | 尾部插入、头部删除 O(1) | 无序   | 可重复     | deque 或 list 封闭头端开口，不用 vector 的原因应该是容量大小有限制，扩容耗时 |
| [priority_queue](/tech/Cpp-STL-Basics/#priority_queue) | 元素的次序是由作用于所存储的值对上的某种谓词决定的的一种队列 | vector + max-heap | 插入、删除 O(log2n)     | 有序   | 可重复     | vector容器+heap处理规则                                      |

#### 关联容器

关联容器实现了可快速搜索的排序数据结构（O(log n)复杂度）。

| 容器                                                         | 描述                  | 底层数据结构 | 时间复杂度                | 有无序 | 可不可重复 | 其他 |
| ------------------------------------------------------------ | --------------------- | ------------ | ------------------------- | ------ | ---------- | ---- |
| [set](/tech/Cpp-STL-Basics/#set) | 集合                  | 红黑树       | 插入、删除、查找 O(log2n) | 有序   | 不可重复   |      |
| [multiset](/tech/Cpp-STL-Basics/#multiset) | 可重复集合            | 红黑树       | 插入、删除、查找 O(log2n) | 有序   | 可重复     |      |
| [map](/tech/Cpp-STL-Basics/#map) | 由 键-值 对组成的集合 | 红黑树       | 插入、删除、查找 O(log2n) | 有序   | 不可重复   |      |
| [multimap](/tech/Cpp-STL-Basics/#multimap) | 可重复 map            | 红黑树       | 插入、删除、查找 O(log2n) | 有序   | 可重复     |      |

##### 使用类型

| 容器  | 描述    | 底层数据结构 | 时间复杂度 | 有无序 | 可不可重复 | 其他 |
| ----- | ------- | ------------ | ---------- | ------ | ---------- | ---- |
| tuple | 元组    |              |            |        |            |      |
| pair  | 键-值对 |              |            |        |            |      |

#### 无序关联容器

无序关联容器实现了可快速搜索的无排序（散列）数据结构（平均复杂度为 O(1)，最差复杂度为 O(n)）。

| 容器                                                         | 描述           | 底层数据结构 | 时间复杂度                      | 有无序 | 可不可重复 | 其他 |
| ------------------------------------------------------------ | -------------- | ------------ | ------------------------------- | ------ | ---------- | ---- |
| [unordered_set](/tech/Cpp-STL-Basics/#unordered_set) | 无序集合       | 哈希表       | 插入、删除、查找 O(1) 最差 O(n) | 无序   | 不可重复   |      |
| [unordered_multiset](/tech/Cpp-STL-Basics/#unordered_multiset) | 无序可重复集合 | 哈希表       | 插入、删除、查找 O(1) 最差 O(n) | 无序   | 可重复     |      |
| [unordered_map](/tech/Cpp-STL-Basics/#unordered_map) | 无序 map       | 哈希表       | 插入、删除、查找 O(1) 最差 O(n) | 无序   | 不可重复   |      |
| [unordered_multimap](/tech/Cpp-STL-Basics/#unordered_multimap) | 无序可重复 map | 哈希表       | 插入、删除、查找 O(1) 最差 O(n) | 无序   | 可重复     |      |

<br />

**注意**：对于所有详细介绍，都有如下定义：

```c++
template<typename T, std::size_t N>
{
    typedef T                           value_type;
    typedef value_type*                 pointer;
    typedef const value_type*           const_pointer;
    typedef value_type&                 reference;
    typedef const value_type&           const_reference;
    typedef value_type*                 iterator;
    typedef const value_type*           const_iterator;
    typedef std::size_t                             size_type;
    typedef std::ptrdiff_t                          difference_type;
    typedef std::reverse_iterator<iterator>         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
}
```

<br />

### array

array 是固定大小的顺序容器，它们保存了一个以严格的线性顺序排列的特定数量的元素。

#### 特性说明

* **高效**：`array`的元素存储在连续的内存空间中，这使得它在访问元素时具有良好的缓存局部性，能够提高访问效率。`vector`需要动态分配内存，而`array`的内存分配在编译时就已经确定，没有额外的动态内存管理开销。
* **兼容性**：`array`可以通过`data()`函数获取指向数组首元素的指针，从而与C语言的数组进行交互。例如，可以将`array`的指针传递给C语言的函数。

#### 成员函数

| 方法       | 原型                                                         | 简介                                                         |
| ---------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| array      | `array(std::initializer_list<T> ilist)`                      | 使用初始化列表初始化                                         |
| array      | `array(const array& other) noexcept`                         | 使用另一个 `array` 对象初始化新对象                          |
| array      | `array(array&& other) noexcept`                              | 使用另一个 `array` 对象的资源初始化新对象，并将原对象置于有效但未指定的状态 |
| operator[] | `reference operator[](size_type n)`,<br/>`const_reference operator[](size_type pos) const` | 返回容器中第 n 个位置的元素的引用，不进行边界检查            |
| at         | `reference at(size_type n)`                                  | 返回容器中第 n 个位置的元素的引用                            |
| front      | `reference front()`                                          | 返回对容器中第一个元素的引用                                 |
| back       | `reference back()`                                           | 返回对容器中最后一个元素的引用                               |
| data       | `pointer data()`                                             | 返回指向容器中第一个元素的指针                               |
| size       | `size_type size()`                                           | 返回数组容器中元素的数量（在 `array` 中等于 `max_size()`）   |
| max_size   | `size_type max_size()`                                       | 返回数组容器可容纳的最大元素数                               |
| empty      | `bool empty()`                                               | 返回一个布尔值，指示数组容器是否为空                         |
| fill       | `fill(const T& value)`                                       | 用 value 填充数组所有元素                                    |
| swap       | `swap(array<T, N>& other)`                                   | 交换2个数组的内容（两个数组的类型和大小需一致）              |

#### 非成员函数

| 方法     | 原型                                                         | 简介                                                         |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| operator | `template <class T, class Allocator>`<br/>`bool operator==(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)` | 按词典顺序比较两个数组的值：==、!=、<、<=、>、>=、<=>        |
| get      | `T& get(array<T, N>& arr)`,<br />`const T& get(const array<T, N>& arr)` | 形如 `std::get<0>(myarray)`；传入一个数组容器，返回指定位置元素的引用 |
| get      | `T&& get(array<T, N>&& arr)`,<br />`const T&& get(const array<T, N>&& arr)` | 移动语义的 get                                               |
| swap     | `template< class T, std::size_t N >`<br/>`void swap( std::array<T, N>& lhs,`<br/>`           std::array<T, N>& rhs )` | 专门为 `std::array` 提供的 `std::swap` 算法。复杂度为 O(n)。 |

#### 迭代操作

| 方法    | 原型                                                         | 简介                                                     |
| ------- | ------------------------------------------------------------ | -------------------------------------------------------- |
| begin   | `iterator begin() noexcept`,<br />`const_iterator begin() const noexcept` | 返回指向数组容器中第一个元素的迭代器                     |
| end     | `iterator end() noexcept`,<br />`const_iterator end() const noexcept` | 返回指向数组容器中最后一个元素之后的理论元素的迭代器     |
| rbegin  | `reverse_iterator rbegin() noexcept`,<br />`const_reverse_iterator rbegin() const noexcept` | 返回指向数组容器中最后一个元素的反向迭代器               |
| rend    | `reverse_iterator rend() noexcept`,<br />`const_reverse_iterator rend() const noexcept` | 返回一个反向迭代器，指向数组中第一个元素之前的理论元素   |
| cbegin  | `const_iterator cbegin() const noexcept`                     | 返回指向数组容器中第一个元素的常量迭代器                 |
| cend    | `const_iterator cend() const noexcept`                       | 返回指向数组容器中最后一个元素之后的理论元素的常量迭代器 |
| crbegin | `const_reverse_iterator crbegin() const noexcept`            | 返回指向数组容器中最后一个元素的常量反向迭代器           |
| crend   | `const_reverse_iterator crend() const noexcept`              | 返回指向数组中第一个元素之前的理论元素的常量反向迭代器   |

#### Example

[array_test.cpp](/resources/stl/array_test.cpp)

<br/>

### vector

vector 是表示可以改变大小的数组的序列容器。

#### 特性说明

* **支持随机访问**：由于 `vector` 的元素存储在连续的内存中，因此可以通过下标（`operator[]` 或 `at()`）快速访问任意位置的元素。
* **自动管理内存**：当添加或删除元素时，它会自动调整内存分配。当 `vector` 被销毁时，它会自动释放分配的内存。
* **插入和删除**：在中间或头部插入或删除元素效率较低，因为需要移动后续元素，时间复杂度为 O(n)。
* **线程安全**：标准的 `std::vector` 本身不是线程安全的。如果需要在多线程环境中使用，需要手动添加同步机制（如互斥锁）来保护对 `vector` 的访问。

#### 成员函数

| 方法          | 原型                                                         | 简介                                                         |
| ------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| vector        | `vector()`                                                   | 默认构造函数，创建一个空的 vector                            |
| vector        | `vector(size_type count, const T& value, const Allocator& alloc = Allocator())` | **带大小和值的构造函数**：创建一个包含 `count` 个元素的 `vector`，所有元素初始化为 `value` |
| vector        | `vector(const vector& other)`                                | **拷贝构造函数**：从另一个 `vector` 拷贝内容                 |
| vector        | `vector(vector&& other)`                                     | **移动构造函数**：从另一个 `vector` 移动内容                 |
| vector        | `vector(initializer_list<T> init, const Allocator& alloc = Allocator())` | **初始化列表构造函数**：使用初始化列表构造 `vector`          |
| vector        | `vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())` | **范围构造函数**：从迭代器范围 `[first, last)` 构造 `vector` |
| operator=     | `vector& operator=(const vector& x)`,<br/>`vector& operator=(vector&& x) noexcept`,<br/>`vector& operator=(initializer_list<value_type> l)` | 将新内容分配给容器，替换其当前内容，并相应地修改其大小       |
| assign        | `void assign(size_type count, const T& value)`,<br/>`template <class InputIt>`<br/>`void assign(InputIt first, InputIt last)`,<br/>`void assign(initializer_list<T> ilist)` | 将新内容分配给 vector，替换其当前内容，并相应地修改其 size   |
| operator[]    | `reference operator[](size_type pos)`,<br/>`const_reference operator[](size_type pos) const` | 通过下标访问元素，不进行边界检查                             |
| at            | `reference at(size_type pos)`,<br/>`const_reference at(size_type pos) const` | 通过下标访问元素，进行边界检查，越界时抛出 `std::out_of_range` |
| front         | `reference front()`,<br/>`const_reference front() const`     | 返回对容器中第一个元素的引用                                 |
| back          | `reference back()`,<br/>`const_reference back() const`       | 返回对容器中最后一个元素的引用                               |
| data          | `pointer data() noexcept`,<br/>`const_pointer data() const noexcept` | 返回指向容器中第一个元素的指针                               |
| size          | `size_type size() const noexcept`                            | 返回容器中元素的数量                                         |
| max_size      | `size_type max_size() const noexcept`                        | 返回容器可容纳的最大元素数                                   |
| capacity      | `size_type capacity() const noexcept`                        | 返回当前为 vector 分配的存储空间（容量）的大小               |
| empty         | `bool empty() const noexcept`                                | 返回 vector 是否为空                                         |
| resize        | `void resize(size_type count)`,<br/>`void resize(size_type count, const T& value)` | 调整容器的大小，使其包含 n（参数）个元素                     |
| shrink_to_fit | `void shrink_to_fit()`                                       | 要求容器减小其 capacity（容量）以适应其 size（元素数量）     |
| push_back     | `void push_back(const T& value)`,<br/>`void push_back(T&& value)` | 在容器的最后一个元素之后添加一个新元素                       |
| pop_back      | `void pop_back()`                                            | 删除容器中的最后一个元素，有效地将容器 size 减少一个         |
| insert        | `iterator insert(const_iterator pos, const T& value)`,<br/>`iterator insert(const_iterator pos, T&& value)`,<br/>`iterator insert(const_iterator pos, size_type count, const T& value)`,<br/>`template <class InputIt>`<br/>`iterator insert(const_iterator pos, InputIt first, InputIt last)`,<br/>`iterator insert(const_iterator pos, initializer_list<T> ilist)` | 通过在指定位置的元素之前插入新元素来扩展该容器，通过插入元素的数量有效地增加容器大小 |
| erase         | `iterator erase(const_iterator pos)`,<br/>`iterator erase(const_iterator first, const_iterator last)` | 从 vector 中删除单个元素（`position`）或一系列元素（`[first，last)`），这有效地减少了被去除的元素的数量，从而破坏了容器的大小 |
| reserve       | `void reserve(size_type new_cap)`                            | 请求 vector 容量至少足以包含 n（参数）个元素                 |
| swap          | `void swap(vector& other) noexcept`                          | 通过 x（参数）的内容交换容器的内容，x 是另一个类型相同、size 可能不同的 vector 对象 |
| clear         | `void clear() noexcept`                                      | 从 vector 中删除所有的元素（被销毁），留下 size 为 0 的容器  |
| emplace       | `iterator emplace(const_iterator position, _Args&&... args)` | 通过在 position（参数）位置处插入新元素 args（参数）来扩展容器 |
| emplace_back  | `emplace_back(_Args&&... args)`                              | 在 vector 的末尾插入一个新的元素，紧跟在当前的最后一个元素之后 |
| get_allocator | `allocator_type get_allocator() const`                       | 返回与 vector 关联的构造器对象的副本                         |

#### 非成员函数

| 方法        | 原型                                                         | 简介                                                         |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| operator==  | `template <class T, class Allocator>`<br/>`bool operator==(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)` |                                                              |
| operator!=  | `template <class T, class Allocator>`<br/>`bool operator!=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)` |                                                              |
| operator<=> | `template <class T, class Allocator>`<br/>`__detail::__synth3way_t<T> operator<=>(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)` | 三路比较运算符，返回值是一个枚举类型，表示比较的结果：<br/> `std::strong_ordering::less`：如果左边的值小于右边的值。 <br/>`std::strong_ordering::equal`：如果左边的值等于右边的值。 <br/>`std::strong_ordering::greater`：如果左边的值大于右边的值。 |
| operator<   | `template <class T, class Allocator>`<br/>`bool operator<(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)` |                                                              |
| operator<=  | `template <class T, class Allocator>`<br/>`bool operator<=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)` |                                                              |
| operator>   | `template <class T, class Allocator>`<br/>`bool operator>(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)` |                                                              |
| operator>=  | `template <class T, class Allocator>`<br/>`bool operator>=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)` |                                                              |
| swap        | `template <class T, class Allocator>`<br/>`void swap(vector<T, Allocator>& lhs, vector<T, Allocator>& rhs)` |                                                              |

#### 迭代操作

| 方法    | 原型                                                         | 简介                                                     |
| ------- | ------------------------------------------------------------ | -------------------------------------------------------- |
| begin   | `iterator begin() noexcept`,<br/>`const_iterator begin() const noexcept` | 返回指向数组容器中第一个元素的迭代器                     |
| end     | `iterator end() noexcept`,<br/>`const_iterator end() const noexcept` | 返回指向数组容器中最后一个元素之后的理论元素的迭代器     |
| rbegin  | `reverse_iterator rbegin() noexcept`,<br/>`const_reverse_iterator rbegin() const noexcept` | 返回指向数组容器中最后一个元素的反向迭代器               |
| rend    | `reverse_iterator rend() noexcept`,<br/>`const_reverse_iterator rend() const noexcept` | 返回一个反向迭代器，指向数组中第一个元素之前的理论元素   |
| cbegin  | `const_iterator cbegin() const noexcept`                     | 返回指向数组容器中第一个元素的常量迭代器                 |
| cend    | `const_iterator cend() const noexcept`                       | 返回指向数组容器中最后一个元素之后的理论元素的常量迭代器 |
| crbegin | `const_reverse_iterator crbegin() const noexcept`            | 返回指向数组容器中最后一个元素的常量反向迭代器           |
| crend   | `const_reverse_iterator crend() const noexcept`              | 返回指向数组中第一个元素之前的理论元素的常量反向迭代器   |

#### Example

[vector_test.cpp](/resources/stl/vector_test.cpp)

<br/>

### deque

deque（['dek]）（双端队列）是 double-ended queue 的一个不规则缩写。

deque 是具有动态大小的序列容器，可以在两端（前端或后端）扩展或收缩。

#### 特性说明

- **随机访问**：支持 `operator[]` 和 `at()`，时间复杂度为 **O(1)**。
- **高效插入/删除**：头部和尾部插入/删除的时间复杂度为 **O(1)**，中间插入/删除为 **O(n)**。
- **内存结构**：通常由多个固定大小的内存块组成，这些内存块通过指针连接在一起。`std::deque` 的内存分配策略使得它在动态扩展时比 `std::vector` 更灵活，但可能会占用更多的内存。
- **适用场景**：适用于需要在两端频繁插入和删除元素的场景，例如实现队列、栈或其他需要动态操作两端的算法。

#### 成员函数

| 方法          | 原型                                                         | 简介                                                         |
| ------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| deque         | `deque()`                                                    | 默认构造函数，创建一个空的双端队列                           |
| deque         | `deque(size_type count)`                                     | 创建一个包含 `count` 个默认初始化的元素的双端队列            |
| deque         | `deque(size_type count, const T& value)`                     | 创建一个包含 `count` 个元素的双端队列，所有元素初始化为 `value` |
| deque         | `deque(const std::deque& other)`                             | 复制构造函数，创建一个与 `other` 相同的双端队列              |
| deque         | `deque(std::deque&& other)`                                  | 移动构造函数，将 `other` 的内容移动到新创建的双端队列中      |
| deque         | `deque(InputIterator first, InputIterator last)`             | 从范围 `[first, last)` 中的元素构造双端队列                  |
| deque         | `deque(std::initializer_list<T> init)`                       | 使用初始化列表构造双端队列                                   |
| operator=     | `operator=(const std::deque& other)`                         | 复制赋值运算符，将 `other` 的内容复制到当前双端队列中        |
| operator=     | `operator=(std::deque&& other)`                              | 移动赋值运算符，将 `other` 的内容移动到当前双端队列中        |
| operator=     | `operator=(std::initializer_list<T> init)`                   | 使用初始化列表赋值                                           |
| assign        | `assign(size_type count, const T& value)`                    | 使用 `count` 个 `value` 替换当前双端队列的内容               |
| assign        | `assign(std::initializer_list<T> init)`                      | 使用初始化列表替换当前双端队列的内容                         |
| assign        | `assign(InputIterator first, InputIterator last)`            | 使用范围 `[first, last)` 中的元素替换当前双端队列的内容      |
| operator[]    | `T& operator[](size_type pos)`,<br/>`const T& operator[](size_type pos) const` | 通过下标访问元素（无边界检查）                               |
| at            | `T& at(size_type pos)`,<br/>`const T& at(size_type pos) const` | 访问指定位置元素，若越界抛出 `std::out_of_range`             |
| front         | `T& front()`,<br/>`const T& front() const`                   | 返回头部元素的引用                                           |
| back          | `T& back()`,<br/>`const T& back() const`                     | 返回尾部元素的引用                                           |
| size          | `size()`                                                     | 返回双端队列中元素的数量                                     |
| max_size      | `max_size()`                                                 | 返回双端队列可以容纳的最大元素数量                           |
| empty         | `empty()`                                                    | 检查双端队列是否为空，返回布尔值                             |
| resize        | `void resize(size_type count)`,<br/>`void resize(size_type count, const T& value)` | 调整双端队列的大小，新元素默认初始化<br/>调整双端队列的大小，新元素使用 `value` 初始化 |
| shrink_to_fit | `shrink_to_fit()`                                            | 请求减少内存使用，但不保证成功                               |
| push_front    | `void push_front(const T& value)`,<br/>`void push_front(T&& value)` | 在 deque 容器的开始位置插入一个新的元素，位于当前的第一个元素之前 |
| push_back     | `void push_back(const T& value)`,<br/>`void push_back(T&& value)` | 在当前的最后一个元素之后 ，在 deque 容器的末尾添加一个新元素 |
| pop_front     | `void pop_front()`                                           | 删除 deque 容器中的第一个元素，有效地减小其大小              |
| pop_back      | `void pop_back()`                                            | 删除 deque 容器中的最后一个元素，有效地将容器大小减少一个    |
| emplace_front | `template<class... Args>`<br/>`reference emplace_front(Args&&... args)` | 在 deque 的开头插入一个新的元素， 直接在目标位置构造对象，避免了不必要的拷贝或移动操作 |
| emplace_back  | `template<class... Args>`<br/>`reference emplace_back(Args&&... args)` | 在 deque 的末尾插入一个新的元素， 直接在目标位置构造对象，避免了不必要的拷贝或移动操作 |
| insert        | `iterator insert(const_iterator pos, const T& value)`,<br/>`iterator insert(const_iterator pos, T&& value)`,<br/>`iterator insert(const_iterator pos, size_type count, const T& value)`,<br/>`template<class InputIt>`<br/>`iterator insert(const_iterator pos, InputIt first, InputIt last)`,<br/>`iterator insert(const_iterator pos, std::initializer_list<T> ilist)` | 在指定位置插入一个元素<br/>在指定位置插入一个元素（移动语义）<br/>在指定位置插入 `count` 个 `value`<br/>在指定位置插入范围 `[first, last)` 中的元素<br/>在指定位置插入初始化列表中的元素 |
| erase         | `iterator erase(const_iterator pos)`,<br/>`iterator erase(const_iterator first, const_iterator last)` | 删除指定位置的元素<br/>删除范围 `[first, last)` 中的元素     |
| clear         | `clear()`                                                    | 清空双端队列中的所有元素                                     |
| swap          | `void swap(deque& other) noexcept`                           | 交换当前双端队列与 `other` 的内容                            |
| get_allocator | `Allocator get_allocator() const noexcept`                   | 获取迭代器                                                   |

#### 非成员函数

| 方法    | 原型                                                         | 简介                                             |
| ------- | ------------------------------------------------------------ | ------------------------------------------------ |
| swap    | `swap(std::deque& lhs, std::deque& rhs)`                     | 非成员函数，交换两个双端队列的内容               |
| reverse | `template <class BidirectionalIterator>`<br/>`void reverse(BidirectionalIterator first, BidirectionalIterator last)` | 反转容器中元素的顺序，支持所有支持双向迭代的容器 |

#### 迭代操作

| 方法    | 原型                                                         | 简介                                                     |
| ------- | ------------------------------------------------------------ | -------------------------------------------------------- |
| begin   | `iterator begin() noexcept`, `const_iterator begin() const noexcept` | 返回指向数组容器中第一个元素的迭代器                     |
| end     | `iterator end() noexcept`, `const_iterator end() const noexcept` | 返回指向数组容器中最后一个元素之后的理论元素的迭代器     |
| rbegin  | `reverse_iterator rbegin() noexcept`, `const_reverse_iterator rbegin() const noexcept` | 返回指向数组容器中最后一个元素的反向迭代器               |
| rend    | `reverse_iterator rend() noexcept`, `const_reverse_iterator rend() const noexcept` | 返回一个反向迭代器，指向数组中第一个元素之前的理论元素   |
| cbegin  | `const_iterator cbegin() const noexcept`                     | 返回指向数组容器中第一个元素的常量迭代器                 |
| cend    | `const_iterator cend() const noexcept`                       | 返回指向数组容器中最后一个元素之后的理论元素的常量迭代器 |
| crbegin | `const_reverse_iterator crbegin() const noexcept`            | 返回指向数组容器中最后一个元素的常量反向迭代器           |
| crend   | `const_reverse_iterator crend() const noexcept`              | 返回指向数组中第一个元素之前的理论元素的常量反向迭代器   |

#### Example

[deque_test.cpp](/resources/stl/deque_test.cpp)

<br/>

### forward\_list

forward_list（单向链表）是序列容器，允许在序列中的任何地方进行恒定的时间插入和擦除操作。

#### 特性说明

- **单向链表**：仅支持前向遍历，每个节点保存下一个节点的指针，无反向迭代器。
- **高效插入/删除**：头部插入/删除时间复杂度为 **O(1)**，指定位置后插入/删除为 **O(1)**（需已知前驱节点）。
- **内存结构**：节点离散分布，无预分配内存，适合频繁插入/删除但无需随机访问的场景。
- 与 `std::list` 对比：
  - 优点：内存占用更小（每个节点少一个指针）。
  - 缺点：无法直接访问尾部元素，无反向迭代器，无 `size()` 函数。
- **适用场景**：实现轻量级链表结构（如内存受限环境），或需要高频在任意位置插入/删除的场景。

#### 成员函数

| 方法          | 原型                                                         | 简介                                                         |
| ------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| forward\_list | `explicit forward_list(const Allocator& alloc = Allocator())` | 创建一个空的单向链表，可选分配器参数                         |
| forward_list  | `explicit forward_list(size_type count, const T& value, const Allocator& alloc = Allocator())` | 创建包含 `count` 个相同元素 `value` 的链表                   |
| forward_list  | `template<class InputIt>`<br/>`forward_list(InputIt first, InputIt last, const Allocator& alloc = Allocator())` | 通过其他容器的迭代器范围初始化链表                           |
| forward_list  | `forward_list(const forward_list& other)`,<br/>`forward_list(forward_list&& other)` | 支持深拷贝或移动语义构造                                     |
| forward_list  | `forward_list(std::initializer_list<T> init, const Allocator& alloc = Allocator())` | 通过初始化列表构造                                           |
| assign        | `void assign(size_type count, const T& value)`,<br/>`template<class InputIt>`<br/>`void assign(InputIt first, InputIt last)`,<br/>`void assign(std::initializer_list<T> ilist)` | 填充 count 个 value<br/>用迭代器范围赋值<br/>用初始化列表赋值 |
| front         | `T& front()`,<br/>`const T& front() const`                   | 返回头部元素的引用（无法直接访问尾部元素）                   |
| push_front    | `void push_front(const T& value)`,<br/>`void push_front(T&& value)` | 头部插入                                                     |
| insert_after  | `iterator insert_after(const_iterator pos, const T& value)`,<br/>`iterator insert_after(const_iterator pos, T&& value)`,<br/>`iterator insert_after(const_iterator pos, size_type count, const T& value)`,<br/>`template<class InputIt>`<br/>`iterator insert_after(const_iterator pos, InputIt first, InputIt last)`,<br/>`iterator insert_after(const_iterator pos, std::initializer_list<T> ilist)` | 指定位置后插入                                               |
| emplace_after | `template<class... Args>`<br/>`iterator emplace_after(const_iterator pos, Args&&... args)`,<br/>`template<class... Args>`<br/>`reference emplace_front(Args&&... args)` | 就地构造元素                                                 |
| pop_front     | `void pop_front()`                                           | 头部删除                                                     |
| erase_after   | `iterator erase_after(const_iterator pos)`                   | 删除 pos 后的元素                                            |
| erase_after   | `iterator erase_after(const_iterator first, const_iterator last)` | 删除区间                                                     |
| clear         | `void clear() noexcept`                                      | 清空链表                                                     |
| merge         | `void merge(forward_list& other)`,<br/>`void merge(forward_list&& other)`,<br/>`template<class Compare>`<br/>`void merge(forward_list& other, Compare comp)` | 合并链表                                                     |
| splice_after  | `void splice_after(const_iterator pos, forward_list& other)`,<br/>`void splice_after(const_iterator pos, forward_list&& other)`,<br/>`void splice_after(const_iterator pos, forward_list& other, const_iterator it)`,<br/>`void splice_after(const_iterator pos, forward_list&& other, const_iterator it)`,<br/>`void splice_after(const_iterator pos, forward_list& other, const_iterator first, const_iterator last)` | 链表拼接                                                     |
| remove        | `void remove(const T& value)`                                | 删除所有等于 value 的元素                                    |
| remove_if     | `template<class Predicate>`<br/>`void remove_if(Predicate p)` | 删除满足条件的元素                                           |
| reverse       | `void reverse() noexcept`                                    | 反转链表顺序                                                 |
| sort          | `void sort()`                                                | 排序（默认升序）                                             |
| sort          | `template<class Compare>`<br/>`void sort(Compare comp)`      | 按比较器排序                                                 |
| unique        | `void unique()`                                              | 删除连续重复元素                                             |
| unique        | `template<class BinaryPredicate>`<br/>`void unique(BinaryPredicate p)` | 按谓词删除连续重复                                           |
| empty         | `bool empty() const noexcept`                                | 是否为空                                                     |
| max_size      | `size_type max_size() const noexcept`                        | 支持的最大容量                                               |
| swap          | `void swap(forward_list& other) noexcept`                    | 交换两个链表的内容（高效，仅交换指针）                       |
| get_allocator | `Allocator get_allocator() const noexcept`                   | 返回关联的分配器                                             |

#### 迭代操作

| 方法         | 原型                                                         | 简介                                                 |
| ------------ | ------------------------------------------------------------ | ---------------------------------------------------- |
| begin        | `iterator begin() noexcept`,<br/>`const_iterator begin() const noexcept` | 返回指向数组容器中第一个元素的迭代器                 |
| end          | `iterator end() noexcept`,<br/>`const_iterator end() const noexcept` | 返回指向数组容器中最后一个元素之后的理论元素的迭代器 |
| before_begin | `iterator before_begin() noexcept`,<br/>`const_iterator before_begin() const noexcept` | 返回头节点之前的迭代器（用于插入/删除）              |

#### Example

[forward_list_test.cpp](/resources/stl/forward_list_test.cpp)

<br/>

### list

`std::list` ，双向链表，是序列容器。

每个元素（节点）包含两个指针，分别指向前驱和后继节点，允许双向遍历。

`std::list` 的内存不连续，因此**不支持随机访问**，但能保证插入和删除的时间复杂度为 **O(1)**（前提是已获取操作位置的迭代器）。

#### 特性说明

1. **数据结构**
   - 双向链表结构，每个节点保存前驱和后继指针。
   - 内存分散分配，无预分配机制，按需动态增减节点。
2. **时间复杂度**
   - **插入/删除**：在已知位置（通过迭代器）的插入或删除操作为 **O(1)**。
   - **查找/访问**：随机访问（如通过下标）需遍历链表，时间复杂度为 **O(n)**。
   - **排序**：成员函数 `sort()` 的时间复杂度为 **O(n log n)**。
   - **合并/拼接**：`merge()` 和 `splice()` 操作的时间复杂度为 **O(1)** 或 **O(n)**（取决于操作范围）。
3. **功能特性**
   - **双向迭代器**：支持正向和反向遍历（提供 `begin()`, `end()`, `rbegin()`, `rend()`）。
   - **高效中间操作**：无需移动其他元素即可插入/删除任意位置元素。
   - **内置算法**：提供链表专用的成员函数（如 `sort()`, `merge()`, `unique()`），比通用算法（如 `std::sort()`）更高效。
   - **无迭代器失效**：插入操作不会使其他迭代器失效；删除操作仅使被删元素的迭代器失效。
4. **内存特性**
   - 每个节点额外存储两个指针，内存开销大于 `std::vector` 和 `std::forward_list`。
   - 无容量（capacity）概念，内存占用与元素数量严格一致。

#### 适用场景

1. **频繁的任意位置插入/删除**
   例如：实现实时更新的任务队列、事件管理器等。
2. **避免迭代器失效**
   在需要长期持有迭代器且容器可能修改的场景（如游戏中的实体管理）。
3. **需要稳定排序或合并操作**
   利用成员函数 `sort()` 和 `merge()` 高效处理链表数据。
4. **无需随机访问**
   当算法仅需顺序遍历时（如 LRU 缓存实现）。

#### 成员函数

| 方法          | 原型                                                         | 简介                                               |
| ------------- | ------------------------------------------------------------ | -------------------------------------------------- |
| list          | `explicit list(const Allocator& alloc = Allocator())`        | 创建空链表，可选分配器参数                         |
| list          | `list(size_type n, const T& value, const Allocator& alloc = Allocator())` | 创建含`n`个相同元素`value`的链表                   |
| list          | `template\<class InputIt> list(InputIt first, InputIt last, const Allocator& alloc = Allocator())` | 通过其他容器的迭代器范围初始化链表                 |
| list          | `list(const list& other)`                                    | 深拷贝的构造方式                                   |
| list          | `list(list&& other)`                                         | 资源转移的构造方式                                 |
| list          | `list(std::initializer_list<T> init, const Allocator& alloc = Allocator())` | 通过初始化列表构造                                 |
| list          | `list& operator=(const list& other)`                         | 支持拷贝赋值                                       |
| list          | `list& operator=(list&& other)`                              | 支持拷贝赋值                                       |
| list          | `list& operator=(std::initializer_list<T> ilist)`            | 支持初始化列表赋值                                 |
| push_front    | `void push_front(const T& value)`,<br/>`void push_front(T&& value)` | 头部插入元素                                       |
| push_back     | `void push_back(const T& value)`,<br/>`void push_back(T&& value)` | 尾部插入元素                                       |
| insert        | `iterator insert(iterator pos, const T& value)`              | 在指定位置插入单个元素，返回新元素迭代器           |
| pop_front     | `void pop_front()`                                           | 删除头部元素                                       |
| pop_back      | `void pop_back()`                                            | 删除尾部元素                                       |
| erase         | `iterator erase(iterator pos)`                               | 删除指定位置元素，返回下一个元素迭代器             |
| splice        | `void splice(const_iterator pos, list& other)`               | 将`other`链表的全部元素转移到当前链表的`pos`位置前 |
| merge         | `void merge(list& other)`                                    | 合并两个已排序链表，合并后`other`为空              |
| emplace_front | `template<class... Args>`<br/>`reference emplace_front(Args&&... args)` | 在头部直接构造元素，避免拷贝                       |
| emplace_back  | `template<class... Args>`<br/>`reference emplace_back(Args&&... args)` | 在尾部直接构造元素                                 |
| size          | `size_type size() const`                                     | 返回元素数量                                       |
| empty         | `bool empty() const`                                         | 判断链表是否为空                                   |
| resize        | `void resize(size_type count)`                               | 调整链表大小，新增元素默认初始化                   |
| front         | `T& front()`,<br/>`const T& front() const`                   | 返回头部元素引用                                   |
| back          | `T& back()`,<br/>`const T& back() const`                     | 返回尾部元素引用                                   |
| begin         | `iterator begin() noexcept`                                  | 返回指向第一个元素的迭代器                         |
| end           | `iterator end() noexcept`                                    | 返回尾后迭代器                                     |
| rbegin        | `reverse_iterator rbegin() noexcept`                         | 返回指向末尾的反向迭代器                           |
| rend          | `reverse_iterator rend() noexcept`                           | 返回指向头部的反向迭代器                           |
| sort          | `void sort()`                                                | 对链表元素进行升序排序（平均O(n log n)）           |
| reverse       | `void reverse() noexcept`                                    | 反转链表元素顺序                                   |
| remove        | `size_type remove(const T& value)`                           | 删除所有等于`value`的元素                          |
| remove_if     | `template\<class UnaryPredicate>`<br/>`size_type remove_if(UnaryPredicate p)` | 删除满足谓词条件的元素                             |

#### 迭代操作

#### Example

[list_test.cpp](/resources/stl/list_test.cpp)

<br/>

### stack

stack 是一种容器适配器，用于在LIFO（后进先出）的操作，其中元素仅从容器的一端插入和提取。

#### 成员函数

| 方法    | 原型                                                         | 简介                                         |
| ------- | ------------------------------------------------------------ | -------------------------------------------- |
| stack   | `explicit stack(const Container& cont = Container())`        | 创建一个空栈，可选指定底层容器               |
| stack   | `stack(const stack& other)`                                  | 通过另一个栈的副本构造新栈                   |
| stack   | `explicit stack(const Container& cont)`                      | 用已有容器 `cont` 的内容初始化栈             |
| top     | `T& top()`,<br/>`const T& top() const`                       | 返回栈顶元素的引用（若栈为空则行为未定义）   |
| empty   | `bool empty() const`                                         | 返回栈是否为空                               |
| size    | `size_type size() const`                                     | 返回栈中元素的数量                           |
| push    | `void push(const T& value)`,<br/>`void push(T&& value)`      | 将元素 `value` 压入栈顶                      |
| emplace | `template<class... Args>`<br/>`void emplace(Args&&... args)` | 直接在栈顶构造元素（避免拷贝或移动）         |
| pop     | `void pop()`                                                 | 移除栈顶元素（若栈为空则行为未定义）         |
| swap    | `void swap(stack& other) noexcept`                           | 交换两个栈的内容（高效，仅交换底层容器指针） |

#### 迭代操作

#### Example

[stack_test.cpp](/resources/stl/stack_test.cpp)

<br/>

### queue

`std::queue` 是一个**容器适配器**（Container Adapter），基于FIFO（先进先出）原则实现。

它通过封装底层容器（默认为 `std::deque`）提供队列的操作接口，仅允许在队尾插入元素、队首删除元素，不支持直接访问中间元素。

#### 成员函数

| 方法    | 原型                                                         | 简介                                                 |
| ------- | ------------------------------------------------------------ | ---------------------------------------------------- |
| queue   | `explicit queue(const Container& cont = Container())`        | 创建一个空队列，可选指定底层容器                     |
| queue   | `queue(const queue& other)`                                  | 通过另一个队列的副本构造新队列                       |
| queue   | `explicit queue(const Container& cont)`                      | 用已有容器的内容初始化队列（元素顺序与容器顺序一致） |
| front   | `T& front()`,<br/>`const T& front() const`                   | 返回队首元素的引用（若队列为空则行为未定义）         |
| back    | `T& back()`,<br/>`const T& back() const`                     | 返回队尾元素的引用（若队列为空则行为未定义）         |
| empty   | `bool empty() const`                                         | 返回队列是否为空                                     |
| size    | `size_type size() const`                                     | 返回队列中元素的数量                                 |
| push    | `void push(const T& value)`,<br/>`void push(T&& value)`      | 将元素 `value` 插入队尾                              |
| emplace | `template<class... Args>`<br/>`void emplace(Args&&... args)` | 直接在队尾构造元素（避免拷贝或移动）                 |
| pop     | `void pop()`                                                 | 移除队首元素（若队列为空则行为未定义）               |
| swap    | `void swap(queue& other) noexcept`                           | 交换两个队列的内容（高效，仅交换底层容器指针）       |
|         |                                                              |                                                      |
|         |                                                              |                                                      |
|         |                                                              |                                                      |
|         |                                                              |                                                      |
|         |                                                              |                                                      |
|         |                                                              |                                                      |
|         |                                                              |                                                      |
|         |                                                              |                                                      |

#### **比较运算符**

支持 `==`, `!=`, `<`, `>`, `<=`, `>=` 等比较操作，比较两个队列的底层容器内容：

```cpp
template<class T, class Container>
bool operator==(const queue<T, Container>& lhs, const queue<T, Container>& rhs);

template<class T, class Container>
bool operator!=(const queue<T, Container>& lhs, const queue<T, Container>& rhs);

// 其他运算符类似...
```

#### 迭代操作

#### Example

```c++

```

### priority_queue

`std::priority_queue` 是一个**容器适配器**，提供基于优先级的元素访问。元素按优先级从高到低排列，默认使用最大堆实现（即顶部元素最大）。底层容器默认为 `std::vector`，但可指定其他支持随机访问迭代器和 `front()`、`push_back()`、`pop_back()` 操作的容器（如 `std::deque`）。

#### 特性说明

- **时间复杂度**：
  - `push()`、`emplace()`：均摊 **O(log n)**。
  - `pop()`：均摊 **O(log n)**。
  - `top()`、`size()`、`empty()`：**O(1)**。
- **迭代器**：不支持迭代器（无法遍历元素）。
- **内存管理**：由底层容器决定（如 `std::vector` 可能预分配内存）。
- **适用场景**：需要快速获取最高/最低优先级元素的场景（如任务调度、Dijkstra算法）。
- **空队列操作**：调用 `top()` 或 `pop()` 前需确保队列非空，否则导致未定义行为。
- **性能选择**：若需高频插入/删除，优先选择 `std::vector` 作为底层容器。
- **比较器方向**：默认比较器 `std::less<T>` 生成最大堆，`std::greater<T>` 生成最小堆。

#### 成员函数

| 方法           | 原型                                                         | 简介                                                         |
| -------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| priority_queue | `explicit priority_queue(const Compare& comp = Compare(), const Container& cont = Container())` | 创建空优先队列，可选比较器（如 `std::greater<T>`）和底层容器 |
| priority_queue | `template<class InputIt>`<br/>`priority_queue(InputIt first, InputIt last, const Compare& comp = Compare(), const Container& cont = Container())` | 用迭代器范围 `[first, last)` 的元素初始化队列，并构造堆      |
| priority_queue | `priority_queue(priority_queue&& other)`                     | 移动语义构造优先队列                                         |
| top            | `const T& top() const`                                       | 返回优先级最高的元素（即堆顶元素，若队列为空则行为未定义）   |
| empty          | `bool empty() const`                                         | 返回队列是否为空                                             |
| size           | `size_type size() const`                                     | 返回队列中的元素数量                                         |
| push           | `void push(const T& value)`,<br/>`void push(T&& value)`      | 插入元素并调整堆结构                                         |
| emplace        | `template<class... Args>`<br/>`void emplace(Args&&... args)` | 直接在底层容器中构造元素，避免拷贝/移动                      |
| pop            | `void pop()`                                                 | 移除堆顶元素（若队列为空则行为未定义）                       |
| swap           | `void swap(priority_queue& other) noexcept`                  | 交换两个优先队列的内容（高效，仅交换底层容器指针）           |
|                |                                                              |                                                              |
|                |                                                              |                                                              |
|                |                                                              |                                                              |
|                |                                                              |                                                              |
|                |                                                              |                                                              |
|                |                                                              |                                                              |
|                |                                                              |                                                              |
|                |                                                              |                                                              |
|                |                                                              |                                                              |

#### 比较运算符（C++20 起）

支持 `==`, `!=`, `<`, `>`, `<=`, `>=`，比较两个优先队列的底层容器内容（按堆顺序比较）：

```cpp
template<class T, class Container, class Compare>
bool operator==(const priority_queue<T, Container, Compare>& lhs,
                const priority_queue<T, Container, Compare>& rhs);

// 其他运算符类似...
```

#### 交换函数（C++11 起）

```cpp
template<class T, class Container, class Compare>
void swap(priority_queue<T, Container, Compare>& lhs,
          priority_queue<T, Container, Compare>& rhs) noexcept;
```

#### 底层容器选择

默认底层容器为 `std::vector`，但可显式指定其他容器：

```cpp
#include <queue>
#include <deque>

// 使用 deque 作为底层容器
std::priority_queue<int, std::deque<int>> pq;
```

#### 自定义比较函数

通过比较器（`Compare` 类型参数）定义优先级规则：

```cpp
#include <queue>
#include <functional>

// 最小堆（顶部元素最小）
std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;

// 自定义比较器
struct Compare {
    bool operator()(const int& a, const int& b) {
        return a % 10 > b % 10; // 按个位数降序排列
    }
};
std::priority_queue<int, std::vector<int>, Compare> custom_pq;
```

#### Example

[priority_queue_test.cpp](/resources/stl/priority_queue_test.cpp)

<br/>

### set

`std::set` 是一个**关联容器**，存储唯一元素（不允许重复），元素默认按升序排列（可自定义排序规则）。

底层通常实现为红黑树（自平衡二叉搜索树），保证插入、删除和查找的时间复杂度为 **O(log n)**。

#### 特性说明

- **唯一性**：所有元素唯一（重复插入会被忽略）。
- **有序性**：元素始终按比较器（如默认 `std::less<T>`）排序。
- **性能**：插入、删除、查找均为 **O(log n)**。
- **迭代器稳定性**：插入或删除元素不会使其他元素的迭代器失效。
- 与 `std::unordered_set` 对比：
  - 优点：有序，支持范围查询（如 `lower_bound`）。
  - 缺点：查找速度略慢于哈希表实现的 `unordered_set`。

#### 成员函数

| 方法        | 原型                                                         | 简介                                                     |
| ----------- | ------------------------------------------------------------ | -------------------------------------------------------- |
| set         | `explicit set(const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 创建空集合，可选比较器（如 `std::greater<T>`）和分配器   |
| set         | `template<class InputIt>`<br/>`set(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 用迭代器范围 `[first, last)` 的元素初始化集合            |
| set         | `set(const set& other)`,<br/>`set(set&& other)`              | 深拷贝或移动语义构造                                     |
| set         | `set(std::initializer_list<T> init, const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 通过初始化列表构造                                       |
| insert      | `std::pair<iterator, bool> insert(const T& value)`,<br/>`std::pair<iterator, bool> insert(T&& value)` | 返回插入位置的迭代器和是否成功（若元素已存在则失败）     |
| insert      | `iterator insert(const_iterator hint, const T& value)`       | 提供 `hint` 迭代器提示插入位置（可能优化插入速度）       |
| insert      | `template<class InputIt>`<br/>`void insert(InputIt first, InputIt last)`,<br/>`void insert(std::initializer_list<T> ilist)` | 插入范围或列表                                           |
| emplace     | `template<class... Args>`<br/>`std::pair<iterator, bool> emplace(Args&&... args)` | 直接构造元素（避免拷贝）                                 |
| erase       | `iterator erase(const_iterator pos)`                         | 删除指定位置的元素                                       |
| erase       | `size_type erase(const T& key)`                              | 删除所有匹配 `key` 的元素（实际最多删除1个，因元素唯一） |
| erase       | `iterator erase(const_iterator first, const_iterator last)`  | 删除范围                                                 |
| clear       | `void clear() noexcept`                                      | 清空集合                                                 |
| find        | `iterator find(const T& key)`,<br/>`const_iterator find(const T& key) const` | 返回指向 `key` 的迭代器，若未找到返回 `end()`            |
| count       | `size_type count(const T& key) const`                        | 返回 `key` 的数量（0或1，因元素唯一）                    |
| lower_bound | `iterator lower_bound(const T& key)`                         | 第一个 >= key 的元素                                     |
| upper_bound | `iterator upper_bound(const T& key)`                         | 第一个 > key 的元素                                      |
| equal_range | `std::pair<iterator, iterator> equal_range(const T& key)`    | 返回 [lower_bound, upper_bound)                          |
| key_comp    | `key_compare key_comp() const`                               | 返回用于比较元素的函数对象（如 `std::less<T>`）          |
| value_comp  | `value_compare value_comp() const`                           | 与 `key_comp()` 相同（因 `value` 即 `key`）              |
| empty       | `bool empty() const noexcept`                                | 是否为空                                                 |
| size        | `size_type size() const noexcept`                            | 元素数量                                                 |
| max_size    | `size_type max_size() const noexcept`                        | 系统支持的最大容量                                       |
|             |                                                              |                                                          |

#### 非成员函数

1. **比较运算符**
   支持 `==`, `!=`, `<`, `>`, `<=`, `>=`，按字典序比较两个集合。

2. **交换函数**

   cpp

   ```cpp
   template<class T, class Compare, class Alloc>
   void swap(set<T, Compare, Alloc>& lhs, set<T, Compare, Alloc>& rhs);
   ```

#### 迭代操作

#### Example

[set_test.cpp](/resources/stl/set_test.cpp)

<br/>

### multiset

`std::multiset` 是一个**关联容器**，存储可重复的元素，元素默认按升序排列。

底层通常实现为红黑树，保证插入、删除和查找的时间复杂度为 **O(log n)**。与 `std::set` 不同，`multiset` 允许键重复。

#### 特性说明

- **键可重复**：允许插入重复元素。
- **有序性**：元素始终按比较器排序（默认升序）。
- **性能**：插入、删除、查找均为 **O(log n)**。
- **迭代器稳定性**：插入或删除元素不会使其他元素的迭代器失效。
- 与 `std::unordered_multiset` 对比：
  - 优点：有序，支持范围查询。
  - 缺点：查找速度略慢于哈希表实现的 `unordered_multiset`。
- **插入重复元素**：`insert()` 总是成功，返回新插入元素的迭代器。
- **删除操作**：`erase(key)` 会删除所有匹配的键。
- **性能优化**：使用 `equal_range()` 快速获取所有重复键的范围。

#### 成员函数

| 方法        | 原型                                                         | 简介                                                   |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------ |
| multiset    | `explicit multiset(const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 创建空集合，可选比较器（如 `std::greater<T>`）和分配器 |
| multiset    | `template<class InputIt>`<br />`multiset(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 用迭代器范围 `[first, last)` 的元素初始化集合          |
| multiset    | `multiset(const set& other)`,<br />`multiset(multiset&& other)` | 深拷贝或移动语义构造                                   |
| multiset    | `multiset(std::initializer_list<T> init, const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 通过初始化列表构造                                     |
| insert      | `iterator  insert(const T& value)`, <br />`iterator  insert(T&& value)` | 返回插入位置的迭代器和是否成功（若元素已存在则失败）   |
| insert      | `iterator insert(const_iterator hint, const T& value)`       | 提供 `hint` 迭代器提示插入位置（可能优化插入速度）     |
| insert      | `template<class InputIt>`<br />`void insert(InputIt first, InputIt last)`, <br />`void insert(std::initializer_list<T> ilist)` | 插入范围或列表                                         |
| emplace     | `template<class... Args>`<br /> `std::pair<iterator, bool> emplace(Args&&... args)` | 直接构造元素（避免拷贝）                               |
| erase       | `iterator erase(const_iterator pos)`                         | 删除指定位置的元素                                     |
| erase       | `size_type erase(const T& key)`                              | 删除所有匹配 `key` 的元素，返回删除的数量（可能大于1） |
| erase       | `iterator erase(const_iterator first, const_iterator last)`  | 删除范围                                               |
| clear       | `void clear() noexcept`                                      | 清空集合                                               |
| merge       | `void merge(multiset& other)`                                | 将 `other` 中所有元素移动到当前集合（保留重复性）      |
| find        | `iterator find(const T& key)`,<br /> `const_iterator find(const T& key) const` | 返回指向 `key` 的迭代器，若未找到返回 `end()`          |
| count       | `size_type count(const T& key) const`                        | 返回 `key` 的数量（可能大于1）                         |
| lower_bound | `iterator lower_bound(const T& key)`                         | 第一个 >= key 的元素                                   |
| upper_bound | `iterator upper_bound(const T& key)`                         | 第一个 > key 的元素                                    |
| equal_range | `std::pair<iterator, iterator> equal_range(const T& key)`    | 返回所有匹配 key 的范围                                |
| key_comp    | `key_compare key_comp() const`                               | 返回用于比较元素的函数对象（如 `std::less<T>`）        |
| value_comp  | `value_compare value_comp() const`                           | 与 `key_comp()` 相同（因 `value` 即 `key`）            |
| empty       | `bool empty() const noexcept`                                | 是否为空                                               |
| size        | `size_type size() const noexcept`                            | 元素数量                                               |
| max_size    | `size_type max_size() const noexcept`                        | 系统支持的最大容量                                     |

#### 比较运算符

支持 `==`, `!=`, `<`, `>`, `<=`, `>=`，按字典序比较两个集合。

#### Example

[multiset_test.cpp](/resources/stl/multiset_test.cpp)

<br/>

### map

map 是关联容器，按照特定顺序存储由 key value (键值) 和 mapped value (映射值) 组合形成的元素。

#### 特性说明

- **键唯一性**：每个键唯一，重复插入会被忽略（需用 `std::multimap` 支持重复键）。
- **有序性**：键始终按比较器排序（默认升序）。
- **性能**：插入、删除、查找均为 **O(log n)**。
- **迭代器稳定性**：插入或删除元素不会使其他元素的迭代器失效。
- 与 `std::unordered_map` 对比：
  - 优点：有序，支持范围查询。
  - 缺点：查找速度略慢于哈希表实现的 `unordered_map`。

#### 成员函数

| 方法        | 原型                                                         | 简介                                                         |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| map         | `explicit map(const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 创建空映射，可选比较器（如 `std::greater<Key>`）和分配器     |
| map         | `map(const map& other)`,<br/>`map(map&& other)`              | 深拷贝或移动语义构造                                         |
| map         | `template<class InputIt>`<br/>`map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 用迭代器范围 `[first, last)` 的键值对初始化映射              |
| map         | `map(std::initializer_list<value_type> init, const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 通过 `{{key1, val1}, {key2, val2}}` 初始化映射               |
| operator[]  | `T& operator[](const Key& key)`                              | 若 key 不存在，插入默认值并返回引用                          |
| operator[]  | `T& operator[](Key&& key)`                                   | 移动语义版本                                                 |
| at          | `T& at(const Key& key)`,<br/>`const T& at(const Key& key) const` | 若键不存在，抛出 `std::out_of_range` 异常                    |
| insert      | `std::pair<iterator, bool> insert(const value_type& value)`,<br/>`std::pair<iterator, bool> insert(value_type&& value)` | 返回插入位置的迭代器和是否成功（若键已存在则失败）           |
| insert      | `iterator insert(const_iterator hint, const value_type& value)` | 提供 `hint` 迭代器优化插入位置                               |
| insert      | `template<class InputIt>`<br/>`void insert(InputIt first, InputIt last)`,<br/>`void insert(std::initializer_list<value_type> ilist)` | 插入范围或列表                                               |
| emplace     | `template<class... Args>`<br/>`std::pair<iterator, bool> emplace(Args&&... args)` | 直接构造 pair<Key, T>                                        |
| erase       | `iterator erase(const_iterator pos)`                         | 通过迭代器删除                                               |
| erase       | `size_type erase(const Key& key)`                            | 通过键删除                                                   |
| erase       | `iterator erase(const_iterator first, const_iterator last)`  | 删除范围                                                     |
| empty       | `bool empty() const noexcept`                                | 是否为空                                                     |
| size        | `size_type size() const noexcept`                            | 键值对数量                                                   |
| max_size    | `size_type max_size() const noexcept`                        | 系统支持的最大容量                                           |
| clear       | `void clear() noexcept`                                      | 清空映射                                                     |
| merge       | `void merge(map& other)`                                     | 将 `other` 中所有键值对移动到当前映射（保留键唯一性）        |
| extract     | `node_type extract(const_iterator pos)`,<br/>`node_type extract(const Key& key)` | 取键对应的节点（不释放内存）                                 |
| key_comp    | `key_compare key_comp() const`                               | 返回容器用于比较键的比较对象的副本                           |
| value_comp  | `value_compare value_comp() const`                           | 返回可用于比较两个元素的比较对象，以获取第一个元素的键是否在第二个元素之前 |
| find        | `iterator find(const Key& key)`,<br/>`const_iterator find(const Key& key) const` | 在容器中搜索具有等于 k（参数）的键的元素，如果找到则返回一个迭代器，否则返回 map::end 的迭代器 |
| count       | `size_type count(const Key& key) const`                      | 返回键的数量（0或1，因键唯一）                               |
| lower_bound | `iterator lower_bound(const Key& key)`                       | 返回一个非递减序列 `[first, last)`（参数）中的第一个大于等于值 val（参数）的位置的迭代器 |
| upper_bound | `iterator upper_bound(const Key& key)`                       | 返回一个非递减序列 `[first, last)`（参数）中第一个大于 val（参数）的位置的迭代器 |
| equal_range | `std::pair<iterator, iterator> equal_range(const Key& key)`  | 获取相同元素的范围，返回包含容器中所有具有与 k（参数）等价的键的元素的范围边界（`pair< map<char,int>::iterator, map<char,int>::iterator >`） |

#### 迭代操作

#### Example

[map_test.cpp](/resources/stl/map_test.cpp)

<br/>

### multimap

`std::multimap` 是一个**关联容器**，存储键值对（`pair<const Key, T>`），允许键重复。元素默认按键的升序排列，底层通常实现为红黑树。插入、删除和查找的时间复杂度为 **O(log n)**。与 `std::map` 不同，`multimap` 允许同一键关联多个值。

#### 特性说明

- **键可重复**：允许同一键关联多个值。
- **有序性**：键值对始终按键排序（默认升序）。
- **性能**：插入、删除、查找均为 **O(log n)**。
- **迭代器稳定性**：插入或删除不会使其他元素的迭代器失效。
- 与 `std::unordered_multimap` 对比：
  - 优点：有序，支持范围查询。
  - 缺点：查找速度略慢于哈希表实现的 `unordered_multimap`。
- **键值对插入**：需显式构造 `std::pair` 或使用 `emplace` 直接传递键和值。
- **删除操作**：`erase(key)` 删除所有匹配键的键值对。
- **范围查询**：`equal_range(key)` 结合循环可高效处理同一键的所有值。
- **与 `std::multiset` 区别**：`multimap` 存储键值对，`multiset` 仅存储单一值。

#### 成员函数

| 方法        | 原型                                                         | 简介                                                         |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| multimap    | `explicit multimap(const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 创建空映射，可选比较器（如 `std::greater<Key>`）和分配器     |
| multimap    | `multimap(const map& other)`, <br />`multimap(map&& other)`  | 深拷贝或移动语义构造                                         |
| multimap    | `template<class InputIt>`<br /> `multimap(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 用迭代器范围 `[first, last)` 的键值对初始化映射              |
| multimap    | `multimap(std::initializer_list<value_type> init, const Compare& comp = Compare(), const Allocator& alloc = Allocator())` | 通过 `{{key1, val1}, {key2, val2}}` 初始化映射               |
| operator[]  | `T& operator[](const Key& key)`                              | 若 key 不存在，插入默认值并返回引用                          |
| operator[]  | `T& operator[](Key&& key)`                                   | 移动语义版本                                                 |
| at          | `T& at(const Key& key)`, `const T& at(const Key& key) const` | 若键不存在，抛出 `std::out_of_range` 异常                    |
| insert      | `iterator insert(const std::pair<const Key, T>& value)`,<br/>`iterator insert(std::pair<const Key, T>&& value)` | 返回插入位置的迭代器和是否成功（若键已存在则失败）           |
| insert      | `iterator insert(const_iterator hint, const std::pair<const Key, T>& value)` | 提供 `hint` 迭代器优化插入位置                               |
| insert      | `template<class InputIt>`<br/>`void insert(InputIt first, InputIt last)`,<br/>`void insert(std::initializer_list<std::pair<const Key, T>> ilist)` | 插入范围或列表                                               |
| emplace     | `template<class... Args>`<br /> `std::pair<iterator, bool> emplace(Args&&... args)` | 直接构造键值对（避免拷贝）                                   |
| erase       | `iterator erase(const_iterator pos)`                         | 通过迭代器删除                                               |
| erase       | `size_type erase(const Key& key)`                            | 通过键删除                                                   |
| erase       | `iterator erase(const_iterator first, const_iterator last)`  | 删除范围                                                     |
| empty       | `bool empty() const noexcept`                                | 是否为空                                                     |
| size        | `size_type size() const noexcept`                            | 键值对数量                                                   |
| max_size    | `size_type max_size() const noexcept`                        | 系统支持的最大容量                                           |
| clear       | `void clear() noexcept`                                      | 清空映射                                                     |
| merge       | `void merge(multimap& other)`                                | 将 `other` 中所有键值对移动到当前映射（保留重复键）          |
| extract     | `node_type extract(const_iterator pos)`, `node_type extract(const Key& key)` | 取键对应的节点（不释放内存）                                 |
| key_comp    | `key_compare key_comp() const`                               | 返回容器用于比较键的比较对象的副本                           |
| value_comp  | `value_compare value_comp() const`                           | 返回可用于比较两个元素的比较对象，以获取第一个元素的键是否在第二个元素之前 |
| find        | `iterator find(const Key& key)`,<br /> `const_iterator find(const Key& key) const` | 在容器中搜索具有等于 k（参数）的键的元素，如果找到则返回一个迭代器，否则返回 map::end 的迭代器 |
| count       | `size_type count(const Key& key) const`                      | 返回键的数量（0或1，因键唯一）                               |
| lower_bound | `iterator lower_bound(const Key& key)`                       | 返回一个非递减序列 `[first, last)`（参数）中的第一个大于等于值 val（参数）的位置的迭代器 |
| upper_bound | `iterator upper_bound(const Key& key)`                       | 返回一个非递减序列 `[first, last)`（参数）中第一个大于 val（参数）的位置的迭代器 |
| equal_range | `std::pair<iterator, iterator> equal_range(const Key& key)`  | 获取相同元素的范围，返回包含容器中所有具有与 k（参数）等价的键的元素的范围边界（`pair< map<char,int>::iterator, map<char,int>::iterator >`） |

#### 比较运算符

支持 `==`, `!=`, `<`, `>`, `<=`, `>=`，按字典序比较两个映射

#### Example

[multimap_test.cpp](/resources/stl/multimap_test.cpp)

<br/>

### unordered_set

`std::unordered_set` 是一个**无序关联容器**，存储唯一元素（不允许重复），基于哈希表实现。插入、删除和查找的平均时间复杂度为 **O(1)**，最坏情况为 **O(n)**。元素无序存储，与 `std::set` 不同，`unordered_set` 不维护元素的顺序，但提供高效的哈希查找。

#### 特性说明

- **唯一性**：元素必须唯一，重复插入会被忽略。
- **哈希冲突**：性能依赖哈希函数的质量，劣质哈希函数会导致频繁冲突。
- **迭代器失效**：插入操作可能导致迭代器失效（若触发 `rehash`）。
- **无序性**：遍历顺序不确定，可能与插入顺序无关。
- **线程安全**：多个读操作安全，写操作需外部同步。
- **与 `std::unordered_multiset` 区别**：`unordered_set` 不允许重复元素，而 `unordered_multiset` 允许。

#### 成员函数

| 方法             | 原型                                                         | 简介                                                         |
| ---------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| unordered_set    | `explicit unordered_set(size_type bucket_count, `<br/>`                      const Hash& hash = Hash(), `<br/>`                      const KeyEqual& equal = KeyEqual(),`<br/>`                      const Allocator& alloc = Allocator())` | 创建空集合，可指定初始桶数量、哈希函数、键相等比较器和分配器 |
| unordered_set    | `template<class InputIt>`<br/>`unordered_set(InputIt first, InputIt last, `<br/>`             size_type bucket_count,`<br/>`             const Hash& hash = Hash(), `<br/>`             const KeyEqual& equal = KeyEqual(),`<br/>`             const Allocator& alloc = Allocator())` | 用迭代器范围 `[first, last)` 的元素初始化集合                |
| unordered_set    | `unordered_set(const unordered_set& other)`,<br/>`unordered_set(unordered_set&& other)` | 深拷贝或移动语义构造                                         |
| unordered_set    | `unordered_set(std::initializer_list<value_type> init, `<br/>`             size_type bucket_count,`<br/>`             const Hash& hash = Hash(), `<br/>`             const KeyEqual& equal = KeyEqual(),`<br/>`             const Allocator& alloc = Allocator())` | 通过初始化列表构造                                           |
| insert           | `std::pair<iterator, bool> insert(const T& value)`, `std::pair<iterator, bool> insert(T&& value)` | 返回插入位置的迭代器和是否成功（若元素已存在则失败）         |
| insert           | `iterator insert(const_iterator hint, const T& value)`       | 提供 `hint` 迭代器提示插入位置（可能优化插入速度）           |
| insert           | `template<class InputIt>`<br/>`void insert(InputIt first, InputIt last)`,<br/>`void insert(std::initializer_list<value_type> ilist)` | 插入范围或列表                                               |
| emplace          | `template<class... Args>` <br />`std::pair<iterator, bool> emplace(Args&&... args)` | 直接构造元素（避免拷贝）                                     |
| erase            | `iterator erase(const_iterator pos)`                         | 删除指定位置的元素                                           |
| erase            | `size_type erase(const Key& key)`                            | 删除所有匹配 `key` 的元素（实际最多删除1个，因元素唯一）     |
| erase            | `iterator erase(const_iterator first, const_iterator last)`  | 删除范围                                                     |
| clear            | `void clear() noexcept`                                      | 清空集合                                                     |
| merge            | `void merge(std::unordered_set<Key, Hash, KeyEqual, Allocator>& source)` | 将 `source` 中所有元素移动到当前集合，已存在的元素保留在 `source` 中 |
| extract          | `node_type extract(const_iterator pos)`,<br/>`node_type extract(const Key& key)` | 从集合中移除元素并返回其节点句柄（可用于插入到其他容器）     |
| find             | `iterator find(const Key& key)`, <br />`const_iterator find(const Key& key) const` | 返回指向 `key` 的迭代器，若未找到返回 `end()`                |
| count            | `size_type count(const Key& key) const`                      | 返回 `key` 的数量（对 `unordered_set` 为0或1）               |
| lower_bound      | `iterator lower_bound(const T& key)`                         | 第一个 >= key 的元素                                         |
| upper_bound      | `iterator upper_bound(const T& key)`                         | 第一个 > key 的元素                                          |
| equal_range      | `std::pair<iterator, iterator> equal_range(const T& key)`    | 返回 [lower_bound, upper_bound)                              |
| key_comp         | `key_compare key_comp() const`                               | 返回用于比较元素的函数对象（如 `std::less<T>`）              |
| value_comp       | `value_compare value_comp() const`                           | 与 `key_comp()` 相同（因 `value` 即 `key`）                  |
| empty            | `bool empty() const noexcept`                                | 是否为空                                                     |
| size             | `size_type size() const noexcept`                            | 元素数量                                                     |
| max_size         | `size_type max_size() const noexcept`                        | 系统支持的最大容量                                           |
| key_eq           | `KeyEqual key_eq() const`                                    | 返回用于判断键是否相等的函数对象                             |
| bucket_count     | `size_type bucket_count() const`                             | 哈希策略：当前桶数量                                         |
| max_bucket_count | `size_type max_bucket_count() const`                         | 哈希策略：最大允许桶数量                                     |
| bucket_size      | `size_type bucket_size(size_type n) const`                   | 哈希策略：第n个桶中的元素数量                                |
| bucket           | `size_type bucket(const Key& key) const`                     | 哈希策略：元素key所在的桶索引                                |
| load_factor      | `float load_factor() const`                                  | 哈希策略：当前负载因子（元素数/桶数）                        |
| max_load_factor  | `float max_load_factor() const`                              | 哈希策略：获取最大负载因子                                   |
| max_load_factor  | `void max_load_factor(float ml)`                             | 哈希策略：设置最大负载因子                                   |
| rehash           | `void rehash(size_type count)`                               | 哈希策略：调整桶数量为至少count                              |
| reserve          | `void reserve(size_type count)`                              | 哈希策略：预留空间至少容纳count个元素                        |
| hash_function    | `Hash hash_function() const`                                 | 哈希策略：返回用于计算哈希值的函数对象                       |

#### 比较运算符

支持 `==` 和 `!=`，比较两个集合是否包含相同元素（无序）。

#### 迭代操作

- 使用迭代器 `begin()`, `end()`, `cbegin()`, `cend()` 遍历（无序）。
- **无反向迭代器**（因元素无序存储）。

#### Example

[unordered_set_test.cpp](/resources/stl/unordered_set_test.cpp)

<br/>

### unordered_multiset

`std::unordered_multiset` 是一个**无序关联容器**，允许存储重复元素，基于哈希表实现。插入、删除和查找的平均时间复杂度为 **O(1)**，最坏情况为 **O(n)**。元素无序存储，与 `std::unordered_set` 不同，它允许同一元素多次出现。

#### 特性说明

- **允许重复**：同一元素可多次插入，需确保哈希和相等比较器正确处理重复。

- **哈希冲突**：性能依赖哈希函数质量，劣质哈希函数会导致桶内链表过长。

- **迭代器失效**：插入操作可能导致迭代器失效（若触发 `rehash`）。

- **无序性**：遍历顺序不确定，可能与插入顺序无关。

- 与 `std::unordered_set` 区别：

  * `unordered_multiset` 允许重复元素。

  - `insert()` 不返回 `pair<iterator, bool>`，而是直接返回迭代器（总是成功）。
  - `erase(key)` 可能删除多个元素。
  - `count(key)` 可能返回大于1的值。

- **线程安全**：多个读操作安全，写操作需外部同步。

#### 成员函数

| 方法               | 原型                                                         | 简介                                                         |
| ------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| unordered_multiset | `explicit unordered_multiset(size_type bucket_count, `<br/>`                      const Hash& hash = Hash(), `<br/>`                      const KeyEqual& equal = KeyEqual(),`<br/>`                      const Allocator& alloc = Allocator())` | 创建空集合，可指定初始桶数量、哈希函数、键相等比较器和分配器 |
| unordered_multiset | `template<class InputIt>`<br/>`unordered_multiset(InputIt first, InputIt last, `<br/>`             size_type bucket_count,`<br/>`             const Hash& hash = Hash(), `<br/>`             const KeyEqual& equal = KeyEqual(),`<br/>`             const Allocator& alloc = Allocator())` | 用迭代器范围 `[first, last)` 的元素初始化集合                |
| unordered_multiset | `unordered_multiset(const unordered_multiset& other)`,<br/>`unordered_multiset(unordered_multiset&& other)` | 深拷贝或移动语义构造                                         |
| unordered_multiset | `unordered_multiset(std::initializer_list<value_type> init, `<br/>`             size_type bucket_count,`<br/>`             const Hash& hash = Hash(), `<br/>`             const KeyEqual& equal = KeyEqual(),`<br/>`             const Allocator& alloc = Allocator())` | 通过初始化列表构造                                           |
| insert             | `std::pair<iterator, bool> insert(const T& value)`, `std::pair<iterator, bool> insert(T&& value)` | 返回指向插入元素的迭代器（总是成功，允许重复）               |
| insert             | `iterator insert(const_iterator hint, const T& value)`       | 提供 `hint` 迭代器优化插入，但不强制要求位置正确             |
| insert             | `template<class InputIt>`<br/>`void insert(InputIt first, InputIt last)`,<br/>`void insert(std::initializer_list<value_type> ilist)` | 插入范围或列表                                               |
| emplace            | `template<class... Args>` <br />`std::pair<iterator, bool> emplace(Args&&... args)` | 直接构造元素（避免拷贝）                                     |
| erase              | `iterator erase(const_iterator pos)`                         | 删除指定位置的元素                                           |
| erase              | `size_type erase(const Key& key)`                            | 删除所有匹配 `key` 的元素（实际最多删除1个，因元素唯一）     |
| erase              | `iterator erase(const_iterator first, const_iterator last)`  | 删除范围                                                     |
| clear              | `void clear() noexcept`                                      | 清空集合                                                     |
| merge              | `void merge(std::unordered_multiset<Key, Hash, KeyEqual, Allocator>& source)`,<br/>`void merge(std::unordered_set<Key, Hash, KeyEqual, Allocator>& source)` | 将 `source` 中所有元素移动到当前集合（包括重复元素）         |
| extract            | `node_type extract(const_iterator pos)`,<br/>`node_type extract(const Key& key)` | 从集合中移除元素并返回其节点句柄（可用于插入到其他容器）     |
| find               | `iterator find(const Key& key)`, <br />`const_iterator find(const Key& key) const` | 返回指向 `key` 的迭代器，若未找到返回 `end()`                |
| equal_range        | `std::pair<iterator, iterator> equal_range(const Key& key)`  | 返回所有匹配 `key` 的元素范围（可能包含多个元素）            |
| count              | `size_type count(const Key& key) const`                      | 返回 `key` 的数量（可能大于1）                               |
| lower_bound        | `iterator lower_bound(const T& key)`                         | 第一个 >= key 的元素                                         |
| upper_bound        | `iterator upper_bound(const T& key)`                         | 第一个 > key 的元素                                          |
| equal_range        | `std::pair<iterator, iterator> equal_range(const T& key)`    | 返回 [lower_bound, upper_bound)                              |
| key_comp           | `key_compare key_comp() const`                               | 返回用于比较元素的函数对象（如 `std::less<T>`）              |
| value_comp         | `value_compare value_comp() const`                           | 与 `key_comp()` 相同（因 `value` 即 `key`）                  |
| empty              | `bool empty() const noexcept`                                | 是否为空                                                     |
| size               | `size_type size() const noexcept`                            | 元素数量                                                     |
| max_size           | `size_type max_size() const noexcept`                        | 系统支持的最大容量                                           |
| key_eq             | `KeyEqual key_eq() const`                                    | 返回用于判断键是否相等的函数对象                             |
| bucket_count       | `size_type bucket_count() const`                             | 哈希策略：当前桶数量                                         |
| max_bucket_count   | `size_type max_bucket_count() const`                         | 哈希策略：最大允许桶数量                                     |
| bucket_size        | `size_type bucket_size(size_type n) const`                   | 哈希策略：第n个桶中的元素数量                                |
| bucket             | `size_type bucket(const Key& key) const`                     | 哈希策略：元素key所在的桶索引                                |
| load_factor        | `float load_factor() const`                                  | 哈希策略：当前负载因子（元素数/桶数）                        |
| max_load_factor    | `float max_load_factor() const`                              | 哈希策略：获取最大负载因子                                   |
| max_load_factor    | `void max_load_factor(float ml)`                             | 哈希策略：设置最大负载因子                                   |
| rehash             | `void rehash(size_type count)`                               | 哈希策略：调整桶数量为至少count                              |
| reserve            | `void reserve(size_type count)`                              | 哈希策略：预留空间至少容纳count个元素                        |
| hash_function      | `Hash hash_function() const`                                 | 哈希策略：返回用于计算哈希值的函数对象                       |

#### 比较运算符

支持 `==` 和 `!=`，比较两个集合是否包含相同元素（无序且允许重复）

#### 迭代操作

- 使用迭代器 `begin()`, `end()`, `cbegin()`, `cend()` 遍历（无序）。
- **无反向迭代器**（因元素无序存储）。

#### Example

[unordered_multiset_test.cpp](/resources/stl/unordered_multiset_test.cpp)

<br/>

### unordered_map

`std::unordered_map` 是一个**无序关联容器**，存储键值对（`pair<const Key, T>`），键唯一（不允许重复），基于哈希表实现。插入、删除和查找的平均时间复杂度为 **O(1)**，最坏情况为 **O(n)**。元素无序存储，与 `std::map` 不同，不维护键的排序，但提供高效的哈希查找。

#### 特性说明

- **键唯一性**：每个键只能出现一次，重复插入会被忽略。

- **哈希冲突**：性能依赖哈希函数质量，劣质哈希函数会导致频繁冲突。

- **迭代器失效**：插入操作可能导致迭代器失效（若触发 `rehash`）。

- **无序性**：遍历顺序不确定，可能与插入顺序无关。

- **线程安全**：多个读操作安全，写操作需外部同步。

- 与 `std::unordered_multimap` 区别：

  - `unordered_map` 键唯一，`unordered_multimap` 允许重复键。

  - `insert()` 返回 `pair<iterator, bool>`，而 `unordered_multimap` 直接返回迭代器。

  - `operator[]` 和 `at()` 仅在 `unordered_map` 中可用。

#### 成员函数

| 方法          | 原型                                                         | 简介                                                         |
| ------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| unordered_map | `explicit unordered_map(size_type bucket_count = /*实现定义*/, `<br/>`                      const Hash& hash = Hash(), `<br/>`                      const KeyEqual& equal = KeyEqual(),`<br/>`                      const Allocator& alloc = Allocator())` | 创建空映射，可指定初始桶数量、哈希函数、键相等比较器和分配器 |
| unordered_map | `unordered_map(const unordered_map& other)`,<br/>`unordered_map(unordered_map&& other)` | 深拷贝或移动语义构造                                         |
| unordered_map | `template<class InputIt>`<br/>`unordered_map(InputIt first, InputIt last, `<br/>`             size_type bucket_count = /*实现定义*/,`<br/>`             const Hash& hash = Hash(), `<br/>`             const KeyEqual& equal = KeyEqual(),`<br/>`             const Allocator& alloc = Allocator())` | 用迭代器范围 `[first, last)` 的键值对初始化映射（键必须唯一） |
| unordered_map | `unordered_map(std::initializer_list<std::pair<const Key, T>> init, `<br/>`             size_type bucket_count = /*实现定义*/,`<br/>`             const Hash& hash = Hash(), `<br/>`             const KeyEqual& equal = KeyEqual(),`<br/>`             const Allocator& alloc = Allocator())` | 通过 `{{key1, val1}, {key2, val2}}` 初始化映射               |
| operator[]    | `T& operator[](const Key& key)`                              | 返回键关联的值的引用（若键不存在，插入新键值对，值为默认构造） |
| operator[]    | `T& operator[](Key&& key)`                                   | 移动语义版本                                                 |
| at            | `T& at(const Key& key)`,<br/>`const T& at(const Key& key) const` | 若键不存在，抛出 `std::out_of_range` 异常                    |
| insert        | `std::pair<iterator, bool> insert(const std::pair<const Key, T>& value)`,<br/>`std::pair<iterator, bool> insert(std::pair<const Key, T>&& value)` | 返回插入位置的迭代器和是否成功（若键已存在则失败）           |
| insert        | `iterator insert(const_iterator hint, const value_type& value)` | 提供 `hint` 迭代器优化插入位置                               |
| insert        | `template<class InputIt>`<br/>`void insert(InputIt first, InputIt last)`,<br/>`void insert(std::initializer_list<value_type> ilist)` | 插入范围或列表                                               |
| emplace       | `template<class... Args>`<br/>`std::pair<iterator, bool> emplace(Args&&... args)` | 直接构造 pair<Key, T>                                        |
| erase         | `iterator erase(const_iterator pos)`                         | 通过迭代器删除                                               |
| erase         | `size_type erase(const Key& key)`                            | 通过键删除                                                   |
| erase         | `iterator erase(const_iterator first, const_iterator last)`  | 删除范围                                                     |
| empty         | `bool empty() const noexcept`                                | 是否为空                                                     |
| size          | `size_type size() const noexcept`                            | 键值对数量                                                   |
| max_size      | `size_type max_size() const noexcept`                        | 系统支持的最大容量                                           |
| clear         | `void clear() noexcept`                                      | 清空映射                                                     |
| merge         | `void merge(std::unordered_map<Key, T, Hash, KeyEqual, Allocator>& source)`,<br/>`void merge(std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& source)` | 将 `source` 中所有键值对移动到当前映射（若键冲突，保留原键值对） |
| extract       | `node_type extract(const_iterator pos)`,<br/>`node_type extract(const Key& key)` | 取键对应的节点（不释放内存）                                 |
| key_comp      | `key_compare key_comp() const`                               | 返回容器用于比较键的比较对象的副本                           |
| value_comp    | `value_compare value_comp() const`                           | 返回可用于比较两个元素的比较对象，以获取第一个元素的键是否在第二个元素之前 |
| find          | `iterator find(const Key& key)`,<br/>`const_iterator find(const Key& key) const` | 在容器中搜索具有等于 k（参数）的键的元素，如果找到则返回一个迭代器，否则返回 map::end 的迭代器 |
| count         | `size_type count(const Key& key) const`                      | 返回键的数量（0或1，因键唯一）                               |
| lower_bound   | `iterator lower_bound(const Key& key)`                       | 返回一个非递减序列 `[first, last)`（参数）中的第一个大于等于值 val（参数）的位置的迭代器 |
| upper_bound   | `iterator upper_bound(const Key& key)`                       | 返回一个非递减序列 `[first, last)`（参数）中第一个大于 val（参数）的位置的迭代器 |
| equal_range   | `std::pair<iterator, iterator> equal_range(const Key& key)`  | 获取相同元素的范围，返回包含容器中所有具有与 k（参数）等价的键的元素的范围边界（`pair< map<char,int>::iterator, map<char,int>::iterator >`） |
| key_eq           | `KeyEqual key_eq() const`                                    | 返回用于判断键是否相等的函数对象                             |
| bucket_count     | `size_type bucket_count() const`                             | 哈希策略：当前桶数量                                         |
| max_bucket_count | `size_type max_bucket_count() const`                         | 哈希策略：最大允许桶数量                                     |
| bucket_size      | `size_type bucket_size(size_type n) const`                   | 哈希策略：第n个桶中的元素数量                                |
| bucket           | `size_type bucket(const Key& key) const`                     | 哈希策略：元素key所在的桶索引                                |
| load_factor      | `float load_factor() const`                                  | 哈希策略：当前负载因子（元素数/桶数）                        |
| max_load_factor  | `float max_load_factor() const`                              | 哈希策略：获取最大负载因子                                   |
| max_load_factor  | `void max_load_factor(float ml)`                             | 哈希策略：设置最大负载因子                                   |
| rehash           | `void rehash(size_type count)`                               | 哈希策略：调整桶数量为至少count                              |
| reserve          | `void reserve(size_type count)`                              | 哈希策略：预留空间至少容纳count个元素                        |
| hash_function    | `Hash hash_function() const`                                 | 哈希策略：返回用于计算哈希值的函数对象                       |

#### 迭代操作

- 使用迭代器 `begin()`, `end()`, `cbegin()`, `cend()` 遍历（无序）。
- **无反向迭代器**（因元素无序存储）。

#### Example

```c++

```

### unordered\_multimap

`std::unordered_multimap` 是一个**无序关联容器**，存储键值对（`pair<const Key, T>`），**允许键重复**，基于哈希表实现。插入、删除和查找的平均时间复杂度为 **O(1)**，最坏情况为 **O(n)**。与 `std::unordered_map` 不同，它允许多个键相同的键值对共存，适用于需要键重复的场景。

#### 特性说明

- **允许重复键**：同一键可关联多个值，需确保哈希和相等比较器正确处理重复。

- **哈希冲突**：性能依赖哈希函数质量，劣质哈希函数会导致桶内链表过长。

- **迭代器失效**：插入操作可能导致迭代器失效（若触发 `rehash`）。

- **无序性**：遍历顺序不确定，可能与插入顺序无关。

- 与 `std::unordered_map` 区别：

  - `unordered_multimap` 允许重复键，`unordered_map` 键唯一。

  - `insert()` 直接返回迭代器（不返回 `pair<iterator, bool>`）。

  - `erase(key)` 可能删除多个元素。

  - `count(key)` 可能返回大于1的值。

* **线程安全**：多个读操作安全，写操作需外部同步。

#### 成员函数

| 方法          | 原型                                                         | 简介                                                         |
| ------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| unordered_multimap | `explicit unordered_multimap(size_type bucket_count = /*实现定义*/, `<br/>`                      const Hash& hash = Hash(), `<br/>`                      const KeyEqual& equal = KeyEqual(),`<br/>`                      const Allocator& alloc = Allocator())` | 创建空映射，可指定初始桶数量、哈希函数、键相等比较器和分配器 |
| unordered_multimap | `unordered_multimap(const unordered_multimap& other)`,<br/>`unordered_multimap(unordered_multimap&& other)` | 深拷贝或移动语义构造                                         |
| unordered_multimap | `template<class InputIt>`<br/>`unordered_multimap(InputIt first, InputIt last, `<br/>`             size_type bucket_count = /*实现定义*/,`<br/>`             const Hash& hash = Hash(), `<br/>`             const KeyEqual& equal = KeyEqual(),`<br/>`             const Allocator& alloc = Allocator())` | 用迭代器范围 `[first, last)` 的键值对初始化映射（键必须唯一） |
| unordered_multimap | `unordered_multimap(std::initializer_list<std::pair<const Key, T>> init, `<br/>`             size_type bucket_count = /*实现定义*/,`<br/>`             const Hash& hash = Hash(), `<br/>`             const KeyEqual& equal = KeyEqual(),`<br/>`             const Allocator& alloc = Allocator())` | 通过 `{{key1, val1}, {key2, val2}}` 初始化映射               |
| insert        | `std::pair<iterator, bool> insert(const std::pair<const Key, T>& value)`,<br/>`std::pair<iterator, bool> insert(std::pair<const Key, T>&& value)` | 返回插入位置的迭代器和是否成功（若键已存在则失败）           |
| insert        | `iterator insert(const_iterator hint, const value_type& value)` | 提供 `hint` 迭代器优化插入位置                               |
| insert        | `template<class InputIt>`<br/>`void insert(InputIt first, InputIt last)`,<br/>`void insert(std::initializer_list<value_type> ilist)` | 插入范围或列表                                               |
| emplace       | `template<class... Args>`<br/>`std::pair<iterator, bool> emplace(Args&&... args)` | 直接构造 pair<Key, T>                                        |
| erase         | `iterator erase(const_iterator pos)`                         | 通过迭代器删除                                               |
| erase         | `size_type erase(const Key& key)`                            | 通过键删除                                                   |
| erase         | `iterator erase(const_iterator first, const_iterator last)`  | 删除范围                                                     |
| empty         | `bool empty() const noexcept`                                | 是否为空                                                     |
| size          | `size_type size() const noexcept`                            | 键值对数量                                                   |
| max_size      | `size_type max_size() const noexcept`                        | 系统支持的最大容量                                           |
| clear         | `void clear() noexcept`                                      | 清空映射                                                     |
| merge         | `void merge(std::unordered_map<Key, T, Hash, KeyEqual, Allocator>& source)`,<br/>`void merge(std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& source)` | 将 `source` 中所有键值对移动到当前映射（若键冲突，保留原键值对） |
| extract       | `node_type extract(const_iterator pos)`,<br/>`node_type extract(const Key& key)` | 取键对应的节点（不释放内存）                                 |
| key_comp      | `key_compare key_comp() const`                               | 返回容器用于比较键的比较对象的副本                           |
| value_comp    | `value_compare value_comp() const`                           | 返回可用于比较两个元素的比较对象，以获取第一个元素的键是否在第二个元素之前 |
| find          | `iterator find(const Key& key)`,<br/>`const_iterator find(const Key& key) const` | 在容器中搜索具有等于 k（参数）的键的元素，如果找到则返回一个迭代器，否则返回 map::end 的迭代器 |
| count         | `size_type count(const Key& key) const`                      | 返回键的数量（0或1，因键唯一）                               |
| lower_bound   | `iterator lower_bound(const Key& key)`                       | 返回一个非递减序列 `[first, last)`（参数）中的第一个大于等于值 val（参数）的位置的迭代器 |
| upper_bound   | `iterator upper_bound(const Key& key)`                       | 返回一个非递减序列 `[first, last)`（参数）中第一个大于 val（参数）的位置的迭代器 |
| equal_range   | `std::pair<iterator, iterator> equal_range(const Key& key)`  | 获取相同元素的范围，返回包含容器中所有具有与 k（参数）等价的键的元素的范围边界（`pair< map<char,int>::iterator, map<char,int>::iterator >`） |
| key_eq           | `KeyEqual key_eq() const`                                    | 返回用于判断键是否相等的函数对象                             |
| bucket_count     | `size_type bucket_count() const`                             | 哈希策略：当前桶数量                                         |
| max_bucket_count | `size_type max_bucket_count() const`                         | 哈希策略：最大允许桶数量                                     |
| bucket_size      | `size_type bucket_size(size_type n) const`                   | 哈希策略：第n个桶中的元素数量                                |
| bucket           | `size_type bucket(const Key& key) const`                     | 哈希策略：元素key所在的桶索引                                |
| load_factor      | `float load_factor() const`                                  | 哈希策略：当前负载因子（元素数/桶数）                        |
| max_load_factor  | `float max_load_factor() const`                              | 哈希策略：获取最大负载因子                                   |
| max_load_factor  | `void max_load_factor(float ml)`                             | 哈希策略：设置最大负载因子                                   |
| rehash           | `void rehash(size_type count)`                               | 哈希策略：调整桶数量为至少count                              |
| reserve          | `void reserve(size_type count)`                              | 哈希策略：预留空间至少容纳count个元素                        |
| hash_function    | `Hash hash_function() const`                                 | 哈希策略：返回用于计算哈希值的函数对象                       |

#### 迭代操作

- 使用迭代器 `begin()`, `end()`, `cbegin()`, `cend()` 遍历（无序）。
- **无反向迭代器**（因元素无序存储）。
- 无 `operator[]` 或 `at()`（因键不唯一，无法直接访问单一值）

#### Example

[unordered_map_test.cpp](/resources/stl/unordered_map_test.cpp)

<br/>

### tuple

`std::tuple` 是一个**固定大小的异构容器**，可以存储多个不同类型的值。元素通过编译时索引访问（基于 `std::get<N>`），且支持结构化绑定（C++17起）。常用于函数返回多个值或替代临时结构体。

#### 特性说明

- **固定大小**：元素数量在编译时确定，无法动态增减。
- **异构存储**：支持不同类型元素的组合。
- **编译时操作**：元素访问和类型操作依赖模板元编程。
- **性能**：无运行时开销（访问和操作在编译时解析）。
- **适用场景**：函数多返回值、替代临时结构体、模板元编程。

#### 成员函数

| 方法             | 原型                                                         | 简介                                       |
| ---------------- | ------------------------------------------------------------ | ------------------------------------------ |
| tuple            | `tuple()`                                                    | 创建一个所有元素默认初始化的元组           |
| tuple            | `tuple(const Types&... args)`                                | 从各元素的值构造                           |
| tuple            | `tuple(Types&&... args)`                                     | 移动语义构造                               |
| tuple            | `tuple(const tuple& other) = default`,<br/>`tuple(tuple&& other) = default` | 支持深拷贝或移动语义构造                   |
| tuple            | `template<class... UTypes>`<br/>`tuple(const tuple<UTypes...>& other)` | 从其他兼容元组构造，隐式转换（需类型兼容） |
| get              | `template<size_t I>`<br/>`auto& get(tuple& t) noexcept`      | 返回第 I 个元素的引用（I 为编译时常量）    |
| get              | `template<class T>`<br/>`auto& get(tuple& t) noexcept`       | 返回类型 T 的元素的引用                    |
| make_tuple       | `template<class... Types>`<br/>`tuple<VTypes...> make_tuple(Types&&... args)` | 创建元组                                   |
| tie              | `template<class... Types>`<br/>`tuple<Types&...> tie(Types&... args) noexcept` | 创建引用元组                               |
| forward_as_tuple | `template<class... Types>`<br/>`tuple<Types&&...> forward_as_tuple(Types&&... args) noexcept` | 创建转发引用元组                           |
| tuple_cat        | `template<class... Tuples>`<br/>`auto tuple_cat(Tuples&&... args)` | 连接元组                                   |
| swap             | `void swap(tuple& other) noexcept`                           | 交换内容                                   |

#### 结构化绑定（C++17）

将元组元素解包到变量：

```cpp
auto [x, y, z] = t3; // x=3.14, y="world", z=42
```

#### Example

[tuple_test.cpp](/resources/stl/tuple_test.cpp)

<br/>

### pair

`std::pair` 是一个模板类，用于将两个值（可能类型不同）组合成一个单元。常用于函数返回多个值，或作为关联容器（如 `std::map` 的元素类型）。

#### 特性说明

- **轻量级容器**：仅存储两个元素，无额外开销。
- **广泛用途**：用于多返回值、关联容器元素、结构化绑定等。
- **类型安全**：支持不同类型组合，编译时确定类型。
- **性能**：元素访问无运行时开销。

#### 成员变量

- 公开成员变量：`first` 和 `second`，可直接访问两个元素：

  ```cpp
  std::cout << p1.first << ", " << p1.second << std::endl;
  ```

#### 成员函数

| 方法      | 原型                                                         | 简介                                                         |
| --------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| pair      | `pair()`                                                     | 初始化 `first` 和 `second` 为各自类型的默认值                |
| pair      | `pair(const T1& x, const T2& y)`                             | 通过两个值构造                                               |
| pair      | `pair(T1&& x, T2&& y)`                                       | 移动语义构造                                                 |
| pair      | `pair(const pair&) = default`                                | 拷贝构造                                                     |
| pair      | `pair(pair&&) = default`                                     | 移动语义构造                                                 |
| pair      | `template<class U1, class U2>`<br/>`pair(const pair<U1, U2>& other)` | 类型可隐式转换时构造                                         |
| pair      | `template<class... Args1, class... Args2>`<br/>`pair(std::piecewise_construct_t, `<br/>`     std::tuple<Args1...> first_args,`<br/>`     std::tuple<Args2...> second_args)` | 分段构造，允许分别构造 `first` 和 `second`，避免不必要的拷贝/移动 |
| operator= | `pair& operator=(const pair& other)`                         | 拷贝赋值                                                     |
| operator= | `pair& operator=(pair&& other) noexcept`                     | 移动赋值                                                     |
| operator= | `template<class U1, class U2>`<br/>`pair& operator=(const pair<U1, U2>& other)` | 类型可隐式转换时赋值                                         |
| swap      | `void swap(pair& other) noexcept`                            | 交换两个 `pair` 的内容（要求 `first` 和 `second` 的类型支持 `swap`） |

#### 结构化绑定（C++17 起）

将 `pair` 解包到变量：

```cpp
auto [a, b] = p1; // a=10, b="hello"
```

#### 迭代操作

#### Example

[pair_test.cpp](/resources/stl/pair_test.cpp)

<br/>

## 算法（Algorithms）

### 核心思想

- **泛型编程**：通过模板支持任意容器类型（只需满足迭代器要求）。
- **无侵入性**：算法与容器解耦，通过迭代器操作数据。
- **高效性**：大多数算法时间复杂度优化到极致（如 `sort` 的$O(n log n)$）。
- **可组合性**：可与函数对象、Lambda表达式结合实现复杂逻辑。

### 算法分类及常用函数

#### 非修改型序列算法（Non-modifying）

不改变容器内容，仅读取数据。

| 算法            | 作用                     | 示例                                                         |
| --------------- | ------------------------ | ------------------------------------------------------------ |
| `std::for_each` | 对每个元素执行操作       | `for_each(vec.begin(), vec.end(), [](int x) { cout << x; });` |
| `std::find`     | 查找指定值               | `auto it = find(vec.begin(), vec.end(), 42);`                |
| `std::count`    | 统计元素出现次数         | `int n = count(vec.begin(), vec.end(), 5);`                  |
| `std::all_of`   | 检查所有元素是否满足条件 | `bool ok = all_of(vec.begin(), vec.end(), [](int x){ return x>0; });` |
| `std::search`   | 在序列中查找子序列       | `auto it = search(v1.begin(), v1.end(), v2.begin(), v2.end());` |

#### 修改型序列算法（Modifying）

直接修改容器内容或复制到其他位置。

| 算法             | 作用                   | 示例                                                         |
| ---------------- | ---------------------- | ------------------------------------------------------------ |
| `std::copy`      | 复制元素到目标位置     | `copy(src.begin(), src.end(), back_inserter(dest));`         |
| `std::transform` | 对元素转换后输出       | `transform(src.begin(), src.end(), dest.begin(), [](int x){return x*2;});` |
| `std::replace`   | 替换指定值             | `replace(vec.begin(), vec.end(), 3, 42);`                    |
| `std::fill`      | 填充容器为指定值       | `fill(vec.begin(), vec.end(), 0);`                           |
| `std::remove`    | 删除指定值（逻辑删除） | `auto it = remove(vec.begin(), vec.end(), 99); vec.erase(it, vec.end());` |

#### 排序与相关算法（Sorting/Partitioning）

| 算法               | 作用                         | 示例                                                         |
| ------------------ | ---------------------------- | ------------------------------------------------------------ |
| `std::sort`        | 快速排序（默认升序）         | `sort(vec.begin(), vec.end(), greater<int>());`              |
| `std::stable_sort` | 稳定排序（保持相等元素顺序） | `stable_sort(people.begin(), people.end(), compare_by_age);` |
| `std::nth_element` | 使第n个元素就位              | `nth_element(vec.begin(), vec.begin()+5, vec.end());`        |
| `std::partition`   | 根据条件划分元素             | `auto it = partition(vec.begin(), vec.end(), [](int x){return x%2==0;});` |

#### 搜索与二分查找

| 算法                 | 作用                      | 示例                                                      |
| -------------------- | ------------------------- | --------------------------------------------------------- |
| `std::binary_search` | 二分查找是否存在          | `bool found = binary_search(vec.begin(), vec.end(), 42);` |
| `std::lower_bound`   | 返回第一个≥目标值的迭代器 | `auto it = lower_bound(vec.begin(), vec.end(), 42);`      |
| `std::upper_bound`   | 返回第一个>目标值的迭代器 | `auto it = upper_bound(vec.begin(), vec.end(), 42);`      |
| `std::equal_range`   | 返回目标值的范围          | `auto range = equal_range(vec.begin(), vec.end(), 42);`   |

#### 数值运算算法（Numeric）

需包含头文件`<numeric>`。

| 算法                 | 作用                | 示例                                                         |
| -------------------- | ------------------- | ------------------------------------------------------------ |
| `std::accumulate`    | 累加/自定义累积操作 | `int sum = accumulate(vec.begin(), vec.end(), 0);`           |
| `std::inner_product` | 计算内积            | `int product = inner_product(v1.begin(), v1.end(), v2.begin(), 0);` |
| `std::partial_sum`   | 生成前缀和序列      | `partial_sum(vec.begin(), vec.end(), output.begin());`       |
| `std::iota`          | 填充递增序列        | `iota(vec.begin(), vec.end(), 10); // 10,11,12...`           |

#### 集合算法（Set Operations）

要求输入范围已排序。

| 算法                    | 作用   | 示例                                                         |
| ----------------------- | ------ | ------------------------------------------------------------ |
| `std::set_union`        | 求并集 | `set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), out.begin());` |
| `std::set_intersection` | 求交集 | `set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), out.begin());` |
| `std::set_difference`   | 求差集 | `set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), out.begin());` |

### 关键特性与使用技巧

#### 迭代器类别与算法匹配

| 迭代器类型         | 支持算法示例                            |
| ------------------ | --------------------------------------- |
| **输入迭代器**     | `std::find`, `std::count`               |
| **前向迭代器**     | `std::replace`, `std::search`           |
| **双向迭代器**     | `std::reverse`, `std::prev_permutation` |
| **随机访问迭代器** | `std::sort`, `std::nth_element`         |

#### 谓词（Predicate）与自定义操作
- **比较函数**：
  
  ```cpp
  sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {
      return a.size() < b.size();
  });
  ```
- **状态保持函数对象**：
  
  ```cpp
  struct Counter {
      int count = 0;
      void operator()(int x) { if (x > 0) ++count; }
  };
  Counter c = for_each(vec.begin(), vec.end(), Counter());
  cout << c.count;
  ```

#### 算法并行化（C++17+）
```cpp
#include <execution>
sort(std::execution::par, vec.begin(), vec.end()); // 并行排序
transform(std::execution::par_unseq, src.begin(), src.end(), dest.begin(), fn);
```

---

### 性能与最佳实践
1. **避免不必要的拷贝**：
   - 使用移动语义：
     ```cpp
     std::vector<std::string> data;
     std::sort(data.begin(), data.end(), 
         [](const std::string& a, const std::string& b) {
             return a.size() < b.size();
         });
     ```
2. **预分配内存**：
   ```cpp
   std::vector<int> result;
   result.reserve(src.size());
   std::transform(src.begin(), src.end(), 
                  std::back_inserter(result), 
                  [](int x) { return x * 2; });
   ```

3. **算法选择原则**：
   - 需要排序时优先选`sort`，稳定性要求高用`stable_sort`
   - 只找第k大元素用`nth_element`（O(n)）

---

### 示例代码整合
```cpp
#include <algorithm>
#include <vector>
#include <iostream>
#include <numeric>

int main() {
    std::vector<int> vec = {7, 3, 5, 1, 9, 2};

    // 排序并输出
    std::sort(vec.begin(), vec.end());
    std::for_each(vec.begin(), vec.end(), [](int x) {
        std::cout << x << " ";
    }); // 输出 1 2 3 5 7 9

    // 计算大于5的元素数量
    int count = std::count_if(vec.begin(), vec.end(), 
                             [](int x) { return x > 5; });
    std::cout << "\nCount: " << count; // 输出 2

    // 求前缀和
    std::vector<int> prefix(vec.size());
    std::partial_sum(vec.begin(), vec.end(), prefix.begin());
    // prefix: 1 3 6 11 18 27

    return 0;
}
```

---

### 总结
| **特性**     | **优势**                       | **注意事项**                        |
| ------------ | ------------------------------ | ----------------------------------- |
| **泛型设计** | 适用于所有满足迭代器要求的容器 | 需确保迭代器有效性                  |
| **高性能**   | 多数算法实现接近最优复杂度     | 错误使用可能导致O(n²)（如错误谓词） |
| **可扩展性** | 支持自定义谓词和函数对象       | Lambda中避免捕获大对象              |
| **并行支持** | C++17起支持并行执行            | 需硬件支持且注意线程安全            |

**核心价值**：STL算法通过标准化、泛型化的操作接口，极大提升了C++代码的生产力与性能，是现代化C++开发的核心工具集。

<br/>

## 迭代器（Iterators）


在C++标准模板库（STL）中，**迭代器（Iterators）** 是连接容器与算法的核心抽象，提供了一种统一的方式遍历和操作容器中的元素。以下是STL迭代器的详细分类、特性及使用指南：

---

### 迭代器核心概念
- **作用**：迭代器作为容器元素的访问接口，允许算法以相同的方式处理不同容器（如`vector`、`list`、`map`）。
- **类比指针**：支持类似指针的操作（如解引用`*it`、递增`++it`），但功能更丰富且类型安全。

---

### 迭代器分类及能力
STL定义了**五种迭代器类别**，按功能从弱到强排序：

| **迭代器类别**     | **支持操作**                                            | **典型容器/场景**              |
| ------------------ | ------------------------------------------------------- | ------------------------------ |
| **输入迭代器**     | 只读，单次遍历（`++`, `*`, `==`, `!=`）                 | `istream_iterator`             |
| **输出迭代器**     | 只写，单次遍历（`++`, `*`赋值）                         | `ostream_iterator`             |
| **前向迭代器**     | 可读写，支持多次遍历（继承输入/输出迭代器，可保存状态） | `forward_list`, `unordered_xx` |
| **双向迭代器**     | 支持前向和后向移动（`++`, `--`）                        | `list`, `set`, `map`           |
| **随机访问迭代器** | 支持跳跃访问（`it + n`, `it[n]`, `it1 - it2`）          | `vector`, `deque`, `array`     |

#### 操作对比
| **操作**            | 输入 | 输出 | 前向 | 双向 | 随机访问 |
| ------------------- | ---- | ---- | ---- | ---- | -------- |
| 读 (`*it`)          | ✔️    | ❌    | ✔️    | ✔️    | ✔️        |
| 写 (`*it = value`)  | ❌    | ✔️    | ✔️    | ✔️    | ✔️        |
| 递增 (`++it`)       | ✔️    | ✔️    | ✔️    | ✔️    | ✔️        |
| 递减 (`--it`)       | ❌    | ❌    | ❌    | ✔️    | ✔️        |
| 随机访问 (`it + 5`) | ❌    | ❌    | ❌    | ❌    | ✔️        |
| 比较顺序 (`<`, `>`) | ❌    | ❌    | ❌    | ❌    | ✔️        |

---

### 迭代器在STL算法中的应用
不同算法要求不同迭代器类别，例如：
- **`std::sort`**：需要**随机访问迭代器**（仅适用于`vector`, `deque`, `array`）。
- **`std::advance(it, n)`**：根据迭代器类别自动选择最优移动方式（线性或常数时间）。

#### **示例：算法与迭代器匹配**
```cpp
// 随机访问迭代器示例（vector）
std::vector<int> vec = {5, 3, 9, 1};
std::sort(vec.begin(), vec.end());  // OK

// 双向迭代器示例（list）
std::list<int> lst = {5, 3, 9, 1};
lst.sort();  // list必须使用成员函数sort，因为std::sort需要随机访问迭代器
```

---

### 迭代器适配器
适配器扩展迭代器功能，常见类型：

#### 1. 插入迭代器
- **`std::inserter`**：将元素插入容器指定位置。
  
  ```cpp
  std::vector<int> dest;
  std::copy(src.begin(), src.end(), std::inserter(dest, dest.begin()));
  ```

#### 2. 反向迭代器
- **`rbegin()`, `rend()`**：逆序遍历容器。
  ```cpp
  for (auto rit = vec.rbegin(); rit != vec.rend(); ++rit) {
      std::cout << *rit << " ";  // 逆序输出
  }
  ```

#### 3. 流迭代器
- **`istream_iterator`**：从输入流读取数据。
  ```cpp
  std::istream_iterator<int> input(std::cin), eof;
  std::vector<int> data(input, eof);  // 从标准输入读取整数
  ```
- **`ostream_iterator`**：向输出流写入数据。
  ```cpp
  std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "));
  ```

---

### 迭代器失效问题
某些容器操作会导致迭代器失效，需特别注意：

| **容器**              | **导致失效的操作**                        | **安全操作**                                                 |
| --------------------- | ----------------------------------------- | ------------------------------------------------------------ |
| `vector`/`deque`      | `insert`, `emplace`, `push_back`, `erase` | 插入后所有迭代器可能失效；`erase`后指向被删元素及之后的迭代器失效 |
| `list`/`forward_list` | `erase`                                   | 只有被删元素的迭代器失效                                     |
| `map`/`set`           | `erase`                                   | 只有被删元素的迭代器失效                                     |

#### **安全实践**
```cpp
std::vector<int> vec = {1, 2, 3, 4};
auto it = vec.begin() + 2;
vec.push_back(5);  // 可能导致it失效！
// 正确做法：在插入后重新获取迭代器
vec.push_back(5);
it = vec.begin() + 2;  // 重新赋值
```

---

### 自定义迭代器
为自定义容器实现迭代器需：
1. 定义嵌套类型：`iterator`, `const_iterator`。
2. 重载必要操作符：`++`, `--`, `*`, `->`, `==`, `!=`等。
3. 提供迭代器类别标签（如`std::random_access_iterator_tag`）。

#### **示例框架**
```cpp
class MyContainer {
public:
    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        // 重载操作符...
    };
    iterator begin() { /*...*/ }
    iterator end() { /*...*/ }
};
```

---

### C++11/14/17迭代器增强
- **基于范围的for循环**：依赖`begin()`和`end()`返回的迭代器。
  ```cpp
  for (int x : vec) { /* 等价于使用迭代器遍历 */ }
  ```
- **`std::begin()`/`std::end()`**：通用获取迭代器的方式，支持数组和容器。
  ```cpp
  int arr[] = {1, 2, 3};
  auto it = std::begin(arr);  // 返回指向首元素的指针
  ```

---

### 总结：迭代器核心价值
| **特性**     | **优势**                               |
| ------------ | -------------------------------------- |
| **泛型编程** | 统一容器与算法的接口，增强代码复用性   |
| **性能优化** | 随机访问迭代器支持O(1)复杂度的跳跃访问 |
| **灵活性**   | 适配器扩展功能（如反向遍历、流操作）   |
| **安全性**   | 类型系统防止越界访问（与裸指针相比）   |

**正确使用迭代器是高效利用STL的关键**，需结合容器特性和算法需求选择合适的迭代器类型。

<br/>

## 函数对象（Function Objects）


在C++标准模板库（STL）中，**函数对象（Function Objects）**，也称为**仿函数（Functors）**，是通过重载`operator()`的类实例，提供类似函数调用的行为。它们广泛用于STL算法中，实现灵活的操作逻辑，同时支持状态存储和高效内联优化。以下是详细解析：

---

### 核心概念
1. **定义**：
   - 函数对象是**类对象**，通过重载`operator()`实现函数调用语法。
   - 示例：
     ```cpp
     struct Adder {
         int increment;
         Adder(int inc) : increment(inc) {}
         int operator()(int x) const { 
             return x + increment; 
         }
     };
     Adder add5(5);
     cout << add5(3);  // 输出 8
     ```

2. **与函数指针的区别**：
   
   - **状态管理**：函数对象可携带成员变量，保存多次调用间的状态。
   - **内联优化**：编译器可内联函数对象的调用，提升性能。
   - **泛型支持**：函数对象作为模板参数时，类型信息完整，便于编译器优化。

---

### STL中的预定义函数对象
STL在`<functional>`中提供常用函数对象模板，用于算术、比较和逻辑操作。

#### 算术操作
| 函数对象             | 作用           | 示例                                |
| -------------------- | -------------- | ----------------------------------- |
| `std::plus<T>`       | 加法 (`x + y`) | `std::plus<int>()(3, 5) → 8`        |
| `std::minus<T>`      | 减法 (`x - y`) | `std::minus<int>()(7, 2) → 5`       |
| `std::multiplies<T>` | 乘法 (`x * y`) | `std::multiplies<int>()(4, 3) → 12` |

#### 比较操作
| 函数对象           | 作用            | 示例                                |
| ------------------ | --------------- | ----------------------------------- |
| `std::less<T>`     | 小于 (`x < y`)  | `std::less<int>()(3, 5) → true`     |
| `std::greater<T>`  | 大于 (`x > y`)  | `std::greater<int>()(7, 2) → true`  |
| `std::equal_to<T>` | 等于 (`x == y`) | `std::equal_to<int>()(5, 5) → true` |

#### 逻辑操作
| 函数对象              | 作用              | 示例                                            |
| --------------------- | ----------------- | ----------------------------------------------- |
| `std::logical_and<T>` | 逻辑与 (`x && y`) | `std::logical_and<bool>()(true, false) → false` |
| `std::logical_not<T>` | 逻辑非 (`!x`)     | `std::logical_not<bool>()(true) → false`        |

---

### 函数对象在STL算法中的应用
STL算法（如`sort`, `transform`, `remove_if`）通过函数对象实现灵活操作。

#### 排序算法（`std::sort`）
```cpp
std::vector<int> vec = {5, 3, 9, 1};
// 降序排序
std::sort(vec.begin(), vec.end(), std::greater<int>());
// 结果：{9, 5, 3, 1}
```

#### 变换算法（`std::transform`）
```cpp
std::vector<int> src = {1, 2, 3};
std::vector<int> dst(3);
// 每个元素加10
std::transform(src.begin(), src.end(), dst.begin(), [](int x) { return x + 10; });
// 结果：{11, 12, 13}
```

#### 条件移除（`std::remove_if`）
```cpp
std::vector<int> data = {2, 5, 8, 10, 3};
// 移除偶数
auto it = std::remove_if(data.begin(), data.end(), [](int x) { return x % 2 == 0; });
data.erase(it, data.end());
// 结果：{5, 3}
```

---

### 自定义函数对象与Lambda表达式
#### 自定义函数对象
```cpp
struct IsEven {
    bool operator()(int x) const { return x % 2 == 0; }
};
std::remove_if(data.begin(), data.end(), IsEven());
```

#### Lambda表达式（C++11+）
Lambda自动生成匿名函数对象，简化代码：
```cpp
std::sort(vec.begin(), vec.end(), [](int a, int b) { return a > b; });
```

---

### 函数对象适配器
适配器（如`std::bind`, `std::not1`）用于组合或修改函数对象行为。

#### `std::bind`（绑定参数）
```cpp
using namespace std::placeholders;
auto isGreaterThan5 = std::bind(std::greater<int>(), _1, 5);
std::vector<int> nums = {3, 7, 2, 8};
auto it = std::find_if(nums.begin(), nums.end(), isGreaterThan5);
// 找到第一个大于5的元素：7
```

#### `std::not1`（逻辑取反）
```cpp
auto isOdd = [](int x) { return x % 2 != 0; };
auto isEven = std::not1(std::function<bool(int)>(isOdd));
std::remove_if(data.begin(), data.end(), isEven);
```

---

### 性能优势与设计模式
- **性能优势**：函数对象作为模板参数时，编译器可内联调用，比函数指针更高效。
- **策略模式**：通过传递不同函数对象，动态改变算法行为（如多种排序策略）。

---

### 总结
| **特性**            | **函数对象**           | **函数指针**                |
| ------------------- | ---------------------- | --------------------------- |
| **状态管理**        | 支持（通过成员变量）   | 不支持                      |
| **内联优化**        | 支持（编译器可内联）   | 通常不支持                  |
| **泛型支持**        | 高（模板参数类型明确） | 低（类型擦除）              |
| **现代C++替代方案** | Lambda表达式（更简洁） | `std::function`（类型擦除） |

**核心作用**：函数对象为STL提供了高度灵活、高效且可扩展的操作逻辑实现方式，是泛型编程的重要基石。

<br/>

## 适配器（Adapters）


以下是C++标准模板库（STL）中**适配器（Adapters）**的全面解析，涵盖其定义、分类、用法及底层设计原理：

---

### 适配器核心概念
**适配器（Adapter）**是一种设计模式，用于**修改现有组件的接口或行为**，使其适应不同的使用场景。在STL中，适配器主要分为三类：  
1. **容器适配器**（如`stack`, `queue`）  
2. **迭代器适配器**（如`reverse_iterator`, `insert_iterator`）  
3. **函数对象适配器**（如`std::bind`, `std::not1`）  

---

### 容器适配器
#### **1. 作用与特点**
- **封装底层容器**：通过组合已有容器（如`deque`, `vector`），提供特定数据结构接口。  
- **限制操作**：仅暴露特定方法（如`stack`的`push`, `pop`）。  

#### **2. 常用容器适配器**
| 适配器                   | 默认底层容器 | 核心方法                     | 典型用途       |
| ------------------------ | ------------ | ---------------------------- | -------------- |
| `std::stack<T>`          | `deque<T>`   | `push()`, `pop()`, `top()`   | LIFO栈         |
| `std::queue<T>`          | `deque<T>`   | `push()`, `pop()`, `front()` | FIFO队列       |
| `std::priority_queue<T>` | `vector<T>`  | `push()`, `pop()`, `top()`   | 优先队列（堆） |

#### **3. 示例代码**
```cpp
#include <stack>
#include <queue>

// 使用deque作为底层容器实现栈
std::stack<int> s;
s.push(10);
s.push(20);
s.pop(); // 弹出20

// 使用vector作为底层容器实现优先队列
std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
pq.push(5);
pq.push(3);
pq.top(); // 3（最小堆）
```

#### **4. 自定义底层容器**
```cpp
// 使用vector作为stack的底层容器
std::stack<int, std::vector<int>> custom_stack;
```

---

### 迭代器适配器
#### **1. 作用与特点**
- **扩展迭代器功能**：提供反向遍历、插入操作、流操作等特殊迭代方式。  
- **透明性**：适配后的迭代器可与STL算法无缝协作。  

#### **2. 常用迭代器适配器**
| 适配器                  | 作用                   | 示例                                    |
| ----------------------- | ---------------------- | --------------------------------------- |
| `std::reverse_iterator` | 逆序遍历容器           | `vec.rbegin()`, `vec.rend()`            |
| `std::insert_iterator`  | 插入元素到容器指定位置 | `std::inserter(vec, vec.begin() + 2)`   |
| `std::back_inserter`    | 在容器末尾插入元素     | `std::back_inserter(vec)`               |
| `std::ostream_iterator` | 向输出流写入数据       | `std::ostream_iterator<int>(cout, " ")` |

#### **3. 示例代码**
```cpp
#include <iterator>
#include <algorithm>

std::vector<int> src = {1, 2, 3}, dest;

// 使用反向迭代器逆序复制
std::copy(src.rbegin(), src.rend(), std::back_inserter(dest));
// dest: {3, 2, 1}

// 将数据输出到控制台
std::copy(dest.begin(), dest.end(), std::ostream_iterator<int>(std::cout, " "));
```

---

### 函数对象适配器
#### **1. 作用与特点**
- **组合或修改函数对象**：通过绑定参数、逻辑取反等方式生成新函数对象。  
- **增强灵活性**：适配后的函数对象可更精确匹配算法需求。  

#### **2. 常用函数适配器**
| 适配器                   | 作用                       | 示例                            |
| ------------------------ | -------------------------- | ------------------------------- |
| `std::bind` (C++11)      | 绑定函数参数或调整参数顺序 | `std::bind(func, _2, _1)`       |
| `std::not1`, `std::not2` | 对谓词结果取反             | `std::not1(isEven)`             |
| `std::mem_fn`            | 将成员函数转换为函数对象   | `std::mem_fn(&Person::getName)` |

#### **3. 示例代码**
```cpp
#include <functional>

// 绑定参数：将二元函数转换为一元函数
auto add5 = std::bind(std::plus<int>(), std::placeholders::_1, 5);
int result = add5(3); // 8

// 取反适配器：筛选非偶数
auto isOdd = std::not1(std::function<bool(int)>([](int x) { return x % 2 == 0; }));
std::vector<int> nums = {1, 2, 3, 4};
auto it = std::remove_if(nums.begin(), nums.end(), isOdd);
// nums: {2, 4, 3, 4}（需手动erase）
```

---

### 适配器的底层实现原理
#### 组合模式
- **容器适配器**：通过内部持有底层容器对象（如`std::deque`），并重定向方法调用。  
  ```cpp
  template<typename T, typename Container = std::deque<T>>
  class stack {
  protected:
      Container c; // 底层容器
  public:
      void push(const T& value) { c.push_back(value); }
      void pop() { c.pop_back(); }
      // ...
  };
  ```

#### 运算符重载
- **迭代器适配器**：通过重载`operator++`, `operator*`等操作符改变迭代行为。  
  ```cpp
  template<typename Iterator>
  class reverse_iterator {
      Iterator current;
  public:
      reverse_iterator& operator++() { --current; return *this; }
      auto& operator*() { return *(current - 1); }
      // ...
  };
  ```

#### 3类型擦除（`std::function`）
- **函数适配器**：利用模板和闭包保存函数状态，统一调用接口。  
  ```cpp
  template<typename Fn, typename... Args>
  auto bind(Fn&& f, Args&&... args) {
      return [=](auto&&... rest) {
          return f(args..., std::forward<decltype(rest)>(rest)...);
      };
  }
  ```

---

### 适配器的现代C++替代方案
Lambda表达式替代函数适配器

```cpp
// 使用Lambda替代std::bind
auto add5 = [](int x) { return x + 5; };
```

范围库（Ranges Library, C++20）

```cpp
// 使用范围视图替代迭代器适配器
#include <ranges>
std::vector<int> vec = {1, 2, 3};
for (int x : vec | std::views::reverse) {
    std::cout << x << " "; // 3 2 1
}
```

---

### 总结
| **适配器类型**     | **核心价值**                       | **适用场景**                     |
| ------------------ | ---------------------------------- | -------------------------------- |
| **容器适配器**     | 快速实现特定数据结构（栈、队列）   | 需要LIFO/FIFO逻辑的场合          |
| **迭代器适配器**   | 扩展迭代器功能（反向遍历、流操作） | 逆序处理、数据流输入输出         |
| **函数对象适配器** | 动态生成灵活谓词或函数对象         | 参数绑定、逻辑组合、成员函数调用 |

**核心设计思想**：通过适配器模式，STL实现了对现有组件的高效复用和功能扩展，显著提升了代码的灵活性和可维护性。

<br/>

## 分配器（Allocator）

在C++标准模板库（STL）中，**分配器（Allocator）**是用于管理容器内存分配和释放的核心组件。

以下是分配器的详细解析，涵盖其设计目的、接口规范、实现要点及实际应用场景：

---

### 分配器的核心作用
1. **解耦内存管理**：
   允许容器（如`vector`, `list`）与底层内存分配策略分离，使开发者可自定义内存管理方式（如内存池、共享内存等）。  
2. **统一接口**：
   所有STL容器通过`allocator_traits`访问分配器，屏蔽不同分配器实现的差异。  
3. **性能优化**：
   针对特定场景（高频分配、固定大小对象）定制分配器，减少内存碎片，提升效率。

---

### 分配器接口规范
#### **1. 必需的类型定义**
```cpp
template<class T>
class Allocator {
public:
    using value_type = T;         // 分配元素的类型
    using pointer = T*;           // 指针类型
    using const_pointer = const T*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    // C++11起，以下类型通过allocator_traits自动推导，可省略
    template<class U> struct rebind { using other = Allocator<U>; };
};
```

#### **2. 核心成员函数**
| 函数                                                         | 作用                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `pointer allocate(size_type n)`                              | 分配至少容纳`n`个`T`类型对象的内存块，返回未初始化的内存指针。 |
| `void deallocate(pointer p, size_type n)`                    | 释放从`p`开始的`n`个`T`类型对象内存，`n`必须与分配时一致。   |
| `template<class U, class... Args> void construct(U* p, Args&&... args)` | 在`p`处构造`U`类型对象（C++11后通常由容器直接调用placement new）。 |
| `template<class U> void destroy(U* p)`                       | 销毁`p`处的对象（C++11后通常由容器直接调用析构函数）。       |

#### **3. 其他可选接口**
- **C++20前**：`address()`, `max_size()`等（C++20起通过`allocator_traits`自动提供）。  
- **移动语义**：分配器应支持无状态或正确处理状态迁移。

---

### 自定义分配器实现示例
以下是一个**跟踪内存分配**的简单分配器实现：

```cpp
#include <cstdlib>
#include <iostream>

template<typename T>
class TracingAllocator {
public:
    using value_type = T;

    TracingAllocator() = default;

    template<typename U>
    TracingAllocator(const TracingAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        std::size_t size = n * sizeof(T);
        T* p = static_cast<T*>(std::malloc(size));
        std::cout << "Allocated " << size << " bytes at " << p << std::endl;
        return p;
    }

    void deallocate(T* p, std::size_t n) noexcept {
        std::cout << "Deallocating " << n * sizeof(T) << " bytes at " << p << std::endl;
        std::free(p);
    }
};

// 支持rebind（C++11后可选，但某些容器如list仍需要）
template<typename T, typename U>
bool operator==(const TracingAllocator<T>&, const TracingAllocator<U>&) { return true; }

template<typename T, typename U>
bool operator!=(const TracingAllocator<T>&, const TracingAllocator<U>&) { return false; }
```

**使用方式**：
```cpp
#include <vector>
std::vector<int, TracingAllocator<int>> vec;
vec.push_back(42);  // 触发分配器日志输出
```

---

### 分配器在容器中的工作流程
以`std::vector`为例，其内存管理步骤为：
1. **扩容时**：调用`allocate(n)`申请新内存。  
2. **构造元素**：使用`allocator_traits::construct`（或直接`placement new`）。  
3. **迁移数据**：复制或移动旧元素到新内存。  
4. **析构旧元素**：调用`allocator_traits::destroy`（或显式析构）。  
5. **释放旧内存**：调用`deallocate(p, old_capacity)`。  

**关键点**：  

- C++11起，容器通过`allocator_traits`间接调用分配器方法，自动处理缺失的接口。  
- 无状态分配器（如`std::allocator`）可自由复制，有状态分配器需确保状态正确传递。

---

### 多态分配器（C++17 PMR）
C++17引入`std::pmr::polymorphic_allocator`，通过动态分派支持运行时多态内存管理。

#### **1. 核心特性**
- **类型擦除**：通过`memory_resource`基类抽象内存策略，允许运行时切换分配器行为。  
- **预定义内存资源**：  
  - `new_delete_resource`：使用全局`new`/`delete`。  
  - `monotonic_buffer_resource`：基于栈式缓冲区的快速分配器。  
  - `synchronized_pool_resource`：线程安全的内存池。  

#### **2. 使用示例**
```cpp
#include <memory_resource>
#include <vector>

// 使用单调缓冲区资源（预分配内存池）
char buffer[1024];
std::pmr::monotonic_buffer_resource pool{std::data(buffer), std::size(buffer)};
std::pmr::vector<int> vec(&pool);

vec.push_back(1);  // 在buffer内部分配内存
```

---

### 分配器设计注意事项
1. **无状态优先**：无状态分配器更易实现且兼容性高。  
2. **正确处理rebind**：确保容器内部结构（如`list`的节点）能获取对应分配器。  
3. **线程安全**：若分配器有状态，需自行保证线程安全。  
4. **内存对齐**：C++11后可通过`align_val_t`参数支持对齐分配。  

---

### 调试与验证技巧
- **日志跟踪**：在`allocate`/`deallocate`中添加输出，观察内存生命周期。  
- **工具检测**：使用 Valgrind、AddressSanitizer 检测内存泄漏或越界。  
- **性能测试**：对比自定义分配器与默认分配器的内存分配耗时及碎片情况。  

---

### **总结**
| **特性**     | **传统分配器**     | **多态分配器（PMR）**  |
| ------------ | ------------------ | ---------------------- |
| **接口类型** | 静态模板类         | 动态多态（类型擦除）   |
| **状态管理** | 通常无状态         | 关联`memory_resource`  |
| **适用场景** | 编译时确定内存策略 | 运行时动态切换内存策略 |
| **性能开销** | 低（内联优化）     | 稍高（虚函数调用）     |

**核心价值**：通过定制分配器，开发者可针对特定场景优化内存管理，提升应用性能和资源控制能力。

<br/>

## 获取 STL 源码

### cygwin

以 [Cygwin](https://www.cygwin.com/) 网络发行版安装程序 version 2.925 为例，安装于 `C:\cygwin64`。

代码所在目录：`C:\cygwin64\lib\gcc\x86_64-pc-cygwin\12\include\c++`

### 从  GNU

* `git clone git://gcc.gnu.org/git/gcc.git`
* `git checkout releases/gcc-12.4.0`
* STL 部分主要位于目录：`libstdc++-v3\include\std`

<br/>

## 参考

* [Containers library - cppreference.com](https://en.cppreference.com/w/cpp/container)
* [C++语言标准 | Tom's develop Blog (tomsworkspace.github.io)](https://tomsworkspace.github.io/2020/11/23/C++语言标准/)
* 《STL 源码剖析》 —— 侯捷
* [C++ 标准库头文件 - C++中文 - API参考文档](https://www.apiref.com/cpp-zh/cpp/header.html)

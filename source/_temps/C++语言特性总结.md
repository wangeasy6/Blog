---
title: C++语言特性总结
toc: true
categories:
  - Technology
tags:
  - 待续
---

比雅尼·斯特劳斯特鲁普博士在贝尔实验室工作期间在20世纪80年代发明并实现了C++。起初，这种语言被称作“C with Classes”（“包含‘类’的C语言”），作为C语言的增强版出现。
随后，C++不断增加新特性。虚函数、运算符重载、多继承、标准模板库、异常处理、运行时类型信息、名字空间等概念逐渐纳入标准。这些新技术的出现以及和原有技术的融合，使C++已经成为当今主流程序设计语言中最复杂的一员。

作为C语言的增强版，C++包含了C语言的所有基本概念，所以这里不再复数C语言的特性，而是专注于和C的不同和扩展部分，类的部分主要和python做对比。

<!--more-->

<br/>

# C++98

## 名字空间

一个标识符可在多个名字空间中定义，它在不同名字空间中的含义是互不相干的。此方法通过将逻辑上相关的标识符组织成相应的名字空间，可使整个系统更加模块化。名字空间通过作用域操作符（::）显示的指定命名空间。

## 流运算符（<<、 >>）

用于将左侧（右侧）运算对象的值写到右侧（左侧）运算对象表示的流，其本质是运算符重载。C++的输入输出系统较之C更加智能，它能够自己识别类型，节省了使用%d等字符的麻烦。另外，它是可扩展的，可以通过重定义运算符，使之识别所开发的新数据类型。

## 基础类型对比

| C                  | C++98                   | C++11              |
| ------------------ | ----------------------- | ------------------ |
| void               | void                    | void               |
|                    | bool                    | bool               |
| short              | short                   | short              |
| unsigned short     | unsigned short          | unsigned short     |
| int                | int                     | int                |
| unsigned int       | unsigned int            | unsigned int       |
| long               | long                    | long               |
| unsigned long      | unsigned long           | unsigned long      |
| long long          | long long               | long long          |
| unsigned long long | unsigned long long      | unsigned long long |
| float              | float                   | float              |
| unsigned float     | unsigned float          | unsigned float     |
| double             | double                  | double             |
| unsigned double    | unsigned double         | unsigned double    |
|                    | long double             |                    |
| char               | char                    | char               |
| unsigned char      | unsigned char           | unsigned char      |
|                    | wchar_t(L'' wcin wcout) | wchar_t            |
|                    |                         | char16_t(u'')      |
|                    |                         | char32_t(U'')      |
|                    | string                  | string             |
| 数组 []            | 数组 []                 | 数组 []            |
|                    | vector                  | vector             |
|                    | array                   | array              |
| struct             | struct                  | struct             |
| union              | union                   | union              |
| enum               | enum                    | enum               |
| 指针 *             | 指针 *                  | 指针 *             |
|                    | auto_ptr                |                    |
|                    | shared_ptr              | shared_ptr         |
|                    | weak_ptr                | weak_ptr           |
|                    |                         | unique_ptr         |
| 取地址 &           | 取地址 &                | 取地址 &           |
|                    | 引用 &                  | 引用 &             |
|                    | class                   | class              |
|                    |                         | auto               |



## 引用类型

引用并非typedef，也不是#define，引用是对象的别名，且在初始化之后引用不可更改。

'&'在做左值的时候是引用类型声明，在做右值的时候是取地址符。



## decltype 类型

是auto类型的一种补充，此类型希望从表达式的类型推断出要定义的变量的类型，但是不想用该表达式的值初始化变量。如果表达式是一个变量，则decltype返回该变量的类型，包括顶层const和引用在内。

## string类型

  可变长字符串，其本质是一个类类型。

## vector类型

  可变长集合，其本质是一个类模板。

## 迭代器

迭代器是string和vector的配套成员函数，常被用于访问其中的元素。如begin()返回第一个元素和end()返回最后一个元素之后的空位置。

## 重载运算符

对于类类型的运算对象，用户可以自定义操作运算符的含义。



## const

### 用于普通变量前

```c++
const int age = 10;
#define age 10;
```

const与define，两者都可以用来定义常量。

但是const定义时，定义了常量的类型，所以更精确一些。

#define只是简单的文本替换，除了可以定义常量外，还可以用来定义一些简单的函数，有点类似内联函数(Inline)。

const和define定义的常量可以放在头文件里面。（小注：可以多次声明，但只能定义一次）

### 用于指针变量前

```c++
int age = 10;
const int* p1 = &age; // p1可变，*p1不可变，此时不能用*p1来修改值，但是p1可以转向
int* const p2 = &age; // p2不可变，*p2可变，此时允许*p2修改值，但是p2不能转向
const int* const p3 = &age; // p3不可变，*p3也不可变
```

赋值时，const指针可以接受const和非const地址，但是非const指针只能接受非const地址。

```c++
const int a = 10;
const int *pa= &a;    // 合法

const int b = 10;
int *pb = &b;         // 非法
```



### 用于函数返回

```c++
const int function();   //此时const无意义
const int& function();
const user_class function();
const user_class& function();
void function() const;  // 表示声明本函数为常成员函数, 常成员函数是不能改变成员变量值的函数
```

## 智能指针

### C++98

```c++
std::auto_ptr<std::string> ps (new std::string(str))；
```

### C++ 11

1. shared_ptr
2. unique_ptr
3. weak_ptr
4. auto_ptr（被 C++11 弃用）

- Class shared_ptr 实现共享式拥有（shared ownership）概念。多个智能指针指向相同对象，该对象和其相关资源会在 “最后一个 reference 被销毁” 时被释放。为了在结构较复杂的情景中执行上述工作，标准库提供 weak_ptr、bad_weak_ptr 和 enable_shared_from_this 等辅助类。
- Class unique_ptr 实现独占式拥有（exclusive ownership）或严格拥有（strict ownership）概念，保证同一时间内只有一个智能指针可以指向该对象。你可以移交拥有权。它对于避免内存泄漏（resource leak）——如 new 后忘记 delete ——特别有用。

#### shared_ptr

多个智能指针可以共享同一个对象，对象的最末一个拥有着有责任销毁对象，并清理与该对象相关的所有资源。

- 支持定制型删除器（custom deleter），可防范 Cross-DLL 问题（对象在动态链接库（DLL）中被 new 创建，却在另一个 DLL 内被 delete 销毁）、自动解除互斥锁

#### weak_ptr

weak_ptr 允许你共享但不拥有某对象，一旦最末一个拥有该对象的智能指针失去了所有权，任何 weak_ptr 都会自动成空（empty）。因此，在 default 和 copy 构造函数之外，weak_ptr 只提供 “接受一个 shared_ptr” 的构造函数。

- 可打破环状引用（cycles of references，两个其实已经没有被使用的对象彼此互指，使之看似还在 “被使用” 的状态）的问题

#### unique_ptr

unique_ptr 是 C++11 才开始提供的类型，是一种在异常时可以帮助避免资源泄漏的智能指针。采用独占式拥有，意味着可以确保一个对象和其相应的资源同一时间只被一个 pointer 拥有。一旦拥有着被销毁或编程 empty，或开始拥有另一个对象，先前拥有的那个对象就会被销毁，其任何相应资源亦会被释放。

- unique_ptr 用于取代 auto_ptr

#### auto_ptr

被 c++11 弃用，原因是缺乏语言特性如 “针对构造和赋值” 的 `std::move` 语义，以及其他瑕疵。

#### auto_ptr 与 unique_ptr 比较

- auto_ptr 可以赋值拷贝，复制拷贝后所有权转移；unqiue_ptr 无拷贝赋值语义，但实现了`move` 语义；
- auto_ptr 对象不能管理数组（析构调用 `delete`），unique_ptr 可以管理数组（析构调用 `delete[]` ）；

## 面向对象

包含了封装、继承、多态等功能的新型数据类型——类（class）。

声明语法：

```c++
class 类名 <：访问限定符 基类名1,访问限定符 基类名2, ... ,访问限定符 基类名n>
{
    成员1;    //默认为 private

public:
    成员2;    // 可以被任意实体访问（对外接口）

protected:
    成员3;    // 只允许被子类及本类的成员函数访问

private:
    成员4;    // 只允许被本类的成员函数、友元类或友元函数访问
};
```

### 封装

把客观事物封装成抽象的类，并且类可以把自己的数据和方法只让可信的类或者对象操作，对不可信的进行信息隐藏。

### 继承

被继承的类型称为基类或超类，新产生的类为派生类或子类。基类和派生类的集合称作类继承层次结构。

1. 支持多继承

2. 访问控制有三种：公有（public）方式，保护（protected）方式和私有（private）方式，相应的继承亦称公有继承、保护继承和私有继承。

3. 创建派生类对象时，程序首先调用基类构造函数，然后再调用派生类构造函数。结束时，首先调用派生类析构，再调用基类析构。

4. 基类指针可以在不进行显示类型转换的情况下指向派生类对象；基类引用可以在不进行显示类型转换的情况下引用派生类对象。基类指针或引用只能调用基类方法。

    ```c++
    TennisPlayer p("Easy");
    Player * p1 = p;
    Player & p2 = p;
    p1->Name();
    p2.Name();
    ```

5. 在派生类中，如果有和基类重载的函数而要调用基类方法，需要使用作用域解析运算符来调用基类方法。



### 多态

1. 重载多态（Ad-hoc Polymorphism，编译期）：函数重载、运算符重载
2. 子类型多态（Subtype Polymorphism，运行期）：虚函数
3. 参数多态性（Parametric Polymorphism，编译期）：类模板、函数模板
4. 强制多态（Coercion Polymorphism，编译期/运行期）：基本类型转换、自定义类型转换

## 面向泛型

泛型，即对不定类型的编程支持。

### 迭代器

# C++11

## auto类型

  和C一样，C++是静态类型语言，需要先声明变量类型，才能使用。但C++提供了一种auto类型，允许编译器根据表达式的值来推算变量的类型。这让语言具有了一些动态类型语言的特性。

## lambda函数

该形式是C++中的lambda函数（匿名函数）。

基本形式如下：

`[capture](parameters)->return-type {body}`

[]叫做捕获说明符，表示一个lambda表达式的开始。接下来是参数列表，即这个匿名的lambda函数的参数，->return-type表示返回类型，如果没有返回类型，则可以省略这部分。最后就是函数体部分了。

我们可以这样输出"hello,world"

```c++
auto func = [] () { cout << "hello,world"; };
func(); // now call the function
```

变量捕获与lambda闭包实现

```
string hello = "hello,world";
[&](){cout << hello;}();
```

lambda函数能够捕获lambda函数外的具有自动存储时期的变量。函数体与这些变量的集合合起来叫闭包。

    [] 不截取任何变量
    [&} 截取外部作用域中所有变量，并作为引用在函数体中使用
    [=] 截取外部作用域中所有变量，并拷贝一份在函数体中使用
    [=, &foo] 截取外部作用域中所有变量，并拷贝一份在函数体中使用，但是对foo变量使用引用
    [bar] 截取bar变量并且拷贝一份在函数体重使用，同时不截取其他变量
    [x, &y] x按值传递，y按引用传递
    [this] 截取当前类中的this指针。如果已经使用了&或者=就默认添加此选项。



# 参考

* [C++ [](){}匿名函数](https://blog.csdn.net/zhizhuodewo6/article/details/126155356)
* [huihut/interview: 📚 C/C++ 技术面试基础知识总结(github.com)](https://github.com/huihut/interview)






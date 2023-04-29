---
title: C++语言特性总结
toc: true
categories:
  - Technology
tags:
  - 待续
---

比雅尼·斯特劳斯特鲁普博士在贝尔实验室工作期间在20世纪80年代发明并实现了C++。起初，这种语言被称作“C with Classes”（“包含‘类’的C语言”），作为C语言的增强版出现。
随后，C++不断增加新特性。虚函数（virtual function）、运算符重载（operator overloading）、多继承（multiple inheritance）、标准模板库（standard template library, STL）、异常处理（exception）、运行时类型信息（Runtime type information）、名字空间（namespace）等概念逐渐纳入标准。这些新技术的出现以及和原有技术的融合，使C++已经成为当今主流程序设计语言中最复杂的一员。

作为C语言的增强版，C++包含了C语言的所有基本概念，所以这里不再复数C语言的特性，而是专注于和C的不同和扩展部分。想了解C的特性请看我之前的一篇总结： [C语言特性总结](/2018/08/06/C语言特性总结)，类的部分主要和python做对比。

<!--more-->

<br/>

# 基础

## 名字空间

一个标识符可在多个名字空间中定义，它在不同名字空间中的含义是互不相干的。此方法通过将逻辑上相关的标识符组织成相应的名字空间，可使整个系统更加模块化。名字空间通过作用域操作符（::）显示的指定命名空间。

## 流运算符（<<、 >>）

用于将左侧（右侧）运算对象的值写到右侧（左侧）运算对象表示的流，其本质是运算符重载。C++的输入输出系统较之C更加智能，它能够自己识别类型，节省了使用%d等字符的麻烦。另外，它是可扩展的，可以通过重定义运算符，使之识别所开发的新数据类型。

## 引用类型

引用并非typedef，也不是#define，引用是对象的别名，且在初始化之后引用不可更改。

'&'在做左值的时候是引用类型声明，在做右值的时候是取地址符。

## auto类型
  和C一样，C++是静态类型语言，需要先声明变量类型，才能使用。但C++提供了一种auto类型，允许编译器根据表达式的值来推算变量的类型。这让语言具有了一些动态类型语言的特性。

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



# const

## 用于普通变量前

```c++
const int age = 10;
#define age 10;
```

const与define，两者都可以用来定义常量。

但是const定义时，定义了常量的类型，所以更精确一些。

#define只是简单的文本替换，除了可以定义常量外，还可以用来定义一些简单的函数，有点类似内联函数(Inline)。

const和define定义的常量可以放在头文件里面。（小注：可以多次声明，但只能定义一次）

## 用于指针变量前

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



## 用于函数返回

```c++
const int function();   //此时const无意义
const int& function();
const user_class function();
const user_class& function();
void function() const;  // 表示声明本函数为常成员函数, 常成员函数是不能改变成员变量值的函数
```

# \[](){}匿名函数

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

---
title: QT库被C#调用
toc: true
categories:
  - null
tags:
  - null
---

一库两用之写QT库给C#用。

<!--more-->

<br/>

## 函数调用

### 在QT中

新建一个C++ Library：

![QT Create A C++ Library](https://thecodeprogram.com/img/contents/2020/07/28/qt-create-a-c---library.webp)

选择Shared Library：

![Select the Shared Library in Qt](https://thecodeprogram.com/img/contents/2020/07/28/select-the-shared-library-in-qt.webp)

编写代码：

```c++
// QTLIBRARYEXAMPLE_GLOBAL.H
#ifndef QTLIBRARYEXAMPLE_GLOBAL_H
#define QTLIBRARYEXAMPLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTLIBRARYEXAMPLE_LIBRARY)
#  define QTLIBRARYEXAMPLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTLIBRARYEXAMPLESHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" __declspec(dllexport) double add(int a, int b);
extern "C" __declspec(dllexport) double subtrack(int a, int b);
extern "C" __declspec(dllexport) double multiply(int a, int b);
extern "C" __declspec(dllexport) double divide(int a, int b);

#endif // QTLIBRARYEXAMPLE_GLOBAL_H
```



```c++
// QTLIBRARYEXAMPLE.H
#include "qtlibraryexample.h"
#include <stdio.h>

QtLibraryExample::QtLibraryExample()
{
}

extern "C" double add(int a, int b)
{
    return a + b;
}

extern "C" double subtrack(int a, int b)
{
    return a - b;
}

extern "C" double multiply(int a, int b)
{
    return a * b;
}

extern "C" QTLIBRARYEXAMPLESHARED_EXPORT double divide(int a, int b)
{
    return a / b;
}
```

选择32bit编译器（和C#中保持一致即可），编译但不要运行。

![Qt Create library as 32bit library](https://thecodeprogram.com/img/contents/2020/07/28/qt-create-library-as-32bit-library.webp)

会在build目录下产生一个dll文件。

### 在C#中

创建一个工程，在Program.cs中先引用并声明函数：

```c#
[DllImport("QtLibraryExample.dll", CallingConvention = CallingConvention.Cdecl)]
static extern double add(int a, int b);

[DllImport("QtLibraryExample.dll", CallingConvention = CallingConvention.Cdecl)]
static extern double subtrack(int a, int b);

[DllImport("QtLibraryExample.dll", CallingConvention = CallingConvention.Cdecl)]
static extern double multiply(int a, int b);

[DllImport("QtLibraryExample.dll", CallingConvention = CallingConvention.Cdecl)]
static extern  double divide(int a, int b);
```

在主函数中使用：

```c#
static void Main(string[] args)
{
    Console.WriteLine("Result of 5 + 7 is " + add(5, 7).ToString());
    Console.WriteLine("Result of 21 - 10 is " + subtrack(21, 10).ToString());
    Console.WriteLine("Result of 9 x 3 is " + multiply(9, 3).ToString());
    Console.WriteLine("Result of 70 / 5 is " + divide(70, 5).ToString());
}
```

将此工程的解决方案配置为 Release + x86，然后把QT中生成的dll放置到此项目的build目录下，点击运行。

![result1](C:\Users\Easy\Desktop\result1.png)

[完整示例代码](https://github.com/thecodeprogram/QtLibraryForCSharpApplication)

## 类调用



## 参考

* ## [How to use QT C++ library in C#](https://thecodeprogram.com/how-to-use-qt-c---library-in-c-#)
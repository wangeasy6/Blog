---
title: SQLite数据库操作
toc: true
categories:
  - null
tags:
  - null
---

SQLite是一个C语言库，它实现了一个小型、快速、独立、高可靠性、功能齐全的SQL数据库引擎。 SQLite 是世界上使用最广泛的数据库引擎。 SQLite 内置于所有手机和大多数计算机中，并捆绑在人们每天使用的无数其他应用程序中。

试验版本：3.45.2

<!--more-->

<br/>

## 数据库操作



## 数据类型



## 表格操作

### 新建

### 删除

### 修改

### 查询

## QT 中使用

使用 QSqlDatabase 类来操作 SQLite。

在工程中添加 sql 库：

| 编译器 | 添加方法                                                     |
| ------ | ------------------------------------------------------------ |
| CMake: | find_package(Qt6 REQUIRED COMPONENTS Sql) target_link_libraries(mytarget PRIVATE Qt6::Sql) |
| qmake: | QT += sql                                                    |

代码：

````c++
#include <QSqlDatabase>

````





## 参考

* [SQLite | Home Page](https://www.sqlite.org/index.html)

* [SQLite Documentation](https://www.sqlite.org/docs.html)

* [Downloads - DB Browser for SQLite](https://sqlitebrowser.org/dl/)


---
title: QT库转Python库
toc: true
categories:
  - null
tags:
  - null
---

QT具有全平台发布的能力，但是如果我想用Python来调用QT的库呢。

<!--more-->

<br/>

## 生成QT库

1. 选择新建类型：
   

## 转Python库

python操作C/C++动态库是通过ctypes这个内建的包，官方文档[ctypes](http://docs.python.org/library/ctypes.html)。

当Python使用QT库时，Lib的编译器位数需要和Python安装的位数一致。

### Python查看版本位数

1. 在cmd中输入python，回车查看版本位数信息：

   ```shell
   Python 3.8.5 (tags/v3.8.5:580fbb0, Jul 20 2020, 15:57:54) [MSC v.1924 64 bit (AMD64)] on win32
   ```

2. 如果上述信息中没有位数信息，在python环境中输入：

   ```shell
   >>> import platform
   >>> platform.architecture()
   ```
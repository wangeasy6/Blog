---
title: Python学习笔记
date: 2018-08-06 16:14:01
toc: true
categories:
  - tech
---

* 查看包情况： dir() 和 help() 函数非常有用:
```
>>> import os
>>> dir(os)
<returns a list of all module functions>
>>> help(os)
<returns an extensive manual page created from the module's docstrings>
```

* SyntaxError: Non-ASCII character '\xe6'
在开头加入：`# -*- coding: UTF-8 -*-`

<!--more-->

### 更换pip源到国内镜像
pip国内的一些镜像地址：

* 阿里云 http://mirrors.aliyun.com/pypi/simple/
* 中国科技大学 https://pypi.mirrors.ustc.edu.cn/simple/
* 清华大学 https://pypi.tuna.tsinghua.edu.cn/simple/
* 中国科学技术大学 http://pypi.mirrors.ustc.edu.cn/simple/

**临时使用：**

  使用 -i 指定源：
  例： sudo pip3 install cython -i  https://pypi.mirrors.ustc.edu.cn/simple/

**永久修改：**

  Linux：修改 ~/.pip/pip.conf (没有就创建一个)， 内容如下：
  ```
  [global]
  index-url = https://pypi.tuna.tsinghua.edu.cn/simple
  ```

  Windows：直接在user目录中创建一个pip目录，如：C:\Users\xx\pip，新建文件pip.ini，内容如下
  ```
  [global]
  index-url = https://pypi.tuna.tsinghua.edu.cn/simple
  ```

<br/>

### 源码加密

Python 是以文本形式存储和运行的，在发布的时候常常需要将其加密。

我们使用 `pyInstaller`方案来加密和生成可执行文件。[pyinstaller](http://www.pyinstaller.org)可以运行在2.7和3.5版本，可在Windows、Linux、Mac OS X(64bit)等系统平台上运行。

以`Linux` + `Python3.5`为例：

1 下载 [pyInstaller](https://github.com/pyinstaller/pyinstaller/releases/download/v3.4/PyInstaller-3.4.tar.gz)

2 解压：`tar -xzvf PyInstaller-3.4.tar.gz `

3 配置

1. `cd PyInstaller-3.4/bootloader`
2. `python ./waf distclean all`
3. `python setup.py install`

4 生成加密执行文件

`python3 pyinstaller.py --key "key" src_path/src_name.py`

5 生成路径

`src_name/dist/src_name`

6 patch 版本

`rm -rf *.so* Crypto`
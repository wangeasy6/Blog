---
title: Python学习笔记
date: 2018-08-06 16:14:01
toc: true
categories:
  - Technology
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

pip 查询可用版本：`pip insatall xx==`

<br/>

### 文档注释

在Python中，出现在模块、函数、类、方法定义下的第一个语句如果是注释，会自动变成属性\_\_doc\_\_。

通过访问\_\_doc\_\_属性可以得到注释文档，通过help()方法可以得到类以及类方法的所有信息，而\_\_doc\_\_则只包含类注释。

**pydoc**，是Python自带的一个文档生成工具。可以在线查看或者生成文件的html。

启动在线服务（python环境变量下的模块内容）：

```shell
python -m pydoc [-p 1234]
```

直接输出类文档（test_py.py）：

```shell
python -m pydoc test_py
```

生成html（test_py.py）：

```shell
python -m pydoc -w test_py
```

查找模块（会在当前工程目录以及python环境变量目录下查找包含关键词的模块信息）：

```shell
python3 -m pydoc -k test_py
```

<br/>

#### 注释风格

对文档字符串的惯例是使用三重双引号”“”，第一句是以句号， 问号或惊叹号结尾的概述，接着是一个空行(或者该文档字符串单纯只有一行)， 接着是文档字符串剩下的部分。

以下是人们常用的几种注释文档风格：

**javadoc**

这是曾经比较流行的一直类似于javadoc的风格。

```python
"""
This is a javadoc style.

@param param1: this is a first param
@param param2: this is a second param
@return: this is a description of what is returned
@raise keyError: raises an exception
"""
```

**reST**

这是现在流行的一种风格，reST风格，Sphinx的御用格式。

```python
"""
This is a reST style.

:param param1: this is a first param
:param param2: this is a second param
:returns: this is a description of what is returned
:raises keyError: raises an exception
"""
```

**Google风格**

```python
"""
This is a groups style docs.

Parameters:
  param1 - this is the first param
  param2 - this is a second param

Returns:
  This is a description of what is returned

Raises:
  KeyError - raises an exception
"""
```

**Numpy风格**

```python
"""
My numpydoc description of a kind
of very exhautive numpydoc format docstring.

Parameters
----------
first : array_like
  the 1st param name `first`
second :
  the 2nd param
third : {'value', 'other'}, optional
  the 3rd param, by default 'value'

Returns
-------
string
  a value in a string

Raises
------
KeyError
  when a key error
OtherError
  when an other error
"""
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

<br/>

### 加载.ini配置文件

使用ConfigParser来加载.ini配置文件

如下配置信息：

```ini
; config.ini

# 版本信息
[Ver]
Version=1.1.2

# 日志级别配置
[Log]
PrintLevel=10
SaveLevel=20
SavePath=./
```

```python
from configparser import ConfigParser

if __name__ == '__main__':
    cfg = ConfigParser()
    cfg.read("config.ini", encoding='utf-8')
    cfg.sections()

    print(cfg.get('Ver', 'Version'))
    print_level = int(cfg.get('Log', 'PrintLevel'))
    save_level = int(cfg.get('Log', 'SaveLevel'))
```

<br/>

### Logging

使用logging包来管理Log。

```python
import logging
from logging import handlers

if __name__ == '__main__':
    # 打印Log设置
    pl = logging.StreamHandler()
    pl_formatter = logging.Formatter('[%(levelname)s] %(module)s:%(lineno)d - %(message)s')
    pl.setFormatter(pl_formatter)
    print_level = 10
    pl.setLevel(print_level)

    # 保存Log设置
    LOG_FILE = "program.log"
    fl = handlers.RotatingFileHandler(filename=LOG_FILE, maxBytes=5*1024*1024, backupCount=100)
    fl_formatter = logging.Formatter('%(asctime)s [%(levelname)s] %(module)s:%(lineno)d - %(message)s')
    fl.setFormatter(fl_formatter)
    save_level = 20
    fl.setLevel(save_level)

    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)
    logger.addHandler(pl)
    logger.addHandler(fl)
```

此设置，即设置了整个Python程序的Log输出格式，此设置会同时输出打印和保存到日志文件。

通过setLevel来设置日志级别：

* 10 - Debug
* 20 - Info
* 30 - Warning
* 40 - Error
* 50 - Critical

分别使用以下函数来使用日志输出：

* logging.debug()
* logging.info(）
* logging.warning()
* logging.error()
* logging.critical()

数字越大越严重，当使用低级别时会默认打印此级别及以上级别日志，如使用Info级别是，会默认输出Waring、Error、Critical。

使用 logging.Formatter来设置日志格式，logging模块中定义好的可以用于format格式字符串说明：

| 使用字段            | 说明                                                         |
| ------------------- | ------------------------------------------------------------ |
| %(asctime)s         | 将日志的时间构造成可读的形式，默认情况下是‘2016-02-08 12:00:00,123’精确到毫秒 |
| %(name)s            | 所使用的日志器名称，默认是'root'，因为默认使用的是 rootLogger |
| %(filename)s        | 调用日志输出函数的模块的文件名； pathname的文件名部分，包含文件后缀 |
| %(funcName)s        | 由哪个function发出的log， 调用日志输出函数的函数名           |
| %(levelname)s       | 日志的最终等级（被filter修改后的）                           |
| %(message)s         | 日志信息， 日志记录的文本内容                                |
| %(lineno)d          | 当前日志的行号， 调用日志输出函数的语句所在的代码行          |
| %(levelno)s         | 该日志记录的数字形式的日志级别（10, 20, 30, 40, 50）         |
| %(pathname)s        | 完整路径 ，调用日志输出函数的模块的完整路径名，可能没有      |
| %(process)s         | 当前进程， 进程ID。可能没有                                  |
| %(processName)s     | 进程名称，Python 3.1新增                                     |
| %(thread)s          | 当前线程， 线程ID/线程名称。可能没有                         |
| %(module)s          | 调用日志输出函数的模块名， filename的名称部分，不包含后缀即不包含文件后缀的文件名 |
| %(created)f         | 当前时间，用UNIX标准的表示时间的浮点数表示； 日志事件发生的时间--时间戳，就是当时调用time.time()函数返回的值 |
| %(relativeCreated)d | 输出日志信息时的，自Logger创建以 来的毫秒数； 日志事件发生的时间相对于logging模块加载时间的相对毫秒数 |
| %(msecs)d           | 日志事件发生事件的毫秒部分。logging.basicConfig()中用了参数datefmt，将会去掉asctime中产生的毫秒部分，可以用这个加上 |


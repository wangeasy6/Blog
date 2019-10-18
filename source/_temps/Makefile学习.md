---
title: Makefile学习
date: 2018-11-24 10:56:46
toc: true
categories:
  - tech
tags:
  - 待续
---

在学习 BootLoader 和 Linux 的时候，make 都是他们默认的自动化编译工具，所以makefile是对理解系统架构、了解编译流程绕不开的槛。
对make工具的掌握也间接决定了构建大型项目的能力。

<!--more-->

### 基本概念

一个工程中的源文件不计其数，其按类型、功能、模块分别放在若干个目录中，Make 工具能够使用一行命令来完成“自动化编译”。

Makefile 文件描述了整个工程的编译、连接等规则。其中包括：工程中的哪些源文件需要编译以及如何编译、需要创建哪些库文件以及如何创建这些库文件、如何最后产生我们想要的可执行文件。

Makefile 有自己的书写格式、关键字、函数。像C 语言有自己的格式、关键字和函数一样。而且在Makefile 中可以使用系统shell所提供的任何命令来完成想要的工作。

### 基本形式

每一项 Makefile 都是由 目标、依赖、命令 组成：

```
target: prerequisites
	command
```

依赖项可以展开为另一个拥有依赖的目标，以此递归地构建整个项目，生成目标文件。



### 系统变量



| 变量         | 代表                   |
| ------------ | ---------------------- |
| PATH         | 系统环境变量           |
| CURDIR       | 当前工作目录           |
| SHELL        | shell bin所在路径      |
| MAKELEVEL    | 当前makefile的调用层数 |
| MAKE         | make 命令              |
| MAKECMDGOALS | 终极目标的列表         |
| MAKEFLAGS    | 命令行的输入参数       |
| MAKE_VERSION | make 版本号（make -v） |
|              |                        |

系统级环境变量SHELL与MAKEFLAGS，总传递到下层makefile。

选项`-C, -f, -o,-W`不能放入变量MAKEFLAGS中，这些选项不能向下传递。



### 赋值

| 符号    | 含义                                                         |
| ------- | ------------------------------------------------------------ |
| **=**   | 用 "=" 定义的变量是递归展开的变量                            |
| **:=**  | 用 ": =" 或 ": =" 定义的变量只是展开的变量                   |
| **::=** | 在GNU make 中，同":="，2012年被添加到POSIX标准，之前的版本不支持 |
| **?=**  | 用 "?=" 定义的变量仅在尚未设置变量的情况下将其设置为值       |
| **!=**  | shell 赋值运算符 '! = ' 可用于执行 shell 脚本并为其输出设置变量。此运算符首先计算右侧, 然后将该结果传递给用于执行的 shell |
| **+=**  | 向已定义的变量的值添加文本                                   |



### 特殊符号

| 符号             | 含义                                    |
| ---------------- | --------------------------------------- |
| # | 注释符。行的第一个非空字符为#，则此行为注释行（Tab之后使用的#，会被解释为命令行） |
| \ | 反斜线。换行符 —— 在行的结尾表示续行；特殊字符 —— 解释特殊字符 |
|  | 通常，make会把其要执行的命令行在命令执行前输出到屏幕上。“@”字符在命令行前，那么，此命令将不被make显示出来 |
| $@               | 目标项，例： `cc -c $(CFLAGS) $< -o $@` |
| $<               | 第一个依赖项                            |
| $^               | 所有依赖项                              |
| $+               | 也是所有依赖项，但是这个不去除重复的项  |
| $%               | 仅当目标是函数库文件中，                |
| $*               | 这个变量表示目标模式中“%”及其之前的部分 |
| $? | 表示比目标还要新的依赖文件列表 |
| $(@D) / $(@F) | $@ 中的目录+文件名部分                  |
| $(\*D) / $(\*F) | $* 中的目录+文件名部分                   |
| $(%D) / $(%F) | $% 中的目录+文件名部分                   |
| $(<D) / $(<F) | $< 中的目录+文件名部分                   |
| $(^D) / $(^F) | $^ 中的目录+文件名部分                   |
| $(+D) / $(\+F) | $+ 中的目录+文件名部分                   |
| $(?D) / $(?F) | $? 中的目录+文件名部分                   |



### 常用函数

| 示例                          | 说明                                                         |
| ----------------------------- | ------------------------------------------------------------ |
| $(warning xx)                 | 打印警告信息（文件名:行号 xx）                               |
| $(error xx)                   | 打印错误信息，并终止程序                                     |
| $(wildcard *.cpp)             | 在Makefile规则中，通配符会被自动展开。但在变量的定义和函数引用时，通配符将失效。此时则使用函数 wildcard |
| $(patsubst %c, %o, $(SRC_C))  | 替换函数，把 $SRC_C中每一项的c换为 o                         |
| $(SRC_C:%.c=%.o)              | 效果同上                                                     |
| $(notdir $(src))              | 去除路径，去除变量中的文件路径部分                           |
| $(origin xx)                  | 获取变量来源（undefined、environment、default、file、command line、override、automatic） |
| $(filter PATTERN…,TEXT)       | 过滤函数—filter。过滤掉字串“TEXT”中所有不符合模式“PATTERN”的单词，保留所有符合此模式的单词。可以使用多个模式，模式表达式之间使用空格分割。 |
| $(filter-out PATTERN...,TEXT) | 反过滤函数—filter-out。和“filter”函数实现的功能相反。        |



### MAKEFLAGS += -rR --include-dir=$(CURDIR)

##### 内置隐含规则和变量设置

> 1. 隐含规则则是内建在make 中，为make 提供了重建某一类目标文件(.o 等)的通用方法，同时这些隐含规则所用到的变量也就是所谓的隐含变量。
> 2. 隐含规则的好处是在Makefile 中不需要明确给出重建某一个目标的命令，甚至可以不需要规则。make会为你自动搜寻匹配的隐含规则链。
> 3. 隐含规则的代价之一就是低效，系统必须搜索可能的隐含规则链。同时隐含规则也有可能应用了不是你想要的规则而引入很难debug的错误。
> 4. 变量SHELL与MAKEFLAGS一样，默认情况（没有用“unexport”声明）下在整个make的执行过程中被自动的传递给所有的子make。

make工具内置的隐含规则和变量设置可能会导致一些无法预料的结果，应该将他们禁用掉。

```makefile
MAKEFLAGS += -rR --include-dir=$(CURDIR)
```

> 环境变量 MAKEFILES
> -r, --no-builtin-rules ，表示禁用隐含规则
> -R, --no-builtin-variables ，表示禁用隐含变量定义
> ”–include-dir” 指明嵌套脚本的搜索路径
> CURDIR是make的内嵌变量，自动设置为当前目录

##### 引用其他的Makefile

make命令开始时，会把找寻include所指出的其它Makefile，并把其内容安置在当前的位置。就好像C/C++的#include指令一样。如果文件都没有指定绝对路径或是相对路径的话，make会在当前目录下首先寻找，如果当前目录下没有找到，那么，make还会在下面的几个目录下找：

> 1. 如果make执行时，有“-I”或“--include-dir”参数，那么make就会在这个参数所指定的目录下去寻找。
> 2. 如果目录/include（一般是：/usr/local/bin或/usr/include）存在的话，make也会去找。



### 语言区域设置

获取当前区域设置的数字格式习惯

```makefile
unexport LC_ALL
LC_COLLATE=C
LC_NUMERIC=C
export LC_COLLATE LC_NUMERIC
```

> export是make语法中的操作符即传递变量到下级Makefile。
> unexport 即不让变量传递到下级Makefile

在Linux中通过locale来设置程序运行的不同语言环境，locale由ANSI C提供支持。

locale的命名规则为<语言>_<地区>.<字符集编码>，如zh_CN.UTF-8，zh代表中文，CN代表大陆地区，UTF-8表示字符集。在locale环境中，有一组变量，代表国际化环境中的不同设置。

1. LA_ALL

   它是一个宏，如果该值设置了，则该值会覆盖所有LC_*的设置值。LANG的值不受该宏影响。

   C"是系统默认的locale，"POSIX"是"C"的别名。所以当我们新安装完一个系统时，默认的locale就是C或POSIX。

2. LANG

   LC_*的默认值，是最低级别的设置，如果LC_*没有设置，则使用该值。类似于 LC_ALL。

3. LC_COLLATE

   定义该环境的排序和比较规则,

4. LC_NUMERIC

   非货币的数字显示格式

5. LC_MONETARY

   货币格式

6. LC_CTYPE

   用于字符分类和字符串处理，控制所有字符的处理方式，包括字符编码，字符是单字节还是多字节，如何打印等。是最重要的一个环境变量。

7. LC_TIME

   时间和日期格式

8. LC_MESSAGES

   提示信息的语言。另外还有一个LANGUAGE参数，它与LC_MESSAGES相似，但如果该参数一旦设置，则LC_MESSAGES参数就会失效。LANGUAGE参数可同时设置多种语言信息，如LANGUANE="zh_CN.GB18030:zh_CN.GB2312:zh_CN"。



### 避免shell环境干扰

```makefile
unexport GREP_OPTIONS
```



### 递归make子文件夹

`$(MAKE) -C subdir`

其等价于：

`cd subdir && $(MAKE)`



### make读取命令行参数
使用方法：
例：使用 O= 来指定输出目录
`make O=/tmp/build`

代码实现：
```
ifdef O
ifeq ("$(origin O)", "command line")
BUILD_DIR := $(O)
endif
endif
```

代码分析：
真正起作用的是 BUILD_DIR := $(O)
${O} 就会把输入里 O= 后面的值取出

ifdef O，检测变量O是否为空
${origin O}，使用origin函数，得到变量的出处
ifeq ("$(origin O)", "command line")，检测是否是从命令行输入定义



### FORCE

强制生成。
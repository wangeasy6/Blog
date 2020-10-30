---
title: 命名规范-C++
date: 2019-11-27 17:28:08
toc: true
categories:
  - Technology
---
![img](/resources/Cover/naming_spec.gif)

有别于学校的学习，对于一个大型软件项目来说，命名规范是一个必须解决的问题，不规范的命名会严重降低代码的可阅读性和可维护性，快速地让整个项目变成一堆Shit。
个人认为，整洁在某种意义上是属于神性的，整洁的代码和优美的设计都带有不可侵犯的神性，在这样的环境下，人们对于自己的思想和行为会做出更高的要求。

<!--more-->

<br/>

## 命名系统设计原则

```
1. 同一类型命名保持一致；
2. 不同类型命名尽量使用不同的命名规则，通过名称形式就能快速分辨类型；
3. 命名需要简洁，见名知意，通俗易懂；
4. 对于存在过多的同类型名称，应添加表述以消除歧义；
5. 使用正确的美式英语。例如，使用美式英语的 license、color，而非英式英语的 licence、colour；
6. 除了常见的英文缩写，尽量避免使用缩写；
7. 禁止使用中文、中文拼英（除地名）。
```


<br/>

## 常见的命名规则

| 名称         | 规则                                                 | 示例                 | 常用场景               |
| ------------ | ---------------------------------------------------- | -------------------- | ---------------------- |
| 驼峰命名法   | 第一个单词首字母小写，余下的单词首字母大写，无连接符 | openButton           | 函数名                 |
| 帕斯卡命名法 | 单词首字母大写，无连接符                             | MainForm             | 类名                   |
| 下划线法命名 | 全小写，单词间使用`_`作为连接符                      | user_id              | 文件名、函数名、变量名 |
| 宏命名       | 全大写，单词间使用`_`作为连接符                      | USER_TYPE2           | 宏名、常量名           |
| 下划线标记   | 在名称前/后加上`_`作为标记                           | \_user_id、user_id\_ | 类的成员变量/私有变量  |
| 匈牙利命名法 | ：：                                                 |  |  |  |


<br/>

#### 匈牙利命名法

据说这种命名法是一位叫 Charles Simonyi 的匈牙利程序员发明的，他在微软工作的时候采用了这种命名方法，于是这种命名法就通过微软的各种产品和文档资料向世界传播开来。

匈牙利命名法的基本形式是：变量名=属性+类型+对象描述。

核心思想是通过在变量名前面加上相应前缀，标识出变量的作用域，类型等。

例：m_lpszStr, 表示指向一个以0字符结尾的字符串的长指针成员变量

属性缩写：

```
g_:全局变量
m_:类成员变量
s_:静态变量
c_:常量
```

类型缩写：

```
b:bool
by:byte
i:int
c/ch:char
s:string
l:long
u:无符号的
d:双精度
a:数组
sz:以零结束的字符串
h:句柄
p:指针
fn:函数
f:flag
```


<br/>

## 常用缩写词

| 全拼           | 缩写      | 译文       |
| -------------- | --------- | ---------- |
| addition       | add       | 加         |
| subtraction    | sub       | 减         |
| multiplication | mul       | 乘         |
| division       | div       | 除         |
| array          | arr       | 数组、集合 |
| average        | avg       | 平均       |
| buffer         | buf、buff | 缓冲、缓存 |
| capture        | cap       | 捕获       |
| count          | cnt       | 计数       |
| number         | num       | 数量、编号 |
| column         | col       | 列         |
| control        | ctrl      | 控制       |
| encode         | enc       | 编码       |
| decode         | dec       | 解码       |
| define         | def       | 定义       |
| delete         | del       | 删除       |
| display        | disp      | 显示       |
| environment    | env       | 环境       |
| error          | err       | 错误       |
| frequency      | freq      | 频率       |
| image          | img       | 图片、镜像 |
| initalize      | init      | 初始化     |
| length         | len       | 长度       |
| memory         | mem       | 内存       |
| message        | msg       | 消息       |
| receive        | recv      | 接收       |
| position       | pos       | 位置       |
| previous       | pre、prev | 之前的     |
| result         | res       | 结果       |
| return         | ret       | 返回       |
| source         | src       | 源         |
| include        | inc       | 包括       |
| table          | tab       | 表         |
| temporary      | tmp、temp | 临时       |
| value          | val       | 值         |

<br/>

## C++命名规则

以C++为例，我设计了一套命名规则。

#### 文件夹命名

* 首字母大写，单词间无字符，缩写单词首字母大写

* 通用和常见的小写文件夹保留小写。（如：`inc`、`include`、`src`、`lib`、`env`、`bin`、`obj`、`doc`）

* 文件文件夹名称应尽量短，有些开发工具对路径长度要求128B内甚至更短

  例：`UsbHub`、`FileTest`

#### 文件命名

* 全小写，使用`_`连接单词。

* C++ 文件要以 `.cc` 结尾, 头文件以 `.h` 结尾。定义类时文件名一般成对出现, 如 `foo_bar.h` 和 `foo_bar.cc`, 对应于类 `FooBar`。

* 通用和常见的文件名保留。（如：`README.md`、`.gitignore`、`CMakeLists.txt`）

  例：`serial_port.cc`

#### 名称空间命名

* 首字母大写，单词间无字符，缩写单词首字母大写

  例：`RfHub`、`FileTest`

#### 类命名

* 首字母大写，单词间无字符，缩写单词首字母大写

  例：`RfHub`、`FileTest`

#### 类变量命名

* 以`_`开头，全小写，使用`_`连接单词

  例：`_is_open`

#### 类函数命名

* 首字母大写，使用`_`连接单词，缩写单词使用全大写

  例：`USB_Set`

#### 函数内变量命名

* 全小写，使用`_`连接单词

  例：`is_open`

#### 打印字符

* 全英文，首单词首字母大写，单词间使用空格

  例：`“Compile your code with -fPIC.”`


<br/>

## 参考

* [命名约定 — Google 开源项目风格指南](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/)

* [命名惯例 | Google Cloud](https://cloud.google.com/apis/design/naming_convention?hl=zh-cn)
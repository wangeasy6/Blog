---
title: Python语言特性总结
date: 2020-08-20 16:40:00
toc: true
categories:
  - Technology
---
![Cover.jpg](/images/Cover/python.jpg)

从C或者其它高级语言转到Python，可能会经历一个适应时期，以熟悉Python特别的语法，所以总结一下Python 语言的特性及其基础概念。

例程基于Python 3.5.4。

<!--more-->

<br/>

### 基础
#### Python 解释器

Python是一种解释型语言（英语：Interpreted language），这种类型的编程语言，会将代码一句一句直接运行，不需要像编译语言（Compiled language）一样，经过编译器先行编译为机器码，之后再运行。

这种编程语言需要利用解释器，在运行期，动态将代码逐句解释（interpret）为机器码，或是已经预先编译为机器码的的子程序。它不会一次把整个程序翻译出来，而是每翻译一行程序叙述就立刻运行，然后再翻译下一行，再运行，如此不停地进行下去。

这一点和Java类似，同时如果你担心解释性语言影响程序执行效率，Python也提供了工具打包编译成相应平台的执行程序。

<br/>

#### 缩进

Python语言利用缩进表示语句块的开始和结束（Off-side规则），而非使用花括号或者某种关键字。增加缩进表示语句块的开始，而减少缩进则表示语句块的结束，同一级语句使用相同缩进。

if、else、函数、类等，不使用花括号了，换成了“:”加缩进来表示。

一般来说，空格和Tab符都能代表缩进，但如果我们混用，代码就会使人混淆。所以，一般使用4个空格作为缩进，这样能保证在任何平台上看到的显示效果都差不多。建议提前设置好自己的编辑器。

报错信息看到：`IndentationError: unexpected indent` ，就是表示缩进不一致。

<br/>

#### 行结尾

Python不需要“;”作为行结尾，而默认每一行为一个行语句。

但是如果你想要两个语句写在一行，也可以在中间加“;”来区分两个语句而达到目的。

另外一个和其它语言差不多的是，当一行写不下的时候使用“\”来续行。

<br/>

#### 变量

  Python 使用赋值声明变量类型，变量不用声明类型，在被赋值时，由解释器确定变量类型。

* Python3包含以下标准数据类型：
  不可变数据：bool（布尔）、Number（数字）、bytes（字节数组）、str（字符串）、tuple（元组）；
  可变数据：bytearray（新字节数组）、list（列表）、dict（字典）、set（集合）。

#####  bool

bool（布尔）代表真/假两种状态，分别是True、False。

在 Python2 中是没有布尔型的，它用数字 0 表示 False，用 1 表示 True。

到 Python3 中，把 True 和 False 定义成关键字了，但它们的值还是 1 和 0，它们可以和数字相加。

```python
b = True               # 布尔变量，False
print(b+1)             # 2

# 在很多判断语句中，会使用变量作为判断条件而引发bool的隐式转换
n = None               # bool(n) == False
a = 0                  # bool(a) == False
s = ""                 # bool(s) == False
t = ()                 # bool(t) == False
l = []                 # bool(l) == False
d = {}                 # bool(d) == False
```

##### Number

Number（数字）又包含 int、float、complex（复数）。

```python
i = 1 	                # type(i) <class 'int'>
i = 1.0                 # type(i) <class 'float'>
i = complex(2,3)        # i 2+3j ;   type(i) <class 'complex'>
```

##### bytes

bytes 是 Python 3.x 新增的类型，在 Python 2.x 中是不存在的。

bytes 以字节序列的形式（二进制形式）来存储数据，可以用于存储字符串、数字、图片、文件等，也常常被用于网络传输。

字符串转bytes：

- 如果字符串的内容都是 ASCII 字符，那么直接在字符串前面添加`b`前缀就可以转换成 bytes。
- bytes 是一个类，调用它的构造方法 bytes()，可以将字符串按照指定的字符集转换成 bytes，默认采用 UTF-8。
- 字符串本身有一个 encode() 方法，该方法专门用来将字符串按照指定的字符集转换成对应的字节串，默认采用 UTF-8。

```python
b = b''                              # 创建一个空的bytes
b = byte()                           # 创建一个空的bytes
b = b'hello'                         # 直接指定ASCII字符串是bytes类型
b = bytes('hello',encoding='utf-8')  #利用内置bytes方法，将字符串转换为指定编码的bytes
b = 'hello'.encode('utf-8')          # 利用字符串的encode方法编码成bytes，默认为utf-8类型
s = b.decode('utf-8')                # 将bytes对象解码成string，默认使用utf-8进行解码
```

##### string

string（字符串）使用`' '、" "、""" """`引用起来，表示字符串。

初始化
```python
s = 'name'             # 字符串
s = "name"             # 字符串
s = """na
    me"""              # 字符串,""" """引起来的字符串可以跨行
s = r"\n"              # (r/R)原始字符串，不转义字符
```

字符串截取
```python
a = "Hello"
b = "World"
print(a + b)           # +    HelloWorld
print(a*2)             # *    HelloHello
print(a[1])            # []   e
print(a[-1])           # []   o
print(a[1:4])          # [:]  ell （左闭右开）
print(a[:4])           # [:]  Hell （缺省左值）
```

格式化字符串（format）
```python
print("a=%s,b=%s" % (a,b))           # a=Hello,b=World （%    格式字符串）
print("a={},b={}".format(a,b))       # a=Hello,b=World  按位置格式化
print("a={1},b={0}".format(b,a))     # a=Hello,b=World  指定位置
print("a={a},b={b}".format(b=b,a=a)) # a=Hello,b=World  指定关键字
```

使用%格式化时的格式化符号：

| 符号 | 描述                                 |
| :-: | ------------------------------------ |
| %c   | 格式化字符及其ASCII码                |
| %s   | 格式化字符串                         |
| %d   | 格式化整数                           |
| %u   | 格式化无符号整型                     |
| %o   | 格式化无符号八进制数                 |
| %x   | 格式化无符号十六进制数               |
| %X   | 格式化无符号十六进制数（大写）       |
| %f   | 格式化浮点数字，可指定小数点后的精度 |
| %e   | 用科学计数法格式化浮点数             |
| %E   | 作用同%e，用科学计数法格式化浮点数   |
| %g   | %f和%e的简写                         |
| %G   | %f 和 %E 的简写                      |
| %p   | 用十六进制数格式化变量的地址         |

格式化操作符辅助指令:

| 符号  | 功能                                                         |
| :-: | ------------------------------------------------------------ |
| *     | 定义宽度或者小数点精度                                       |
| -     | 用做左对齐                                                   |
| +     | 在正数前面显示加号( + )                                      |
| <sp>  | 在正数前面显示空格                                           |
| #     | 在八进制数前面显示零('0')，在十六进制前面显示'0x'或者'0X'(取决于用的是'x'还是'X') |
| 0     | 显示的数字前面填充'0'而不是默认的空格                        |
| %     | '%%'输出一个单一的'%'                                        |
| (var) | 映射变量(字典参数)                                           |
| m.n.  | m 是显示的最小总宽度,n 是小数点后的位数(如果可用的话)        |

字符串操作相关函数：

| 函数                                                         | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [capitalize()](https://www.runoob.com/python3/python3-string-capitalize.html) | 将字符串的第一个字符转换为大写                               |
| [center(width, fillchar)](https://www.runoob.com/python3/python3-string-center.html) | 返回一个指定的宽度 width 居中的字符串，fillchar 为填充的字符，默认为空格。 |
| [count(str, beg= 0,end=len(string))](https://www.runoob.com/python3/python3-string-count.html) | 返回 str 在 string 里面出现的次数，如果 beg 或者 end 指定则返回指定范围内 str 出现的次数 |
| [encode(encoding='UTF-8',errors='strict')](https://www.runoob.com/python3/python3-string-encode.html) | 以 encoding 指定的编码格式编码字符串，如果出错默认报一个ValueError 的异常，除非 errors 指定的是'ignore'或者'replace' |
| [endswith(suffix, beg=0, end=len(string))](https://www.runoob.com/python3/python3-string-endswith.html) | 检查字符串是否以 obj 结束，如果beg 或者 end 指定则检查指定的范围内是否以 obj 结束，如果是，返回 True,否则返回 False. |
| [expandtabs(tabsize=8)](https://www.runoob.com/python3/python3-string-expandtabs.html) | 把字符串 string 中的 tab 符号转为空格，tab 符号默认的空格数是 8 。 |
| [find(str, beg=0, end=len(string))](https://www.runoob.com/python3/python3-string-find.html) | 检测 str 是否包含在字符串中，如果指定范围 beg 和 end ，则检查是否包含在指定范围内，如果包含返回开始的索引值，否则返回-1 |
| [index(str, beg=0, end=len(string))](https://www.runoob.com/python3/python3-string-index.html) | 跟find()方法一样，只不过如果str不在字符串中会报一个异常。    |
| [isalnum()](https://www.runoob.com/python3/python3-string-isalnum.html) | 如果字符串至少有一个字符并且所有字符都是字母或数字则返 回 True，否则返回 False |
| [isalpha()](https://www.runoob.com/python3/python3-string-isalpha.html) | 如果字符串至少有一个字符并且所有字符都是字母或中文字则返回 True, 否则返回 False |
| [isdigit()](https://www.runoob.com/python3/python3-string-isdigit.html) | 如果字符串只包含数字则返回 True 否则返回 False               |
| islower()                                                    | 如果字符串中包含至少一个区分大小写的字符，并且所有这些(区分大小写的)字符都是小写，则返回 True，否则返回 False |
| [isnumeric()](https://www.runoob.com/python3/python3-string-isnumeric.html) | 如果字符串中只包含数字字符，则返回 True，否则返回 False      |
| [isspace()](https://www.runoob.com/python3/python3-string-isspace.html) | 如果字符串中只包含空白，则返回 True，否则返回 False.         |
| [istitle()](https://www.runoob.com/python3/python3-string-istitle.html) | 如果字符串是标题化的(见 title())则返回 True，否则返回 False  |
| [isupper()](https://www.runoob.com/python3/python3-string-isupper.html) | 如果字符串中包含至少一个区分大小写的字符，并且所有这些(区分大小写的)字符都是大写，则返回 True，否则返回 False |
| [join(seq)](https://www.runoob.com/python3/python3-string-join.html) | 以指定字符串作为分隔符，将 seq 中所有的元素(的字符串表示)合并为一个新的字符串 |
| [len(string)](https://www.runoob.com/python3/python3-string-len.html) | 返回字符串长度                                               |
| [ljust(width, fillchar)](https://www.runoob.com/python3/python3-string-ljust.html) | 返回一个原字符串左对齐,并使用 fillchar 填充至长度 width 的新字符串，fillchar 默认为空格。 |
| [lower()](https://www.runoob.com/python3/python3-string-lower.html) | 转换字符串中所有大写字符为小写.                              |
| [ lstrip()](https://www.runoob.com/python3/python3-string-lstrip.html) | 截掉字符串左边的空格或指定字符。                             |
| [maketrans()](https://www.runoob.com/python3/python3-string-maketrans.html) | 创建字符映射的转换表，对于接受两个参数的最简单的调用方式，第一个参数是字符串，表示需要转换的字符，第二个参数也是字符串表示转换的目标。 |
| [max(str)](https://www.runoob.com/python3/python3-string-max.html) | 返回字符串 str 中最大的字母。                                |
| [min(str)](https://www.runoob.com/python3/python3-string-min.html) | 返回字符串 str 中最小的字母。                                |
| [replace(old, new , max)](https://www.runoob.com/python3/python3-string-replace.html) | 把 将字符串中的 str1 替换成 str2,如果 max 指定，则替换不超过 max 次。 |
| rfind(str, beg=0,end=len(string))                            | 类似于 find()函数，不过是从右边开始查找.                     |
| [rindex( str, beg=0, end=len(string))](https://www.runoob.com/python3/python3-string-rindex.html) | 类似于 index()，不过是从右边开始.                            |
| [rjust(width, fillchar)](https://www.runoob.com/python3/python3-string-rjust.html) | 返回一个原字符串右对齐,并使用fillchar(默认空格）填充至长度 width 的新字符串 |
| [rstrip()](https://www.runoob.com/python3/python3-string-rstrip.html) | 删除字符串字符串末尾的空格.                                  |
| [split(str="", num=string.count(str))](https://www.runoob.com/python3/python3-string-split.html) | 以 str 为分隔符截取字符串，如果 num 有指定值，则仅截取 num+1 个子字符串 |
| [splitlines(keepends)](https://www.runoob.com/python3/python3-string-splitlines.html) | 按照行('\r', '\r\n', \n')分隔，返回一个包含各行作为元素的列表，如果参数 keepends 为 False，不包含换行符，如果为 True，则保留换行符。 |
| [ startswith(substr, beg=0,end=len(string))](https://www.runoob.com/python3/python3-string-startswith.html) | 检查字符串是否是以指定子字符串 substr 开头，是则返回 True，否则返回 False。如果beg 和 end 指定值，则在指定范围内检查。 |
| [strip(chars)](https://www.runoob.com/python3/python3-string-strip.html) | 在字符串上执行 lstrip()和 rstrip()                           |
| [swapcase()](https://www.runoob.com/python3/python3-string-swapcase.html) | 将字符串中大写转换为小写，小写转换为大写                     |
| [title()](https://www.runoob.com/python3/python3-string-title.html) | 返回"标题化"的字符串,就是说所有单词都是以大写开始，其余字母均为小写(见 istitle()) |
| [translate(table, deletechars="")](https://www.runoob.com/python3/python3-string-translate.html) | 根据 str 给出的表(包含 256 个字符)转换 string 的字符, 要过滤掉的字符放到 deletechars 参数中 |
| [upper()](https://www.runoob.com/python3/python3-string-upper.html) | 转换字符串中的小写字母为大写                                 |
| [zfill (width)](https://www.runoob.com/python3/python3-string-zfill.html) | 返回长度为 width 的字符串，原字符串右对齐，前面填充0         |
| [isdecimal()](https://www.runoob.com/python3/python3-string-isdecimal.html) | 检查字符串是否只包含十进制字符，如果是返回 true，否则返回 false。 |

##### bytearray

和bytes和string不同，bytearray是可变序列。

```python
bytearray()                               # 空字节数组
bytearray(int)                            # 初始化一个指定长度的空数组
bytearray(iterable of ints)               # 从[0,255)的int序列中初始化
bytearray(bytes or buffer)                # 从bytes 或者 buffer 初始化
bytearray(string, encoding[, errors])     # 按照指定的 encoding 将字符串转换为字节序列
```

除了可以更改元素，其它用法和基本bytes相同。

```python
bytearray()                         # bytearray(b'')
bytearray(5)                        # bytearray(b'\x00\x00\x00\x00\x00')
bytearray((97,98))                  # bytearray(b'ab')
bytearray([97,98])                  # bytearray(b'ab')
bytearray(b'ab')                    # bytearray(b'ab')
ba = bytearray("hello", 'utf-8')    # bytearray(b'hello')
ba[0] = ord('H')                    # bytearray(b'Hello')
```

##### tuple

元组与列表类似，不同之处在于元组的元素不能修改。

```python
tup1 = ('G', 'R', 1997, 2000)    # <class 'tuple'>
tup2 = (1, 2, 3, 4, 5 )          # <class 'tuple'>
tup3 = "a", "b", "c", "d"        # <class 'tuple'>
tup4 = (50)                      # int
tup4 = (50,)                     # <class 'tuple'>
tup5 = tup1 + tup2               # (1, 2, 3, 4, 5, "a", "b", "c", "d")
tup6 = (1,) * 5                  # (1, 1, 1, 1, 1)
tup1[0] = 'A'                    # TypeError: 'tuple' object does not support item assignment
del(tup1)
print(tup1)                      # NameError: name 'tup1' is not defined
len(tup2)                        # 5
max(tup2)                        # 5
min(tup2)                        # 1
tup2.count(1)                    # 1 （计算元素在序列中出现的次数）
```

##### list

tuple一旦初始化，变无法新增或者删除元素，只能相加后赋给一个新的元祖。

list的初始化和使用和tuple基本相同，但因为它是可变的，所以多了以下这些用法：

```python
list = ['A', 'B']
del list[1]                               # ['A']
list.extend(['B', 'B'])                   # ['A', 'B', 'B']
list.remove('B')                          # ['A', 'B']
list.append(['B', 'B'])                   # ['A', 'B', ['B', 'B']]
list.insert(0, 'I')                       # ['I', 'A', 'B', ['B', 'B']]
list.pop()                                # ['I', 'A', 'B']
list.pop(0)                               # ['A', 'B']
list.reverse()                            # ['B', 'A']
list.sort( key=None, reverse=False)       # ['A', 'B']
list.clear()                              # []
```

##### dict

字典的每个键值 **key=>value** 对用冒号 **:** 分割，每个对之间用逗号(**,**)分割，整个字典包括在花括号 **{}** 中 ,格式如下所示：

```python
d = {key1 : value1, key2 : value2 }
```

key 必须是唯一的，必须是不可变的类型。

value 可以不唯一，可以取任何数据类型。

```python
dict = { 'a': 123, 98.6: 37 }
dict['a']                             # 123
dict[98.6]                            # 37
dict['b']                             # KeyError: 'a'
dict['a'] = 111                       # {98.6: 37, 'a': 111}
dict['b'] = 1                         # {98.6: 37, 'b': 1, 'a': 111}
'a' in dict                           # True
dict.keys()                           # dict_keys([98.6, 'b', 'a'])
dict.values()                         # dict_values([37, 1, 111])
dict.items()                          # dict_items([(98.6, 37), ('b', 1), ('a', 111)])
del dict['b']                         # {98.6: 37, 'a': 111}
dict.update({'a':222, 'b':2})         # {98.6: 37, 'b': 2, 'a': 222}
dict.pop('b')                         # {98.6: 37, 'a': 222}
dict.clear()                          # 清空字典
del dict                              # 删除字典
```

##### set

集合（set）是一个无序的不重复元素序列。

可以使用大括号 **{ }** 或者 **set()** 函数创建集合，创建一个空集合必须用 **set()** 而不是 **{ }**，因为 **{ }** 是用来创建一个空字典。

```python
a = set('abr')         # {'r', 'b', 'a'}
b = set('ala')         # {'l', 'a'}
a - b                  # {'r', 'b'}
a | b                  # {'r', 'l', 'b', 'a'}
a & b                  # {'a'}
a ^ b                  # {'r', 'b', 'l'}
```

集合内置方法

| 方法                                                         | 描述                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [add()](https://www.runoob.com/python3/ref-set-add.html)     | 为集合添加元素                                               |
| [clear()](https://www.runoob.com/python3/ref-set-clear.html) | 移除集合中的所有元素                                         |
| [copy()](https://www.runoob.com/python3/ref-set-copy.html)   | 拷贝一个集合                                                 |
| [difference()](https://www.runoob.com/python3/ref-set-difference.html) | 返回多个集合的差集                                           |
| [difference_update()](https://www.runoob.com/python3/ref-set-difference_update.html) | 移除集合中的元素，该元素在指定的集合也存在。                 |
| [discard()](https://www.runoob.com/python3/ref-set-discard.html) | 删除集合中指定的元素                                         |
| [intersection()](https://www.runoob.com/python3/ref-set-intersection.html) | 返回集合的交集                                               |
| [intersection_update()](https://www.runoob.com/python3/ref-set-intersection_update.html) | 返回集合的交集。                                             |
| [isdisjoint()](https://www.runoob.com/python3/ref-set-isdisjoint.html) | 判断两个集合是否包含相同的元素，如果没有返回 True，否则返回 False。 |
| [issubset()](https://www.runoob.com/python3/ref-set-issubset.html) | 判断指定集合是否为该方法参数集合的子集。                     |
| [issuperset()](https://www.runoob.com/python3/ref-set-issuperset.html) | 判断该方法的参数集合是否为指定集合的子集                     |
| [pop()](https://www.runoob.com/python3/ref-set-pop.html)     | 随机移除元素                                                 |
| [remove()](https://www.runoob.com/python3/ref-set-remove.html) | 移除指定元素                                                 |
| [symmetric_difference()](https://www.runoob.com/python3/ref-set-symmetric_difference.html) | 返回两个集合中不重复的元素集合。                             |
| [symmetric_difference_update()](https://www.runoob.com/python3/ref-set-symmetric_difference_update.html) | 移除当前集合中在另外一个指定集合相同的元素，并将另外一个指定集合中不同的元素插入到当前集合中。 |
| [union()](https://www.runoob.com/python3/ref-set-union.html) | 返回两个集合的并集                                           |
| [update()](https://www.runoob.com/python3/ref-set-update.html) | 给集合添加元素                                               |

##### 推导式

 推导式可以从一个数据序列构建另一个新的数据序列的结构体。元组、列表、集合都可以使用推导式推导，且在Python2和3中都有支持。

```python
def getSquare(x):
    return x*x

X = [1,2,3,4,5]
squares = {i: getSquare(i) for i in X}        # {1: 1, 2: 4, 3: 9, 4: 16, 5: 25}
```

##### 类型检查

*isinstance(object, type)*，判断一个对象是否是一个已知的类型。type也可以是类/类型的元组。

```python
a = 2
isinstance (a,int)                  # True
isinstance (a,str)                  # False
isinstance (a,(str,int,list))       # True
```

*type(object)*，返回对象的类型。

```python
import time
a = 2
type(a) == int         # True
type('runoob')         # <type 'str'>
type([2])              # <type 'list'>
type({0:'zero'})       # <type 'dict'>
type(len)              # <class 'builtin_function_or_method'> （内置函数）
type(lambda x: x)      # <class 'function'>
type(time)             # <class 'module'>
type(int)              # <class 'type'>
```

isinstance 和 type 的区别在于：

- type 不会认为子类是一种父类类型。
- isinstance 会认为子类是一种父类类型。
- issubclass 用于判断object是否是类型的子类。

```python
class A:
    pass

class B(A):
    pass

isinstance(A(), A)    # True
type(A()) == A        # True

isinstance(B(), A)    # True
type(B()) == A        # False

issubclass(B(), A)    # True （type同样可以是元组）
```

##### 变量作用域

作用于表示变量在哪些位置可以访问。
Python 中有 4 种基础作用域：
  L （Local） 局部作用域
  E （Enclosing） 闭包函数外的函数中
  G （Global） 全局作用域
  B （Built-in） 内建作用域
  以 L –> E –> G –>B 的规则查找
Python 中只有模块（module），类（class）以及函数（def、lambda）才会引入新的作用域。

<br/>

#### 三元表达式

和C、Java中的三元表达式（? :）不同，Python的条件写在中间，成立执行前面，不成立执行else后面的语句。

这样的语法，不知道Python是怎么解析的，但是感觉没有C的方便，挺怪异的，一开始都以为Python没有三元表达式。

```python
x = -1 if y < 0 else 1
t = "True" if x == 1 else "False"

# 只写单个条件是不行的
print("True") if x == 1                             # SyntaxError: invalid syntax
if x == 1 else print("False")                       # SyntaxError: invalid syntax
```

#### 函数

函数代码块以 **def** 关键词开头，后接函数标识符名称和圆括号 **(传入参数)**，函数内容以冒号起始，并且缩进。

* 函数的第一行语句可以选择性地使用文档字符串——用于存放函数说明。

* 由于Python变量的赋值声明特性，而无需声明变量类型，所以Python函数在声明时，无需声明传参、返回值的类型。

* 由于Python是由解释器逐条执行，所以Python在return时，可以不同语句返回不同类型。

```python
def 函数名（参数列表）:
    函数体
```

调用函数时可使用的参数类型：

- 必需参数
- 关键字参数
- 默认参数
- 不定长参数
- 强制位置参数

```python
def hello(name, age = 18, *args_tuple, **args_dict):
    print("Hello %s!" % name)
    print(age)
    if len(args_tuple) > 0:
        print(args_tuple)
    if len(args_dict) > 0:
        print(args_dict)

hello()                              # TypeError: hello() missing 1 required positional argument: 'name'
hello(age=20, name='Micheal')        # 关键字匹配，可以不按顺序传参
hello('Micheal')                     # age 是默认参数，不传参默认为18
hello('Micheal', 20, "T1", "T2")     # 加了星号 * 的参数会以元组(tuple)的形式导入，存放所有未命名的变量参数
hello('Micheal', a="T1", b="T2")     # 加了两个星号 ** 的参数会以字典的形式导入，存放所有用关键字传入的变量参数
hello(name='Micheal', 20)            # SyntaxError: positional argument follows keyword argument(位置参数不能在关键字参数后面)

# Python3.8 新增了一个函数形参语法 / 用来指明函数形参必须使用指定位置参数，不能使用关键字参数的形式
def f(a, b, /, c, d, *, e, f):		 # 形参 a、 b 必须使用位置参数，c、 d 可以是位置形参或关键字形参，而 e、 f 必须为关键字形参
    print(a, b, c, d, e, f)
```

**匿名函数**

匿名，即不再使用 def 语句这样标准的形式定义一个函数，从而简化表达。

Python 使用 lambda 来创建匿名函数。lambda的主体是一个表达式，而不是一个代码块。

lambda 函数拥有自己的命名空间，且不能访问自己参数列表之外或全局命名空间里的参数。

```python
# 定义函数（lambda表达式）
sum = lambda x, y:x + y

# 执行函数
result = sum(1, 2)
```

<br/>

#### 迭代器

迭代器对象从集合的第一个元素开始访问，直到所有的元素被访问完结束。迭代器只能往前不会后退。

字符串，列表或元组对象都可用于创建迭代器。

迭代器有两个基本的方法：**iter()** 和 **next()**。

```python
list=[1,2,3,4]
it = iter(list)    # 创建迭代器对象
while True:
    try:
        print( next(it) )
    except StopIteration:
        break
```

可以在类中实现两个方法 `__iter__()` 与 `__next__()` ，把一个类作为一个迭代器。

```python
class MyNumbers:
    def __iter__(self):
        self.a = 1
        return self

    def __next__(self):
        if self.a <= 20:
            x = self.a
            self.a += 1
            return x
        else:
            raise StopIteration

myclass = MyNumbers()
myiter = iter(myclass)

for x in myiter:
    print(x)
```

<br/>

#### 生成器

在 Python 中，使用了 yield 的函数被称为生成器（generator）。

在调用生成器运行的过程中，每次遇到 yield 时函数会暂停并保存当前所有的运行信息，返回 yield 的值, 并在下一次执行 next() 方法时从当前位置继续运行。

```python
#!/usr/bin/python3

def fibonacci(n):              # 生成器函数 -- 斐波那契
    a, b, counter = 0, 1, 0
    while True:
        if (counter > n):
            return
        yield a
        a, b = b, a + b
        counter += 1

f = fibonacci(10)              # type(f) == <class 'generator'>

# 0 1 1 2 3 5 8 13 21 34 55
while True:
    try:
        print (next(f), end=" ")
    except StopIteration:
        break
```

<br/>

#### 引用

Python 使用 `import`和`from import`语句导入其它模块。

```python
import module
from module import fun1[, fun2...]
from module import *
```

Python 使用一个文件夹包含同一类模块，在目录中包含 "\__init__.py" 来声明为一个包。

引用包中的模块：

```python
import package.module
from package.module import fun1[, fun2...]
from package import *
```

<br/>

#### 异常机制

异常机制是语言提供的报告错误和处理错误的途径，它可以将这个问题传递给调用方解决。

使用异常可以把错误处理代码和正常的业务处理代码区别开，便于开发和维护。

异常捕捉使用 **try/except** 语句:

```python
try:
    x = int(input("请输入一个数字: "))
    break
except ValueError:
    print("您输入的不是数字，请再次尝试输入！")
```

一个 try 语句可能包含多个except子句，分别来处理不同的特定的异常。最多只有一个分支会被执行。

```python
import sys

try:
    f = open('myfile.txt')
    s = f.readline()
    i = int(s.strip())
except OSError as err:
    print("OS error: {0}".format(err))
except ValueError:
    print("Could not convert data to an integer.")
except:
    print("Unexpected error:", sys.exc_info()[0])
```

一个except子句可以同时处理多个异常，这些异常将被放在一个括号里成为一个元组，例如:

```python
except (RuntimeError, TypeError, NameError):
    pass
```

**try/except/else** 语句，else 子句将在 try 子句没有发生任何异常的时候执行。

**try/finally** 语句无论是否发生异常都将执行最后的代码。

```python
try:
    test()
except AssertionError as error:
    print(error)
else:
    print("ok")
finally:
    print('这句话，无论异常是否发生都会执行。')
```

**抛出异常**

```python
raise [Exception [, args [, traceback]]]
```

例：

```python
x = 10
if x > 5:
    raise Exception('x 不能大于 5。x 的值为: {}'.format(x))
```

执行上例输出：

```
Traceback (most recent call last):
  File "test.py", line 3, in <module>
    raise Exception('x 不能大于 5。x 的值为: {}'.format(x))
Exception: x 不能大于 5。x 的值为: 10
```

raise 唯一的一个参数指定了要被抛出的异常。它必须是一个异常的实例或者是异常的类（也就是 Exception 的子类）。

如果你只想知道这是否抛出了一个异常，并不想去处理它，那么一个简单的 raise 语句就可以再次把它抛出。

```python
try:
    raise NameError('HiThere')
except NameError:
    print('An exception flew by!')
    raise

An exception flew by!
Traceback (most recent call last):
  File "<stdin>", line 2, in ?
NameError: HiThere
```

<br/>

#### with语句

```
with context_expression [as target(s)]:
        with-body
```

with基于上下文管理器，context_expression返回一个上下文管理器，在进入body之前会调用上下文管理器的\_\_enter\_\_()函数，整个body执行完后，会自动调用上下文管理器的\_\_exit\_\_()，可以用于自动关闭文件、线程锁的自动获取和释放等。

如果指定了as语句，会将上下文管理器的 \_\_enter\_\_() 方法的返回值赋值给 target(s)，target(s) 可以是单个变量，或者由“()”括起来的元组。

```python
with open("myfile.txt") as f:
    for line in f:
        print(line)
```

<br/>

### 面向对象
* 类(Class):
  用来描述具有相同的属性和方法的对象的集合。它定义了该集合中每个对象所共有的属性和方法。对象是类的实例。

  ```python
  class ClassName:
      <statement-1>
      .
      .
      .
      <statement-N>
  ```

* 方法：类中定义的函数。

  类的方法与普通的函数只有一个特别的区别——它们必须有一个额外的**第一个参数名称**, 按照惯例它的名称是 self。

* 类变量
  类变量在整个实例化的对象中是公用的。类变量定义在类中且在函数体之外。类变量通常不作为实例变量使用。

* 实例变量
  定义在方法中的变量，只作用于当前实例的类。

* 继承
  即一个派生类（derived class）继承基类（base class）的字段和方法。
  Python允许多重继承、继承多个基类，派生类可以覆盖基类中的任何方法，方法中可以调用基类中的同名方法。

* 方法重写
  如果从父类继承的方法不能满足子类的需求，可以对其进行改写，这个过程叫方法的覆盖（override），也称为方法的重写。

* 实例化
  创建一个类的实例，类的具体对象。

* 对象
  通过类定义的数据结构实例。对象包括两个数据成员（类变量和实例变量）和方法。

* 构造方法/析构方法

  ```
  __init__ : 构造函数，在生成对象时调用
  __del__ : 析构函数，释放对象时使用
  ```

* 类私有属性/私有方法
  `__private_attrs`：两个下划线开头，声明该属性为私有，只能在类的内部调用 ，不能在类地外部调用。

* 运算符重载
  重写运算的专有方法。

  ```
  __repr__       打印，转换
  __setitem__    按照索引赋值
  __getitem__    按照索引获取值
  __len__        获得长度
  __cmp__        比较运算
  __call__       函数调用
  __add__        加运算
  __sub__        减运算
  __mul__        乘运算
  __truediv__    除运算
  __mod__        求余运算
  __pow__        乘方
  ```

<br/>

### 参考

* [菜鸟教程](https://www.runoob.com/python3/python3-tutorial.html)


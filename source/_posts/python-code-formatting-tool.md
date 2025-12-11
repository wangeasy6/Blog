---
title: Python编码格式化工具
date: 2020-10-20 17:24:00
toc: true
categories:
  - tech
tags:
  - Python
---

![pep8.png](/resources/Cover/pep8.png)

Python由语法简介和自由而著名，如果你不想库里的代码乱七八糟，统一编码规范尤其重要。

比起人来做大量的代码审查，使用软件进行代码检查和优化对于编码规范是一个更好的主意。

以下，主要探索了pylint、flake8、black、yapf、isort等软件的安装及使用。

<!--more-->

<br/>

### 编码检查

统一代码的第一步是统一编码，主要有两个方面需要注意，fileencoding和fileformat。

根据PEP8的规定，Python3使用UTF-8编码，Python2使用ASCII编码。fileformat根据所在系统确定。

另外，在Python标准库中，所有标识符、字符串文本、注释都必须使用ASCII。

#### 列出所有文件的编码信息

如果没有子目录：

`file *`

如果有子目录：

`find . -name '*' | xargs file | grep -v directory`

如果有`with CRLF line terminators`，说明是文件类型是`dos`。

列出所有`dos`编码文件：

`find . -name '*' | xargs file | grep CRLF`

#### 批量转码

**将所有`GBK(ISO-8859)`转为`UTF-8`:**

依赖：`enca`

`find . -name '*' | xargs file | grep ISO-8859 |  xargs -d: -n1 | grep .py | xargs enca -L zh_CN -x utf-8`

**将所有`dos`转为`unix`:**

依赖：`dos2unix`

`find . -name '*' | xargs file | grep CRLF |  xargs -d: -n1 | grep .py | xargs dos2unix`

<br/>

### Pylint

#### 简介

[Pylint](http://pylint.pycqa.org/en/latest/)是一个Python代码检查工具，它使用编码标准来检查代码问题，并提出重构建议。

在最后Pylint会输出代码全局评分分数，这可以用来测试修改代码后的效果。

#### 安装

```shell
sudo pip install pylint
```

`pylint3 --version`

```
pylint3 2.2.2
astroid 2.1.0
Python 3.7.3 (default, Jul 25 2020, 13:03:44)
[GCC 8.3.0]
```

#### 使用

```shell
pylint [options] <modules_or_packages>
```

<br/>

### flake8

#### 简介

[Flake8](https://pypi.org/project/flake8/) 是由Python官方发布的一款辅助检测Python代码是否规范的工具，相对于目前热度比较高的Pylint来说，Flake8检查规则灵活，支持集成额外插件，扩展性强。

Flake8是对下面三个工具的封装：

1）[PyFlakes](https://flake8.pycqa.org/en/latest/#)：静态检查Python代码逻辑错误的工具。

2）[pycodestyle](https://pypi.org/project/pycodestyle/)： 静态检查PEP8编码风格的工具。

3）[NedBatchelder’s McCabe script](https://pypi.org/project/mccabe/)：静态分析Python代码复杂度的工具。

#### 安装

```shell
sudo python -m pip install flake8
```

Flake8最好安装在对应的Python版本。如果要想Flake8能够正确地解析 Python 3.5中的新语言特性(例如) ，那么需要将它安装在3.5上，以便Flake8能够理解这些特性。

#### 使用

```shell
flake8 <modules_or_packages>
```

如果在多个Python版本上安装了Flake8，最好使用`Python<version> -m Flake8`来运行Flake8。

如果只想看特定的警告或错误：

```shell
flake8 --select E123,W503 <modules_or_packages>
```

如果想忽略一个特定的警告或错误：

```shell
flake8 --ignore E24,W504 <modules_or_packages>
```

<br/>

### Black

#### 简介

[Black](https://pypi.org/project/black/)是不妥协的Python代码格式化程序。这代表着用户不能细化代码风格。

Black默认会直接重构文件。

#### 安装

```shell
sudo pip install black / pip install black
```

当前版本：`black, version 20.8b1`

#### 使用

```shell
black [OPTIONS] {source_file_or_directory} / python -m black [OPTIONS] {source_file_or_directory}
```

不加参数时，Black默认会直接用新文件覆盖旧文件。

| Option               | 参数                                       | 说明                                                         |
| -------------------- | ------------------------------------------ | ------------------------------------------------------------ |
| -c                   | TEXT                                       | 格式化作为字符串传入的代码                                   |
| -l                   | INTEGER                                    | 每行允许多少字符                                             |
| -t                   | [py27\py33\py34\py35\py36\py37\py38] | 输出支持的Python版本                                         |
| --pyi                |                                            | 格式化所有输入文件，而不考虑文件扩展名（在将源管道输送到标准输入时很有用） |
| -S                   |                                            | 跳过字符串规范化，不规范化字符串引号或前缀                   |
| --check              |                                            | 不要写回文件，只需返回状态。0表示不会返回任何代码。返回代码1表示某些文件将被重新格式化。返回代码123表示存在内部错误。 |
| --diff               |                                            | 不要写回文件，只需在stdout上为每个文件输出一个diff           |
| --color / --no-color |                                            | 当 --diff设置，此项决定是否着色diff输出                      |
| --fast / --safe      |                                            | 如果有  --fast，跳过临时的健全性检查                         |
| --include            | TEXT                                       | 匹配递归搜索中应包含的文件和目录的正则表达式。空值表示包含所有文件，而不考虑其名称。对所有平台（Windows）上的目录使用正斜杠。先计算排除，再计算包含。 |
| --exclude            | TEXT                                       | 匹配递归搜索时应排除的文件和目录的正则表达式。空值表示不排除任何路径。对所有平台（Windows）上的目录使用正斜杠。 |
| --force-exclude      | TEXT                                       | 类似--exclude，但与此正则表达式匹配的文件和目录将被排除，即使它们作为参数显式传递。 |
| -q                   |                                            | 不向stderr发出非错误消息。仍会发出错误；使用2>/dev/null使其静音 |
| -v                   |                                            | 还向stderr发送有关由于--exclude而未更改或被忽略的文件的消息  |
| --version            |                                            | 输出版本信息                                                 |
| --config             | FILE                                       | 从文件路径读取配置                                           |
| -h                   |                                            | 输出帮助信息                                                 |

用这个软件在我的某个pyhton文件试了一下，代码变得好看了不少：

```diff
-class DBOperation():
+class DBOperation:

-        self.conn = sqlite3.connect('acceessrecord.db')
+        self.conn = sqlite3.connect("acceessrecord.db")

-    def dbSave(self,data,issend):
+    def dbSave(self, data, issend):

-        exe_str = 'INSERT INTO records (ftimestamp,person_id,person_name,cap_img,direction,stuff,issend) \
-      VALUES ("%s","%s","%s","%s","%s","%s","%s")' \
-                  % (data["timestamp"],data["person_id"],data["person_name"],data["cap_img"],data["direction"],data["stuff"],issend)
+        exe_str = (
+            'INSERT INTO records (ftimestamp,person_id,person_name,cap_img,direction,stuff,issend) \
+      VALUES ("%s","%s","%s","%s","%s","%s","%s")'
+            % (
+                data["timestamp"],
+                data["person_id"],
+                data["person_name"],
+                data["cap_img"],
+                data["direction"],
+                data["stuff"],
+                issend,
+            )
+        )
```



<br/>

### yapf

#### 简介

目前用于 Python 的大多数格式化程序——例如 autopep8和 pep8ify ——都是用来从代码中删除 lint 错误的。这有一些明显的局限性。例如，符合 PEP 8准则的代码可能不会被重新格式化。但这并不意味着代码看起来很好。

[YAPF](https://pypi.org/project/yapf/) 采取了不同的方法。它是基于[Clang格式](https://clang.llvm.org/docs/ClangFormat.html) ，由丹尼尔贾斯珀开发。本质上，算法将代码重新格式化为符合样式指南的最佳格式，即使原始代码没有违反样式指南。

这个想法也类似于 Go 编程语言的“ gofmt”工具: 结束所有关于格式化的神圣战争——如果一个项目的整个代码库只是简单地通过 YAPF 管道进行修改，那么这种风格在整个项目中保持一致，并且在每次代码审查中争论风格也没有意义。

#### 安装

YAPF 支持 Python 2.7 和 3.6.4+。YAPF 需要在程序代码对应的Python版本下运行。

```
pip install yapf
```

如果你使用的是Python2.7并且希望启动多进程：

```
pip install futures
```

#### 使用

```
usage: yapf [-h] [-v] [-d | -i] [-r | -l START-END] [-e PATTERN]
            [--style STYLE] [--style-help] [--no-local-style] [-p]
            [-vv]
            [files [files ...]]

可选参数:
  -h, --help            show this help message and exit
  -v, --version         show version number and exit
  -d, --diff            输出修改代码和原代码的diff
  -i, --in-place        直接替换原代码
  -r, --recursive       在目录上递归运行
  -l START-END, --lines START-END
                        格式化范围（以行为单位）
  -e PATTERN, --exclude PATTERN
                        排除匹配PATTERN的文件
  --style STYLE         指定格式样式：可以是样式名（例如“pep8”或“google”），也可以是具有样式设置的文件的名称。默认值为pep8
  --style-help          显示样式设置并退出; 此输出可以保存到.style.yapf
  --no-local-style      不搜索本地样式定义
  -p, --parallel        格式化多个文件时并行运行yapf。
  -vv, --verbose        处理时打印出文件名
```

YAPF默认是输出格式化之后的程序代码，加上-d之后输出的是更改的地方，加上-i之后是直接覆盖原代码。

<br/>

### isort

#### 简介

[isort](https://pypi.org/project/isort/)是import sort的缩写，用于自动排序imports。

#### 安装

```shell
pip install isort
```

#### 使用

示例，优化之前：

```python
from my_lib import Object

import os

from my_lib import Object3

from my_lib import Object2

import sys

from third_party import lib15, lib1, lib2, lib3, lib4, lib5, lib6, lib7, lib8, lib9, lib10, lib11, lib12, lib13, lib14

import sys

from __future__ import absolute_import

from third_party import lib3

print("Hey")
print("yo")
```

优化之后：

```python
from __future__ import absolute_import

import os
import sys

from third_party import (lib1, lib2, lib3, lib4, lib5, lib6, lib7, lib8,
                         lib9, lib10, lib11, lib12, lib13, lib14, lib15)

from my_lib import Object, Object2, Object3

print("Hey")
print("yo")
```

```
usage: __main__.py [-h] [-a ADD_IMPORTS] [-ac] [-af]
                   [-b KNOWN_STANDARD_LIBRARY] [-c] [-ca] [-cs] [-d] [-df]
                   [-ds] [-dt] [-e] [-f KNOWN_FUTURE_LIBRARY] [-fas] [-fass]
                   [-ff FROM_FIRST] [-fgw [FORCE_GRID_WRAP]] [-fss]
                   [-i INDENT] [-j JOBS] [-k] [-l LINE_LENGTH]
                   [-lai LINES_AFTER_IMPORTS] [-lbt LINES_BETWEEN_TYPES]
                   [-le LINE_ENDING] [-ls] [-m {0,1,2,3,4,5}] [-nis]
                   [-nlb NO_LINES_BEFORE] [-ns NOT_SKIP]
                   [-o KNOWN_THIRD_PARTY] [-ot] [-p KNOWN_FIRST_PARTY] [-q]
                   [-r REMOVE_IMPORTS] [-rc] [-s SKIP] [-sd DEFAULT_SECTION]
                   [-sg SKIP_GLOB] [-sl] [-sp SETTINGS_PATH] [-t FORCE_TO_TOP]
                   [-tc] [-up] [-v] [-vb] [--virtual-env VIRTUAL_ENV] [-vn]
                   [-w LINE_LENGTH] [-wl WRAP_LENGTH] [-ws] [-y]
                   [files [files ...]]

optional arguments:
  -h, --help            show this help message and exit
  -a ADD_IMPORTS, --add-import ADD_IMPORTS
                        将指定的导入行添加到所有文件，
                        自动确定正确的位置。
  -ac, --atomic         确保在生成的文件包含语法错误时不保存输出。
  -af, --force-adds     即使原始文件为空，也强制导入添加。
  -b KNOWN_STANDARD_LIBRARY, --builtin KNOWN_STANDARD_LIBRARY
                        强制sortImports将模块识别为python标准库的一部分。
  -c, --check-only      检查文件中是否存在未排序/未格式化的导入，并在不修改文件的情况下将其打印到命令行。
  -ca, --combine-as     在同一行合并为导入。
  -cs, --combine-star   确保如果存在星型导入，则不会从该命名空间导入其他内容。
  -d, --stdout          将结果输出强制到stdout，而不是直接覆盖。
  -df, --diff           打印isort对文件所做的所有更改的差异，而不是直接覆盖。
  -ds, --no-sections    将所有导入放入同一个节段中
  -dt, --dont-order-by-type
                        只按字母顺序导入，不要尝试类型排序
  -e, --balanced        平衡换行以产生尽可能一致的线条长度
  -f KNOWN_FUTURE_LIBRARY, --future KNOWN_FUTURE_LIBRARY
                        强制sortImports将模块识别为未来兼容库的一部分。
  -fas, --force-alphabetical-sort
                        强制将所有导入作为单个节排序
  -fass, --force-alphabetical-sort-within-sections
                        强制在节中按字母顺序对所有导入进行排序
  -ff FROM_FIRST, --from-first FROM_FIRST
                        切换典型的顺序偏好，imports优先
  -fgw [FORCE_GRID_WRAP], --force-grid-wrap [FORCE_GRID_WRAP]
                        强制从导入数（默认为2）进行网格换行，而不考虑行的长度
  -fss, --force-sort-within-sections
                        强制导入按模块排序，与导入类型无关
  -i INDENT, --indent INDENT
                        用于缩进的字符串默认为“”（4个空格）。
  -j JOBS, --jobs JOBS  要并行处理的文件数。
  -k, --keep-direct-and-as
                        关闭在存在导入时删除直接导入的默认行为。
  -lai LINES_AFTER_IMPORTS, --lines-after-imports LINES_AFTER_IMPORTS
  -lbt LINES_BETWEEN_TYPES, --lines-between-types LINES_BETWEEN_TYPES
  -le LINE_ENDING, --line-ending LINE_ENDING
                        强制行尾为指定值。如果未设置，则每个文件将猜测值。
  -ls, --length-sort    按字符串长度对导入进行排序。
  -m {0,1,2,3,4,5}, --multi-line {0,1,2,3,4,5}
                        多行输出（0-网格，1-垂直，2-悬挂，3-垂直悬挂，
                        4-垂直网格，5-垂直网格-分组，6-垂直网格-分组-无逗号）
  -nis, --no-inline-sort
                        保留"from imports with multiple imports"原样
                        (例如 `from foo import a, c ,b`)
  -nlb NO_LINES_BEFORE, --no-lines-before NO_LINES_BEFORE
                        不应与以前的分段用空行分隔的节
  -ns NOT_SKIP, --dont-skip NOT_SKIP
                        对导入进行排序的文件决不能跳过
  -o KNOWN_THIRD_PARTY, --thirdparty KNOWN_THIRD_PARTY
                        强制sortImports将模块识别为第三方库的一部分
  -ot, --order-by-type  除了按字母顺序外，还可以按类型顺序导入
  -p KNOWN_FIRST_PARTY, --project KNOWN_FIRST_PARTY
                        强制sortImports将模块识别为当前python项目的一部分
  -q, --quiet           显示额外的安静输出，只输出错误
  -r REMOVE_IMPORTS, --remove-import REMOVE_IMPORTS
                        从所有文件中删除指定的导入
  -rc, --recursive      递归地查找要对导入进行排序的Python文件
  -s SKIP, --skip SKIP  对导入进行排序的文件应跳过。如果要跳过多个文件，应指定两次：
                        --skip file1 --skip file2.
  -sd DEFAULT_SECTION, --section-default DEFAULT_SECTION
                        设置导入的默认节（默认为FIRSTPARTY）选项： ('FUTURE',
                         'STDLIB', 'THIRDPARTY', 'FIRSTPARTY', 'LOCALFOLDER')
  -sg SKIP_GLOB, --skip-glob SKIP_GLOB
                        对导入进行排序的文件应跳过
  -sl, --force-single-line-imports
                        强制所有来自imports的数据都显示在自己的行上
  -sp SETTINGS_PATH, --settings-path SETTINGS_PATH
                        显式设置设置路径，而不是根据文件位置自动确定
  -t FORCE_TO_TOP, --top FORCE_TO_TOP
                        将特定导入强制到其相应部分的顶部
  -tc, --trailing-comma
                        在包含括号的多行导入中包含尾随逗号
  -up, --use-parentheses
                        在长度限制上使用圆括号代替斜杠
  -v, --version
  -vb, --verbose        显示详细的输出，例如跳过文件或检查成功时
  --virtual-env VIRTUAL_ENV
                        用于确定包是否为第三方的虚拟环境
  -vn, --version-number
                        只返回不带logo的当前版本号
  -w LINE_LENGTH, --line-width LINE_LENGTH
                        导入行的最大长度(used for wrapping long imports)
  -wl WRAP_LENGTH, --wrap-length WRAP_LENGTH
                        指定换行的行的长度,如果未设置，使用line_length
  -ws, --ignore-whitespace
                        告诉isort在使用--check-only时忽略空白差异
  -y, --apply           告诉isort递归地应用更改，而无需询问
```

<br/>

### pre-commit

#### 简介

Git hooks 可让您在想要提交或推送时运行检查脚本。这使我们能够在每次提交/推送时，对将要提交的代码进行检查、优化代码格式、或者对提交的图片进行压缩等等任务。

钩子都被存储在 Git 目录下的 hooks 子目录中。 也即绝大部分项目中的 .git/hooks 。 当你用 git init 初始化一个新版本库时，Git 默认会在这个目录中放置一些示例脚本。这些脚本除了本身可以被调用外，它们还透露了被触发时所传入的参数。 所有的示例都是 shell 脚本，其中一些还混杂了 Perl 代码，不过，任何正确命名的可执行脚本都可以正常使用 —— 你可以用 Ruby 或 Python，或其它语言编写它们。 这些示例的名字都是以.sample 结尾，如果你想启用它们，得先移除这个后缀。

把一个正确命名且可执行的文件放入 Git 目录下的 hooks 子目录中，即可激活该钩子脚本。 这样一来，它就能被 Git 调用。

[pre-commit](https://pre-commit.com/)用于检查即将提交的快照。在`git add`之后，在执行`git commit`时执行脚本，如果脚本以0值退出就继续提交，非0就驳回提交的操作。可以用 `git commit --no-verify` 来绕过这个环节。

<br/>

### 参考

* [PEP8](https://www.python.org/dev/peps/pep-0008/)
* [如何建立一个完美的 Python 项目_a419240016的专栏-CSDN博客](https://blog.csdn.net/a419240016/article/details/108459618)


---
title: AStyle 使用笔记
date: 2023-12-29 22:49:00
toc: true
categories:
  - tech
tags:
  - Tools
---

AStyle(Artistic Style) 是一个免费、快速、小巧的代码格式化工具，支持 C, C++, C++/CLI, Objective-C, C#, Java 源码。

* 官网：[Artistic Style - Index (sourceforge.net)](https://astyle.sourceforge.net/)
* Windows 下载：[Download Artistic Style (sourceforge.net)](https://sourceforge.net/projects/astyle/files/latest/download)
* Ubuntu 安装：`sudo apt-get install astyle`
* 测试环境：3.1 @ Ubuntu 

<!--more-->

<br/>

### 常用命令

`astyle -A1 -t file_name.c`：将 file_name.c 格式化为 allman 样式，并使用 tab 缩进。

`astyle -A1 -t -r *.c,*h`：将所有此目录及以下的 .c/.h 文件进行格式化， allman 样式，tab 缩进。

`astyle -A1 -t -p -n -z2 -r *.c `：将所有此目录及以下的 .c 文件进行格式化，allman 样式，tab 缩进，在操作符前后填充空格，不保留原始文件备份，使用 linux (LF) 换行符。

<br/>

### 使用说明

官方说明：[Artistic Style (sourceforge.net)](https://astyle.sourceforge.net/astyle.html)

```bash
astyle [OPTIONS] File1 File2 File3 [...]
```

* **--help / -h / -?**
* **--version / -V**
* **--style** 大括号设置：包含 换行大括号、换行并缩进大括号、行尾大括号，可以细致到 命令空间、类名、函数名、if-else、数组、struct、enum 分别用不同样式的大括号。
  * **--attach-namespaces / -xn**
  * **--attach-classes / -xc**
  * **--attach-inlines / -xl**
  * **--attach-extern-c / -xk**
  * **--attach-closing-while / -xV**
  * **--indent-classes / -C**
  * **--break-closing-braces / -y**
  * **--break-elseifs / -e**
  * **--break-one-line-headers / -xb**
  * **--add-braces / -j**
  * **--remove-braces / -xj**
  * **--add-one-line-braces / -J**
* **--indent** 缩进设置：包含 空格、Tab，还可单独指定空格数量。可以细致的对缩进进行设置。
  * **--indent-classes / -C**
  * **--indent-modifiers / -xG**
  * **--indent-switches / -S**
* **--break-blocks / -f** 填充选项，可以在 if、for、while 前后填充空行。
* **--pad-oper / -p** 在操作符前后填充空格。
* **--pad-comma / -xg** 在逗号后填充空格。
* **--[un]pad-paren / -P** 在括号的左右填充空格。
* **--pad-header / -H** 在 if、for、while 后填充空格。
* **--[un]pad-brackets** 在中括号内填充空格。
* **--delete-empty-lines / -xe** 删除函数中的空行。函数之外的空行不会被删除。
* **--squeeze-lines=#** 删除超过给定数量的多余空行。
* **--squeeze-ws** 删除多余的空白。
* **--align-pointer=type/middle/name** 指针或引用符号靠近谁。
* **--align-reference=type/middle/name** 引用符号靠近谁。
* **--keep-one-line-blocks / -O** 保持大括号括起来的一行。
* **--keep-one-line-statements / -o** 保持一行的语句。
* **--max-code-length=#   / -xC#** 一行代码超过#个字符后自动换行。自动从逻辑符号、逗号、分号、空格处换行。注释、引号和数组不会换行。
* **--break-after-logical / -xL** 默认情况下，逻辑条件将首先放在新行中。
* **--suffix=####** 使用 #### 替换本来会添加到原文件后的 .orig。
* **--suffix=none / -n** 不要保留原始文件的备份。
* **--recursive / -r / -R** 对于命令行中的每个目录，递归地处理所有子目录。使用递归选项时，文件名语句应包含通配符（eg "*.cpp"）。
* **--dry-run** 输出报告，但不对源文件做任何修改。
* **--exclude=####** 排除某个文件或文件夹。
* **--formatted / -Q** 仅显示已格式化的文件。不要显示未更改的文件。
* **--lineend=windows/linux/macold / -z123** 强制使用指定的换行符，windows (CRLF)，linux (LF)，macold (CR)。

仅限命令行：

* **--options=####/none** 指定要读取和使用的选项文件####。这将允许文件名从.astylerc或_astylerc更改。“none”选项将禁用默认选项文件（如果存在）。

<br/>

### 常用设置

linux_app.astylerc

```ini
# -A1
--style=allman

# -t
--indent=tab

# -z2
--lineend=linux

# -p
--pad-oper

--squeeze-ws
--squeeze-lines=3
```

运行：

`astyle --project=linux_app.astylerc -r *.c `

或者先设置环境变量：`export ARTISTIC_STYLE_OPTIONS=$(pwd)/linux_app.astylerc`

`astyle -r *.c,*.h,*.cpp`


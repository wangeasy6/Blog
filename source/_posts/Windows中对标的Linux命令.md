---
title: Windows中对标的Linux命令
date: 2019-11-25 16:54:12
toc: true
categories:
  - Technology
---

在Windows环境下打开工程，找到某个文件引用关系，但是又找不到文件的时候，总是怀念Linux中的find命令。

总结一下 Windows 命令提示符的命令中，哪些是和Linux 常用命令能对标的。

<!--more-->

试用环境：Windows 10

<br/>

### 常用命名

| Linux      | Windows                                      | 说明               |
| ---------- | -------------------------------------------- | ------------------ |
| cmd --help | ”help cmd“ or  “cmd /?”                      | Help信息           |
| cd         | cd（切换不同盘需要使用  “D:” 或者加上 "/D"） | 路径切换           |
| ls         | dir                                          | 文件列表           |
| rm         | del                                          | 删除文件           |
| rm -r      | rd /S                                        | 删除文件夹         |
| md         | md                                           | 创建文件夹         |
| mv         | move                                         | 移动文件/文件夹    |
| cp         | copy 、 xcopy /S                             | 复制文件、文件夹   |
| cat        | type                                         | 输出文本内容       |
| vim        | 文件名（会使用默认打开工具打开文件）         | 编辑文本           |
| diff       | fc                                           | 比较文件差异       |
| grep       | find                                         | 过滤字符串         |
| du -d      | -                                            | 文件夹大小         |
| tree       | tree                                         | 列出文件树         |
| md5sum     | certutil -hashfile "file_name" MD5           | 获取文件MD5值      |
| sha1sum    | certutil -hashfile "file_name" SHA1          | 获取文件Hash-1值   |
| sha256sum  | certutil -hashfile "file_name" SHA256        | 获取文件Hash-256值 |
| exit       | exit                                         | 退出               |

<br/>

### find

dos 中的 find 是在文件中搜索字符串，想要实现和Linux find同样的功能要使用for语句：

```
for /r [[drive:]path] %variable in (set) do command [command-parameters]
```

1、set中的文件名如果含有通配符(？或*)，则列举/R参数指定的目录及其下面的所用子目录中与set相符合的所有文件，无相符文件的目录则不列举。
2、如果set中为具体文件名，不含通配符，则枚举该目录树（即列举该目录及其下面的所有子目录）(并在后面加上具体的文件名)，而不管set中的指定文件是否存在。

```
for /r c:\ %i in (*.exe *.ini) do @echo %i    --枚举C盘所有目录下的.exe和.ini文件
for /r c:\ %i in (boot.ini) do @echo %i --枚举c盘所有目录+boot.ini
```

<br/>

### 文件搜索&文件夹搜索

1. 搜索文件中是否包含某一字符串，如果有，则输出那一行
   1. Linux：`grep "test" file1 file2` (末尾加 -n：输出行号)
   2. Windows：`find "test" file1 file2` (末尾加 /N：输出行号)
2. 搜索整个目录文件，输出包含字符串的所有文件+行
   1. Linux：`grep -r -e "test" path`
   2. Windows：`findstr /S "test" * ` (\*代表所有文件 )

<br/>

### 右键打开cmd

在Windows下打开“命令提示符”界面，路径总是从登录目录开始。在执行命令时总要输入一长串路径进行跳转。

将cmd添加到鼠标右键，在任意一个资源管理界面点击右键打开cmd，自动从当前路径开始，为执行命令节省时间。

[下载插件](/images/dos-bash/AutoCMD.zip)，双击运行。你会发现在资源管理点击鼠标右键，列表中多了一个 “cmd_here"。

<br/>

### git bash

对比dos的命令功能，明显是没有Linux shell那么强的。

所以建议直接安装一个[git](https://git-scm.com/downloads) ，安装的时候勾选将git bash添加到鼠标右键，这样不仅能在任意位置打开bash，还可以无缝使用Linux Bash命令。
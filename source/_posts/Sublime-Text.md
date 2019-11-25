---
title: Sublime Text
date: 2018-10-17 16:57:00
toc: true
categories:
  - tools
tags:
  - 待续
---

响应超快，超好用的文本编辑器。
<!--more-->

### 中文乱码BUG

* Sublime Text 默认采用UTF8编码，这就造成了许多采用GBK编码的文件里的中文显示为乱码。需要安装一个专门解决这个问题的插件：ConvertToUTF8 。要安装这个插件，首先要安装 Sublime Text 的 **Package Control**。
* 首先要安装 Sublime Text 的 Package Control
  * 打开Sublime Text，```Ctrl + ~```打开控制台，
  *  https://packagecontrol.io/installation 根据你的 Sublime Text 版本选择对应的安装代码，将代码输入控制台后回车，之后便会自动下载并安装 Package Control
  * 安装完成后需要重启Sublime Text 。
* 接下来就可以安装 ConvertToUTF8 了，按Ctrl + Shift + P打开万能搜索框，然后输入install package回车，这时候会加载所有的packages列表。看到列表之后再输入ConvertToUTF8回车，就会下载安装这个包了。装好之后无需重启Sublime Text你就会惊喜的发现原来的中文乱码都能正常显示了。
* **直接安装**：https://github.com/seanliang/ConvertToUTF8 项目打包下载并解压，将解压后的文件夹名修改为 *ConvertToUTF8* ，然后将此文件夹移动到 Sublime Text 的 *Packages* 文件夹下（可通过 Sublime Text 菜单中的 Preferences > Browse Packages 找到 *Packages* 文件夹）。

### 常用设置

##### 设置缩进

* 右下角可进行方便设置 Space/Tab，Width
* “Preferences”，点击“Settings”，在settings-User中添加行：
  `"tab_size": 4,"translate_tabs_to_spaces": true,`

##### 设置删除多余空格

* “Preferences”，点击“Settings”，在settings-User中添加行：
`"trim_trailing_white_space_on_save": true,`

##### 设置打开文件不覆盖

* “Preferences”，点击“Settings”，在settings-User中添加行：

  `"preview_on_click": false,`

### 工程搜索

1. 在工程中搜索文件：

   `Ctrl + p`，打开搜索框，输入文件名，会自动出现匹配的文件目录，点击即可打开

2. 在工程中搜索包含某个字符串的文本：

   在文件夹目录中点击右键，选择“Find in Folder”



### 快捷键

| 命令               | 说明                       |
| ------------------ | -------------------------- |
| Alt + [n]          | 跳转到文件栏的第n个文件    |
| Ctrl + Tab         | 跳转到文件栏的右边一个文件 |
| Ctrl + Shift + Tab | 跳转到文件栏的左边一个文件 |
| Alt + -            | 跳转到上一个输入点         |
| Alt + Shift + -    | 跳转到下一个输入点         |

### 快捷编译/运行程序

* C
  * https://blog.csdn.net/DragonFreedom/article/details/70159891
* C/C++
  * https://blog.csdn.net/shenwanjiang111/article/details/53728941
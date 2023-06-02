---
title: Sublime Text
date: 2018-10-17 16:57:00
toc: true
categories:
  - Technology
tag:
  - Tools
---

响应超快，超好用的文本编辑器。
<!--more-->
<br/>

### 常用设置

**汇总**

```
{
	"font_size": 14,
	"preview_on_click": false,
	"tab_size": 4,"translate_tabs_to_spaces": true,
	"trim_trailing_white_space_on_save": true
}
```

**设置缩进**

* 右下角可进行方便设置 Space/Tab，Width
* “Preferences”，点击“Settings”，在settings-User中添加行：
  `"tab_size": 4,"translate_tabs_to_spaces": true,`

**设置删除多余空格**

* “Preferences”，点击“Settings”，在settings-User中添加行：
`"trim_trailing_white_space_on_save": true,`

**设置打开文件不覆盖**

* “Preferences”，点击“Settings”，在settings-User中添加行：

  `"preview_on_click": false,`

**设置自动换行**

* “Preferences”，点击“Settings”，在settings-User中添加行：

  `"word_wrap" : true,`

<br/>

### 快捷键

| 命令               | 说明                       |
| ------------------ | -------------------------- |
| Ctrl + /           | 单行注释                   |
| Ctrl + Shift + /   | 多行注释                   |
| Ctrl + b           | 快速运行程序               |
| Alt + -            | 跳转到上一个输入点         |
| Alt + Shift + -    | 跳转到下一个输入点         |
| Alt + [n]          | 跳转到文件栏的第n个文件    |
| Ctrl + Tab         | 跳转到文件栏的右边一个文件 |
| Ctrl + Shift + Tab | 跳转到文件栏的左边一个文件 |
| Ctrl + f           | 查看字符串                 |
| Ctrl + h           | 替换字符串                 |
| Alt + Shift + 1    | 单页显示                   |
| Alt + Shift + 2    | 2列显示                    |
| Alt + Shift + 3    | 3列显示                    |
| Alt + Shift + 4    | 4列显示                    |
| Alt + Shift + 5    | 田字显示                   |
| Alt + Shift + 8    | 2行显示                    |
| Alt + Shift + 9    | 3行显示                    |

**查找/替换换行符：**

要先选中正则表达式（regular expression）匹配模式，如图，点中（.*）按钮。

![replace_line_breaks.png](/resources/sublime/replace_line_breaks.png)

在正则表达式模式下，换行符即为转义字符"\n"，只需要在查找或者换行中输入"\n"即代表换行符。

<br/>

### 工程搜索

* 在工程中搜索文件：

  `Ctrl + p`，打开搜索框，输入文件名，会自动出现匹配的文件目录，点击即可打开。

* 在工程中搜索包含某个字符串的文本：

  在文件夹目录中点击右键，选择`Find in Folder`。

* 函数列表跳转：

  `Ctrl + r`，跳出当前文件函数列表，可以输入字符进行模糊查找后跳转。

<br/>

### 中文乱码BUG

* Sublime Text 默认采用UTF8编码，这就造成了许多采用GBK编码的文件里的中文显示为乱码。需要安装一个专门解决这个问题的插件：ConvertToUTF8 。要安装这个插件，首先要安装 Sublime Text 的 **Package Control**。
* 首先要安装 Sublime Text 的 Package Control
  * 打开Sublime Text，```Ctrl + ~```打开控制台，
  *  https://packagecontrol.io/installation 根据你的 Sublime Text 版本选择对应的安装代码，将代码输入控制台后回车，之后便会自动下载并安装 Package Control
  * 安装完成后需要重启Sublime Text 。
* 接下来就可以安装 ConvertToUTF8 了，按Ctrl + Shift + P打开万能搜索框，然后输入install package回车，这时候会加载所有的packages列表。看到列表之后再输入ConvertToUTF8回车，就会下载安装这个包了。装好之后无需重启Sublime Text你就会惊喜的发现原来的中文乱码都能正常显示了。
* **直接安装**：https://github.com/seanliang/ConvertToUTF8 项目打包下载并解压，将解压后的文件夹名修改为 *ConvertToUTF8* ，然后将此文件夹移动到 Sublime Text 的 *Packages* 文件夹下（可通过 Sublime Text 菜单中的 Preferences > Browse Packages 找到 *Packages* 文件夹）。

<br/>

### 快捷编译/运行程序

* C
  * https://blog.csdn.net/DragonFreedom/article/details/70159891
* C/C++
  * https://blog.csdn.net/shenwanjiang111/article/details/53728941

### 手动刷新文件夹

* Project -> Refresh Folders

### 开启时禁止打开上次未退出文件

* 从菜单栏开始，依次选择：首选项---设置 - 用户
* 接着在打开的文件的末尾(花括号上方)追加以下内容：`"remember_open_files": false`
* 再保存文件。从菜单栏依次选择：文件---保存
* 最后，退出sublime编辑器，再次启动sublime，软件的自动记忆打开功能就关闭了

如果SMB关闭了，上次又打开了过多的文件，那么重启 Sublime 就可能启动不了。

方法一：

* 删除此目录下的所有文件：C:\Users\用户\AppData\Roaming\Sublime Text\Local\

方法二：

* 直接修改C:\Users\用户\AppData\Roaming\Sublime Text\Packages\User\Preferences.sublime-settings，添加`"remember_open_files": false`

### CMake 语法高亮

**1. 下载 cmake 语法高亮包**

地址：[https://github.com/zyxar/Sublim](https://link.zhihu.com/?target=https%3A//github.com/zyxar/Sublime-CMakeLists)

**2. 放到 Sublime Text 3 插件目录**

Sublime 的插件目录可以通过**首选项--->浏览插件目录**的方式打开。然后将下载的高亮包解压在该目录。
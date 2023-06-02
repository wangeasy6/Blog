---
title: Windows Form 界面布局
toc: true
categories:
  - null
tags:
  - null
---



<!--more-->

<br/>

1，锚定功能（Anchor属性）

默认为“Top，Left”，不管窗体大小如果改变，保持相对于窗体左上角的位置。

如果设置为”Top,Bottom,Left,Right”这样，控件的大小将随窗体的大小改变，但距离窗体四边的距离不变。

2，停靠功能（Dock属性）

允许将控件粘在容器的边界上，默认为”None”(StatusStrip控件默认值为Bottom)。

典型应用：分割条切分窗体应用。

3，控件的Z-Order顺序

垂直堆栈上的层次，可决定遮挡关系。调整方法：视图-其他窗口-文档大纲。

4，控件的Tab键顺序

按Tab键的切换顺序。调整方法：视图-Tab键顺序。

5，分隔控件

SplitContainer控件，可修改Orientation属性的值来改变垂直分隔还是水平分隔。在两个面板中可以设置FixedPanel属性来确定是否维持固定的大小。

6，分组

用分隔控件的时候往往需要将一组控件一起作为一部分，这时可以用GroupBox，Panel，TabPage这样的控件将若干控件进行分组。

### WindowsForm 添加用户控件

https://jingyan.baidu.com/article/b24f6c822251b386bfe5daeb.html

## 参考

* [Windows Forms 布局篇](https://www.cnblogs.com/ysz12300/p/5283346.html)


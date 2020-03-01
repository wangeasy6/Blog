---
title: PyQt5代码小结
toc: true
categories:
  - tech
---

基于树莓派**3B+**的**PyQt5**开发，代码小结。
项目界面比较简单，只使用了基础部分，没有使用**QML**、**Qt Quick**等高级用法。

<!--more-->

### 系统环境

硬件：树莓派**3B+**

软件：

* **Python 3.5.3**

*  **PyQt 4.7.1**

<br/>

### Hello World

```python
#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import QApplication, QWidget
from PyQt5.QtGui import QIcon


class Example(QWidget):
    
    def __init__(self):
        super().__init__()
        #界面绘制交给InitUi方法
        self.initUI()
        
    def initUI(self):
        #设置窗口的位置和大小
        self.setGeometry(300, 300, 300, 220)  
        #设置窗口的标题
        self.setWindowTitle('Simple')
        #设置窗口的图标，引用当前目录下的icon.png图片
        self.setWindowIcon(QIcon('icon.png'))        
        
        #显示窗口
        self.show()

        
if __name__ == '__main__':
    #创建应用程序和对象
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())
```

上面的示例代码在屏幕上显示一个小窗口，如下：

![Simple](/images/PyQt5小结/simple.png)

#### QApplication

**QApplication**类管理**GUI**程序控制流和主要设置。包含处理本程序和其他来源的事件循环，程序的初始化和结束，并提供对话管理。**QApplication**处理了系统和应用的大部分设置。

对于用**Qt**写的任何一个**GUI**应用，不管这个应用有没有窗口或多少个窗口，有且只有一个**QApplication**对象，**QApplication**对象必须在绘制设备（设备包括窗口、像素映射、位图等）之前创建。。

而对于用**Qt**写的非**GUI**应用，则有且只有一个**QCoreApplication**对象，并且这个应用不依赖**QtGui**库。

**QApplication.exec_()**，开始事件循环，直到**exit()**被调用。

#### Example

**Example** 是一个继承了[**QWidget**](https://doc.qt.io/qtforpython/PySide2/QtWidgets/QWidget.html#static-functions)的类，**QWidget**是所有控件的父类，如**QLabel**、**QButton**等，当它的父类为空时，它是一个Window，这个窗体本身就用很多属性和方法，如长宽、在屏幕中的位置等，可以使用**setGeometry()**来设置，除此之外，还用一下方法使用得比较多：

| 方法                        | 释义                                                         |
| --------------------------- | ------------------------------------------------------------ |
| setGeometry(w, h, x, y)     | 设置长宽为w,h，移动到x,y坐标                                 |
| QRect  geometry()           | 获取长宽和坐标，QRect 使用width()、height()、x()、y()方法获得单个值 |
| size()                      | 获取窗体长宽                                                 |
| resize(w, h)                | 设置长宽为w,h                                                |
| setFixedSize(w,h)           | 设置为长宽不可变的窗体                                       |
| move(x, y)                  | 移动到x,y坐标                                                |
| setCursor()                 | 设置鼠标样式                                                 |
| setWindowTitle(string)      | 设置窗口标题                                                 |
| setWindowIcon(QIcon)        | 设置窗口图标                                                 |
| setWindowFlags(WindowFlags) | 设置窗口样式                                                 |
| setStyleSheet(string)       | 设置窗口样式表                                               |
| setLayout(*QLayout*)        | 设置当前界面的布局管理器                                     |
| show()                      | 在每次布局完，调用show来显示                                 |
| showFullScreen()            | 全屏显示                                                     |
| close()                     | 关闭部件                                                     |

#### 窗口类型

主要窗口类型有：

1. Qt.Widget：这是QWidget的默认类型。如果它们有父级，这种类型的部件是子部件，如果没有父控件，则为独立窗口。
2. Qt.Window：表示小部件是一个窗口，无论窗口小部件是否具有父控件，通常具有窗口系统框架和标题栏。如果部件没有父控件，则无法取消设置此标记（即如果存在父控件，则可以取消此标记，否则，必须存在此标记用以标识此控件是个独立窗口）。
3. Qt.Dialog：指示部件是应该作为对话框窗口（即标题栏中通常没有最大化或最小化按钮）。这是QDialog的默认类型。如果要将其用作模态对话框，则应从另一个窗口启动它，或者如果有父窗口，则与QWidget.windowModality属性一起使用。如果将其设置为模态，则对话框将阻止应用程序中的其他顶级窗口获取任何输入。我们将具有父级的顶级窗口称为辅助窗口。
4. Qt.Sheet：表示该窗口是Macintosh工作表。 由于使用工作表意味着窗口模态，推荐的方法是使用QWidget.setWindowModality()或QDialog.open()。
5. Qt.Drawer：表示该窗口小部件是Macintosh抽屉。
6. Qt.Popup：表示窗口小部件是弹出式顶级窗口，即它是模态的，但具有适合弹出菜单的窗口系统框架。
7. Qt.Tool：表示窗口小部件是工具窗口。工具窗口通常是一个小窗口，其标题栏和装饰比通常小，通常用于工具按钮的集合。 如果有父部件，则工具窗口将始终保持在其上。 如果没有父部件，您可以考虑使用Qt.WindowStaysOnTopHint使其位于最顶端。 如果窗口系统支持它，工具窗口可以用更轻的框架装饰。它也可以与Qt.FramelessWindowHint结合使用。 在macOS上，工具窗口对应浮动类窗口。 这意味着窗口存在于正常窗口之上; 不可能在它上面放一个普通的窗口。 默认情况下，当应用程序处于非活动状态时，工具窗口将消失。 这可以通过Qt.WA_MacAlwaysShowToolWindow属性来控制。
8. Qt.ToolTip：表示窗口小部件是工具提示。 这在内部用于实现工具提示，没有标题栏和窗口边框。
9. Qt.SplashScreen：表示该窗口是闪屏。 这是QSplashScreen的默认类型。
10. Qt.Desktop：表示此小组件是桌面。 这是QDesktopWidget的类型。
11. Qt.SubWindow：表示此窗口小部件是子窗口，例如QMdiSubWindow窗口小部件。
12. Qt.ForeignWindow：指示此窗口对象是表示由另一个进程创建的本机平台窗口或手动使用本机代码的句柄。
13. Qt.CoverWindow：表示该窗口代表一个封面窗口，例如，当应用程序在BlackBerry平台上最小化时显示。
14. Qt.MSWindowsFixedSizeDialogHint：在Windows上为窗口提供精简的对话框边框。 此样式传统上用于固定大小的对话框。
15. Qt.MSWindowsOwnDC：在Windows上为窗口提供自己的显示上下文。
16. Qt.BypassWindowManagerHint：此标志可用于向平台插件指示应禁用“所有”窗口管理器协议。 此标志的行为会有所不同，具体取决于运行应用程序的操作系统以及正在运行的窗口管理器。 该标志可用于获取没有配置集的本机窗口。
17. Qt.X11BypassWindowManagerHint：完全绕过窗口管理器。 这导致无边界窗口根本不受管理（即，除非您手动调用QWidget.activateWindow()，否则没有键盘输入）。
18. Qt.FramelessWindowHint：生成无边框窗口。 用户无法通过窗口系统移动或调整无边框窗口的大小。 在X11上，标志的结果取决于窗口管理器及其理解Motif和/或NETWM提示的能力。 大多数现有的窗口管理器都可以处理。
19. Qt.NoDropShadowWindowHint：在支持平台上禁用窗口投影。

#### Qt.WindowFlags

Qt.WindowFlags是一个32位的参数，设置此属性时，可以将多个属性进行与或来得到想要的窗口效果。

主要的窗口样式：

```
Qt::CustomizeWindowHint          //关闭默认窗口标题提示
Qt::WindowTitleHint              //标题栏
Qt::WindowSystemMenuHint         //窗口菜单系统
Qt::WindowMinimizeButtonHint     //最小化按钮
Qt::WindowMaximizeButtonHint     //最大化按钮
Qt::WindowMinMaxButtonsHint      //最大化和最小化按钮
Qt::WindowCloseButtonHint		 //关闭窗口按钮
Qt::WindowContextHelpButtonHint  //窗口上下文帮助按钮
Qt::MacWindowToolBarButtonHint   //Mac窗口工具栏按钮
Qt::WindowFullscreenButtonHint   //窗口全屏按钮
Qt::WindowShadeButtonHint		 //窗口阴影按钮
Qt::WindowStaysOnTopHint		 //窗口最在最顶按钮
Qt::WindowStaysOnBottomHint		 //窗口最在最底按钮
Qt::WindowOkButtonHint			 //Ok按钮
Qt::WindowCancelButtonHint		 //Cancel按钮
```

各种组合的样式参照：https://www.cnblogs.com/Jace-Lee/p/5869496.html

#### new

QComboBox(minimumWidth=200, minimumHeight=30)

QLabel(self)

#### 加载qss样式

```python
with open('main.qss', 'r') as f:
    self.setStyleSheet(f.read())
```

<br/>

#### QLabel显示图片并缩放

`QLabel(pixmap = QPixmap("imgs/jzt_logo.png").scaled(155,48))`



### 使用QTableWidget

```
head=['机构', '中队', '姓名', '装备类别', '装备品牌',
        '装备型号', '领用人', '领用时间', '是否归还', '归还时间']
list = QTableWidget()
list.setColumnCount(10)
list.setEditTriggers(QAbstractItemView.NoEditTriggers) # 不允许编辑
list.setHorizontalHeaderLabels(head)
list.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
list.horizontalHeader().setSectionResizeMode(7, QHeaderView.Interactive)
list.horizontalHeader().setSectionResizeMode(9, QHeaderView.Interactive)
```



### 布局管理

addWidget

### Load数据时显示Loading动画



### 页面跳转



### QSS

setObjectName

### 自定义MessageBox

### 日期

```
self.dt_time = QDateTimeEdit(QDate().currentDate())
self.dt_time.setCalendarPopup(True)
```







### 参考：

* [PyQt5教程](http://code.py40.com/face)

* [PyQt API](https://doc.qt.io/qtforpython/api.html)
---
title: PyQt5代码小结
date: 2020-03-29 22:13:56
toc: true
categories:
  - Technology
tags:
  - Python
  - QT
---

![PyQt.jpg](/resources/PyQt5小结/PyQt.jpg)

基于**PyQt5**的开发项目，代码小结。
项目界面比较简单，只使用了基础部分，没有使用**QML**、**Qt Quick**等高级用法。

<!--more-->

<br/>

### 系统环境

硬件：树莓派**3B+**

软件：

* **Python 3.5.3**
*  **PyQt 5.7.1**（环境安装：`sudo apt-get insatll python3-pyqt5`）
*  **PyQt 5.14.2**（环境安装：pip3 install PyQt5==5.14.2（[python setup.py egg_info error 解决](https://www.cnblogs.com/qq952693358/p/9592300.html)））

硬件环境：**Windows 10**

软件：

* **Python 3.5.3**（环境安装：[下载安装对应版本](https://www.python.org/downloads/windows/)）
* **PyQt 5.11.3**（环境安装：`pip install PyQt5==`[5.11.3](https://pypi.org/project/PyQt5/#history)）

[pip下载速度慢解决方法](https://blog.csdn.net/wukai0909/article/details/62427437)

<br/>

### Hello World

```python
#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import QApplication, QWidget
from PyQt5.QtGui import QIcon


class MainWindow(QWidget):

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
    mw = MainWindow()
    sys.exit(app.exec_())
```

示例代码运行会在屏幕上显示一个小窗体，如下：

![Simple](/resources/PyQt5小结/simple.png)

#### QApplication

**QApplication**类管理**GUI**程序控制流和主要设置。包含处理本程序和其他来源的事件循环，程序的初始化和结束，并提供对话管理。**QApplication**处理了系统和应用的大部分设置。

对于用**Qt**写的任何一个**GUI**应用，不管这个应用有没有窗口或多少个窗口，有且只有一个**QApplication**对象，**QApplication**对象必须在绘制设备（设备包括窗口、像素映射、位图等）之前创建。

而对于用**Qt**写的非**GUI**应用，则有且只有一个**QCoreApplication**对象，并且这个应用不依赖**QtGui**库。

**QApplication.exec_()**，开始事件循环，直到**exit()**被调用。

#### MainWindow

**MainWindow**是一个继承了[**QWidget**](https://doc.qt.io/qtforpython/PySide2/QtWidgets/QWidget.html#static-functions)的类，**QWidget**是所有控件的父类，如**QLabel**、**QButton**等，当它的父类为空时，它是一个Window，这个窗体本身就用很多属性和方法，如长宽、在屏幕中的位置等，可以使用**setGeometry()**来设置，除此之外，还用一下方法使用得比较多：

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

#### 窗体类型

本例中，继承的是QWidget，初此之外，还有这些窗体类型：

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

<br/>

### 布局管理

Hello World示例显示了一个窗体，但我们使用图形界面的目的当然不是只显示一个窗体，而是要放入字符标签和按钮等元素。如何按照我们的意愿放置元素便是布局管理做的事情。

具体分为绝对布局、横向/纵向布局、表格布局，详情可参见[PyQt5布局管理](http://code.py40.com/1995.html)。

<br/>



### QSS

#### 加载样式表

```python
# 方在initUI里，调用show之前
with open('qss/main_window.qss', 'r') as f:
    self.setStyleSheet(f.read())
```

<br/>

#### QLabel显示图片并缩放

`QLabel(pixmap = QPixmap("logo.png").scaled(155,48))`

<br/>

#### Title背景效果

显示效果：

![title_bg](/resources/PyQt5小结/title.png)

```python
#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import QApplication, QVBoxLayout, QWidget, QLabel
from PyQt5.QtCore import Qt

class MainWindow(QWidget):

    def __init__(self):
        super().__init__()
        #界面绘制交给InitUi方法
        self.initUI()

    def initUI(self):
        v_body = QVBoxLayout()
        lb_title = QLabel("xx系统", self)
        lb_title.setAlignment(Qt.AlignCenter)
        lb_title.setObjectName('title')
        lb_title.setStyleSheet("QLabel {border-image: url(imgs/title_bg.png);padding: 10 0;}")
        v_body.addWidget(lb_title, 0, Qt.AlignTop)
        v_body.setContentsMargins(0,0,0,0)

        self.setGeometry(300, 300, 350, 250)
        self.setWindowTitle('Simple')
        self.setLayout(v_body)
        self.show()


if __name__ == '__main__':
    #创建应用程序和对象
    app = QApplication(sys.argv)
    mw = MainWindow()
    sys.exit(app.exec_())
```

<br/>

#### 圆形图片按钮

素材如下：![round_button](/resources/PyQt5小结/round_button.png)

```
# *.py
bt = QPushButton(self)
bt.setObjectName('round')

/* Stayle */
QPushButton#round {
    width: 46px;            # 取决于素材
    height: 46px;           # 取决于素材
    border-radius: 23px;    # 取决于素材
    background-image: url(round_button.png);
}
```

<br/>

#### 按钮图片切换

```python
# *.py
    self.__switch = True
    self.bt_switch = QPushButton(self)
    self.bt_switch.setStyleSheet("QPushButton {border-image: url(switch_on.png);}")
    self.bt_switch.clicked.connect(self.switch)

def switch(self):
    self.__switch ^= True
    if self.__switch:
        self.bt_switch.setStyleSheet("QPushButton {border-image: url(switch_on.png);}")
    else:
        self.bt_switch.setStyleSheet("QPushButton {border-image: url(switch_off.png);}")

```

<br/>

#### 表格样式

QTableWidget效果：

![Table](/resources/PyQt5小结/table_stretch.png)

*.py：

```
#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QHBoxLayout, \
    QTableWidget, QAbstractItemView, QHeaderView, QPushButton, QLabel
from PyQt5.QtCore import Qt

class MainWindow(QWidget):

    def __init__(self):
        super().__init__()
        #界面绘制交给InitUi方法
        self.initUI()

    def initUI(self):
        v_body = QVBoxLayout()
        v_body.setContentsMargins(0,0,0,10)
        v_body.setSpacing(5)

        head=['序号', '姓名', '性别', '电话']
        self.table = QTableWidget()
        self.table.setColumnCount(4)
        #self.table.setEnabled(False)             # 禁止编辑，无选中效果
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers) # 禁止编辑
        self.table.setHorizontalHeaderLabels(head)
        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.verticalHeader().hide()               # 默认的首列不显示
        self.table.setAlternatingRowColors(True)         # 奇偶行区分颜色
        v_body.addWidget(self.table, 0, Qt.AlignTop)

        layout_bt = QHBoxLayout()
        bt_clear_table = QPushButton("清空", self)
        bt_clear_table.clicked.connect(self.clearTable)
        bt_add_data = QPushButton("添加", self)
        bt_add_data.clicked.connect(self.addData)
        layout_bt.addWidget(bt_clear_table)
        layout_bt.addWidget(bt_add_data)
        v_body.addLayout(layout_bt)

        self.setGeometry(300, 300, 350, 250)
        self.setWindowTitle('Simple')
        self.setLayout(v_body)
        with open('table_test.qss', 'r') as f:
            self.setStyleSheet(f.read())
        self.show()

    def clearTable(self):
        self.table.setRowCount(0)
        self.table.clearContents()

    def addData(self):
        rowIndex = self.table.rowCount()
        self.table.insertRow(rowIndex)
        self.table.setCellWidget(rowIndex, 0, QLabel(
            str(rowIndex)))
        self.table.setCellWidget(rowIndex, 1, QLabel(
            "Test"+str(rowIndex)))


if __name__ == '__main__':
    #创建应用程序和对象
    app = QApplication(sys.argv)
    mw = MainWindow()
    sys.exit(app.exec_())
```

QSS：

```css
/* table_test.qss */
QTableWidget {
    border-style: none;
    border-radius: 10px;
    gridline-color: white;
    background-color: white;
    alternate-background-color: rgb(245,245,245);
}

QHeaderView::section {
    border-style: none;
    color: white;
    font-size: 16px;
    font-family: "Roboto";
    padding: 6px;
    background-color: #4A4A4A;
}

QTableWidget QLabel {
    font-size: 14px;
}

QScrollBar {
    width:20px;
    background:#EBEBEB;
    border:1px solid #EBEBEB;
}

QScrollBar::handle {
    background:#C7C7C7;
}

QScrollBar::add-line, QScrollBar::sub-line {
    border:none;
}

QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {
    border:none;background:#EBEBEB
}
```



QHeaderView.Stretch模式会根据表的宽度均分设置每列的宽度，但实际中每列的长度是有差异的，且如果每列数据的长度基本一致，可以设置列宽模式为固定的，如上表设置成这样会更合理一些：

![Table](/resources/PyQt5小结/table_fixed.png)

```
self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Fixed)
self.table.setColumnWidth(0,50)
self.table.setColumnWidth(1,100)
self.table.setColumnWidth(2,50)
self.table.setColumnWidth(3,150)
```

<br/>

#### 日期样式

样式效果：

![calendar](/resources/PyQt5小结/calendar.png)

```
self.dt_time = QDateTimeEdit(QDate().currentDate())
self.dt_time.setCalendarPopup(True)
#self.dt_time.setMaximumDate(QDate().currentDate())
```

QSS：

```css
QDateTimeEdit {
    font-size: 15pt;
}

/*QCalendarWidget*/
QCalendarWidget {
    font-size: 16px;
    background-color: #fff;
}

QCalendarWidget QToolButton {
    height: 50px;
    color: white;
    font-size: 28px;
    font-weight: 700;
    icon-size: 35px, 35px;
    background-color: #3bafda;
}

QCalendarWidget QMenu {
    font-size: 20px;
}

QCalendarWidget QToolButton:pressed {
    border: transparent;
    background-color: #4fc1e9;
}

QCalendarWidget QToolButton#qt_calendar_prevmonth {
    height: 40px;
    qproperty-icon: url('imgs/arrow_left.png');
    background-color: #3bafda;
}

QCalendarWidget QToolButton#qt_calendar_nextmonth {
    qproperty-icon: url('imgs/arrow_right.png');
    background-color: #3bafda;
}

/*
QCalendarWidget QToolButton#qt_calendar_monthbutton::menu-indicator {
    background-color: #3bafda;
}

QCalendarWidget QToolButton#qt_calendar_monthbutton::menu-indicator:pressed {
    background-color: #4fc1e9;
}
*/

QCalendarWidget QWidget#qt_calendar_calendarview {
    background-color: #FFF;
    selection-color: #FFF;
    selection-background-color: #3bafda;
}

QCalendarWidget QWidget {
    alternate-background-color: #FFFFFF;
}

QCalendarWidget QWidget#qt_calendar_navigationbar {
    background-color: #3bafda;
}

QCalendarWidget QAbstractItemView {
    font-size: 22px;
    outline: none;
    selection-color: #FFF;
    border: 1px solid #ccd1d9;
    border-bottom-left-radius: 4px;
    border-bottom-right-radius: 4px;
}
```

左右箭头素材：![arrow_left](/resources/PyQt5小结/arrow_left.png)![arrow_right](/resources/PyQt5小结/arrow_right.png)

<br/>

### 异步加载并显示数据

需要从服务器拉取数据，并显示。如果和服务器交互需要比较长的时间，就会造成界面卡顿，我们就必须选择异步加载。

如下，是一个异步显示数据的一个例子，点击“加载数据”按钮，会开启一个线程，线程用户可以将获取到的数据发送给界面，由主线程控制显示，这样就不会阻塞主进程了。

```python
#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys, time
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QPushButton, QMessageBox
from PyQt5.QtCore import QThread, pyqtSignal


class MainWindow(QWidget):


    def __init__(self):
        super().__init__()
        #界面绘制交给InitUi方法
        self.initUI()


    def initUI(self):
        self.lb_data = QLabel(self, minimumWidth=200)
        bt_quit = QPushButton("加载数据", self)
        bt_quit.clicked.connect(self.loadData)
        bt_quit.move(0, 50)

        self.setGeometry(300, 300, 350, 250)
        self.setWindowTitle('Simple')
        self.show()


    def loadData(self):
        self.__load_thread = loadThread("test:")
        self.__load_thread._signal.connect(self.syncFresh)
        self.__load_thread.start()


    def syncFresh(self, data):
        self.lb_data.setText(data)



class loadThread(QThread):
    _signal = pyqtSignal(str)

    def __init__(self, query_str):
        super(loadThread, self).__init__()
        self.__query_str = query_str

    def run(self):
        for i in range(100):
            self._signal.emit(self.__query_str + str(i))
            time.sleep(1)



if __name__ == '__main__':
    #创建应用程序和对象
    app = QApplication(sys.argv)
    mw = MainWindow()
    sys.exit(app.exec_())
```

新建了一个QThread类，声明了一个pyqtSignal成员，pyqtSignal的类型根据所要传输数据的类型而定，在主线程中绑定响应函数，最后在loadThread线程中调用emit就发送到主线程了。

<br/>

### 显示动画

Loading动画：

![Loading](/resources/PyQt5小结/loading.gif)

```python
# 加载loading动画
gif_w = 400         # 实际gif的大小
gif_h = 300         # 实际gif的大小
self.gif = QMovie('loading.gif')
self.gif_label = QLabel(self)
self.gif_label.setWindowFlags(Qt.FramelessWindowHint)
self.gif_label.resize(gif_w, gif_h)
self.gif_label.setMovie(self.gif)

# Move to Center Position
pRect = self.geometry()
x = pRect.width() / 2 - gif_w/2
y = pRect.height() / 2 - gif_h/2
self.gif_label.move(x, y)

self.gif.start()
self.gif_label.show()

# 停止loading动画
self.gif.stop()
self.gif_label.close()
```

<br/>

### 自定义MessageBox

原始的QMessageBox样式比较难看，所以使用QDialog封装一个自定义的消息弹窗，自定义显示标题、消息、按钮三个部分，其中标题、消息为字符串，按钮传入QMessageBox.Yes|QMessageBox.No对应显示确定/取消，只传入其中一个则只显示一个按钮。

效果：

![MessageBox](/resources/PyQt5小结/messagebox.png)

使用自定义MessageBox：

```python
#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QMessageBox
from messagebox import MessageBox


class MainWindow(QWidget):


    def __init__(self):
        super().__init__()
        #界面绘制交给InitUi方法
        self.initUI()


    def initUI(self):
        bt_quit = QPushButton("退出", self)
        bt_quit.clicked.connect(self.closeWindow)

        #设置窗口的位置和大小
        self.setGeometry(300, 300, 350, 250)
        #设置窗口的标题
        self.setWindowTitle('Simple')

        #显示窗口
        self.show()


    def closeWindow(self):
        reply = MessageBox(self).showInfo('提示',
            "请确定是否退出程序?", QMessageBox.Yes |
            QMessageBox.No)
        if reply == QMessageBox.Yes:
            self.close()



if __name__ == '__main__':
    #创建应用程序和对象
    app = QApplication(sys.argv)
    mw = MainWindow()
    sys.exit(app.exec_())
```



具体实现：messagebox.py

```python
# messagebox.py
from PyQt5.QtCore import Qt, QPoint
from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import (QDialog, QVBoxLayout, QHBoxLayout,
    QLabel, QPushButton, QMessageBox, QGraphicsDropShadowEffect)

class MessageBox(QDialog):
    __reply = None

    def __init__(self, parent=None):
        super(MessageBox, self).__init__(parent)

        self.setWindowFlags(Qt.FramelessWindowHint)

        # Set Fixed Size
        w = 300
        h = 185
        self.resize(w, h)
        self.setFixedSize(w, h)

        # Move to Center Position
        pRect = parent.geometry()
        x = pRect.width() / 2 - w / 2
        y = pRect.height() / 2 - h / 2

        self.move(x, y)

        # 阴影效果
        shadow_effect = QGraphicsDropShadowEffect()
        shadow_effect.setOffset(10,10)
        shadow_effect.setColor(QColor(180,180,180))
        shadow_effect.setBlurRadius(65)
        self.setGraphicsEffect(shadow_effect)

        with open('messagebox.qss', 'r') as f:
            self.setStyleSheet(f.read())

    def showInfo(self, title, info, buttons):
        self.setWindowTitle(title)
        layout_bt = QHBoxLayout()

        if buttons & QMessageBox.No:
            button_no = QPushButton("取消")
            button_no.setObjectName('m_bt_back')
            button_no.clicked.connect(self.closeWindow)
            layout_bt.addWidget(button_no, 0, Qt.AlignCenter)
        if buttons & QMessageBox.Yes:
            button_yes = QPushButton("确定")
            button_yes.setObjectName('m_bt_ok')
            button_yes.clicked.connect(self.buttonOk)
            layout_bt.addWidget(button_yes, 0, Qt.AlignCenter)

        layout_body = QVBoxLayout()
        lb_title = QLabel(title+':')
        lb_title.setObjectName('m_title')
        layout_body.addWidget(lb_title, 0, Qt.AlignLeft)
        layout_body.addWidget(QLabel(info), 1, Qt.AlignCenter)
        layout_body.addLayout(layout_bt)
        layout_body.setSpacing(15)
        self.setLayout(layout_body)
        # 用于触摸时，可设置鼠标样式为空
        #self.setCursor(Qt.BlankCursor)
        self.exec_()

        return self.__reply

    def buttonOk(self):
        self.__reply = QMessageBox.Yes
        self.close()

    def closeWindow(self):
        self.__reply = QMessageBox.No
        self.close()
```

具体实现：messagebox.qss：

```css
QDialog {
    border-radius: 10px;
    background-color: white;
    background-image: none;
    border: none;
}

QLabel#m_title {
    font: bold 18px;
    padding: 2px;
}

QLabel {
    font: bold 15px;
    text-align: center;
}

QPushButton {
    color: white;
    font: 16px;
    width: 90px;
    height: 35px;
    border-radius: 5px;
    margin-bottom: 10px;
}

QPushButton#m_bt_back {
    background: qlineargradient(x1:0,y1:0,x2:0.5,y2:0.5,stop:0 #9DACB1,stop:0 #919FA5);
}

QPushButton#m_bt_ok {
    background: qlineargradient(x1:0,y1:0,x2:0.5,y2:0.5,stop:0 #44C1FB,stop:0 #5EA6FF);
}
```

<br/>

### 参考链接：

* [PyQt5教程](http://code.py40.com/face)
* [Qt for Python Modules](https://doc.qt.io/qtforpython/api.html)（Qt Widgets中包含所有控件的操作方法和详细说明）
* [Qt Style Sheets Reference](https://doc.qt.io/qt-5/stylesheet-reference.html)
* [Qt Style Sheets Examples](https://doc.qt.io/qt-5/stylesheet-examples.html)
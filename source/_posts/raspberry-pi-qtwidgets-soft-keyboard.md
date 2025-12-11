---
title: 树莓派QtWidgets软键盘
toc: true
categories:
  - tech
tags:
  - 树莓派
  - Qt
date: 2021-02-11 23:56:30
---


![qtvirtualkeyboard.png](/resources/Cover/qtvirtualkeyboard.png)

Qt作为一个桌面程序设计框架，软键盘应该很简单吧，没想到搞了好几天才获得了一个可以用的键盘。所以记录一下。

限定条件比较多，树莓派，Qt Widgets，想要使用软键盘。

把Qt Widgets换成Qt Quick，这个问题可能会比较简单。一开始是想直接用酷炫的Qt Quick的，尝试了几天，妈耶，令人头大。

Qt Quick在C++之外，同时启动了一个QML的engine。在使用的时候不知道是用C++调用QML刷新界面，还是QML调用C++处理数据，因为这两种方式实现起来都十分麻烦。还是用Qt Widgets吧。

<!--more-->

<br/>

### 环境

* 硬件： 树莓派CM4 + CMIO底板
* 系统： 2020-05-27-raspios-buster-armhf
* QT版本： Qt Creator 4.8.2 + Qt 5.11.3

<br/>

### 功能概述

**实现了：**

* QLineEdit获取到焦点自动弹出，失去焦点自动消失
* 软键盘初始化为全局，可以在所有子页面显示，显示位置、样式统一化
* 可以设置键盘大小，位置
* 可以指定支持的语言，其它语言则不会出现在选项中

**待完善：**

* 当窗体设置为FullScreen或者和Screen分辨率相同时，软键盘不会显示
* 软键盘消隐按钮点击后无法联动背景消隐，所以现在禁用了消隐按钮

<br/>

### 关键代码

#### Qt及软键盘插件安装

```
sudo apt-get install -y qt5-default
sudo apt-get install -y qtcreator

sudo apt-get install -y qtvirtualkeyboard-plugin
sudo apt-get install -y qml-module-qtquick-controls2
sudo apt-get install -y qtdeclarative5-dev
sudo apt-get install -y qml-module-qt-labs-folderlistmodel
```

#### 关键代码

##### \*.pro

```
QT +=  quickwidgets
```

##### virtual_keyboard.h

```
#ifndef VIRTUAL_KEYBOARD_H
#define VIRTUAL_KEYBOARD_H

#include <QtQuickWidgets/QQuickWidget>

extern QQuickWidget *v_keyboard;

#endif // VIRTUAL_KEYBOARD_H
```

##### main.cpp

```
#include <QApplication>
#include "main_window.h"
#include "virtual_keyboard.h"

QQuickWidget *v_keyboard = nullptr;

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
//    qputenv("QT_VIRTUALKEYBOARD_STYLE", QByteArray("custom"));    // 指定软键盘样式

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
```

`qputenv`用于设置环境变量，“[Deployment Guide | Qt Virtual Keyboard 5.15.2](https://doc.qt.io/qt-5/qtvirtualkeyboard-deployment-guide.html)”中有软键盘的基本使用和环境变量的说明。

在`main.cpp`中定义了全局的v_keyboard，这样在所有子界面中，只要引用了`virtual_keyboard.h`且经过一次初始化，就可以使用这个软键盘了。

##### main_window.cpp

```
#include "main_window.h"
#include "ui_main_window.h"
#include "virtual_keyboard.h"

#include <QDesktopWidget>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit->installEventFilter(this);

    // 居中
    QDesktopWidget pDesk;
    this->setFixedSize(1024, 768);
    int shift_y = (pDesk.height() - this->height()) / 2;
    this->move((pDesk.width() - this->width()) / 2, shift_y);

    // 虚拟键盘
    v_keyboard = new QQuickWidget();
    v_keyboard->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);
    v_keyboard->setResizeMode(QQuickWidget::SizeRootObjectToView);
    v_keyboard->setSource(QUrl("qrc:/virtual_keyboard.qml"));
    v_keyboard->setFixedSize(760, 264);
    v_keyboard->move((pDesk.width() - v_keyboard->width()) / 2,
                     pDesk.height() - shift_y - v_keyboard->height() - 10);
    v_keyboard->setAttribute(Qt::WA_AcceptTouchEvents);
    v_keyboard->setAttribute(Qt::WA_ShowWithoutActivating);
    v_keyboard->setFocusPolicy(Qt::NoFocus);
    v_keyboard->hide();
}

MainWindow::~MainWindow()
{
    if(v_keyboard)
    {
        v_keyboard->close();
        delete v_keyboard;
    }
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->lineEdit)
    {
        if (event->type()==QEvent::FocusIn)
        {
            v_keyboard->show();
        }
        else if (event->type()==QEvent::FocusOut)
        {
            v_keyboard->hide();
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

```

在`main_window.ui`中添加了一个QLineEdit控件，在MainWindow实现了eventFilter方法，将LineEdit和eventFilter关联，在eventFilter中将FocusIn、FocusOut事件过滤出来，对应设置软键盘的show()和hide()。

初始化软键盘。其实是初始化了一个QQuickWidget，如果指定父类为this的话，软键盘会嵌入到当前页面，但不能全局显示。

然后将这个窗体设置为无外窗体、总在最上层显示、不获取焦点，不设置不获取焦点的话键盘会闪现，因为当QLineEdit获取到焦点的时候，此窗体弹出会马上把焦点抢走，那么就又会被设置为hide。

QQuickWidget::SizeRootObjectToView，根据窗体的大小自动调整根项的大小。

设置内容为`virtual_keyboard.qml`。

然后设置大小，如果不设置Style，那么就是default的键盘样式，想要正好显示键盘，没有多的白边或者裁剪，宽:高约为2.88:1。当然也可以设置自定义的Style，可以通过设置`keyboardDesignWidth`和`keyboardDesignHeight`来调整宽高比。

设置键盘显示位置，当前我设置的是x为居中，y是距窗体底部10的位置。

设置接收触控事件，最后设置为hide()，当QLineEdit获取到焦点之后才会再显示。

##### virtual_keyboard.qml

```
import QtQuick 2.6
import QtQuick.VirtualKeyboard 2.2
import QtQuick.VirtualKeyboard.Settings 2.2

Item {

    InputPanel {
        id: vkb
        visible: true
        y: Qt.inputMethod.visible ? parent.height - vkb.height : parent.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Component.onCompleted: {
            VirtualKeyboardSettings.locale = "en_GB";
            VirtualKeyboardSettings.activeLocales = ["en_GB", "zh_CN"];
        }
    }

}
```

InputPanel用于设置软键盘的样式。目前这儿有个问题还没有解决，正常InputPanel设置visible为active，那么软键盘就会自动根据QLineEdit的焦点而弹出/消失，也能点击软键盘上的隐退按钮而消失，甚至无需绑定eventFilter。现在的情况是，软键盘消失后，底层的QQuickWidget并不会消失，就有一块白背景显示在那儿。所以现在将visible设置一直为true；`anchors.bottom: parent.bottom`就会使软键盘上的隐退按键无效。

VirtualKeyboardSettings，locale指定当前的输入语言，activeLocales指定可选的语言。

<br/>

#### 还是没有中文

Qt 5.11.3明明是有中文的呀，为什么一直不显示？是因为`apt-get install`的这个版本中，没有配置编译中文。

所有需要手动编译，重新安装一下：

```shell
wget https://download.qt.io/new_archive/qt/5.11/5.11.3/single/qt-everywhere-src-5.11.3.tar.xz
xz -d qt-everywhere-src-5.11.3.tar.xz
tar xvf qt-everywhere-src-5.11.3.tar

cd qt-everywhere-src-5.11.3/qtvirtualkeyboard
qmake CONFIG+="lang-en_GB lang-zh_CN"
make
sudo make install
```

<br/>

### 整个工程

[VirtualKeyboardAtRPi.tar.gz](/resources/VirtualKeyboardAtRPi/VirtualKeyboardAtRPi.tar.gz)

<br/>

### 后记

一开始因为没有安装`qml-module-qt-labs-folderlistmodel`这个模块，软键盘一打开就是全屏的黑屏，导致走了2-3天的弯路，去尝试了使用外置键盘或者自己实现Widget键盘。

#### 使用外置键盘

`matchbox-keyboard 0.1`

首先安装验证了matchbox-keyboard在树莓派上的效果，效果和输入中文都没有问题。剩下的思路还是在eventFilter中，在FocusIn的时候把matchbox-keyboard启动起来，在FocusOut的时候，设置消失。

使用QProcess可以以程序名+启动参数的方式启动外部程序。

matchbox-keyboard启动之后，可以使用`wmctr`来设置保持显示在最上层、Resize、移动窗体。但这个方式不怎么稳定，即使可以，最大、最小化按钮不能设置为不显示，而点击了这个最大化之后，不知道为什么一直在抖。

#### 自实现键盘

[VkIm](https://gitee.com/tracing/VkIm.git)，这是最接近qtvirtualkeyboard的实现方法，注册一个实现了InputService的Dialog来显示键盘按钮，通过QDBusConnection发送给QT。

找到的其它项目都是假的实现，基本上是将点击的按钮的字符复制到输入的QLineEdit，这样就输入不了中文了。

<br/>

### 参考

* [Deployment Guide | Qt Virtual Keyboard 5.15.2](https://doc.qt.io/qt-5/qtvirtualkeyboard-deployment-guide.html)
* [VkIm](https://gitee.com/tracing/VkIm.git)
* [c++ - QtVirtualKeyboard focus problem when editing a cell in a QTableView - Stack Overflow](https://stackoverflow.com/questions/64428819/qtvirtualkeyboard-focus-problem-when-editing-a-cell-in-a-qtableview)
---
title: 关于QT的几个疑问
date: 2021-05-01 17:26:06
toc: true
categories:
  - Technology
tags:
  - QT
---

* [new的控件需要delete吗？](/Technology/关于QT的几个疑问#new的控件需要delete吗？)

* [发送窗体close后信号槽还需要disconnect吗？](/Technology/关于QT的几个疑问#发送窗体close后信号槽还需要disconnect吗？)

* [connect时使用SIGNAL(clicked())还是&QPushButton::clicked？](/Technology/关于QT的几个疑问/#connect%E6%97%B6%E4%BD%BF%E7%94%A8SIGNAL-clicked-%E8%BF%98%E6%98%AF-amp-QPushButton-clicked%EF%BC%9F)

<!--more-->

<br/>

### 软件环境

* Qt 5.14.1

<br/>

### new的控件需要delete吗？

1、继承了QObject的子类（所有控件都继承QObject），并且指定了父类，不需要手动delete，此时当父类执行析构函数时由父类销毁。
2、通过addLayout添加到布局控件中的布局或者控件不需要手动delete，addLayout会默认将要加入的控件作为父类。
3、通过setLayout添加到控件中的，如果setLayout的对象没有添加到其他布局直接显示、或者没有指定父类的，这个需要手动delete。

<br/>

### 发送窗体close后信号槽还需要disconnect吗？

如果只想要信号调用一次，可以在信号抵达后调用disconnect。

但是，两个窗体之间连接的信号槽，好像在发送窗口close之后，这个信号连接就自动解绑了？

这个问题归根到底还是对于信号槽机制了解的不够彻底。在这里再次梳理一下QT的信号槽机制：

1. 信号槽机制依赖于Qt的元对象系统（Meta-Object System），使用对象必须继承QObject，并在类的顶行声明Q_OBJECT宏；
2. 信号槽机制提供了connect和disconnect函数，来实现信号槽的连接和断开；
3. 一个信号槽连接，一般需要指定以下几个参数：发送者、信号函数、接收者、槽函数、连接方式；
4. 信号函数和槽函数对应位置的参数要匹配（类型一致或可以转化），信号的参数数量必须大于等于槽函数的参数数量，多余的参数则被忽略；
5. 信号槽机制不仅允许信号和槽的绑定，还允许信号和信号绑定；
6. 一个信号可以连接多个槽或者信号，多个信号可以连接一个槽；
7. 如果一个信号连接了多个，信号依据绑定的先后顺序发送给接收者；
8. 当发送者或接收者被销毁时，连接将自动断开；
9. 信号函数由MOC自动生成，前面不可加public、private和protected进行修饰，不要在`.cpp`中实现，且返回值只能是`void`；
10. 槽函数是普通的c++函数，可以正常调用，它们唯一的特点是信号可以与之相连。而使用信号连接的时候，可以连接到私有槽函数而无视其访问级别；
11. 连接方式有：`Qt::AutoConnection`、`Qt::DirectConnection`、`Qt::QueuedConnection`、`Qt::BlockingQueuedConnection`、`Qt::UniqueConnection`；
12. 连接方式如不指定，则默认是`Qt::AutoConnection`类型，此时实际的连接类型是在信号发送时决定的，如果槽函数和信号在同一线程，则使用`Qt::DirectConnection`，否则使用`Qt::QueuedConnection`；
13. `Qt::DirectConnection`：信号发送后函数立即执行，槽函数和信号在同一个线程；
14. `Qt::QueuedConnection`：信号发送之后，当控制权回到接收者线程的时间循环时，调用槽函数；
15. `Qt::BlockingQueuedConnection`：和`Qt::QueuedConnection`相似，只是信号线程会阻塞直到槽函数执行完毕。当信号和槽函数在同一线程时会造成死锁。
16. `Qt::UniqueConnection`：这是一个标志，可以使用位或（`|`）与上述任何一种连接类型组合。设置`Qt::UniqueConnection`时，如果连接已经存在（即，同一信号已连接到同一对对象的同一槽函数），则`connect`将失败。`Qt::UniqueConnection`不适用于lambda、非成员函数，只适用于连接到成员函数。

那么，根据第8条，回答是不需要disconnect。

<br/>

### connect时使用SIGNAL(clicked())还是&QPushButton::clicked？

这个涉及到connect的重载函数，QT提供了多个connect的重载函数，使用不同传参的时候实际上是调用的不同的函数。

我们来一起看看connect的这些重载函数，学习一下它们的使用方法。

<br/>

**函数定义1：**

```c++
static QMetaObject::Connection QObject::connect(const QObject *sender, const char *signal, const QObject *receiver, const char *method, Qt::ConnectionType type = Qt::AutoConnection)
```

使用时，用`SIGNAL()`和`SLOT()`宏，将函数转为`char *`。此方法不适用于lamda表达式。

使用示例：

```c++
QLineEdit *line_edit = new QLineEdit(this);
QLabel *label = new QLabel(this);
label->move(0,40);

QObject::connect(line_edit, SIGNAL(textChanged(QString)), label, SLOT(setText(QString)));
```

<br/>

**函数定义2：**

```c++
QMetaObject::Connection QObject::connect(const QObject *sender, const char *signal, const char *method, Qt::ConnectionType type = Qt::AutoConnection) const
```

此定义非`static`函数，是一个成员函数，缺省了`receiver`为`this`。

使用示例：

```c++
label->connect(line_edit, SIGNAL(textChanged(QString)), SLOT(setText(QString)));
```

<br/>

**函数定义3：**

```c++
static QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, const QObject *receiver, PointerToMemberFunction method, Qt::ConnectionType type = Qt::AutoConnection)
```

`PointerToMemberFunction`是一种指向成员函数的指针。如果成员函数是一个非虚函数，就可以使用`&Object::Function`得到一个地址，其值是该成员函数在class中的相对位置，它是一个不完整的值，需要绑定到某个对象的地址才可以访问。

使用这种方法，无法指定有重载函数的信号/槽函数。

使用示例：

```c++
QObject::connect(line_edit, &QLineEdit::textChanged, label, &QLabel::setText);
```

<br/>

**函数定义4：**

```c++
static QMetaObject::Connection QObject::connect(const QObject *sender, const QMetaMethod &signal, const QObject *receiver, const QMetaMethod &method, Qt::ConnectionType type = Qt::AutoConnection)
```

此函数的工作方式与定义1相同，但它使用QMetaMethod指定信号和方法。

对于信号，可以使用QMetaObject的静态方法fromSignal()来获得相对于该信号的QMetaMethod对象；

对于普通成员函数和槽函数，可以使用类的QMetaObject对象来间接获取。

使用示例：

```c++
QLabel *label = new QLabel;
QScrollBar *scrollBar = new QScrollBar;
QMetaMethod signal = label->
label->connect(scrollBar, SIGNAL(valueChanged(int)),  SLOT(setNum(int)));
```

<br/>

**函数定义5：**

```c++
static QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, Functor functor)
```

连接发送者的信号和触发函数，没有接收者。此处的槽函数可以是一个Lambda表达式。

使用示例：

```c++
QObject::connect(line_edit, &QLineEdit::textChanged, [=](QString text){label->setText(text);});
```

<br/>

**函数定义6：**

```c++
static QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, const QObject *context, Functor functor, Qt::ConnectionType type = Qt::AutoConnection)
```

将sender对象中的signal连接到放置在上下文的特定事件循环中的functor。

使用示例：

```c++
QObject::connect(line_edit, &QLineEdit::textChanged, this, [=](QString text){label->setText(text);});
```

<br/>

**[整体示例](/resources/qt_widgets_ui_advanced/ConnectTest.tar.gz)：**

```c++
// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QMetaMethod>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLineEdit *line_edit = new QLineEdit(this);
    QLabel *label = new QLabel(this);
    label->move(0,40);

    // connect 1
    QObject::connect(line_edit, SIGNAL(textChanged(QString)), label, SLOT(setText(QString)));

    // connect 2
//    label->connect(line_edit, SIGNAL(textChanged(QString)), SLOT(setText(QString)));

    // connect 3
//    QObject::connect(line_edit, &QLineEdit::textChanged, label, &QLabel::setText);

    // connect 4
//    QMetaMethod signal = QMetaMethod::fromSignal(&QLineEdit::textChanged);
//    int slot_index = label->metaObject()->indexOfMethod("setText(QString)");
//    QMetaMethod slot = label->metaObject()->method( slot_index );
//    QObject::connect(line_edit, signal, label, slot);

    // connect 5
//    QObject::connect(line_edit, &QLineEdit::textChanged, [=](QString text){label->setText(text);});

    // connect 6
//    QObject::connect(line_edit, &QLineEdit::textChanged, this, [=](QString text){label->setText(text);});
}

MainWindow::~MainWindow()
{
    delete ui;
}
```


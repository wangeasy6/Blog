---
title: Qt - 信号槽机制
date: 2024-08-03 17:00:00
toc: true
categories:
  - tech
tags:
  - Qt
---

<img src="/resources/Cover/qt_signal_and_slot.png" width=480 height=240 />

Qt 信号槽是 Qt 元对象系统（The Meta-Object System）提供的主要功能之一，属于 Qt Core 模块，在 Qt 中无处不在，是整个 Qt 软件的基础。

本文所总结内容大多来源于官方文档，参考的官方文档链接放在开头，参考的博客或者书籍链接放在最后。

<!--more-->

<br/>

## 官方参考链接

* [Qt Documentation | Home](https://doc.qt.io/)
* [Qt Core | Qt 6](https://doc.qt.io/qt-6/qtcore-index.html)
* [The Meta-Object System | Qt Core 6](https://doc.qt.io/qt-6/metaobjects.html)
* [Signals & Slots | Qt Core 6](https://doc.qt.io/qt-6/signalsandslots.html)
* [QMetaObject Struct | Qt Core 6](https://doc.qt.io/qt-6/qmetaobject.html)
* [QObject Class | Qt Core 6](https://doc.qt.io/qt-6/qobject.html)
* [Using the Meta-Object Compiler (moc) | Qt 6](https://doc.qt.io/qt-6/moc.html)
* [Why Does Qt Use Moc for Signals and Slots? | Qt 6](https://doc.qt.io/qt-6/why-moc.html)

<br/>

## 信号槽机制

1. 信号槽机制依赖于 Qt 的元对象系统（Meta-Object System），使用对象必须继承 `QObject`，并在类的顶行声明 `Q_OBJECT` 宏；
2. 信号槽机制提供了 `connect`、`disconnect` 函数，来实现信号槽的连接和断开；
3. 一个信号槽连接，一般需要指定以下几个参数：发送者、信号函数、接收者、槽函数、连接方式；
4. 信号函数和槽函数对应位置的参数要匹配（类型一致或可以转化），信号的参数数量必须大于等于槽函数的参数数量，多余的参数则被忽略；
5. 一个信号可以连接多个槽或者信号，多个信号可以连接一个槽；
6. 信号槽机制不仅允许信号和槽的绑定，还允许信号和信号绑定；
7. 如果一个信号连接了多个，信号依据绑定的先后顺序发送给接收者；
8. 当发送者或接收者被销毁时，连接将自动断开；
9. 信号函数由 moc 自动生成，前面不可加 `public`、`private` 和 `protected` 进行修饰，不要在 `.cpp` 中实现，且返回值只能是`void`；
10. 槽函数是普通的 c++ 函数，可以正常调用，它们唯一的特点是信号可以与之相连。而使用信号连接的时候，可以连接到私有槽函数而无视其访问级别；
11. 通过 `QObject* sender() const` 函数可以获取发送信号的对象。

<br/>

## 使用示例

```c++
#include <QObject>

class Counter : public QObject
{
    Q_OBJECT

// Note. The Q_OBJECT macro starts a private section.
// To declare public members, use the 'public:' access modifier.
public:
    Counter() { m_value = 0; }

    int value() const { return m_value; }

public slots:
    void setValue(int value);

signals:
    void valueChanged(int newValue);

private:
    int m_value;
};
```

<br/>

## 信号槽的几种连接函数

一个信号槽连接，一般需要指定以下几个参数：发送者、信号函数、接收者、槽函数、连接方式。

为了对不同的传参提供了使用便利，Qt 提供了多个 connect 重载函数。

<br/>

**函数定义1：**

使用 `SIGNAL()`和`SLOT()`宏，将函数转为`char *`。此方法不适用于 lamda 表达式。

```c++
static QMetaObject::Connection QObject::connect(const QObject *sender, const char *signal, const QObject *receiver, const char *method, Qt::ConnectionType type = Qt::AutoConnection)
```

使用示例：

```c++
QLineEdit *line_edit = new QLineEdit(this);
QLabel *label = new QLabel(this);
label->move(0,40);

QObject::connect(line_edit, SIGNAL(textChanged(QString)), label, SLOT(setText(QString)));
```

<br/>

**函数定义2：**

此定义非 `static` 函数，是一个成员函数，缺省 `receiver` 为 `this`。

```c++
QMetaObject::Connection QObject::connect(const QObject *sender, const char *signal, const char *method, Qt::ConnectionType type = Qt::AutoConnection) const
```

使用示例：

```c++
label->connect(line_edit, SIGNAL(textChanged(QString)), SLOT(setText(QString)));
```

<br/>

**函数定义3：**

对于信号和槽函数，使用相对地址（`&Object::Function`）表示，可以不提供参数列表。使用这种方法，无法指定有重载函数的信号/槽函数。

```c++
static QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, const QObject *receiver, PointerToMemberFunction method, Qt::ConnectionType type = Qt::AutoConnection)
```

`PointerToMemberFunction`是一种指向成员函数的指针。如果成员函数是一个非虚函数，就可以使用`&Object::Function`得到一个地址，其值是该成员函数在class中的相对位置，它是一个不完整的值，需要绑定到某个对象的地址才可以访问。

使用示例：

```c++
QObject::connect(line_edit, &QLineEdit::textChanged, label, &QLabel::setText);
```

<br/>

**函数定义4：**

此函数的工作方式与定义1相同，但它使用 QMetaMethod 指定信号和槽函数。

```c++
static QMetaObject::Connection QObject::connect(const QObject *sender, const QMetaMethod &signal, const QObject *receiver, const QMetaMethod &method, Qt::ConnectionType type = Qt::AutoConnection)
```

对于信号，可以使用 QMetaObject的 静态方法 fromSignal() 来获得相对于该信号的QMetaMethod对象；

对于普通成员函数和槽函数，可以使用类的 QMetaObject 对象来间接获取。

使用示例：

```c++
QLabel *label = new QLabel;
QScrollBar *scrollBar = new QScrollBar;
QMetaMethod signal = label->
label->connect(scrollBar, SIGNAL(valueChanged(int)),  SLOT(setNum(int)));
```

<br/>

**函数定义5：**

连接发送者的信号和触发函数，没有接收者。此处的槽函数可以是一个 Lambda 表达式。

```c++
static QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, Functor functor)
```

使用示例：

```c++
QObject::connect(line_edit, &QLineEdit::textChanged, [=](QString text){label->setText(text);});
```

<br/>

**函数定义6：**

将信号连接到放置在上下文的特定事件循环中的 functor。

```c++
static QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, const QObject *context, Functor functor, Qt::ConnectionType type = Qt::AutoConnection)
```

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

<br/>

## 信号槽的几种连接类型

[Qt::ConnectionType]([Qt Namespace | Qt Core 6.7.2](https://doc.qt.io/qt-6/qt.html#ConnectionType-enum)) 是一个枚举变量，描述了信号和槽之间可以使用的连接类型。特别是，它确定了特定信号是立即发送到槽还是排队在稍后的时间发送。

连接类型有：

| enum                           | 价值    | 描述                                                         |
| ------------------------------ | ------- | ------------------------------------------------------------ |
| `Qt::AutoConnection`           | `0`     | **（默认）**如果接收者在发出信号的线程，则使用`Qt::DirectConnection`。否则，将使用 `Qt::QueuedConnection`。连接类型是在发出信号时确定的。 |
| `Qt::DirectConnection`         | `1`     | 当信号发出时，槽函数会立即被调用。槽函数在信号线程中执行。所以 emit 语句后的代码将在槽函数执行完毕后被执行。（同步调用） |
| `Qt::QueuedConnection`         | `2`     | 当控制权返回到接收器线程的事件循环时，将调用槽函数。槽函数在接收器的线程中执行。所以 emit 语句后的代码将在发出信号后立即被执行，无需等待槽函数执行完毕。（异步调用） |
| `Qt::BlockingQueuedConnection` | `3`     | 与`Qt::QueuedConnection`相同，不同之处在于信号线程会阻塞，直到槽函数返回，也叫完全同步队列，用于在不同的线程之间执行同步调用。如果接收器位于信号线程中，则**不得**使用此连接，否则应用程序将死锁。 |
| `Qt::UniqueConnection`         | `0x80`  | 这是一个标志，可以使用按位或，和上述任何一种连接类型结合使用。当设置了`Qt::QueuedConnection` 时，如果连接已经存在，connect() 将失败（即，如果同一信号已经连接到同一对对象的同一插槽）。`Qt::UniqueConnection` 不适用于 lambda、非成员函数，只适用于连接到成员函数。 |
| `Qt::SingleShotConnection`     | `0x100` | 这是一个标志，可以使用按位或，和上述任何一种连接类型结合使用。当设置了此标志，插槽将只被调用一次，信号发出后，连接将自动断开。这个标志是在Qt 6.0中引入的。 |

对于 `Qt::QueuedConnection` 连接，参数必须是 Qt 元对象系统已知的类型，因为 Qt 需要复制参数以将它们存储在事件中。如果使用排队连接并收到错误消息：

```
QObject::connect: Cannot queue arguments of type 'MyType'
```

需要在建立连接之前调用 [qRegisterMetaType](https://doc.qt.io/qt-6/qmetatype.html#qRegisterMetaType-2)() 来注册数据类型。

<br/>

## Qt信号和槽的本质是什么？

信号和槽的本质可以分为以下两个方面：

* 单线程的信号与槽实现原理主要由观察者模式与函数指针的联合使用，槽函数会订阅自己感兴趣的信号，当信号触发时，会通过一个队列获取到函数参数，完成功能的实现。这样实现便可以做到一个信号可以连接多个槽，多个信号也可以连接一个槽。
* 多线程的信号与槽实现原理本质上是事件机制，信号发送时会触发一个事件，发送到槽函数所在线程的事件队列中，槽函数会排队执行对应的事件函数，完成信号与槽的调用。

<br/>

## 信号槽机制的优势和不足？

### 优点：

- **类型安全**：编译器会对关联的信号槽的参数类型和数量进行检查，若不一致，则编译器会报错。
- **松耦合**：信号和槽机制减弱了 Qt 对象的耦合度，提高代码的复用性和可维护性。激发信号的Qt对象无需知道是哪个对象的那个槽接收它发出的信号，它只需要在适当的时间发送适当的信号即可，而不需要关心是否被接收和哪个对象接收了。Qt 保证适当的槽得到了调用，即使关联的对象在运行时删除，程序也不会崩溃。
- **灵活性**：一个信号可以关联多个槽，多个信号也可以关联同一个槽。可以动态的连接和断开信号和槽，可以在运行时动态修改信号和槽的参数等。
- **易于扩展**：信号槽机制可以非常容易地扩展新的事件和处理逻辑，只需要定义新的信号和槽即可，无需修改原有代码。
- **跨线程**：信号槽机制可以支持跨线程的事件处理，可以将信号和槽连接在不同的线程中，这样可以实现线程之间的通信。

### 缺点：

* **性能较低**：与回调函数相比，信号和槽机制运行速度比直接调用非虚函数慢10倍左右。具体解释见[调用性能不代表一切](\)。

  原因：

  - 需要定位接收信号的对象。

  - 安全地遍历所有槽。

  - 编组，解组传递参数。

  - 多线程的时候，信号需要排队等候。

* **调试难度加大**：由于信号槽机制是基于事件驱动的编程模型，因此调试起来可能会比较困难，特别是在信号和槽之间存在多层嵌套的情况下。

* **安全性降低**：由于信号槽机制可以动态连接和断开信号和槽，因此在使用时需要注意安全性问题，避免出现槽函数被误调用的情况。

<br/>

## 为什么使用 Moc 来处理信号和槽？

模板是 C++ 中的内置机制，允许编译器根据传递的参数类型动态生成代码。因此，模板对于框架创建者来说非常有趣，在 Qt 的许多地方确实使用了高级模板。但是，也有局限性：有些东西可以用模板轻松表达，有些东西则无法用模板表达。即使对指针类型进行了部分特化，通用向量容器类也很容易表达，而基于以字符串形式给出的 XML 描述来设置图形用户界面的函数则不能用模板来表达。然后中间有一个灰色区域。你可以用模板来解决的东西，但代价是代码大小、可读性、可移植性、可用性、可扩展性、稳健性和最终的设计美感。模板和 C 预处理器都可以扩展来完成令人难以置信的智能和令人难以置信的事情。但仅仅因为这些事情可以完成，并不一定意味着这样做就是正确的设计选择。不幸的是，代码并不是要在书中发布，而是要在现实世界的操作系统上使用现实世界的编译器进行编译。

以下是 Qt 使用 moc 的一些原因：

### 语法优势

语法不仅仅是糖：我们用来表达算法的语法可以显着影响代码的可读性和可维护性。 Qt 信号和槽使用的语法在实践中已被证明非常成功。语法直观、使用简单且易于阅读。学习 Qt 的人发现语法可以帮助他们理解和利用信号和槽的概念——尽管它具有高度抽象和通用的性质。这有助于程序员从一开始就正确设计，甚至无需考虑设计模式。

Qt 的 moc（元对象编译器）提供了一种超越编译语言功能的干净方法。它通过生成可由任何标准 C++ 编译器编译的附加 C++ 代码来实现这一点。 moc 读取 C++ 源文件。如果它找到一个或多个包含 Q_OBJECT 宏的类声明，它会生成另一个 C++ 源文件，其中包含这些类的元对象代码。 moc 生成的 C++ 源文件必须与类的实现进行编译和链接（或者可以 `#included` 到类的源文件中）。通常，moc 不是手动调用的，而是由构建系统自动调用的，因此不需要程序员进行额外的工作。

moc 并不是 Qt 使用的唯一代码生成器。另一个突出的例子是 uic（用户界面编译器）。它采用 XML 格式的用户界面描述并创建用于设置样式的 C++ 代码。在 Qt 之外，代码生成器也很常见。以 rpc（Remote Procedure Call，远程过程调用） 和 idl（Interactive Data Language，交互式数据语言） 为例，它们使程序或对象能够跨进程或机器边界进行通信。或者是各种各样的扫描器和解析器生成器，其中 lex 和 yacc 是最著名的。它们将语法规范作为输入并生成实现状态机的代码。代码生成器的替代品是经过魔改的编译器，专有语言或具有单向对话框或向导的图形编程工具，这些工具在设计时而不是编译时生成晦涩的代码。我们没有将我们的客户锁定在专有的 C++ 编译器或特定的集成开发环境中，而是让他们能够使用他们喜欢的任何工具。我们不强迫程序员将生成的代码添加到源码仓库中，而是鼓励他们将我们的工具添加到他们的构建系统中：更干净、更安全并且更符合 UNIX 的精神。

### GUI 是动态的

C++ 是一种标准化、强大且精心设计的通用语言。它是唯一一种在如此广泛的软件项目中使用的语言，涵盖从整个操作系统、数据库服务器和高端图形应用程序到常见桌面应用程序的各种应用程序。 C++ 成功的关键之一是其可扩展的语言设计，该设计专注于最大性能和最小内存消耗，同时仍保持 ANSI C 兼容性。

尽管有所有这些优点，但也存在一些缺点。对于 C++ 来说，当涉及到基于组件的图形用户界面编程时，静态对象模型相对于 Objective C 的动态消息传递方法来说是一个明显的劣势。对高端数据库服务器或操作系统有利的东西不一定对 GUI 前端是正确的设计选择。借助 moc，我们将这一劣势转化为优势，并增加了应对安全高效的图形用户界面编程挑战所需的灵活性。

我们的方法远远超出了您使用模板所能做的任何事情。例如，我们可以拥有对象属性。我们可以拥有重载的信号和槽，这在使用重载是关键概念的语言进行编程时感觉很自然。我们的信号向类实例的大小添加零字节，这意味着我们可以在不破坏二进制兼容性的情况下添加新信号。

另一个好处是我们可以在运行时探索对象的信号和槽。我们可以使用类型安全的名称调用来建立连接，而不必知道我们正在连接的对象的确切类型。对于基于模板的解决方案来说这是不可能的。这种运行时自省开辟了新的可能性，例如从 Qt Designer 的 XML UI 文件生成和连接的 GUI。

### 调用性能不代表一切

Qt 的信号和槽实现不如基于模板的解决方案快。虽然发出一个信号的成本大约是四个使用通用模板实现的普通函数调用的成本，Qt 所需的工作量相当于大约十个函数调用。这并不奇怪，因为 Qt 机制包括通用编组器、内省、不同线程之间的排队调用以及最终的脚本能力。它不依赖于过多的内联和代码扩展，并且提供了无与伦比的运行时安全性。 Qt 的迭代器是安全的，而更快的基于模板的系统的迭代器则不然。即使在向多个接收器发出信号的过程中，也可以安全地删除这些接收器，而不会导致程序崩溃。如果没有这种安全性，应用程序最终会崩溃，并出现难以调试的释放内存读取或写入错误。

尽管如此，基于模板的解决方案难道不能提高使用信号和槽的应用程序的性能吗？虽然 Qt 确实为通过信号调用时隙的成本增加了少量开销，但调用的成本仅占时隙整个成本的一小部分。针对 Qt 信号和槽系统的基准测试通常是使用空槽来完成的。一旦您在槽中执行任何有用的操作（例如一些简单的字符串操作），调用开销就可以忽略不计。Qt 的系统经过如此优化，以至于任何需要运算符 new 或 delete 的操作（例如，字符串操作或从模板容器中插入/删除某些内容）都比发出信号要昂贵得多。

注意：如果性能关键任务的紧密内部循环中有信号和槽连接，并且您将该连接确定为瓶颈，请考虑使用标准侦听器接口模式而不是信号和槽。如果是这种情况，你可能只需要 1:1 连接。例如，如果有一个从网络下载数据的对象，那么使用信号来指示请求的数据已到达是一个非常明智的设计。但是，如果需要将每个字节一一发送给消费者，请使用侦听器接口而不是信号和槽。

### 无限可能

因为我们有信号和槽的 moc，所以我们可以向其中添加其他用模板无法完成的有用的东西。其中包括通过生成的 tr() 函数进行范围转换，以及具有内省和扩展运行时类型信息的高级属性系统。属性系统本身就是一个很大的优势：如果没有强大且内省的属性系统，像 Qt Designer 这样强大且通用的用户界面设计工具将很难编写。但事情并没有就此结束。Qt 还提供了动态 qobject_cast<T>() 机制，该机制不依赖于系统的 RTTI，因此不具有其局限性。Qt 使用它来安全地从动态加载的组件中查询接口。另一个应用领域是动态元对象。Qt 可以使用例如 ActiveX 组件并在运行时围绕它创建一个元对象。或者可以通过导出其元对象将 Qt 组件导出为 ActiveX 组件。使用模板将无法执行这些操作。

带有 moc 的 C++ 实质上为我们提供了 Objective-C 或 Java 运行时环境的灵活性，同时保持了 C++ 独特的性能和可扩展性优势。这使得 Qt 成为我们灵活而舒适的工具。

<br/>

## 发送的信号一直未被执行有哪些可能性？

1. **没有连接到槽**：信号必须连接到至少一个槽函数，否则即使发出信号，也不会有任何动作。包括了以下情况：
   1. 没有 connect
   2. connect 失败，参数不匹配会造成连接失败
   3. 连接了但调用了 disconnect，已断开连接
   4. 需要多次使用的连接，但是使用了 `Qt::SingleShotConnection` 标志，调用1次后，连接已断开
2. **连接的槽函数未实现**：如果槽函数声明了但没有实现，信号发出时，槽函数将不会执行任何操作。
3. **对象已经被销毁**：如果发出信号的对象已经被销毁，那么信号将不会被执行。
4. **信号的发送被阻止**：可以通过`blockSignals(true)`方法阻止对象发出信号，直到`blockSignals(false)`被调用。
5. **使用了`static_cast`或`qobject_cast`进行错误的类型转换**：在使用多态的情况下，如果信号连接到了错误的派生类，而实际发出信号的对象是另一个派生类，那么信号将不会被执行。
6. **使用了`QMetaObject::invokeMethod`进行动态调用**：如果使用`QMetaObject::invokeMethod`进行动态调用，并且调用失败，那么槽函数也不会被执行。
7. **使用了`QCoreApplication::sendEvent`手动发送事件**：如果信号是通过`QCoreApplication::sendEvent`手动发送的事件，并且事件被忽略或被某个事件过滤器拦截，那么信号可能不会被执行。
8. **线程问题**：如果信号和槽位于不同的线程，而没有正确地使用`QThread`和`moveToThread`进行线程管理，那么信号可能不会被槽接收。
9. **程序死锁**：多线程造成了死锁。

<br/>

## 其它参考

* [C++ Qt常用面试题整理（不定时更新）_qt面试题-CSDN博客](https://blog.csdn.net/qq_33462307/article/details/108998579)
* [Qt 常用面试题整理(不定时整理)_牛客网 (nowcoder.com)](https://www.nowcoder.com/discuss/455307982692196352)
* [Qt开发常见面试题【备战春招秋招】_qt面试题-CSDN博客](https://blog.csdn.net/g300800/article/details/136580287)
* [0voice/qt_interview_reference: 2023年最新整理，qt开发最全面试集锦，含网络，文件系统，数据库，自定义控件，以及视频讲解，文档 (github.com)](https://github.com/0voice/qt_interview_reference)
* [Qt开发常见面试题【备战春招秋招】_qt面试题-CSDN博客](https://blog.csdn.net/g300800/article/details/136580287)

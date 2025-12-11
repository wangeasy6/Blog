---
title: QtQuick QML&C++混合编程
date: 2024-06-22 00:05:00
toc: true
categories:
  - tech
tags:
  - Qt
---

![cpp_integration_demo.gif](\resources\QtQuick\cpp_integration_demo.gif)

QtQuick QML 和 C++ 混合编程，总结了以下 6 种方式：

- 在QML中读写C++类的属性（MEMBER）

- 在QML中读写C++类的属性（READ/WRITE）

- 在QML中使用C++对象

- 在QML中通过信号槽操纵C++类

- 在C++中连接信号槽操纵QML界面

- 在C++中继承QQuickItem实现QML界面元素

试验环境：

* Qt Creator 13.0.1(Windows)
* Qt6 6.2

<!--more-->

<br/>

# 传统方法：使用信号槽

## 在C++中通过信号槽操纵QML界面

在 QtWidgets 开发中，我们使用 `.ui` 和 `.qss` 来描绘界面元素，使用 C++ 获取控件，在 C++ 中通过信号槽绑定控件编写交互逻辑和运算逻辑。

在 QtQuick 中，我们仍然能使用这种方式，但由于 QML 不仅能描述界面，而且也提供了绑定和部分计算的功能，这种开发方式在 QtQuick 中并不推荐，一般作为辅助开发方式。

首先我们来看**界面元素**：

```js
// .qml
Window {
    objectName: "rootObject";
    ColumnLayout {
        spacing: 10
        Label {
            text: "在C++中通过\r\n信号槽操纵QML界面"
        }
        Rectangle {
            objectName: "ccd2_ret"
            color: "black"
            width: 50
            height: 50
        }
        Button {
            objectName: "ccd2_bt"
            text: qsTr("Next Color")
            checkable: false
            font.pointSize: 10
            scale : 0.6
        }
    }
}
```

这里的关键点是设置 objectName，后面会通过这个在 C++ 中找到这个界面元素。

**类定义：**

```cpp
// my_class.h
class ColorConn : public QObject
{
    Q_OBJECT

public:
    enum Colors{
        Gray = 0,
        Red,
        Green,
        Blue,
        Max
    };
    explicit ColorConn(QObject *parent = nullptr);
    QColor color(void) const {return _color;};

public slots:
    void nextColor();

signals:
    void colorChanged(const QColor &_color);

private:
    Colors _current_index = Gray;
    QList<QColor> default_color = {QColor(0xDD, 0xDD, 0xDD),
                                   QColor(0xD3, 0x00, 0x00),
                                   QColor(0x00, 0xD3, 0x00),
                                   QColor(0x00, 0x00, 0xD3)};
    QColor _color = QColor(0xDD, 0xDD, 0xDD);
};
```

声明了一个当颜色变化的信号函数和一个改变颜色的槽函数。

**函数实现：**

```cpp
// my_class.cpp
void ColorConn::nextColor()
{
    _current_index = static_cast<Colors>(_current_index + 1);

    if (_current_index == Max)
    {
        _current_index = Gray;
    }

    _color = default_color[_current_index];
    emit colorChanged(_color);
}

ColorConn::ColorConn(QObject *parent)
    : QObject{parent}
{}
```

nextColor 在默认的 4 个颜色（灰、红、绿、蓝）中顺序切换。

**在主函数中绑定：**

```cpp
// main.cpp
    QObject * root = NULL;
    QList<QObject*> rootObjects = engine.rootObjects();
    int count = rootObjects.size();
    for(int i = 0; i < count; i++)
    {
        if(rootObjects.at(i)->objectName() == "rootObject")
        {
            root = rootObjects.at(i);
            break;
        }
    }

    ColorConn ccd2;
    if(root)
    {
        QObject * color_ret = root->findChild<QObject*>("ccd2_ret");
        if(color_ret)
        {
            color_ret->setProperty("color", ccd2.color());
            QObject::connect(&ccd2, &ColorConn::colorChanged,
                             [color_ret, &ccd2](){color_ret->setProperty("color", ccd2.color());});
        }
        QObject * next_bt = root->findChild<QObject*>("ccd2_bt");
        if(next_bt)
        {
            QObject::connect(next_bt, SIGNAL(clicked()),
                             &ccd2, SLOT(nextColor()));
        }
    }
```

首先在根节点中找到 rootObject，然后在根节点中找到显示的 `Rectangle` 和按钮 `Button`，为它们连接信号槽函数。

整个程序的执行顺序是：点击按钮后，调用 nextColor 槽函数改变颜色，并发出颜色已改变的信号，然后设置显示方块的颜色。

通过元对象系统，可以查询 QObject 的某个派生类的类名、有哪些信号、槽、属性、可调用方法等信息。对于使用 Q_PROPERTY 定义的属性，可以使用 QObject 的 property() 方法访问属性，如果该属性定义了 WRITE 方法，还可以使用 setProperty() 修改属性。另外也可以使用 QMetaObject::invokeMethod() 调用 QObject 的某个注册到元对象系统中的方法。

**总结：**

可以看到，在 QtWidgets 开发中，使用界面元素不用这么麻烦。那是因为在 QtWidgets 中，`.ui` 会被 Qt 的 ui 处理工具 uic 转换为标准 C++ 文件，对于界面元素的索引，是非常方便的。

而在 QtQuick 中，界面元素运行在一个独立的 ECMA 环境中，和 C++ 环境为平行关系。而要使用界面元素，则要通过类似于 DOM 的 API 去查找。

<br/>

## 在QML中通过信号槽操纵C++类

（先看“在 QML 中使用 C++”，再看此小节，这里主要是因为归类，放到了此处）

折中于“在 C++ 中实现，在 C++ 中绑定” 和 “在C++中实现，导出属性到 QML”，也可以“在C++中实现信号槽，导出到 QML，在 QML 中绑定信号槽”。

```c++
// my_class.h
class ColorClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)

public:
    enum Colors{
        Gray = 0,
        Red,
        Green,
        Blue,
        Max
    };
    Q_ENUM(Colors)
    explicit ColorClass(QObject *parent = nullptr);
    QColor color(void) const {return _color;};

public slots:
    Q_INVOKABLE void nextColor();

signals:
    void colorChanged(const QColor &_color);

private:
    Colors _current_index = Gray;
    QList<QColor> default_color = {QColor(0xDD, 0xDD, 0xDD),
                                   QColor(0xD3, 0x00, 0x00),
                                   QColor(0x00, 0xD3, 0x00),
                                   QColor(0x00, 0x00, 0xD3)};
    QColor _color = QColor(0xDD, 0xDD, 0xDD);
};
```

这里的关键是，使用 Q_INVOKABLE 声明的函数，可以在 QML 中直接调用。

<br/>

# 在 QML 中使用 C++

QtQuick 推荐的开发方式是在 QML 中使用 C++。

Qt 提供了两种在 QML 环境中使用 C++ 对象的方式：

* 在 C++ 中实现一个类，注册为 QML 环境的一个类型，在 QML 环境中使用该类型创建对象。（继承 Q_Object 或 QQuickItem）
* 在 C++ 中构造一个对象，将这个对象设置为 QML 的上下文属性，在 QML 环境中直接使用该属性。（不能通过类名来引用枚举值）

<br/>

## 在QML中读写C++类的属性

在 C++ 中，使用 Q_PROPERTY 宏声明继承 QObject 的类中的属性，可以导入QML，并在 QML 中访问。

### READ/WRITE 声明

先看 C++ 声明：

```c++
// my_class.h
class ColorPropertyRW : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit ColorPropertyRW(QObject *parent = nullptr);
    QColor color(void) const {return _color;};
    void setColor(QColor color){_color = color; emit colorChanged(_color);};

signals:
    void colorChanged(const QColor &_color);

private:
    QColor _color = QColor(0xDD, 0xDD, 0xDD);
};
```

在主函数中注册 QML 类型：

```c++
qmlRegisterType<ColorPropertyRW>("easy.qt.ColorPropertyRW", 1, 0, "ColorPropertyRW");
```

在 QML 中引入并使用：

```js
import easy.qt.ColorPropertyRW 1.0

    Rectangle {
        color: "#EEEEEE"
        Layout.preferredWidth: 150
        Layout.preferredHeight: 150

        ColorPropertyRW {
            id: color_p2
        }

        ColumnLayout {
            spacing: 10
            Label {
                text: "在QML中读写\r\nC++类的属性(RW)"
            }
            Rectangle {
                color: color_p2.color
                width: 50
                height: 50
            }
            Rectangle
            {
                width: 120
                height: 35
                color: "#FFFFFF"
                border.color: "grey"

                TextInput {
                    anchors.fill: parent
                    anchors.margins: 2
                    font.pointSize: 10
                    selectByMouse : true
                    text: "#DDDDDD"

                    onEditingFinished: color_p2.color = text
                }
            }
        }
    }
```

如上所示，我们在 QML 中实例化了一个 id 为 color_p2 的 ColorPropertyRW 类对象，并将其 color 属性和显示的 Rectangle 的颜色绑定（READ），在 TextInput 中输入完颜色后回车，便会触发 onEditingFinished 并将输入值写回到 color（WRITE），同时改变了 Rectangle 的颜色（NOTIFY）。

Q_PROPERTY 宏还有很多其它的参数，用于提供不同的暴露效果，详情如下所示：

#### Q_PROPERTY

Q_PROPERTY 宏用于声明继承 QObject 的类中的属性。属性的行为类似于类数据成员，但它们具有可通过 元对象系统 访问的附加功能。

```c++
Q_PROPERTY(type name
           (READ getFunction [WRITE setFunction] |
            MEMBER memberName [(READ getFunction | WRITE setFunction)])
           [RESET resetFunction]
           [NOTIFY notifySignal]
           [REVISION int | REVISION(int[, int])]
           [DESIGNABLE bool]
           [SCRIPTABLE bool]
           [STORED bool]
           [USER bool]
           [BINDABLE bindableProperty]
           [CONSTANT]
           [FINAL]
           [REQUIRED])
```

属性名称和类型以及 `READ` 函数是必要的。其他项目是可选的，但 `WRITE` 功能是常见的。除 `USER` 之外，其他属性默认为 `true`，默认为 `false`。`READ`、`WRITE` 和 `RESET` 功能可以继承，它们也可以是虚拟的。当它们在使用多重继承的类中继承时，它们必须来自第一个继承的类。

* `type `可以是 [QVariant](https://doc.qt.io/qt-6/qvariant.html) 支持的任何类型，也可以是用户定义的类型。此时，C++ 中的基础类型会被解释为QML中的基础类型，例如 QString -> String，QList -> Array。

* `READ`：用于读取属性值。如果未指定 `MEMBER` 变量，则 `READ` 是必须声明的。`getFunction` 必须返回属性的类型或对该类型的 const 引用（推荐）。

* `WRITE`：用于设置属性值。只读属性不需要 `WRITE` 函数。`setFunction` 必须返回 `void`，并且必须只接受一个参数，可以是属性的类型，也可以是指向该类型的指针或引用。如果同时指定 `BINDABLE` 和 `WRITE`，默认则将从 `BINDABLE` 生成 `WRITE` 访问器。生成的 `WRITE` 访问器不会显式发出任何用 `NOTIFY` 声明的信号。您应该将信号注册为 `BINDABLE` 的更改处理程序，例如使用 [Q_OBJECT_BINDABLE_PROPERTY](https://doc.qt.io/qt-6/qobjectbindableproperty.html#Q_OBJECT_BINDABLE_PROPERTY)。

* `MEMBER`：声明成员变量可读可写的快速方法，而无需创建 `READ` 和 `WRITE` 访问器函数。如果需要控制变量访问，除了 `MEMBER` 变量关联之外，仍然可以使用 `READ` 或 `WRITE` 访问器函数（但不能同时使用两者）。

  ```c++
      Q_PROPERTY(QColor color MEMBER m_color NOTIFY colorChanged)
  
  signals:
      void colorChanged();
  
  private:
      QColor  m_color;
  ```

* `RESET`：它用于将属性设置回其上下文特定的默认值。 `resetFunction` 必须返回 void 并且不带任何参数。（什么条件会触发）

* `NOTIFY`：如果定义了，它应该指定该类中的一个现有信号，每当属性值发生变化时就会发出该信号。`MEMBER` 变量的 `NOTIFY` 信号必须采用零个或一个参数，该参数必须与属性具有相同的类型。该参数将采用属性的新值。 `NOTIFY `信号仅应在属性确实发生更改时发出，以避免在 QML 中不必要地重新评估绑定。当通过 Qt API（QObject::setProperty、QMetaProperty 等）更改属性时，会自动发出该信号，但直接更改 `MEMBER` 时不会发出该信号。当定义一个非常量的属性时，如果缺少了 `NOTIFY`，会提示 `Warns when a non-CONSTANT Q_PROPERTY is missing a NOTIFY signal.`，因为数据绑定实际上依赖于 `NOTIFY`。

* `REVISION`：它定义了要在 API 的特定修订版中使用的属性及其通知程序信号（通常用于暴露 QML）。如果不包含，则默认为 0。

* `DESIGNABLE`：指示该属性是否应在 GUI 设计工具（例如 Qt Designer）的属性编辑器中可见。大多数属性都是可设计的（默认为 true）。有效值为 true 和 false。

* `SCRIPTABLE`：指示脚本引擎是否可以访问此属性（默认为 true）。有效值为 true 和 false。

* `STORED`：指示该属性是否应被视为独立存在或取决于其他值。它还指示在存储对象状态时是否必须保存属性值。大多数属性都是 STORED （默认为 true），但例如 QWidget::minimumWidth() 的 STORED false，因为它的值只是从属性 QWidget::minimumSize() 的宽度部分获取。

* `USER`：指示该属性是否被指定为该类的面向用户的属性或用户可编辑的属性。通常，每个类只有一个 USER 属性（默认 false）。例如，QAbstractButton::checked 是（可检查）按钮的用户可编辑属性。请注意，QItemDelegate 获取并设置小部件的 USER 属性。

* `BINDABLE`：指示该属性支持绑定，并且可以通过元对象系统 (QMetaProperty) 设置和检查对此属性的绑定。 `bindableProperty` 声明一个 `QBindable<T>` 类型的类成员，其中 T 是属性类型。该属性是在 Qt 6.0 中引入的。

* `CONSTANT`：表明属性值是常量。对于给定的对象实例，常量属性的 `READ` 方法每次调用时都必须返回相同的值。对于对象的不同实例，该常量值可能不同。常量属性不能有 `WRITE` 方法或 `NOTIFY` 信号。

* `FINAL`：表明该属性不会被派生类覆盖。在某些情况下，这可以用于性能优化，但 moc 不强制执行。必须小心，切勿覆盖 `FINAL` 属性。

* `REQUIRED`：表明该属性应由该类的使用者设置。这不是由 moc 强制执行的，并且对于暴露于 QML 的类来说最有用。在 QML 中，除非设置了所有必需属性，否则无法实例化具有 `REQUIRED` 属性的类。

##### Q_REVISION

将此宏应用于成员函数的声明，以使用元对象系统中的修订号来标记它们。宏写在返回类型之前，如下例所示：

```c++
class Window : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int normalProperty READ normalProperty)
    Q_PROPERTY(int newProperty READ newProperty REVISION(2, 1))

public:
    Window();
    int normalProperty();
    int newProperty();
public slots:
    void normalMethod();
    Q_REVISION(2, 1) void newMethod();
};
```

当使用元对象系统将对象动态公开给另一个 API 时，可以用于匹配其他 API 的多个版本所期望的版本。

考虑以下简化示例：

```c++
    Window window;
    int expectedRevision = 0;
    const QMetaObject *windowMetaObject = window.metaObject();
    for (int i=0; i < windowMetaObject->methodCount(); i++)
        if (windowMetaObject->method(i).revision() <= expectedRevision)
            exposeMethod(windowMetaObject->method(i));
    for (int i=0; i < windowMetaObject->propertyCount(); i++)
        if (windowMetaObject->property(i).revision() <= expectedRevision)
            exposeProperty(windowMetaObject->property(i));
```

使用与前面的示例相同的 Window 类，只有当预期版本为 2.1 或更高版本时，newProperty 和 newMethod 才会在此代码中公开。

<br/>

### MEMBER 声明

所以根据 Q_PROPERTY 的定义，ColorPropertyRW 可以替换为以下等同的定义：

```c++
class ColorPropertyM : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color MEMBER _color NOTIFY colorChanged)

public:
    explicit ColorPropertyM(QObject *parent = nullptr);

signals:
    void colorChanged(const QColor &_color);

private:
    QColor _color = QColor(0xDD, 0xDD, 0xDD);
};
```

此定义更为简便，省却了读写函数，默认将其和成员变量绑定。除了类名，其它部分代码基本上不用变。

<br/>

## 在QML中使用C++对象

除了将类导入到 QML，还可以单独将 C++ 对象引入到 QML，如下所示：

```c++
// main.cpp
#include <QQmlContext>

int main(int argc, char* argv[]) {
    ColorPropertyM color_p3;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("color_p3", &color_p3);
}
```

<br/>

## 在C++中继承QQuickItem实现QML界面元素

超出了 QML 作显示，C++ 作后台计算的开发模式，也可以在 C++ 中通过继承 QQuickItem 直接实现 QML 界面元素，用于需要定制特殊界面元素的情况，特别是界面和计算结合得比较紧密的情形（因为 QML 本身也提供了良好的扩展性，但更偏显示方向）。

C++ 声明：

```c++
// my_item.h
#ifndef MY_ITEM_H
#define MY_ITEM_H

#include <QObject>
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QTimer>

class MyRect : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)

public:
    explicit MyRect(QQuickItem *parent = nullptr);
    ~MyRect();
    enum Colors{
        Gray = 0,
        Red,
        Green,
        Blue,
        Max
    };
    Q_ENUM(Colors)
    QColor color(void) const {return _color;};


public slots:
    Q_INVOKABLE void nextColor();

protected:
    void componentComplete() override;
    QSGNode *updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData) override;

signals:
    void colorChanged(const QColor &_color);

private:
    QSGSimpleRectNode *_node;
    QTimer _timer;
    Colors _current_index = Gray;
    QList<QColor> default_color = {QColor(0xDD, 0xDD, 0xDD),
                                   QColor(0xD3, 0x00, 0x00),
                                   QColor(0x00, 0xD3, 0x00),
                                   QColor(0x00, 0x00, 0xD3)};
    QColor _color = QColor(0xDD, 0xDD, 0xDD);
};

#endif // MY_ITEM_H
```

需要注意的是，此处的类名需要以大写字母开头，否则会报错：

```
Invalid QML element name "my_item"; type names must begin with an uppercase letter
```

C++ 实现：

```c++
// my_item.cpp
#include "my_item.h"


MyRect::MyRect(QQuickItem *parent)
    : QQuickItem(parent), _node(nullptr)
{
    setFlag(QQuickItem::ItemHasContents, true);     // 标记这个item有视觉内容并且应由场景图渲染
    _timer.setInterval(1000);
    QObject::connect(&_timer, SIGNAL(timeout()),
                     this, SLOT(nextColor()));
    _timer.start();
}

MyRect::~MyRect()
{
    _timer.stop();
    // if (_node) {
    //     delete _node;    // 会被父级Item自动清理
    // }
}

void MyRect::nextColor()
{
    _current_index = static_cast<Colors>(_current_index + 1);

    if (_current_index == Max)
    {
        _current_index = Gray;
    }

    _color = default_color[_current_index];
    emit colorChanged(_color);
    update();                               // 将对 updatePaintNode() 的调用列入计划表
}

void MyRect::componentComplete() {
    QQuickItem::componentComplete();
    if (!_node) {
        _node = new QSGSimpleRectNode();    // 创建一个新的矩形节点
        _node->setRect(boundingRect());     // 设置矩形的边界
        setImplicitWidth(50);               // 设置默认宽度
        setImplicitHeight(50);              // 设置默认高度
    }
}

QSGNode *MyRect::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{
    if (_node) {
        _node->setColor(_color);            // 设置矩形颜色
    }
    return _node;
}
```

在内部定义了一个Timer，每隔 1s 改变一次颜色。

导入 QML：

```c++
// main.cpp
qmlRegisterType<MyRect>("easy.qt.MyRect", 1, 0, "MyRect");
```

在 QML 中显示：

```js
import easy.qt.MyRect 1.0
    Rectangle {
        color: "#EEEEEE"
        Layout.preferredWidth: 150
        Layout.preferredHeight: 150

        ColumnLayout {
            spacing: 10
            Label {
                text: "在C++中继承\r\nQQuickItem\r\n实现QML界面元素"
            }
            MyRect {
                width: 50
                height: 50
            }
        }
    }
```

<br/>

### updatePaintNode

```
QSGNode *QQuickItem::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
```

当需要将 Item 的状态与场景图同步时在渲染线程上调用。

如果用户在项目上设置了 QQuickItem::ItemHasContents 标志，则该函数将作为 QQuickItem::update() 的结果被调用。

该函数应返回该项目的场景图子树的根。大多数实现将返回一个包含该项目的视觉表示的 QSGGeometryNode。 oldNode 是上次调用该函数时返回的节点。 updatePaintNodeData 提供指向与此 QQuickItem 关联的 QSGTransformNode 的指针。

执行此函数时，主线程被阻塞，因此可以安全地从 QQuickItem 实例和主线程中的其他对象读取值。

如果没有调用 QQuickItem::updatePaintNode() 导致实际的场景图更改，例如 QSGNode::markDirty() 或添加和删除节点，则底层实现可能会决定不再渲染场景，因为视觉结果是相同的。

警告：至关重要的是，图形操作和与场景图的交互仅发生在渲染线程上，主要是在 QQuickItem::updatePaintNode() 调用期间。最好的经验法则是仅在 QQuickItem::updatePaintNode() 函数中使用带有“QSG”前缀的类。

警告：此函数在渲染线程上调用。这意味着创建的任何 QObject 或线程本地存储都将与渲染线程具有关联性，因此在此函数中执行渲染以外的任何操作时请务必小心。与信号类似，这些信号将在渲染线程上发出，因此通常会通过排队连接传递。

注意：所有带有 QSG 前缀的类都应该仅在场景图的渲染线程上使用。有关详细信息，参阅 [场景图和渲染](https://doc.qt.io/qt-6/qtquick-visualcanvas-scenegraph.html#scene-graph-and-rendering)。

另参见 [QSGMaterial](https://doc.qt.io/qt-6/qsgmaterial.html), [QSGGeometryNode](https://doc.qt.io/qt-6/qsggeometrynode.html), [QSGGeometry](https://doc.qt.io/qt-6/qsggeometry.html), [QSGFlatColorMaterial](https://doc.qt.io/qt-6/qsgflatcolormaterial.html), [QSGTextureMaterial](https://doc.qt.io/qt-6/qsgtexturematerial.html), [QSGNode::markDirty](https://doc.qt.io/qt-6/qsgnode.html#markDirty)(), [Graphics Resource Handling](https://doc.qt.io/qt-6/qquickitem.html#graphics-resource-handling).

<br/>

# Tips

## 属性绑定 vs 信号连接

属性绑定允许一个对象的属性值与另一个属性或表达式相关联。当绑定的源属性发生变化时，目标属性会自动更新以反映新的值。这是一种数据流的方式，通常用于实现数据模型和视图之间的同步。

**特点**:

- 是一种单向的数据绑定。
- 用于在属性值之间建立动态的联系。
- 当源属性改变时，目标属性会自动更新。

信号连接用于在对象的信号（当某个事件发生时发出的通知）和槽（响应信号的函数）之间建立联系。当信号被触发时，连接的槽函数会被调用。

**特点**:

- 用于对象之间的事件通信。
- 是一种双向的通信机制，可以传递参数。
- 通常用于响应用户交互或其他事件。

<br/>

# 源码

```sh
wget https://www.easywang.site/resources/QtQuick/cppintegration -O cppintegration.zip
```

<br/>

# 参考

* 《Qt Quick核心编程》

* [C++与QML交互总结二_qml c++ 调用 四种方式-CSDN博客](https://blog.csdn.net/hsy12342611/article/details/133309866)
* [QML自学之路(六)-用C++语言编写QML对象类型_qml 创建单例qml对象-CSDN博客](https://blog.csdn.net/qq_30574227/article/details/128853333)
* [The Property System](https://doc.qt.io/qt-6/properties.html)
* [QQmlEngine Class | qmlRegisterType](https://doc.qt.io/qt-6/qqmlengine.html#qmlRegisterType)
* [QQuickItem Class](https://doc.qt.io/qt-6/qquickitem.html)


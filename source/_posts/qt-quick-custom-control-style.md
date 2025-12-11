---
title: QtQuick 自定义控件样式
date: 2024-06-24 19:00:00
toc: true
categories:
  - tech
tags:
  - Qt
---

在 QtWidgets 中，可以使用 qss 对一批需要相同样式的控件进行设置，避免大量的重复代码。

然而在 QtQuick 中，qss 完全不支持了，但是对于上述需求依然是存在的，那么在 QML 中，要如何实现呢？

<!--more-->

<br/>

## 自定义控件

Qt Quick Controls 由项目层次结构（树）组成。为了提供自定义的外观和感觉，每个项目的默认 QML 实现都可以替换为自定义的。

### 自定义单个控件

如果打算在多个地方使用相同的样式，可以采取以下方式创建元素，以自定义圆形按钮为例：

1.将基本样式 Button.qml 复制到当前项目，并另存为 MyButton.qml。该文件可以在 Qt 安装的以下路径中找到：

   ```
   $QTDIR/qml/QtQuick/Controls/Basic/Button.qml
   ```

2.修改 MyButton.qml，添加：

   ```js
   radius: 4
   ```

3.在应用程序中使用该控件，直接通过其文件名引用它：

   ````js
   import QtQuick.Controls
   
   ApplicationWindow {
       MyButton {
           text: qsTr("A Special Button")
       }
   }
   ````

<br/>

### 自定义一组控件

上述方法对于添加一组自定义控件来说，还是稍显繁琐，需要修改大量的文件名。

对此，我们可以把所有控件放到一个文件中，通过直接引入真个文件夹解决了问题。

例如，将 Button.qml 放入项目中名为 controls 的子文件夹中，按如下方式导入 QML：

```js
import QtQuick.Controls
import "controls" as MyControls

ApplicationWindow {
    MyControls.Button {
        text: qsTr("A Special Button")
    }
}
```

注意：此方法不适用于自定义附加的工具提示（[ToolTip](https://doc.qt.io/qt-6/qml-qtquick-controls-tooltip.html)），因为这是内部创建的共享项。要对工具提示进行一次性自定义，请参阅自定义工具提示（[Custom Tool Tips](https://doc.qt.io/qt-6/qml-qtquick-controls-tooltip.html#custom-tool-tips)）。要自定义附加的工具提示，必须将其作为自定义样式的一部分提供。

<br/>

## 创建自定义样式

### 风格的定义

在 Qt Quick Controls 中，样式本质上是单个目录中的一组 QML 文件。样式可用需要满足四个要求：

* 必须至少存在一个名称与控件（例如 Button.qml）匹配的 QML 文件。

* 每个 QML 文件必须包含来自 QtQuick.Templates 导入的相关类型作为根项。例如，Button.qml 必须包含一个 Button 模板作为其根项。

  如果我们像上一节中那样使用 QtQuick.Controls 导入中的相应类型，它将不起作用：我们定义的控件将尝试从自身派生。

* qmldir 文件必须与 QML 文件一起存在。以下是提供按钮的样式的简单 qmldir 文件示例：

  ```
  module MyStyle
  Button 2.15 Button.qml
  ```

  如果使用编译时样式选择，qmldir 还应该导入后备样式：

  ```
  # ...
  import QtQuick.Controls.Basic auto
  ```

  这也可以用于运行时样式选择，而不是使用 QQuickStyle::setFallbackStyle() 等。

  这种样式的目录结构如下所示：

  ```
  MyStyle
  ├─── Button.qml
  └─── qmldir
  ```

* 这些文件必须位于可通过 QML 导入路径找到的目录中。

  例如，如果上面提到的 MyStyle 目录的路径是 /home/user/MyApp/MyStyle，则 /home/user/MyApp 必须添加到 QML 导入路径中。

  要在 MyApp 中使用 MyStyle，请按名称引用它：`./MyApp -style MyStyle`

  样式名称必须与样式目录的大小写一致；不支持传递 mystyle 或 MYSTYLE。

默认情况下，样式系统使用基本样式作为未实现的控件的后备样式。要自定义或扩展任何其他内置样式，可以使用 QQuickStyle 指定不同的后备样式。

这意味着可以为自定义样式实现任意数量的控件，并将它们放置在几乎任何地方。它还允许用户为应用程序创建自己的样式。

<br/>

### 在 Qt Quick Designer 中预览自定义样式

使用上述方法，可以在 Qt Quick Designer 中预览自定义样式。为此，请确保项目具有 qtquickcontrols2.conf 文件，并且存在以下条目：

```
[Controls]
Style=MyStyle
```

有关更多信息，请查看平面样式示例（[Flat Style example](https://doc.qt.io/qt-6/qtquickcontrols-flatstyle-example.html)）。

<br/>

### 特定于样式的 C++ 扩展

有时可能需要使用 C++ 来扩展自定义样式。

如果使用该类型的样式是应用程序使用的唯一样式，请通过添加 QML_ELEMENT 宏并使该文件成为 QML 模块的一部分来向 QML 引擎注册该类型：

CMake：

```cmake
qt_add_qml_module(ACoolItem
    URI MyItems
    VERSION 1.0
    SOURCES
        acoolcppitem.cpp acoolcppitem.h
)
```

qmake：

```makefile
CONFIG += qmltypes
QML_IMPORT_NAME = MyItems
QML_IMPORT_MAJOR_VERSION = 1
```

如果无法从项目的包含路径访问声明类的标头，则可能需要修改包含路径，以便可以编译生成的注册代码。

```makefile
INCLUDEPATH += MyItems
```

有关更多信息，请参阅从 C++ 定义 QML 类型（[Defining QML Types from C++](https://doc.qt.io/qt-6/qtqml-cppintegration-definetypes.html)）和构建 QML 应用程序（[Building a QML application](https://doc.qt.io/qt-6/cmake-build-qml-application.html)）。

如果使用该类型的样式是应用程序使用的多种样式之一，请考虑将每种样式放入单独的模块中。然后将按需加载模块。

<br/>

### 自定义样式的注意事项

在实现自己的样式和自定义控件时，需要记住一些要点，以确保应用程序尽可能高效。

#### 避免将 id 分配给项目委托的样式实现

如样式定义中所述，当为控件实现自己的样式时，将从该控件的相关模板开始。例如，样式的 Button.qml 的结构与此类似：

```js
T.Button {
    // ...

    background: Rectangle {
        // ...
    }

    contentItem: Text {
        // ...
    }

    // ...
}
```

当在应用程序中使用 Button 时，将创建背景和 contentItem 项并将其作为根 Button 项的父级：

```js
// Creates the Button root item, the Rectangle background,
// and the Text contentItem.
Button {
    text: qsTr("Confirm")
}
```

假设需要对按钮进行一次性自定义（如自定义控件中所述）：

```js
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 400
    height: 400
    visible: true

    Button {
        id: button
        text: "A Special Button"
        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 40
            color: button.down ? "#d6d6d6" : "#f6f6f6"
            border.color: "#26282a"
            border.width: 1
            radius: 4
        }
    }
}
```

在 QML 中，这通常会导致默认背景实现和一次性的自定义背景项被创建。 Qt Quick Controls 使用了一种避免创建这两项的技术，而只创建自定义背景，从而大大提高了控件的创建性能。

此技术依赖于该项目的样式实现中缺少 id。如果分配了 id，则该技术无法工作，并且将创建两个项目。例如，将 id 分配给背景或 contentItem 可能很诱人，以便文件中的其他对象可以引用这些项目：

```js
T.Button {
    // ...

    background: Rectangle {
        id: backgroundRect
        // ...
    }

    contentItem: Text {
        // Use backgroundRect in some way...
    }

    // ...
}
```

使用此代码，每次创建具有自定义背景的 Button 实例时，都会创建两个背景，从而导致创建性能不佳。

在 Qt 5.15 之前，旧的、未使用的背景将被删除以释放与其关联的资源。但是，由于控件不拥有这些项目，因此不应删除它们。从 Qt 5.15 开始，旧项目不再被删除，因此 backgroundRect 项目的生存时间将比它需要的时间更长——通常直到应用程序退出为止。尽管旧项目将被隐藏，在视觉上从控件中取消父子关系，并从可访问性树中删除，但在此上下文中分配 id 时，请务必记住这些未使用项目的创建时间和内存使用情况。

#### 避免自定义项目的强制分配

上一节中提到的技术仅在第一次以声明方式分配项目时才有效，因此命令式分配将导致孤立项目。如果可能，请始终使用声明性绑定来分配自定义项。

#### 不要在 QML 实现中导入 QtQuick.Controls

当为控件的样式实现编写 QML 时，重要的是不要导入 QtQuick.Controls。这样做将阻止 QML 编译器编译 QML。

#### 实现其他类型使用的类型

假设在应用程序中使用 [ScrollView](https://doc.qt.io/qt-6/qml-qtquick-controls-scrollview.html)，并决定要自定义其滚动条。仅仅实现一个自定义的 [ScrollBar](https://doc.qt.io/qt-6/qml-qtquick-controls-scrollbar.html).qml 并让 [ScrollView ](https://doc.qt.io/qt-6/qml-qtquick-controls-scrollview.html)自动选取自定义的 [ScrollBar](https://doc.qt.io/qt-6/qml-qtquick-controls-scrollbar.html) 是很诱人的。然而，这是行不通的，必须同时实现 [ScrollBar](https://doc.qt.io/qt-6/qml-qtquick-controls-scrollbar.html).qml 和 [ScrollView](https://doc.qt.io/qt-6/qml-qtquick-controls-scrollview.html).qml。

#### 附加属性

样式通常具有适用于所有控件的某些特性或特性。附加属性是在 QML 中扩展项目的好方法，而无需修改属于该项目的任何现有 C++。例如，Material 和 Universal 样式都有一个附加的主题属性，用于控制项目及其子项是否以浅色主题或深色主题渲染。

作为示例，让我们添加一个控制海拔的附加属性。我们的风格将用投影来说明立面；海拔越高，阴影越大。

第一步是在 Qt Creator 中创建一个新的 Qt Quick Controls 应用程序（[create a new Qt Quick Controls application](https://doc.qt.io/qtcreator/creator-project-creating.html#selecting-project-type)）。之后，我们添加一个存储海拔的 C++ 类型（[add a C++ type](https://doc.qt.io/qtcreator/creator-project-creating.html#creating-c-classes)）。由于该类型将用于我们的样式支持的每个控件，并且因为我们可能希望稍后添加其他附加属性，所以我们将其称为 MyStyle。这是 MyStyle.h：

```c++
#ifndef MYSTYLE_H
#define MYSTYLE_H

#include <QObject>
#include <QtQml>

class MyStyle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int elevation READ elevation WRITE setElevation NOTIFY elevationChanged)

public:
    explicit MyStyle(QObject *parent = nullptr);

    static MyStyle *qmlAttachedProperties(QObject *object);

    int elevation() const;
    void setElevation(int elevation);

signals:
    void elevationChanged();

private:
    int m_elevation;
};

QML_DECLARE_TYPEINFO(MyStyle, QML_HAS_ATTACHED_PROPERTIES)

#endif // MYSTYLE_H
```

MyStyle.cpp:

```c++
#include "mystyle.h"

MyStyle::MyStyle(QObject *parent) :
    QObject(parent),
    m_elevation(0)
{
}

MyStyle *MyStyle::qmlAttachedProperties(QObject *object)
{
    return new MyStyle(object);
}

int MyStyle::elevation() const
{
    return m_elevation;
}

void MyStyle::setElevation(int elevation)
{
    if (elevation == m_elevation)
        return;

    m_elevation = elevation;
    emit elevationChanged();
}
```

MyStyle 类型很特殊，因为它不应被实例化，而应用于其附加属性。因此，我们在 main.cpp 中按以下方式注册它：

```c++
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mystyle.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterUncreatableType<MyStyle>("MyStyle", 1, 0, "MyStyle", "MyStyle is an attached property");

    QQmlApplicationEngine engine;
    // Make the directory containing our style known to the QML engine.
    engine.addImportPath(":/");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
```

然后，我们将 $QTDIR/qml/QtQuick/Controls/Basic/ 中的 Basic 样式中的 Button.qml 复制到项目目录中的新 myproject 文件夹中。将新复制的 Button.qml 添加到 qml.qrc，这是包含我们的 QML 文件的资源文件。

接下来，我们向 Button 的背景委托添加阴影：

```js
// ...
import QtQuick.Effects
import MyStyle
// ...

background: Rectangle {
    // ...

    layer.enabled: control.enabled && control.MyStyle.elevation > 0
    layer.effect: MultiEffect {
        shadowEnabled: true
        shadowHorizontalOffset: 3
        shadowVerticalOffset: 3
        shadowColor: control.visualFocus ? "#330066ff" : "#aaaaaa"
        shadowBlur: control.pressed ? 0.8 : 0.4
    }
}
```

请注意：

* 当海拔为 0 时，不要费心使用投影
* 根据按钮是否具有焦点来更改阴影的颜色
* 使阴影的大小取决于海拔

为了尝试附加属性，我们在 main.qml 中创建一个带有两个按钮的 Row：

```c++
import QtQuick
import QtQuick.Controls

import MyStyle 1.0

ApplicationWindow {
    id: window
    width: 400
    height: 400
    visible: true

    Row {
        spacing: 20
        anchors.centerIn: parent

        Button {
            text: "Button 1"
        }
        Button {
            text: "Button 2"
            MyStyle.elevation: 10
        }
    }
}
```

一个按钮没有高程，另一个按钮的高程为 10。

完成后，我们就可以运行我们的示例了。为了告诉应用程序使用我们的新样式，我们将 -style MyStyle 作为应用程序参数传递，但是有很多方法可以指定要使用的样式。

最终结果：

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-customize-buttons.png)

请注意，导入 MyStyle 1.0 语句仅是必要的，因为我们正在使用属于 MyStyle 的附加属性。即使我们要删除导入，这两个按钮都将使用我们的自定义样式。

<br/>

## 定制参考

### ApplicationWindow

[ApplicationWindow](https://doc.qt.io/qt-6/qml-qtquick-controls-applicationwindow.html) 由一项视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-applicationwindow.html#background-prop)。

```js
import QtQuick
import QtQuick.Controls.Basic

ApplicationWindow {
    visible: true

    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0; color: "#ffffff" }
            GradientStop { position: 1; color: "#c1bbf9" }
        }
    }
}
```

同样只有一个 background 的还有 Drawer、Frame、Label、Pane、[TextArea](https://doc.qt.io/qt-6/qml-qtquick-controls-textarea.html)、[TextField](https://doc.qt.io/qt-6/qml-qtquick-controls-textfield.html)、[ToolBar](https://doc.qt.io/qt-6/qml-qtquick-controls-toolbar.html)。

### BusyIndicator

[BusyIndicator](https://doc.qt.io/qt-6/qml-qtquick-controls-busyindicator.html) 由两个视觉项组成: [background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [contentItem](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-busyindicator-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

BusyIndicator {
    id: control

    contentItem: Item {
        implicitWidth: 64
        implicitHeight: 64

        Item {
            id: item
            x: parent.width / 2 - 32
            y: parent.height / 2 - 32
            width: 64
            height: 64
            opacity: control.running ? 1 : 0

            Behavior on opacity {
                OpacityAnimator {
                    duration: 250
                }
            }

            RotationAnimator {
                target: item
                running: control.visible && control.running
                from: 0
                to: 360
                loops: Animation.Infinite
                duration: 1250
            }

            Repeater {
                id: repeater
                model: 6

                Rectangle {
                    id: delegate
                    x: item.width / 2 - width / 2
                    y: item.height / 2 - height / 2
                    implicitWidth: 10
                    implicitHeight: 10
                    radius: 5
                    color: "#21be2b"

                    required property int index

                    transform: [
                        Translate {
                            y: -Math.min(item.width, item.height) * 0.5 + 5
                        },
                        Rotation {
                            angle: delegate.index / repeater.count * 360
                            origin.x: 5
                            origin.y: 5
                        }
                    ]
                }
            }
        }
    }
}
```

### Button

Button 由两个视觉项组成: [background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-button-custom.png)

```
import QtQuick
import QtQuick.Controls.Basic

Button {
    id: control
    text: qsTr("Button")

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#17a81a" : "#21be2b"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        border.color: control.down ? "#17a81a" : "#21be2b"
        border.width: 1
        radius: 2
    }
}
```

[RoundButton](https://doc.qt.io/qt-6/qml-qtquick-controls-roundbutton.html)、[TabButton](https://doc.qt.io/qt-6/qml-qtquick-controls-tabbutton.html) 可以按照与 Button 相同的方式进行自定义。

### CheckBox

[CheckBox](https://doc.qt.io/qt-6/qml-qtquick-controls-checkbox.html) 由三个视觉项组成: [background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [contentItem](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop) and [indicator](https://doc.qt.io/qt-6/qml-qtquick-controls-abstractbutton.html#indicator-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-checkbox-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

CheckBox {
    id: control
    text: qsTr("CheckBox")
    checked: true

    indicator: Rectangle {
        implicitWidth: 26
        implicitHeight: 26
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 3
        border.color: control.down ? "#17a81a" : "#21be2b"

        Rectangle {
            width: 14
            height: 14
            x: 6
            y: 6
            radius: 2
            color: control.down ? "#17a81a" : "#21be2b"
            visible: control.checked
        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#17a81a" : "#21be2b"
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }
}
```

### CheckDelegate

[CheckDelegate](https://doc.qt.io/qt-6/qml-qtquick-controls-checkdelegate.html) 由三个视觉项组成: [background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [contentItem](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop) and [indicator](https://doc.qt.io/qt-6/qml-qtquick-controls-abstractbutton.html#indicator-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-checkdelegate-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

CheckDelegate {
    id: control
    text: qsTr("CheckDelegate")
    checked: true

    contentItem: Text {
        rightPadding: control.indicator.width + control.spacing
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#17a81a" : "#21be2b"
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
    }

    indicator: Rectangle {
        implicitWidth: 26
        implicitHeight: 26
        x: control.width - width - control.rightPadding
        y: control.topPadding + control.availableHeight / 2 - height / 2
        radius: 3
        color: "transparent"
        border.color: control.down ? "#17a81a" : "#21be2b"

        Rectangle {
            width: 14
            height: 14
            x: 6
            y: 6
            radius: 2
            color: control.down ? "#17a81a" : "#21be2b"
            visible: control.checked
        }
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        visible: control.down || control.highlighted
        color: control.down ? "#bdbebf" : "#eeeeee"
    }
}
```

### ComboBox

[ComboBox](https://doc.qt.io/qt-6/qml-qtquick-controls-combobox.html) 由以下视觉项组成： [background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop), [popup](https://doc.qt.io/qt-6/qml-qtquick-controls-combobox.html#popup-prop), [indicator](https://doc.qt.io/qt-6/qml-qtquick-controls-combobox.html#indicator-prop), and [delegate](https://doc.qt.io/qt-6/qml-qtquick-controls-combobox.html#delegate-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-combobox-custom.png)

```js
pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Basic

ComboBox {
    id: control
    model: ["First", "Second", "Third"]

    delegate: ItemDelegate {
        id: delegate

        required property var model
        required property int index

        width: control.width
        contentItem: Text {
            text: delegate.model[control.textRole]
            color: "#21be2b"
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: control.highlightedIndex === index
    }

    indicator: Canvas {
        id: canvas
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 8
        contextType: "2d"

        Connections {
            target: control
            function onPressedChanged() { canvas.requestPaint(); }
        }

        onPaint: {
            context.reset();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width / 2, height);
            context.closePath();
            context.fillStyle = control.pressed ? "#17a81a" : "#21be2b";
            context.fill();
        }
    }

    contentItem: Text {
        leftPadding: 0
        rightPadding: control.indicator.width + control.spacing

        text: control.displayText
        font: control.font
        color: control.pressed ? "#17a81a" : "#21be2b"
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 40
        border.color: control.pressed ? "#17a81a" : "#21be2b"
        border.width: control.visualFocus ? 2 : 1
        radius: 2
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: "#21be2b"
            radius: 2
        }
    }
}
```

如 ComboBox 模型角色（[ComboBox Model Roles](https://doc.qt.io/qt-6/qml-qtquick-controls-combobox.html#combobox-model-roles)）中所述，[ComboBox](https://doc.qt.io/qt-6/qml-qtquick-controls-combobox.html) 支持多种类型的模型。

由于所有模型都提供带有 modelData 的匿名属性（[all the models provide an anonymous property](https://doc.qt.io/qt-6/qtquick-modelviewsdata-modelview.html#qml-data-models)），因此以下表达式在所有情况下都会检索正确的文本：

```js
text: model[control.textRole]
```

当提供特定的 textRole 和具有提供所选角色的结构化数据的模型时，此表达式是常规属性查找。当提供具有单一数据（例如字符串列表）和空 textRole 的模型时，此表达式将检索 modelData。

### DelayButton

[DelayButton](https://doc.qt.io/qt-6/qml-qtquick-controls-delaybutton.html) 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-delaybutton-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

DelayButton {
    id: control
    checked: true
    text: qsTr("Delay\nButton")

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 100
        opacity: enabled ? 1 : 0.3
        color: control.down ? "#17a81a" : "#21be2b"
        radius: size / 2

        readonly property real size: Math.min(control.width, control.height)
        width: size
        height: size
        anchors.centerIn: parent

        Canvas {
            id: canvas
            anchors.fill: parent

            Connections {
                target: control
                function onProgressChanged() { canvas.requestPaint(); }
            }

            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)
                ctx.strokeStyle = "white"
                ctx.lineWidth = parent.size / 20
                ctx.beginPath()
                var startAngle = Math.PI / 5 * 3
                var endAngle = startAngle + control.progress * Math.PI / 5 * 9
                ctx.arc(width / 2, height / 2, width / 2 - ctx.lineWidth / 2 - 2, startAngle, endAngle)
                ctx.stroke()
            }
        }
    }
}
```

### Dial

Dial 由两个视觉部分组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [handle](https://doc.qt.io/qt-6/qml-qtquick-controls-dial.html#handle-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-dial-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

Dial {
    id: control
    background: Rectangle {
        x: control.width / 2 - width / 2
        y: control.height / 2 - height / 2
        implicitWidth: 140
        implicitHeight: 140
        width: Math.max(64, Math.min(control.width, control.height))
        height: width
        color: "transparent"
        radius: width / 2
        border.color: control.pressed ? "#17a81a" : "#21be2b"
        opacity: control.enabled ? 1 : 0.3
    }

    handle: Rectangle {
        id: handleItem
        x: control.background.x + control.background.width / 2 - width / 2
        y: control.background.y + control.background.height / 2 - height / 2
        width: 16
        height: 16
        color: control.pressed ? "#17a81a" : "#21be2b"
        radius: 8
        antialiasing: true
        opacity: control.enabled ? 1 : 0.3
        transform: [
            Translate {
                y: -Math.min(control.background.width, control.background.height) * 0.4 + handleItem.height / 2
            },
            Rotation {
                angle: control.angle
                origin.x: handleItem.width / 2
                origin.y: handleItem.height / 2
            }
        ]
    }
}
```

### GroupBox

[GroupBox](https://doc.qt.io/qt-6/qml-qtquick-controls-groupbox.html) 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [label](https://doc.qt.io/qt-6/qml-qtquick-controls-groupbox.html#label-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-groupbox-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

GroupBox {
    id: control
    title: qsTr("GroupBox")

    background: Rectangle {
        y: control.topPadding - control.bottomPadding
        width: parent.width
        height: parent.height - control.topPadding + control.bottomPadding
        color: "transparent"
        border.color: "#21be2b"
        radius: 2
    }

    label: Label {
        x: control.leftPadding
        width: control.availableWidth
        text: control.title
        color: "#21be2b"
        elide: Text.ElideRight
    }

    Label {
        text: qsTr("Content goes here!")
    }
}
```

### ItemDelegate

[ItemDelegate](https://doc.qt.io/qt-6/qml-qtquick-controls-itemdelegate.html) 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-itemdelegate-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

ItemDelegate {
    id: control
    text: qsTr("ItemDelegate")

    contentItem: Text {
        rightPadding: control.spacing
        text: control.text
        font: control.font
        color: control.enabled ? (control.down ? "#17a81a" : "#21be2b") : "#bdbebf"
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        color: control.down ? "#dddedf" : "#eeeeee"

        Rectangle {
            width: parent.width
            height: 1
            color: control.down ? "#17a81a" : "#21be2b"
            anchors.bottom: parent.bottom
        }
    }
}
```

### Menu

* [Menu](https://doc.qt.io/qt-6/qml-qtquick-controls-menu.html) consists of a visual [background](https://doc.qt.io/qt-6/qml-qtquick-controls-popup.html#background-prop) item
* [MenuItem](https://doc.qt.io/qt-6/qml-qtquick-controls-menuitem.html) consists of four visual items: [background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop), [indicator](https://doc.qt.io/qt-6/qml-qtquick-controls-abstractbutton.html#indicator-prop), and [arrow](https://doc.qt.io/qt-6/qml-qtquick-controls-menuitem.html#arrow-prop).
* [MenuSeparator](https://doc.qt.io/qt-6/qml-qtquick-controls-menuseparator.html) consists of a visual [background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop).

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-menu-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

Menu {
    id: menu

    Action { text: qsTr("Tool Bar"); checkable: true }
    Action { text: qsTr("Side Bar"); checkable: true; checked: true }
    Action { text: qsTr("Status Bar"); checkable: true; checked: true }

    MenuSeparator {
        contentItem: Rectangle {
            implicitWidth: 200
            implicitHeight: 1
            color: "#21be2b"
        }
    }

    Menu {
        title: qsTr("Advanced")
        // ...
    }

    topPadding: 2
    bottomPadding: 2

    delegate: MenuItem {
        id: menuItem
        implicitWidth: 200
        implicitHeight: 40

        arrow: Canvas {
            x: parent.width - width
            implicitWidth: 40
            implicitHeight: 40
            visible: menuItem.subMenu
            onPaint: {
                var ctx = getContext("2d")
                ctx.fillStyle = menuItem.highlighted ? "#ffffff" : "#21be2b"
                ctx.moveTo(15, 15)
                ctx.lineTo(width - 15, height / 2)
                ctx.lineTo(15, height - 15)
                ctx.closePath()
                ctx.fill()
            }
        }

        indicator: Item {
            implicitWidth: 40
            implicitHeight: 40
            Rectangle {
                width: 26
                height: 26
                anchors.centerIn: parent
                visible: menuItem.checkable
                border.color: "#21be2b"
                radius: 3
                Rectangle {
                    width: 14
                    height: 14
                    anchors.centerIn: parent
                    visible: menuItem.checked
                    color: "#21be2b"
                    radius: 2
                }
            }
        }

        contentItem: Text {
            leftPadding: menuItem.indicator.width
            rightPadding: menuItem.arrow.width
            text: menuItem.text
            font: menuItem.font
            opacity: enabled ? 1.0 : 0.3
            color: menuItem.highlighted ? "#ffffff" : "#21be2b"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 40
            opacity: enabled ? 1 : 0.3
            color: menuItem.highlighted ? "#21be2b" : "transparent"
        }
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        color: "#ffffff"
        border.color: "#21be2b"
        radius: 2
    }
}
```

### MenuBar

[MenuBar](https://doc.qt.io/qt-6/qml-qtquick-controls-menubar.html) 可以有 [background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop)

[MenuBarItem](https://doc.qt.io/qt-6/qml-qtquick-controls-menubaritem.html) 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-menubar-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

MenuBar {
    id: menuBar

    Menu { title: qsTr("File") }
    Menu { title: qsTr("Edit") }
    Menu { title: qsTr("View") }
    Menu { title: qsTr("Help") }

    delegate: MenuBarItem {
        id: menuBarItem

        contentItem: Text {
            text: menuBarItem.text
            font: menuBarItem.font
            opacity: enabled ? 1.0 : 0.3
            color: menuBarItem.highlighted ? "#ffffff" : "#21be2b"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 40
            implicitHeight: 40
            opacity: enabled ? 1 : 0.3
            color: menuBarItem.highlighted ? "#21be2b" : "transparent"
        }
    }

    background: Rectangle {
        implicitWidth: 40
        implicitHeight: 40
        color: "#ffffff"

        Rectangle {
            color: "#21be2b"
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
        }
    }
}
````

### PageIndicator

[PageIndicator](https://doc.qt.io/qt-6/qml-qtquick-controls-pageindicator.html) consists of a [background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop), and [delegate](https://doc.qt.io/qt-6/qml-qtquick-controls-pageindicator.html#delegate-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-pageindicator-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

PageIndicator {
    id: control
    count: 5
    currentIndex: 2

    delegate: Rectangle {
        implicitWidth: 8
        implicitHeight: 8

        radius: width / 2
        color: "#21be2b"

        opacity: index === control.currentIndex ? 0.95 : pressed ? 0.7 : 0.45

        required property int index

        Behavior on opacity {
            OpacityAnimator {
                duration: 100
            }
        }
    }
}
```

### Popup

Popup consists of a [background](https://doc.qt.io/qt-6/qml-qtquick-controls-popup.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-popup.html#contentItem-prop).

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-popup-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

Popup {
    id: popup
    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 200
        border.color: "#444"
    }
    contentItem: Column {}
}
```

### ProgressBar

[ProgressBar](https://doc.qt.io/qt-6/qml-qtquick-controls-progressbar.html) 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-progressbar-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

ProgressBar {
    id: control
    value: 0.5
    padding: 2

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 6
        color: "#e6e6e6"
        radius: 3
    }

    contentItem: Item {
        implicitWidth: 200
        implicitHeight: 4

        // Progress indicator for determinate state.
        Rectangle {
            width: control.visualPosition * parent.width
            height: parent.height
            radius: 2
            color: "#17a81a"
            visible: !control.indeterminate
        }

        // Scrolling animation for indeterminate state.
        Item {
            anchors.fill: parent
            visible: control.indeterminate
            clip: true

            Row {
                spacing: 20

                Repeater {
                    model: control.width / 40 + 1

                    Rectangle {
                        color: "#17a81a"
                        width: 20
                        height: control.height
                    }
                }
                XAnimator on x {
                    from: 0
                    to: -40
                    loops: Animation.Infinite
                    running: control.indeterminate
                }
            }
        }
    }
}
```

上面，内容项也被动画化以表示不确定（[indeterminate](https://doc.qt.io/qt-6/qml-qtquick-controls-progressbar.html#indeterminate-prop)）的进度条状态。

### RadioButton

[RadioButton](https://doc.qt.io/qt-6/qml-qtquick-controls-radiobutton.html) 由三个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop) and [indicator](https://doc.qt.io/qt-6/qml-qtquick-controls-abstractbutton.html#indicator-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-radiobutton-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

RadioButton {
    id: control
    text: qsTr("RadioButton")
    checked: true

    indicator: Rectangle {
        implicitWidth: 26
        implicitHeight: 26
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 13
        border.color: control.down ? "#17a81a" : "#21be2b"

        Rectangle {
            width: 14
            height: 14
            x: 6
            y: 6
            radius: 7
            color: control.down ? "#17a81a" : "#21be2b"
            visible: control.checked
        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#17a81a" : "#21be2b"
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }
}
```

### RadioDelegate

[RadioDelegate](https://doc.qt.io/qt-6/qml-qtquick-controls-radiodelegate.html) 由三个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [contentItem](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop) and [indicator](https://doc.qt.io/qt-6/qml-qtquick-controls-abstractbutton.html#indicator-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-radiodelegate-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

RadioDelegate {
    id: control
    text: qsTr("RadioDelegate")
    checked: true

    contentItem: Text {
        rightPadding: control.indicator.width + control.spacing
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#17a81a" : "#21be2b"
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
    }

    indicator: Rectangle {
        implicitWidth: 26
        implicitHeight: 26
        x: control.width - width - control.rightPadding
        y: parent.height / 2 - height / 2
        radius: 13
        color: "transparent"
        border.color: control.down ? "#17a81a" : "#21be2b"

        Rectangle {
            width: 14
            height: 14
            x: 6
            y: 6
            radius: 7
            color: control.down ? "#17a81a" : "#21be2b"
            visible: control.checked
        }
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        visible: control.down || control.highlighted
        color: control.down ? "#bdbebf" : "#eeeeee"
    }
}
```

### RangeSlider

[RangeSlider](https://doc.qt.io/qt-6/qml-qtquick-controls-rangeslider.html) 由三个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [first.handle](https://doc.qt.io/qt-6/qml-qtquick-controls-rangeslider.html#first-prop) and [second.handle](https://doc.qt.io/qt-6/qml-qtquick-controls-rangeslider.html#second.handle-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-rangeslider-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

RangeSlider {
    id: control
    first.value: 0.25
    second.value: 0.75

    background: Rectangle {
        x: control.leftPadding
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 4
        width: control.availableWidth
        height: implicitHeight
        radius: 2
        color: "#bdbebf"

        Rectangle {
            x: control.first.visualPosition * parent.width
            width: control.second.visualPosition * parent.width - x
            height: parent.height
            color: "#21be2b"
            radius: 2
        }
    }

    first.handle: Rectangle {
        x: control.leftPadding + control.first.visualPosition * (control.availableWidth - width)
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 26
        implicitHeight: 26
        radius: 13
        color: control.first.pressed ? "#f0f0f0" : "#f6f6f6"
        border.color: "#bdbebf"
    }

    second.handle: Rectangle {
        x: control.leftPadding + control.second.visualPosition * (control.availableWidth - width)
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 26
        implicitHeight: 26
        radius: 13
        color: control.second.pressed ? "#f0f0f0" : "#f6f6f6"
        border.color: "#bdbebf"
    }
}
```

### ScrollBar

[ScrollBar](https://doc.qt.io/qt-6/qml-qtquick-controls-scrollbar.html) 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-scrollbar-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

ScrollBar {
    id: control
    size: 0.3
    position: 0.2
    active: true
    orientation: Qt.Vertical

    contentItem: Rectangle {
        implicitWidth: 6
        implicitHeight: 100
        radius: width / 2
        color: control.pressed ? "#81e889" : "#c2f4c6"
        // Hide the ScrollBar when it's not needed.
        opacity: control.policy === ScrollBar.AlwaysOn || (control.active && control.size < 1.0) ? 0.75 : 0

        // Animate the changes in opacity (default duration is 250 ms).
        Behavior on opacity {
            NumberAnimation {}
        }
    }
}
```

### ScrollIndicator

[ScrollIndicator](https://doc.qt.io/qt-6/qml-qtquick-controls-scrollindicator.html) 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-scrollbar-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

ScrollIndicator {
    id: control
    size: 0.3
    position: 0.2
    active: true
    orientation: Qt.Vertical

    contentItem: Rectangle {
        implicitWidth: 2
        implicitHeight: 100
        color: "#c2f4c6"
    }
}
```

### ScrollView

[ScrollView](https://doc.qt.io/qt-6/qml-qtquick-controls-scrollview.html) 由三个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) , horizontal and vertical scroll bars.

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-scrollview-custom.png)

```js
ScrollView {
    id: control

    width: 200
    height: 200
    focus: true

    Label {
        text: "ABC"
        font.pixelSize: 224
    }

    ScrollBar.vertical: ScrollBar {
        parent: control
        x: control.mirrored ? 0 : control.width - width
        y: control.topPadding
        height: control.availableHeight
        active: control.ScrollBar.horizontal.active
    }

    ScrollBar.horizontal: ScrollBar {
        parent: control
        x: control.leftPadding
        y: control.height - height
        width: control.availableWidth
        active: control.ScrollBar.vertical.active
    }

    background: Rectangle {
        border.color: control.activeFocus ? "#21be2b" : "#bdbebf"
    }
}
```

### Slider

Slider 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), and [handle](https://doc.qt.io/qt-6/qml-qtquick-controls-slider.html#handle-prop).

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-slider-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

Slider {
    id: control
    value: 0.5

    background: Rectangle {
        x: control.leftPadding
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 4
        width: control.availableWidth
        height: implicitHeight
        radius: 2
        color: "#bdbebf"

        Rectangle {
            width: control.visualPosition * parent.width
            height: parent.height
            color: "#21be2b"
            radius: 2
        }
    }

    handle: Rectangle {
        x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 26
        implicitHeight: 26
        radius: 13
        color: control.pressed ? "#f0f0f0" : "#f6f6f6"
        border.color: "#bdbebf"
    }
}
```

### SpinBox

[SpinBox](https://doc.qt.io/qt-6/qml-qtquick-controls-spinbox.html) 由四个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [contentItem](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop), [up indicator](https://doc.qt.io/qt-6/qml-qtquick-controls-spinbox.html#up.indicator-prop), and [down indicator](https://doc.qt.io/qt-6/qml-qtquick-controls-spinbox.html#down.indicator-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-spinbox-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

SpinBox {
    id: control
    value: 50
    editable: true

    contentItem: TextInput {
        z: 2
        text: control.textFromValue(control.value, control.locale)

        font: control.font
        color: "#21be2b"
        selectionColor: "#21be2b"
        selectedTextColor: "#ffffff"
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: Qt.ImhFormattedNumbersOnly
    }

    up.indicator: Rectangle {
        x: control.mirrored ? 0 : parent.width - width
        height: parent.height
        implicitWidth: 40
        implicitHeight: 40
        color: control.up.pressed ? "#e4e4e4" : "#f6f6f6"
        border.color: enabled ? "#21be2b" : "#bdbebf"

        Text {
            text: "+"
            font.pixelSize: control.font.pixelSize * 2
            color: "#21be2b"
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    down.indicator: Rectangle {
        x: control.mirrored ? parent.width - width : 0
        height: parent.height
        implicitWidth: 40
        implicitHeight: 40
        color: control.down.pressed ? "#e4e4e4" : "#f6f6f6"
        border.color: enabled ? "#21be2b" : "#bdbebf"

        Text {
            text: "-"
            font.pixelSize: control.font.pixelSize * 2
            color: "#21be2b"
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    background: Rectangle {
        implicitWidth: 140
        border.color: "#bdbebf"
    }
}
```

### SplitView

[SplitView](https://doc.qt.io/qt-6/qml-qtquick-controls-splitview.html) consists of a visual [handle](https://doc.qt.io/qt-6/qml-qtquick-controls-splitview.html#handle-prop) delegate.

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-splitview-custom.png)

```js
SplitView {
    id: splitView
    anchors.fill: parent

    handle: Rectangle {
        implicitWidth: 4
        implicitHeight: 4
        color: SplitHandle.pressed ? "#81e889"
            : (SplitHandle.hovered ? Qt.lighter("#c2f4c6", 1.1) : "#c2f4c6")
    }

    Rectangle {
        implicitWidth: 150
        color: "#444"
    }
    Rectangle {
        implicitWidth: 50
        color: "#666"
    }
}
```

### StackView

[StackView](https://doc.qt.io/qt-6/qml-qtquick-controls-stackview.html) 可以有一个 [background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop)，它允许自定义用于推送、弹出和替换操作（push, pop, and replace）的转换。

```js
import QtQuick
import QtQuick.Controls.Basic

StackView {
    id: control

    popEnter: Transition {
        XAnimator {
            from: (control.mirrored ? -1 : 1) * -control.width
            to: 0
            duration: 400
            easing.type: Easing.OutCubic
        }
    }

    popExit: Transition {
        XAnimator {
            from: 0
            to: (control.mirrored ? -1 : 1) * control.width
            duration: 400
            easing.type: Easing.OutCubic
        }
    }
}
```

### SwipeDelegate

[SwipeDelegate](https://doc.qt.io/qt-6/qml-qtquick-controls-swipedelegate.html) 由六个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop), [indicator](https://doc.qt.io/qt-6/qml-qtquick-controls-abstractbutton.html#indicator-prop), swipe.left, swipe.right, and swipe.behind.

```js
import QtQuick
import QtQuick.Controls.Basic

SwipeDelegate {
    id: control
    text: qsTr("SwipeDelegate")

    Component {
        id: component

        Rectangle {
            color: SwipeDelegate.pressed ? "#333" : "#444"
            width: parent.width
            height: parent.height
            clip: true

            Label {
                text: qsTr("Press me!")
                color: "#21be2b"
                anchors.centerIn: parent
            }
        }
    }

    swipe.left: component
    swipe.right: component

    contentItem: Text {
        text: control.text
        font: control.font
        color: control.enabled ? (control.down ? "#17a81a" : "#21be2b") : "#bdbebf"
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter

        Behavior on x {
            enabled: !control.down
            NumberAnimation {
                easing.type: Easing.InOutCubic
                duration: 400
            }
        }
    }
}
```

### SwipeView

[SwipeView](https://doc.qt.io/qt-6/qml-qtquick-controls-swipeview.html) 可以有一个 [background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop)。 导航（navigation）功能是使用 [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop) 实现的。

```js
import QtQuick
import QtQuick.Controls.Basic

SwipeView {
    id: control

    background: Rectangle {
        color: "#eeeeee"
    }
}
```

### Switch

Switch 由三个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop) and [indicator](https://doc.qt.io/qt-6/qml-qtquick-controls-abstractbutton.html#indicator-prop).

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-switch-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

Switch {
    id: control
    text: qsTr("Switch")

    indicator: Rectangle {
        implicitWidth: 48
        implicitHeight: 26
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 13
        color: control.checked ? "#17a81a" : "#ffffff"
        border.color: control.checked ? "#17a81a" : "#cccccc"

        Rectangle {
            x: control.checked ? parent.width - width : 0
            width: 26
            height: 26
            radius: 13
            color: control.down ? "#cccccc" : "#ffffff"
            border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#17a81a" : "#21be2b"
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }
}
```

### SwitchDelegate

[SwitchDelegate](https://doc.qt.io/qt-6/qml-qtquick-controls-switchdelegate.html) 由三个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [contentItem](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop) and [indicator](https://doc.qt.io/qt-6/qml-qtquick-controls-abstractbutton.html#indicator-prop).

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-switchdelegate-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

SwitchDelegate {
    id: control
    text: qsTr("SwitchDelegate")
    checked: true

    contentItem: Text {
        rightPadding: control.indicator.width + control.spacing
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#17a81a" : "#21be2b"
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
    }

    indicator: Rectangle {
        implicitWidth: 48
        implicitHeight: 26
        x: control.width - width - control.rightPadding
        y: parent.height / 2 - height / 2
        radius: 13
        color: control.checked ? "#17a81a" : "transparent"
        border.color: control.checked ? "#17a81a" : "#cccccc"

        Rectangle {
            x: control.checked ? parent.width - width : 0
            width: 26
            height: 26
            radius: 13
            color: control.down ? "#cccccc" : "#ffffff"
            border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
        }
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        visible: control.down || control.highlighted
        color: control.down ? "#bdbebf" : "#eeeeee"
    }
}
```

### TabBar

[TabBar](https://doc.qt.io/qt-6/qml-qtquick-controls-tabbar.html) 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), and [contentItem](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop).

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-tabbar-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

TabBar {
    id: control

    background: Rectangle {
        color: "#eeeeee"
    }

    TabButton {
        text: qsTr("Home")
    }
    TabButton {
        text: qsTr("Discover")
    }
    TabButton {
        text: qsTr("Activity")
    }
}
```

### ToolButton

[ToolButton](https://doc.qt.io/qt-6/qml-qtquick-controls-toolbutton.html) 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop)

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-toolbutton-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

ToolButton {
    id: control
    text: qsTr("ToolButton")
    width: 120

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#17a81a" : "#21be2b"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 40
        implicitHeight: 40
        color: Qt.darker("#33333333", control.enabled && (control.checked || control.highlighted) ? 1.5 : 1.0)
        opacity: enabled ? 1 : 0.3
        visible: control.down || (control.enabled && (control.checked || control.highlighted))
    }
}
```

### ToolSeparator

[ToolSeparator](https://doc.qt.io/qt-6/qml-qtquick-controls-toolseparator.html) 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop).

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-toolseparator-custom.png)

```js
ToolBar {
    RowLayout {
        anchors.fill: parent

        ToolButton {
            text: qsTr("Action 1")
        }
        ToolButton {
            text: qsTr("Action 2")
        }

        ToolSeparator {
            padding: vertical ? 10 : 2
            topPadding: vertical ? 2 : 10
            bottomPadding: vertical ? 2 : 10

            contentItem: Rectangle {
                implicitWidth: parent.vertical ? 1 : 24
                implicitHeight: parent.vertical ? 24 : 1
                color: "#c3c3c3"
            }
        }

        ToolButton {
            text: qsTr("Action 3")
        }
        ToolButton {
            text: qsTr("Action 4")
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
```

### ToolTip

[ToolTip](https://doc.qt.io/qt-6/qml-qtquick-controls-tooltip.html) 由两个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-popup.html#background-prop) and [content item](https://doc.qt.io/qt-6/qml-qtquick-controls-popup.html#contentItem-prop).

```js
import QtQuick
import QtQuick.Controls.Basic

ToolTip {
    id: control
    text: qsTr("A descriptive tool tip of what the button does")

    contentItem: Text {
        text: control.text
        font: control.font
        color: "#21be2b"
    }

    background: Rectangle {
        border.color: "#21be2b"
    }
}
```

注意：要自定义附加的工具提示（[attached ToolTip](https://doc.qt.io/qt-6/qml-qtquick-controls-tooltip.html#attached-tool-tips)），必须将其作为自定义样式的一部分提供。要对工具提示进行一次性自定义，请参阅自定义工具提示（[Custom Tool Tips](https://doc.qt.io/qt-6/qml-qtquick-controls-tooltip.html#custom-tool-tips)）。

### Tumbler

Tumbler 由三个视觉项组成：[background](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#background-prop), [contentItem](https://doc.qt.io/qt-6/qml-qtquick-controls-control.html#contentItem-prop), and [delegate](https://doc.qt.io/qt-6/qml-qtquick-controls-tumbler.html#delegate-prop).

![img](/resources/QtQuick/CustomControlStyle/qtquickcontrols-tumbler-custom.png)

```js
import QtQuick
import QtQuick.Controls.Basic

Tumbler {
    id: control
    model: 15

    background: Item {
        Rectangle {
            opacity: control.enabled ? 0.2 : 0.1
            border.color: "#000000"
            width: parent.width
            height: 1
            anchors.top: parent.top
        }

        Rectangle {
            opacity: control.enabled ? 0.2 : 0.1
            border.color: "#000000"
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
        }
    }

    delegate: Text {
        text: qsTr("Item %1").arg(modelData + 1)
        font: control.font
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        opacity: 1.0 - Math.abs(Tumbler.displacement) / (control.visibleItemCount / 2)

        required property var modelData
        required property int index
    }

    Rectangle {
        anchors.horizontalCenter: control.horizontalCenter
        y: control.height * 0.4
        width: 40
        height: 1
        color: "#21be2b"
    }

    Rectangle {
        anchors.horizontalCenter: control.horizontalCenter
        y: control.height * 0.6
        width: 40
        height: 1
        color: "#21be2b"
    }
}
```

如果想定义自己的 contentItem，请使用 [ListView](https://doc.qt.io/qt-6/qml-qtquick-listview.html) 或 [PathView](https://doc.qt.io/qt-6/qml-qtquick-pathview.html) 作为 root item。对于 wrapping Tumbler，请使用 [PathView](https://doc.qt.io/qt-6/qml-qtquick-pathview.html)：

```js
Tumbler {
    id: tumbler

    contentItem: PathView {
        id: pathView
        model: tumbler.model
        delegate: tumbler.delegate
        clip: true
        pathItemCount: tumbler.visibleItemCount + 1
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        dragMargin: width / 2

        path: Path {
            startX: pathView.width / 2
            startY: -pathView.delegateHeight / 2
            PathLine {
                x: pathView.width / 2
                y: pathView.pathItemCount * pathView.delegateHeight - pathView.delegateHeight / 2
            }
        }

        property real delegateHeight: tumbler.availableHeight / tumbler.visibleItemCount
    }
}
```

对于 non-wrapping Tumbler，请使用 [ListView](https://doc.qt.io/qt-6/qml-qtquick-listview.html)：

```js
Tumbler {
    id: tumbler

    contentItem: ListView {
        model: tumbler.model
        delegate: tumbler.delegate

        snapMode: ListView.SnapToItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: height / 2 - (height / tumbler.visibleItemCount / 2)
        preferredHighlightEnd: height / 2 + (height / tumbler.visibleItemCount / 2)
        clip: true
    }
}
```

<br/>

## 参考

* [Styling Qt Quick Controls | Qt Quick Controls 风格](https://doc.qt.io/qt-6/qtquickcontrols-styles.html)
* [Customizing Qt Quick Controls | 自定义 Qt Quick 控件](https://doc.qt.io/qt-6/qtquickcontrols-customize.html)

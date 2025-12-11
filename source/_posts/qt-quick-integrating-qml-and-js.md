---
title: QtQuick QML&JS混合编程
date: 2024-06-24 15:00:00
toc: true
categories:
  - tech
tags:
  - Qt
---

QML 语言使用类似 JSON 的语法，并允许将各种表达式和方法定义为 JavaScript 函数。它还允许用户导入 JavaScript 文件并使用这些导入提供的功能。

这使得开发人员和设计人员能够利用他们所拥有的 JavaScript 知识来快速开发用户界面和应用程序逻辑。

<!--more-->

<br/>

QML 与 JavaScript 深度集成，允许在 JavaScript 中定义信号处理程序和方法。QML 的另一个核心功能是使用属性绑定（也是使用 JavaScript 定义）来指定和执行对象属性之间的关系。

<br/>

## JavaScript 主机环境

QML 提供了专为编写 QML 应用程序而定制的 JavaScript 主机环境。该环境不同于浏览器提供的主机环境或服务器端 JavaScript 环境（例如 Node.js）。例如，QML 不提供浏览器环境中常见的窗口对象或 DOM API。

### 共同基础

与浏览器或服务器端 JavaScript 环境一样，QML 运行时实现 [ECMAScript 语言规范标准](https://262.ecma-international.org/7.0/)。这提供了对标准定义的所有内置类型和函数的访问，例如对象、数组和数学。 QML 运行时实现了该标准的第七版。

标准 ECMAScript 内置函数没有明确记录在 QML 文档中。有关其使用的更多信息，需要参阅 ECMA-262 第 7 版标准或众多在线 JavaScript 参考和教程网站，例如 [W3Schools JavaScript Reference](https://www.w3schools.com/jsref/default.asp)（JavaScript 部分）。

### QML 全局对象

QML JavaScript 宿主环境实现以下宿主对象和函数。它们是内置的，因此可以从 QML 中加载的任何 JavaScript 代码中使用它们，而无需额外导入：

* [Qt object](https://doc.qt.io/qt-6/qml-qtqml-qt.html)：提供特定于 QML 环境的辅助方法和属性的 QML 对象。
* [qsTr](https://doc.qt.io/qt-6/qml-qtqml-qt.html#qsTr-method)(), [qsTranslate](https://doc.qt.io/qt-6/qml-qtqml-qt.html#qsTranslate-method)(), [qsTrId](https://doc.qt.io/qt-6/qml-qtqml-qt.html#qsTrId-method)(), [QT_TR_NOOP()](https://doc.qt.io/qt-6/qml-qtqml-qt.html#QT_TR_NOOP-method)(), [QT_TRANSLATE_NOOP](https://doc.qt.io/qt-6/qml-qtqml-qt.html#QT_TRANSLATE_NOOP-method)(), [QT_TRID_NOOP](https://doc.qt.io/qt-6/qml-qtqml-qt.html#QT_TRID_NOOP-method)()  函数：可让在 QML 环境中翻译字符串和字符串文字。
* gc() 函数：手动触发垃圾收集的 QML 函数。
* print() 函数：将输出打印到控制台的 QML 函数。
* [console object](https://doc.qt.io/qt-6/qtquick-debugging.html#console-api)：实现 [FireBug Console API](http://getfirebug.com/wiki/index.php/Console_API) 的子集。
* [XMLHttpRequest](https://doc.qt.io/qt-6/qml-qtqml-xmlhttprequest.html)、DOMException：实现 [W3C XMLHttpRequest specification](http://www.w3.org/TR/XMLHttpRequest/) 的子集。

### JavaScript 对象和函数

QML 引擎支持的 JavaScript 对象、函数和属性的列表可以在 [List of JavaScript Objects and Functions](https://doc.qt.io/qt-6/qtqml-javascript-functionlist.html) 中找到。有关详细说明，参阅 ECMA-262 规范。

QML 对本机对象进行了以下修改：

* [arg()](https://doc.qt.io/qt-6/qml-string.html) 函数被添加到 String 原型中。

* 区域设置感知转换函数被添加到 [Date](https://doc.qt.io/qt-6/qml-qtqml-date.html) 和 [Number](https://doc.qt.io/qt-6/qml-qtqml-number.html) 原型中。

* 扩展了instanceof 函数的行为，以允许针对QML 类型进行类型检查。例如：

  ```js
  var v = something();
  if (!v instanceof Item) {
      throw new TypeError("I need an Item type!");
  }
  ```

### JavaScript 环境限制

QML 对 JavaScript 代码实施以下限制：

* .qml 文件中编写的 JavaScript 代码无法修改全局对象。 .js 文件中的 JavaScript 代码可以修改全局对象，导入时这些修改将对 .qml 文件可见。

  * 在 QML 中，全局对象是常量 - 无法修改或删除现有属性，并且不能创建新属性。
  * 大多数 JavaScript 程序不会有意修改全局对象。然而，JavaScript 自动创建未声明的变量是对全局对象的隐式修改，在 QML 中是被禁止的。
  * 任何修改全局对象的尝试（无论是隐式的还是显式的）都将导致异常。如果未捕获，这将导致打印警告，其中包括违规代码的文件和行号。

* 全局代码运行在受限的范围。

  * 在启动期间，如果 QML 文件包含带有“全局”代码的外部 JavaScript 文件，则该文件将在仅包含外部文件本身和全局对象的范围内执行。也就是说，它将无法访问通常可以访问的 QML 对象和属性。
  * 允许仅访问脚本局部变量的全局代码。

* 在大多数情况下，this 在 QML 为 undefined。

  * 从 JavaScript 绑定属性时支持 this 关键字。在 QML 绑定表达式、QML 信号处理程序和 QML 声明函数中，this 指作用域对象（Scope Object）。在所有其他情况下，该值在 QML 中未定义。

  * 要引用特定对象，请提供 id。例如：

    ```js
    Item {
        width: 200; height: 100
        function mouseAreaClicked(area) {
            console.log("Clicked in area at: " + area.x + ", " + area.y);
        }
        // This will pass area to the function
        MouseArea {
            id: area
            y: 50; height: 50; width: 200
            onClicked: mouseAreaClicked(area)
        }
    }
    ```


<br/>

## JavaScript 表达式

QML 提供的 JavaScript 主机环境可以运行有效的标准 JavaScript 结构，例如条件运算符、数组、变量设置和循环。除了标准 JavaScript 属性之外，QML 全局对象还包括许多帮助程序方法，可简化 UI 的构建以及与 QML 环境的交互。

QML 文档的各个部分都可以包含 JavaScript 代码：

1. 属性绑定的主体。这些 JavaScript 表达式描述了 QML 对象属性之间的关系。当属性的依赖关系发生变化时，该属性也会根据指定的关系自动更新。
2. 信号处理程序的主体。每当 QML 对象发出关联信号时，就会自动评估这些 JavaScript 语句。
3. 自定义方法的定义。在 QML 对象主体中定义的 JavaScript 函数成为该对象的方法。
4. 独立的 JavaScript 资源 (.js) 文件。这些文件实际上与 QML 文档分开，但它们可以导入到 QML 文档中。导入文件中定义的函数和变量可以在属性绑定、信号处理程序和自定义方法中使用。

### 属性绑定中的 JavaScript

在下面的示例中，矩形的颜色属性取决于 TapHandler 的按下属性。这种关系用条件表达式来描述：

```js
import QtQuick 2.12

Rectangle {
    id: colorbutton
    width: 200; height: 80;

    color: inputHandler.pressed ? "steelblue" : "lightsteelblue"

    TapHandler {
        id: inputHandler
    }
}
```

事实上，任何 JavaScript 表达式（无论多么复杂）都可以在属性绑定定义中使用，只要表达式的结果是一个其类型可以分配给属性的值。这包括 side effects。但是，不鼓励复杂的绑定和 side effects，因为它们会降低代码的性能、可读性和可维护性。

定义属性绑定有两种方法：最常见的一种如前面的示例所示，在属性初始化中。第二种（也是更罕见的）方法是从命令式 JavaScript 代码中将 Qt.binding() 函数返回的函数分配给属性，如下所示：

```js
import QtQuick 2.12

Rectangle {
    id: colorbutton
    width: 200; height: 80;

    color: "red"

    TapHandler {
        id: inputHandler
    }

    Component.onCompleted: {
        color = Qt.binding(function() { return inputHandler.pressed ? "steelblue" : "lightsteelblue" });
    }
}
```

有关如何定义属性绑定的详细信息，请参阅属性绑定文档（[property bindings](https://doc.qt.io/qt-6/qtqml-syntax-propertybinding.html)）；有关绑定与值分配有何不同的信息，请参阅有关属性分配与属性绑定的文档（[Property Assignment versus Property Binding](https://doc.qt.io/qt-6/qtqml-syntax-propertybinding.html#qml-javascript-assignment)）。

### 信号处理程序中的 JavaScript

QML 对象类型可以发出信号以响应发生的某些事件。这些信号可以由信号处理函数处理，信号处理函数可以由客户端定义以实现自定义程序逻辑。

假设由 Rectangle 类型表示的按钮具有 [TapHandler](https://doc.qt.io/qt-6/qml-qtquick-taphandler.html) 和 Text 标签。当用户按下按钮时，TapHandler 会发出 [tapped](https://doc.qt.io/qt-6/qml-qtquick-taphandler.html#tapped-signal) 信号。客户端可以使用 JavaScript 表达式对 onTapped 处理程序中的信号做出反应。 QML 引擎根据需要执行处理程序中定义的这些 JavaScript 表达式。通常，信号处理程序绑定到 JavaScript 表达式以启动其他事件或分配属性值。

```js
import QtQuick 2.12

Rectangle {
    id: button
    width: 200; height: 80; color: "lightsteelblue"

    TapHandler {
        id: inputHandler
        onTapped: {
            // arbitrary JavaScript expression
            console.log("Tapped!")
        }
    }

    Text {
        id: label
        anchors.centerIn: parent
        text: inputHandler.pressed ? "Pressed!" : "Press here!"
    }
}
```

有关信号和信号处理程序的更多详细信息，请参阅以下主题：

* [Signal and Handler Event System](https://doc.qt.io/qt-6/qtqml-syntax-signals.html)
* [QML Object Attributes](https://doc.qt.io/qt-6/qtqml-syntax-objectattributes.html)

### 独立函数中的 JavaScript

程序逻辑也可以在 JavaScript 函数中定义。这些函数可以在 QML 文档中内联定义（作为自定义方法），也可以在导入的 JavaScript 文件中外部定义。

#### 自定义方法中的 JavaScript

自定义方法可以在 QML 文档中定义，并且可以从信号处理程序、属性绑定或其他 QML 对象中的函数调用。此类方法通常称为内联 JavaScript 函数，因为它们的实现包含在 QML 对象类型定义（QML 文档）中，而不是包含在外部 JavaScript 文件中。

内联自定义方法的示例如下：

```js
import QtQuick 2.12

Item {
    function fibonacci(n){
        var arr = [0, 1];
        for (var i = 2; i < n + 1; i++)
            arr.push(arr[i - 2] + arr[i -1]);

        return arr;
    }
    TapHandler {
        onTapped: console.log(fibonacci(10))
    }
}
```

每当 TapHandler 发出点击信号时，斐波那契函数就会运行。

注意：QML 文档中内联定义的自定义方法会暴露给其他对象，因此 QML 组件中根对象上的内联函数可以由组件外部的调用者调用。如果不需要，可以将该方法添加到非根对象中，或者最好将其写入外部 JavaScript 文件中。

有关使用 JavaScript 在 QML 中定义自定义方法的更多信息，请参阅 QML 对象属性文档（[QML Object Attributes](https://doc.qt.io/qt-6/qtqml-syntax-objectattributes.html)）。

### JavaScript 文件中定义的函数

重要的程序逻辑最好分成单独的 JavaScript 文件。可以使用 import 语句将该文件导入到 QML 中，就像 QML 模块一样。

例如，前面示例中的 fibonacci() 方法可以移动到名为 fib.js 的外部文件中，并按如下方式访问：

```js
import QtQuick 2.12
import "fib.js" as MathFunctions

Item {
    TapHandler {
        onTapped: console.log(MathFunctions.fibonacci(10))
    }
}
```



JavaScript 资源可以通过 QML 文档和其他 JavaScript 资源导入。 JavaScript 资源可以通过相对或绝对 URL 导入。如果是相对 URL，则相对于包含导入的 QML 文档或 JavaScript 资源的位置来解析位置。如果脚本文件不可访问，则会发生错误。如果需要从网络资源获取 JavaScript，则组件的状态将设置为“正在加载”，直到下载脚本为止。

JavaScript 资源还可以导入 QML 模块和其他 JavaScript 资源。 JavaScript 资源中的 import 语句的语法与 QML 文档中的 import 语句略有不同，下面详细记录了 QML 文档中的 import 语句。

#### 从 QML 文档导入 JavaScript 资源

QML 文档可以使用以下语法导入 JavaScript 资源：

```js
import "ResourceURL" as Qualifier
```

例如：

```js
import "jsfile.js" as Logic
```

有关将外部 JavaScript 文件加载到 QML 中的更多信息，请阅读 JavaScript 导入部分。

#### 将信号连接到 JavaScript 函数

发出信号的 QML 对象类型还为其信号提供默认信号处理程序，如上一节所述。然而，有时，当另一个 QML 对象发出信号时，客户端希望触发 QML 对象中定义的函数。这种情况可以通过信号连接来处理。

QML 对象发出的信号可以通过调用信号的 connect() 方法并将 JavaScript 函数作为参数传递来连接到 JavaScript 函数。例如，以下代码将 TapHandler 的 tapped 信号连接到 script.js 中的 jsFunction()：

```js
import QtQuick
import "script.js" as MyScript

Item {
    id: item
    width: 200; height: 200

    TapHandler {
        id: inputHandler
    }

    Component.onCompleted: {
        inputHandler.tapped.connect(MyScript.jsFunction)
    }
}
```

script.js:

```js
function jsFunction() {
    console.log("Called JavaScript function!")
}
```

每当 TapHandler 的点击信号发出时，就会调用 jsFunction()。

有关更多信息，请参阅将信号连接到方法和信号（[Connecting Signals to Methods and Signals](https://doc.qt.io/qt-6/qtqml-syntax-signals.html)）。

<br/>

## 应用程序启动代码中的 JavaScript

有时需要在应用程序（或组件实例）启动时运行一些命令式代码。虽然很容易将启动脚本作为全局代码包含在外部脚本文件中，但这可能会产生严重的限制，因为 QML 环境可能尚未完全建立。例如，某些对象可能尚未创建，或者某些属性绑定可能尚未建立。有关全局脚本代码的确切限制，请参阅 JavaScript 环境限制。

QML 对象在实例化完成时会发出 Component.completed [attached signal](https://doc.qt.io/qt-6/qtqml-syntax-signals.html#attached-signal-handlers)。相应的 Component.onCompleted 处理程序中的 JavaScript 代码在对象实例化后运行。因此，编写应用程序启动代码的最佳位置是顶级对象的 Component.onCompleted 处理程序中，因为当 QML 环境完全建立时，该对象会发出 Component.completed 。

例如：

```js
import QtQuick 2.0

Rectangle {
    function startupFunction() {
        // ... startup code
    }

    Component.onCompleted: startupFunction();
}
```

QML 文件中的任何对象（包括嵌套对象和嵌套 QML 组件实例）都可以使用此附加属性。如果启动时要执行多个 onCompleted() 处理程序，它们将以未定义的顺序依次运行。

同样，每个组件在被销毁之前都会发出一个 [destruction](https://doc.qt.io/qt-6/qml-qtqml-component.html#destruction-signal)() 信号。

<br/>

## JavaScript 导入

导入的 JavaScript 资源始终使用“as”关键字进行限定。 JavaScript 资源的限定符必须以大写字母开头，并且必须是唯一的，因此限定符和 JavaScript 文件之间始终存在一对一的映射。 （这也意味着限定符不能与内置 JavaScript 对象（例如日期和数学）命名相同）。

通过 `Qualifier.functionName(params)` 语法，导入的 JavaScript 文件中定义的函数可用于导入的 QML 文档中定义的对象。 JavaScript 资源中的函数可以采用参数，其类型可以是任何 QML 值类型或对象类型，以及普通的 JavaScript 类型。从 QML 调用此类函数时，普通数据类型转换规则将应用于参数和返回值。

### JavaScript 资源中的导入

在 QtQuick 2.0 中，添加了支持，允许 JavaScript 资源使用标准 QML 导入语法的变体（其中适用所有先前描述的规则和限定）导入其他 JavaScript 资源以及 QML 类型命名空间。

由于 JavaScript 资源能够在 QtQuick 2.0 中以这种方式导入另一个脚本或 QML 模块，因此定义了一些额外的语义：

* 带有导入的脚本不会从导入它的 QML 文档继承导入（例如，访问 Component.errorString 将失败）
* 没有导入的脚本将从导入它的 QML 文档继承导入（例如，访问 Component.errorString 将成功）
* 共享脚本（即定义为 .pragma 库）不会继承任何 QML 文档的导入，即使它没有导入其他脚本或模块

第一个语义在概念上是正确的，因为特定脚本可以由任意数量的 QML 文件导入。出于向后兼容性的目的，保留第二个语义。第三种语义与共享脚本的当前语义保持不变，但在这里针对新的可能情况（脚本导入其他脚本或模块）进行了澄清。

### 从另一个 JavaScript 资源导入 JavaScript 资源

JavaScript 资源可以通过以下方式导入另一个资源：

```js
import * as MathFunctions from "factorial.mjs";
```

或者：

```js
.import "filename.js" as Qualifier
```

前者是用于导入 ECMAScript 模块的标准 ECMAScript 语法，并且仅适用于 ECMAScript 模块内，如 mjs 文件扩展名所示。后者是 QML 引擎提供的 JavaScript 扩展，也适用于非模块。作为被 ECMAScript 标准取代的扩展，不鼓励使用它。

当以这种方式导入 JavaScript 文件时，会使用限定符导入该文件。然后，可以通过限定符（即 Qualifier.functionName(params)）从导入脚本访问该文件中的函数。

有时，希望函数在导入上下文中可用，而不需要限定它们。在这种情况下，应使用不带 as 限定符的 ECMAScript 模块和 JavaScript import 语句。

例如，下方的 QML 代码调用 script.mjs 中的 showCalculations()，而 script.mjs 又可以调用 Factorial.mjs 中的 Factorial()，因为它已使用 import 包含了 Factorial.mjs。

```js
import QtQuick
import "script.mjs" as MyScript

Item {
    width: 100; height: 100

    MouseArea {
        anchors.fill: parent
        onClicked: {
            MyScript.showCalculations(10)
            console.log("Call factorial() from QML:",
                MyScript.factorial(10))
        }
    }
}
```

script.mjs:

```js
import { factorial } from "factorial.mjs"
export { factorial }

export function showCalculations(value) {
    console.log(
        "Call factorial() from script.js:",
        factorial(value));
}
```

factorial.mjs:

```js
export function factorial(a) {
    a = parseInt(a);
    if (a <= 0)
        return 1;
    else
        return a * factorial(a - 1);
}
```

### 从 JavaScript 资源导入 QML 模块

JavaScript 资源可以通过以下方式导入 QML 模块：

```js
.import TypeNamespace MajorVersion.MinorVersion as Qualifier
```

您可以在下面看到一个示例，该示例还展示了如何使用 javascript 中导入的模块中的 QML 类型：

```js
.import Qt.test 1.0 as JsQtTest

var importedEnumValue = JsQtTest.MyQmlObject.EnumValue3
```

特别是，这对于访问通过单例类型提供的功能可能很有用；有关详细信息，请参阅 [QML_SINGLETON](https://doc.qt.io/qt-6/qqmlengine.html#QML_SINGLETON)。

默认情况下，JavaScript 资源可以访问导入该资源的组件的所有导入。如果它被声明为无状态库（使用 .pragma 库）或包含显式 .import 语句，则它无权访问组件的导入。

注意：.import 语法不适用于 [WorkerScript](https://doc.qt.io/qt-6/qml-qtqml-workerscript-workerscript.html) 中使用的脚本

另请参阅在 QML 中定义 JavaScript 资源（[Defining JavaScript Resources in QML](https://doc.qt.io/qt-6/qtqml-javascript-resources.html)）。

<br/>

## JavaScript 资源

QML 应用程序的程序逻辑可以用 JavaScript 定义。 JavaScript 代码可以在 QML 文档中内嵌定义，也可以分为 JavaScript 文件（在 QML 中称为 JavaScript 资源）。

QML 支持两种不同类型的 JavaScript 资源：代码隐藏实现文件和共享（库）文件。这两种 JavaScript 资源都可以由其他 JavaScript 资源导入，或者包含在 QML 模块中。

### 代码隐藏实现资源

大多数导入到 QML 文档中的 JavaScript 文件都是导入它们的 QML 文档的有状态实现。在这些情况下，文档中定义的 QML 对象类型的每个实例都需要 JavaScript 对象和状态的单独副本才能正确运行。

导入 JavaScript 文件时的默认行为是为每个 QML 组件实例提供唯一的、独立的副本。如果该 JavaScript 文件未使用 .import 语句导入任何资源或模块，则其代码将在与 QML 组件实例相同的范围内运行，因此可以访问和操作该 QML 组件中声明的对象和属性。否则，它将有自己独特的范围，并且 QML 组件的对象和属性应该作为参数传递给 JavaScript 文件的函数（如果需要）。

代码隐藏实现资源的示例如下：

```js
// MyButton.qml
import QtQuick 2.0
import "my_button_impl.js" as Logic // A new instance of this JavaScript resource
                                    // is loaded for each instance of Button.qml.

Rectangle {
    id: rect
    width: 200
    height: 100
    color: "red"

    MouseArea {
        id: mousearea
        anchors.fill: parent
        onClicked: Logic.onClicked(rect)
    }
}
```

my_button_impl.js:

```js
var clickCount = 0;   // this state is separate for each instance of MyButton
function onClicked(button) {
    clickCount += 1;
    if ((clickCount % 5) == 0) {
        button.color = Qt.rgba(1,0,0,1);
    } else {
        button.color = Qt.rgba(0,1,0,1);
    }
}
```

一般来说，简单的逻辑应该在 QML 文件中内嵌定义，但更复杂的逻辑应该分离到代码隐藏实现资源中，以提高可维护性和可读性。

### 共享 JavaScript 资源（库）

默认情况下，从 QML 导入的 JavaScript 文件与 QML 组件共享其上下文。这意味着 JavaScript 文件可以访问相同的 QML 对象并可以修改它们。因此，每次导入都必须有这些文件的唯一副本。

上一节介绍了 JavaScript 文件的有状态导入。但是，某些 JavaScript 文件是无状态的，其行为更像是可重用的库，因为它们提供了一组辅助函数，不需要从导入位置获取任何内容。如果您使用特殊的编译指示标记此类库，则可以节省大量内存并加快 QML 组件的实例化速度，如以下示例所示。

```js
// factorial.js
.pragma library

var factorialCount = 0;

function factorial(a) {
    a = parseInt(a);

    // factorial recursion
    if (a > 0)
        return a * factorial(a - 1);

    // shared state
    factorialCount += 1;

    // recursion base-case.
    return 1;
}

function factorialCallCount() {
    return factorialCount;
}
```

`.pragma` 声明必须出现在任何 JavaScript 代码（注释除外）之前。

由于它们是共享的，`.pragma` 库文件无法直接访问 QML 组件实例对象或属性，尽管 QML 值可以作为函数参数传递。

<br/>

## 从 JavaScript 动态创建 QML 对象

QML 支持在 JavaScript 中动态创建对象。这对于将对象的实例化延迟到必要时非常有用，从而缩短应用程序的启动时间。它还允许动态创建视觉对象并将其添加到场景中以响应用户输入或其他事件。

可以通过两种主要方式使用此功能：动态创建对象 和 动态创建组件。

### 动态创建对象

有两种方法可以从 JavaScript 动态创建对象。可以调用 [Qt.createComponent](https://doc.qt.io/qt-6/qml-qtqml-qt.html#createComponent-method)() 动态创建 [Component](https://doc.qt.io/qt-6/qml-qtqml-component.html) 对象，也可以使用 [Qt.createQmlObject](https://doc.qt.io/qt-6/qml-qtqml-qt.html#createQmlObject-method)() 从 QML 字符串创建对象。

如果在 QML 文档中定义了现有组件并且想要动态创建该组件的实例，那么创建组件会更好。另外，当对象 QML 本身是在运行时生成时，从 QML 字符串创建对象非常有用。

### 动态创建组件

要动态加载 QML 文件中定义的组件，请在 [Qt object](https://doc.qt.io/qt-6/qml-qtqml-qt.html) 中调用 [Qt.createComponent](https://doc.qt.io/qt-6/qml-qtqml-qt.html#createComponent-method)() 函数。该函数将 QML 文件的 URL 作为其唯一参数，并从该 URL 创建一个 [Component](https://doc.qt.io/qt-6/qml-qtqml-component.html) 对象。

一旦你有了一个组件，你就可以调用它的 [createObject](https://doc.qt.io/qt-6/qml-qtqml-component.html#createObject-method)() 方法来创建该组件的实例。该函数可以接受一个或两个参数：

* 第一个是新对象的父对象。父对象可以是图形对象（即 [Item](https://doc.qt.io/qt-6/qml-qtquick-item.html) 类型）或非图形对象（即 [QtObject](https://doc.qt.io/qt-6/qml-qtqml-qtobject.html) 或 C++ [QObject](https://doc.qt.io/qt-6/qobject.html) 类型）。只有具有图形父对象的图形对象才会渲染到 Qt Quick 视觉画布上。如果希望稍后设置父级，可以安全地将 null 传递给此函数。
* 第二个是可选的，是属性值对的映射，用于定义对象的初始任何属性值。此参数指定的属性值在对象的创建完成之前应用于该对象，从而避免在必须初始化特定属性以启用其他属性绑定时可能发生的绑定错误。此外，与创建对象后定义属性值和绑定相比，性能优势较小。

这是一个例子。首先是 Sprite.qml，它定义了一个简单的 QML 组件：

```js
import QtQuick

Rectangle { width: 80; height: 50; color: "red" }
```

我们的主应用程序文件 main.qml 导入将创建 Sprite 对象的 componentCreation.js JavaScript 文件：

```js
import QtQuick
import "componentCreation.js" as MyScript

Rectangle {
    id: appWindow
    width: 300; height: 300

    Component.onCompleted: MyScript.createSpriteObjects();
}
```

这是 componentCreation.js。请注意，它会在调用 createObject() 之前检查组件状态是否为 Component.Ready，以防 QML 文件是通过网络加载的，因此尚未立即准备好。

```js
var component;
var sprite;

function createSpriteObjects() {
    component = Qt.createComponent("Sprite.qml");
    if (component.status == Component.Ready)
        finishCreation();
    else
        component.statusChanged.connect(finishCreation);
}

function finishCreation() {
    if (component.status == Component.Ready) {
        sprite = component.createObject(appWindow, {x: 100, y: 100});
        if (sprite == null) {
            // Error Handling
            console.log("Error creating object");
        }
    } else if (component.status == Component.Error) {
        // Error Handling
        console.log("Error loading component:", component.errorString());
    }
}
```

如果您确定要加载的 QML 文件是本地文件，您可以省略 finishCreation() 函数并立即调用 createObject() ：

```js
function createSpriteObjects() {
    component = Qt.createComponent("Sprite.qml");
    sprite = component.createObject(appWindow, {x: 100, y: 100});

    if (sprite == null) {
        // Error Handling
        console.log("Error creating object");
    }
}
```

请注意，在这两个实例中，createObject() 都是以 appWindow 作为父参数传递来调用的，因为动态创建的对象是可视 (Qt Quick) 对象。创建的对象将成为 main.qml 中 appWindow 对象的子对象，并出现在场景中。

当使用具有相对路径的文件时，该路径应该相对于执行 Qt.createComponent() 的文件。

要将信号连接到动态创建的对象（或从动态创建的对象接收信号），请使用 signal connect() 方法。有关更多信息，请参阅将信号连接到方法和信号（[Connecting Signals to Methods and Signals](https://doc.qt.io/qt-6/qtqml-syntax-signals.html#connecting-signals-to-methods-and-signals)）。

还可以通过 [incubateObject](https://doc.qt.io/qt-6/qml-qtqml-component.html#incubateObject-method)() 函数在不阻塞的情况下实例化组件。

### 从 QML 字符串创建对象

警告：从 QML 字符串创建对象非常慢，因为每次执行时引擎都必须编译传递的 QML 字符串。此外，以编程方式构建 QML 代码时很容易产生无效的 QML。将 QML 组件保留为单独的文件并添加属性和方法来自定义其行为比通过字符串操作生成新组件要好得多。

如果直到运行时才定义 QML，可以使用 [Qt.createQmlObject](https://doc.qt.io/qt-6/qml-qtqml-qt.html#createQmlObject-method)() 函数从 QML 字符串创建 QML 对象，如下例所示：

```js
const newObject = Qt.createQmlObject(`
    import QtQuick

    Rectangle {
        color: "red"
        width: 20
        height: 20
    }
    `,
    parentItem,
    "myDynamicSnippet"
);
```

第一个参数是要创建的 QML 字符串。就像在新文件中一样，您需要导入您想要使用的任何类型。第二个参数是新对象的父对象，适用于组件的父参数语义同样适用于 createQmlObject()。第三个参数是与新对象关联的文件路径；这用于错误报告。

如果 QML 字符串使用相对路径导入文件，则该路径应相对于定义父对象（该方法的第二个参数）的文件。

重要提示：构建静态 QML 应用程序时，会扫描 QML 文件以检测导入依赖项。这样，所有必需的插件和资源都会在编译时解析。但是，仅考虑显式导入语句（位于 QML 文件顶部的那些语句），而不考虑包含在字符串文字中的导入语句。因此，为了支持静态构建，您需要确保使用 Qt.createQmlObject() 的 QML 文件除了字符串文字内部之外，还明确在文件顶部包含所有必需的导入。

### 维护动态创建的对象

管理动态创建的对象时，必须确保创建上下文比创建的对象寿命更长。否则，如果创建上下文首先被销毁，动态对象中的绑定和信号处理程序将不再工作。

实际的创建上下文取决于对象的创建方式：

* 如果使用 [Qt.createComponent](https://doc.qt.io/qt-6/qml-qtqml-qt.html#createComponent-method)()，则创建上下文是调用该方法的 [QQmlContext](https://doc.qt.io/qt-6/qqmlcontext.html)
* 如果调用 [Qt.createQmlObject](https://doc.qt.io/qt-6/qml-qtqml-qt.html#createQmlObject-method)()，则创建上下文是传递给此方法的父对象的上下文
* 如果定义了 Component{} 对象并在该对象上调用了 [createObject](https://doc.qt.io/qt-6/qml-qtqml-component.html#createObject-method)() 或 [incubateObject](https://doc.qt.io/qt-6/qml-qtqml-component.html#incubateObject-method)()，则创建上下文就是定义该 Component 的上下文

另请注意，虽然动态创建的对象可以与其他对象一样使用，但它们在 QML 中没有 id。

### 动态删除对象

在许多用户界面中，将视觉对象的不透明度设置为 0 或将视觉对象移出屏幕而不是删除它就足够了。但是，如果您有大量动态创建的对象，则删除未使用的对象可能会带来显着的性能优势。

请注意，您永远不应该手动删除由便利的 QML 对象工厂（例如 [Loader](https://doc.qt.io/qt-6/qml-qtquick-loader.html) 和 [Repeater](https://doc.qt.io/qt-6/qml-qtquick-repeater.html)）动态创建的对象。另外，您应该避免删除不是您自己动态创建的对象。

可以使用 destroy() 方法删除项目。此方法有一个可选参数（默认为 0），指定对象被销毁之前的大约延迟时间（以毫秒为单位）。

这是一个例子。 application.qml 创建 SelfDestroyingRect.qml 组件的五个实例。每个实例都运行一个 [NumberAnimation](https://doc.qt.io/qt-6/qml-qtquick-numberanimation.html)，当动画完成时，在其根对象上调用 destroy() 来销毁自身：

```js
// application.qml
import QtQuick

Item {
    id: container
    width: 500; height: 100

    Component.onCompleted: {
        var component = Qt.createComponent("SelfDestroyingRect.qml");
        for (var i=0; i<5; i++) {
            var object = component.createObject(container);
            object.x = (object.width + 10) * i;
        }
    }
}
```

SelfDestroyingRect.qml:

```js
import QtQuick

Rectangle {
    id: rect
    width: 80; height: 80
    color: "red"

    NumberAnimation on opacity {
        to: 0
        duration: 1000

        onRunningChanged: {
            if (!running) {
                console.log("Destroying...")
                rect.destroy();
            }
        }
    }
}
```

或者，application.qml 可以通过调用 object.destroy() 来销毁创建的对象。

请注意，在该对象内的对象上调用 destroy() 是安全的。对象不会在调用 destroy() 时立即被销毁，而是会在该脚本块结束和下一帧之间的某个时间被清除（除非您指定了非零延迟）。

另请注意，如果 SelfDestroyingRect 实例是像这样静态创建的：

```js
Item {
    SelfDestroyingRect {
        // ...
    }
}
```

这会导致错误，因为对象只有在动态创建的情况下才能被动态销毁。

使用 Qt.createQmlObject() 创建的对象可以类似地使用 destroy() 销毁：

```js
const newObject = Qt.createQmlObject(`
    import QtQuick

    Rectangle {
        color: "red"
        width: 20
        height: 20
    }
    `,
    parentItem,
    "myDynamicSnippet"
);
newObject.destroy(1000);
```

<br/>

## 参考

* [JavaScript Host Environment | JavaScript 主机环境](https://doc.qt.io/qt-6/qtqml-javascript-hostenvironment.html)
* [Integrating QML and JavaScript | 集成 QML 和 JavaScript | Qt QML 6.7.2](https://doc.qt.io/qt-6/qtqml-javascript-topic.html)
* [JavaScript Expressions in QML Documents | QML 文档中的 JavaScript 表达式](https://doc.qt.io/qt-6/qtqml-javascript-expressions.html)
* [Importing JavaScript Resources in QML | 在 QML 中导入 JavaScript 资源](https://doc.qt.io/qt-6/qtqml-javascript-imports.html)
* [Defining JavaScript Resources in QML | 在 QML 中定义 JavaScript 资源](https://doc.qt.io/qt-6/qtqml-javascript-resources.html)
* [Configuring the JavaScript Engine | 配置 JavaScript 引擎](https://doc.qt.io/qt-6/qtqml-javascript-finetuning.html)
* [Dynamic QML Object Creation from JavaScript | 从 JavaScript 创建动态 QML 对象](https://doc.qt.io/qt-6/qtqml-javascript-dynamicobjectcreation.html)
* [JavaScript Tutorial (w3schools.com)](https://www.w3schools.com/js/default.asp)

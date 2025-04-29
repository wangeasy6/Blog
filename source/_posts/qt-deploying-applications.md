---
title: Qt 应用程序发布
date: 2024-07-19 17:21:00
toc: true
categories:
  - Technology
tags:
  - Qt
---

当我们使用 Debug 或 Release 模式编译出一个调试完成 Qt 程序，获取到的可执行文件往往还不能执行，获取一个完整的可供安装的文件集合往往还需要经过一些步骤。其中也有一些需要注意的知识点。

<!--more-->

<br/>

## 静态vs共享库

编译应用程序有两种方法，也决定了发布时的文件状态：

* **静态链接**：静态链接产生独立的可执行文件。优点是只需部署几个文件。缺点是可执行文件很大并且没有灵活性（升级应用程序或 Qt，将需要重复部署过程），并且无法部署插件。
* **共享库**：共享库产生可执行文件+所需共享库。Qt 共享库是通用的，多个应用程序可以使用相同的共享库，使用共享库则减少了文件大小。共享库提供了更小、更灵活的可执行文件，并且可以独立升级应用程序使用的 Qt 库。要部署基于插件的应用程序，需要使用共享库方法。共享库方法的缺点是需要部署更多的文件数。

### 静态链接编译

在Qt Creator中，需要修改项目文件（`.pro` 文件），添加 `CONFIG += static` 来指定使用静态库。

在项目文件中使用 `LIBS` 变量指定静态库的路径，例如：`LIBS += -L/path/to/static/libs -lmylibrary`。

注意事项：

* 确保静态库与目标平台和编译器兼容。
* 静态编译会增加可执行文件的大小，因为它包含了所有依赖的库文件。
* 某些库可能不允许静态链接，需要了解并遵守所有相关库的许可协议。

### 共享库编译

在编译过程中，可以指定 `-dynamic` 选项来生成共享库。

在 Qt Creator 或任何其他 IDE 中，配置项目以链接到Qt的共享库。这通常涉及到设置项目文件（`.pro`文件）中的 `CONFIG` 变量，例如：

```
CONFIG += dll
```

注意事项：

- 确保应用程序的部署包中包含了所有必要的库文件。
- 在不同的操作系统和平台上，共享库的部署和使用可能有所不同。
- 考虑使用Qt的安装器框架来简化应用程序的部署过程。
- 某些Qt模块可能不支持作为共享库使用，需要检查 Qt 的文档以确认。
- 确保遵守Qt的许可协议，特别是商业用途时。

<br/>

## 部署 Qt 的库

除非 Qt 是系统库的一部分，否则它必须与应用程序一起重新分发。最低限度是重新分配应用程序使用的库的运行时。然而，通过静态链接，Qt 运行时可以编译进可执行文件。

一般来说，应该部署 Qt 构建使用的所有插件，排除对应用程序及其用户不必要的插件。

例如，你可能需要部署 JPEG 支持和 SQL 驱动程序的插件，但你还应该部署用户可能需要的插件，包括可访问性的插件。有关插件的更多信息，请参阅 [如何 创建 Qt 插件](https://doc.qt.io/qt-6/plugins-howto.html)。

使用 Qt Qml 的应用程序还需要提供它们使用的 [QML 模块](https://doc.qt.io/qt-6/modules-qml.html)。

使用共享库方法部署应用程序时，必须确保 Qt 库使用正确的路径来查找 Qt 插件、文档、翻译等。为此，可以使用 qt.conf 文件，有关更多信息，请参阅使用 [qt.conf](https://doc.qt.io/qt-6/qt-conf.html)。

根据配置，还必须重新分发特定于编译器的库。

<br/>

## 许可证

Qt 的一些库基于第三方库，这些库未使用与 Qt 相同的双许可模型进行许可。因此，在部署使用这些库的应用程序时必须小心，特别是当应用程序静态链接到它们时。有关更多信息，请参阅 [Qt 中使用的第三方代码](https://doc.qt.io/qt-6/licenses-used-in-qt.html)。

[QtNetwork](https://doc.qt.io/qt-6/qtnetwork-module.html) 的某些配置在运行时使用 OpenSSL。 OpenSSL 库的部署受到许可和出口限制。有关详细信息，请参阅 [Secure Sockets Layer (SSL) Classes](https://doc.qt.io/qt-6/ssl.html)。

<br/>

## 特定平台注意事项

对于不同的平台，部署 Qt 应用程序的过程是不同的。

<br/>

### [X11 平台](https://doc.qt.io/qt-6/linux-deployment.html)

静态链接通常是在 Unix 上分发应用程序的最安全、最简单的方法，因为可以免于分发 Qt 库并确保它们位于目标系统上库的默认搜索路径中的任务。如果不带插件的话，建议使用静态链接的形式发布。

Unix 上没有标准的包管理，因此我们下面介绍的方法是一个通用的解决方案。有关如何创建包的信息，请参阅目标系统的文档。

要部署应用程序，我们必须确保将相关的 Qt 库（对应于应用程序中使用的 Qt 模块）、平台插件和可执行文件复制到同一目录树。如果应用程序依赖于编译器特定的库，则这些库也必须与应用程序一起重新分发。有关更多信息，请参阅应用程序依赖项部分（[Application Dependencies](https://doc.qt.io/qt-6/windows-deployment.html#application-dependencies)）。

共享库的必须确保动态链接器能够找到 Qt 库。除非另有说明，否则动态链接器不会搜索应用程序所在的目录。可以通过以下方法解决这个问题：

* 将 Qt 库安装在系统库路径之一（例如，大多数系统上的 /usr/lib）。
* 链接应用程序时，将预定路径传递给 -rpath 命令行选项。这将告诉动态链接器在启动应用程序时查看此目录。
* 可以为应用程序编写启动脚本，在其中修改动态链接器配置（例如，将应用程序的目录添加到 LD_LIBRARY_PATH 环境变量中）。

第一种方法的缺点是用户必须具有超级用户权限。第二种方法的缺点是用户可能没有安装到预定路径的权限。第三种方法的缺点是应用程序将以“执行时设置用户 ID”的方式运行，并且该应用程序由 root 拥有，则 LD_LIBRARY_PATH 在某些平台上将被忽略。无论哪种方法，用户都无法选择安装到其主目录。

建议使用第三种方法，因为它是最灵活的。例如，plugandpaint.sh 脚本将如下所示：

```sh
#!/bin/sh
appname=`basename $0 | sed s,\.sh$,,`

dirname=`dirname $0`
tmp="${dirname#?}"

if [ "${dirname%$tmp}" != "/" ]; then
dirname=$PWD/$dirname
fi
LD_LIBRARY_PATH=$dirname
export LD_LIBRARY_PATH
$dirname/$appname "$@"
```

通过运行此脚本而不是可执行文件，可以确定动态链接器将找到 Qt 库。只需重命名该脚本即可将其与其他应用程序一起使用。

查找插件时，应用程序会在应用程序可执行文件目录内的plugins 子目录中进行搜索。必须手动将插件复制到插件目录中，或者可以在插件的项目文件中设置 DESTDIR：

```
DESTDIR = /path/to/Qt/plugandpaint/plugins
```

在大多数系统上，共享库的扩展名是 .so。一个值得注意的例外是 HP-UX，它使用 .sl。

#### 补全依赖库

要找出应用程序依赖哪些库，可以运行 ldd 工具（在大多数 Unices 上可用）：

```
ldd ./application
```

这将列出应用程序的所有共享库依赖项。根据配置，这些库必须与您的应用程序一起重新分发。特别是，如果使用与系统编译器二进制不兼容的编译器来编译应用程序，则必须重新分发标准 C++ 库。

当与常规 X11 库动态链接时，因为某些实现将尝试使用 dlopen() 打开其他共享库，如果失败，X11 库可能会导致应用程序崩溃。

Qt 将寻找某些 X11 扩展，例如 Xinerama 和 Xrandr，并可能将它们引入，包括它们链接的所有库。如果您不能保证某个扩展的存在，最安全的方法是在配置 Qt 时禁用它（例如 ./configure -no-xrandr）。

FontConfig 和 FreeType 是并不总是可用或并不总是二进制兼容的库的其他示例。听起来可能很奇怪，一些软件供应商通过在非常旧的机器上编译他们的软件而取得了成功，并且非常小心地不升级在它们上运行的任何软件。

将应用程序链接到静态 Qt 库时，必须显式链接上述依赖库。通过将它们添加到项目文件中的 LIBS 变量来执行此操作。

#### 插件

所有 Qt GUI 应用程序都需要一个在 Qt 中实现 Qt 平台抽象 (QPA) 层的插件。对于 Linux/X11，平台插件的名称是 libqxcb.so。该文件必须位于分发目录下的特定子目录（默认情况下为platforms）中。或者，可以调整 Qt 用于查找其插件的搜索路径，如下所述。

应用程序还可能依赖于一个或多个 Qt 插件，例如 JPEG 图像格式插件或 SQL 驱动程序插件。请务必随应用程序分发需要的所有 Qt 插件。与平台插件类似，每种类型的插件必须位于分发目录中的特定子目录（例如 imageformats 或 sqldrivers）中。

Qt 插件的搜索路径（以及一些其他路径）被硬编码到 QtCore 库中。默认情况下，第一个插件搜索路径将被硬编码为 /path/to/Qt/plugins。如上所述，使用预定路径有一定的缺点，因此您需要检查各种替代方案以确保找到 Qt 插件：

* 使用 [qt.conf](https://doc.qt.io/qt-6/qt-conf.html)。这是推荐的方法，因为它提供了最大的灵活性。
* 使用 [QApplication::addLibraryPath](https://doc.qt.io/qt-6/qcoreapplication.html#addLibraryPath)() 或 [QApplication::setLibraryPaths](https://doc.qt.io/qt-6/qcoreapplication.html#setLibraryPaths)()。
* 使用第三方安装实用程序或目标系统的包管理器来更改 [QtCore](https://doc.qt.io/qt-6/qtcore-module.html) 库中的硬编码路径。

[如何创建 Qt 插件文档](https://doc.qt.io/qt-6/plugins-howto.html)概述了为 Qt 应用程序构建和部署插件时需要注意的问题。

<br/>

### [Windows 平台](https://doc.qt.io/qt-6/windows-deployment.html)

本文档描述了 [Windows](https://doc.qt.io/qt-6/windows.html) 的部署过程。我们在整个文档中引用了 [Plug & Paint](https://code.qt.io/cgit/qt/qtbase.git/tree/tests/manual/examples/widgets/tools/plugandpaint/app?h=6.7) 示例应用程序来演示部署过程。

```
注意：将 Qt 构建目录添加到系统上运行的任何防病毒应用程序的排除目录列表中。
```

#### Windows 部署工具

Windows 部署工具 windeployqt 旨在自动创建可部署文件夹的过程，其中包含从该文件夹运行应用程序所需的 Qt 相关依赖项（库、QML 导入、插件和翻译）。它为Windows桌面应用程序创建一个安装树，可以轻松地将其捆绑到安装包中。

该工具可以在 `QTDIR/bin/windeployqt` 中找到。它需要在构建环境中才能正常运行。使用 Qt Online Installer 时，应使用脚本 `QTDIR/bin/qtenv2.bat` 进行设置。

windeployqt 将 .exe 文件或包含 .exe 文件的目录作为参数，并扫描可执行文件的依赖项。如果使用 --qmldir 参数传递目录，windeployqt 将使用 qmlimportscanner 工具扫描目录内的 QML 文件以获取 QML 导入依赖项。然后将识别出的依赖项复制到可执行文件的目录中。

如果 Qt 是在配置开关 -relocatable 关闭的情况下构建的，windeployqt 将用相对路径替换 Qt6Core.dll 中的硬编码本地路径。

对于 Windows 桌面应用程序，默认情况下编译器所需的运行时文件也会复制到可部署文件夹中（除非指定了 --no-compiler-runtime 选项）。对于使用 Microsoft Visual C++ 的发行版本，这些版本由 Visual C++ 可再发行组件包组成，旨在由应用程序安装程序在目标计算机上进行递归安装。否则，将使用编译器运行时的共享库。

应用程序可能需要额外的第三方库（例如数据库库），windeployqt 未考虑这些库。

工具的帮助输出中描述了其他参数：

```shell
Usage: windeployqt [options] [files]
Qt Deploy Tool 6.0.0

The simplest way to use windeployqt is to add the bin directory of your Qt
installation (e.g. <QT_DIR\bin>) to the PATH variable and then run:
  windeployqt <path-to-app-binary>
If ICU, etc. are not in the bin directory, they need to be in the PATH
variable. If your application uses Qt Quick, run:
  windeployqt --qmldir <path-to-app-qml-files> <path-to-app-binary>

Options:
  -?, -h, --help              Displays help on commandline options.
  --help-all                  Displays help including Qt specific options.
  -v, --version               Displays version information.
  --dir <directory>           Use directory instead of binary directory.
  --qmake <path>              Use specified qmake instead of qmake from PATH.
  --libdir <path>             Copy libraries to path.
  --plugindir <path>          Copy plugins to path.
  --debug                     Assume debug binaries.
  --release                   Assume release binaries.
  --pdb                       Deploy .pdb files (MSVC).
  --force                     Force updating files.
  --dry-run                   Simulation mode. Behave normally, but do not
                              copy/update any files.
  --no-patchqt                Do not patch the Qt6Core library.
  --ignore-library-errors     Ignore errors when libraries cannot be found.
  --no-plugins                Skip plugin deployment.
  --no-libraries              Skip library deployment.
  --qmldir <directory>        Scan for QML-imports starting from directory.
  --qmlimport <directory>     Add the given path to the QML module search
                              locations.
  --no-quick-import           Skip deployment of Qt Quick imports.
  --translations <languages>  A comma-separated list of languages to deploy
                              (de,fi).
  --no-translations           Skip deployment of translations.
  --no-system-d3d-compiler    Skip deployment of the system D3D compiler.
  --compiler-runtime          Deploy compiler runtime (Desktop only).
  --no-virtualkeyboard        Disable deployment of the Virtual Keyboard.
  --no-compiler-runtime       Do not deploy compiler runtime (Desktop only).
  --json                      Print to stdout in JSON format.
  --no-opengl-sw              Do not deploy the software rasterizer library.
  --list <option>             Print only the names of the files copied.
                              Available options:
                               source:   absolute path of the source files
                               target:   absolute path of the target files
                               relative: paths of the target files, relative
                                         to the target directory
                               mapping:  outputs the source and the relative
                                         target, suitable for use within an
                                         Appx mapping file
  --verbose <level>           Verbose level (0-2).

Qt libraries can be added by passing their name (-xml) or removed by passing
the name prepended by --no- (--no-xml). Available libraries:
bluetooth concurrent core declarative designer designercomponents gui qthelp
multimedia multimediawidgets multimediaquick network nfc opengl openglwidgets
positioning printsupport qml qmltooling quick quickparticles quickwidgets script
scripttools sensors serialport sql svg svgwidgets test websockets widgets xml
webenginecore webengine webenginewidgets 3dcore 3drenderer 3dquick
3dquickrenderer 3dinput 3danimation 3dextras geoservices webchannel serialbus
webview

Arguments:
  [files]                     Binaries or directory containing the binary.
```

#### 静态链接

要构建静态应用程序，请通过使用 -static 配置 Qt 来静态构建 Qt：

```cmd
cd C:\path\to\Qt
configure -static <any other options you need>
```

如果稍后需要从同一位置重新配置和重建 Qt，请确保在再次运行 configure 之前进入构建目录并运行 nmake distclean 或 mingw32-make distclean 来删除先前配置的所有痕迹。

#### 补全依赖库

1. 在 release 模式下编译程序

2. 编译完成后会生成 release 版本的 `.exe` 文件，将其拷贝到某个文件夹下（发布目录）。

3. 打开 cmd 进入该发布目录，并在其中执行 `windeployqt "my_app.exe"`，运行完后会在发布目录下补全所依赖的 `.dll` 文件和 `translations` 文件。

4. 如果是 QML 项目，需要将所依赖的文件从 `QT_DIR/qml` 拷贝到此目录的 `qml` 文件夹。否则会报如下的错误：

   ```
   QQmlApplicationEngine failed to load component
   qrc:/qt/qml/Main/main.qml:4:1: module "QtQuick" is not installed
   qrc:/qt/qml/Main/main.qml: module "QtQml.WorkerScript" is not installed
   ```


<br/>

### [macOS 平台](https://doc.qt.io/qt-6/macos.html#deploying-applications-on-macos)

在 macOS 上，必须从捆绑包（Bundle）构建和运行 GUI 应用程序，捆绑包是一种目录结构，在 Finder 中查看时显示为单个实体。应用程序的捆绑包通常包含可执行文件及其所需的所有资源。以下是应用程序包（plugandpaint）结构的快照：

![deployment-mac-bundlestructure](/resources/QtDeployment/deployment-mac-bundlestructure.png)

有关捆绑包的更多信息，请参阅 [Apple 开发者网站](http://developer.apple.com/documentation/CoreFoundation/Conceptual/CFBundles/index.html)。

要使用 CMake 将应用程序构建为捆绑包，请在可执行目标上设置 MACOSX_BUNDLE 属性：

```
set_target_properties(plugandpaint PROPERTIES
    MACOSX_BUNDLE TRUE
)
```

qmake 会自动为您的应用程序生成一个包。要禁用此功能，请将以下语句添加到应用程序的项目文件 (.pro) 中：

```
CONFIG-=app_bundle
```

#### 补全依赖库

以使用 otool 检查您的应用程序链接到哪些其他库：

```
otool -L plugandpaint.app/Contents/MacOs/plugandpaint
```

<br/>

### [Android 平台](https://doc.qt.io/qt-6/deployment-android.html)

Android 上的应用程序可以通过两种方式打包；作为应用程序包 (APK) 或 Android 应用程序包 (AAB)。两者都是一种 ZIP 文件，遵循预定义的目录结构。两者之间的区别在于 APK 文件可以在设备上部署和执行，而 AAB 旨在由 Google Play 商店解释，并用于为不同的设备架构和外形尺寸生成 APK 文件。

对于本地测试应用程序，APK 格式是合适的，因为它可以直接在设备上安装和运行。对于分发到 Google Play 商店，建议使用 AAB，它具有类似的布局。 AAB 的额外便利之处在于，可以将所有目标 ABI 包含在同一包中，而无需增加用户下载的实际包的大小。使用 AAB 时，Google Play 商店会为发出下载请求的设备生成优化的 APK 包，并自动使用您的发布者密钥对其进行签名。有关详细信息，请参阅[发布到 Google Play](https://doc.qt.io/qt-6/android-publishing-to-googleplay.html)。有关 AAB 格式的更多信息，请参阅 [Android 应用程序包](https://developer.android.com/guide/app-bundle)。

无论哪种情况，应用程序包都是从特定的目录结构生成的，其中包含项目的共享库以及应用程序所需的 Qt 依赖项。此外，任何资产、资源、jar 文件或项目 Java 代码都会被编译和包含。

#### 生成应用程序包

建议使用 Qt Creator [部署 Qt for Android 应用程序](https://doc.qt.io/qtcreator/creator-deploying-android.html)。可以在 CMake 或 qmake 的帮助下通过命令行完成相同的操作。有关更多信息，请参阅[从命令行为 Android 项目构建 Qt](https://doc.qt.io/qt-6/android-building-projects-from-commandline.html)。

打包和部署过程由 CMake 或 qmake 处理，它们在幕后使用 [androiddeployqt](https://doc.qt.io/qt-6/android-deploy-qt-tool.html#androiddeployqt) 工具来管理构建和部署 Android 应用程序的细节。

#### 使用 Android 功能扩展 Qt

默认情况下，Qt for Android 会完成大部分繁重的工作来启动并运行 Android 应用程序，直接从 Qt 获取最有用的 API，或使用 [QJniObject](https://doc.qt.io/qt-6/qjniobject.html) 调用不易获得的 API。这对于 CMake 也是有效的，它处理各种构建和部署情况。然而，在其他一些情况下，可能需要拥有原生 Android 设施的全部功能，例如编写 Java/kotlin 代码或使用 Android 资源管理。 Qt 通过允许用户扩展模板集来实现这一点。

Qt 使用的默认模板位于 Qt for Android 安装路径下，例如，对于 Unix，位于 `~/Qt/<version>/android_<abi>/src/android/templates` 下。要使用这些模板，可以让 Qt Creator 将它们复制到项目中，请参阅 [Qt Creator: Editing Manifest Files](http://doc.qt.io/qtcreator/creator-deploying-android.html#editing-manifest-files)。或者可以手动将它们复制到项目源的 android 子目录下。然后确保在 CMakeLists.txt 中定义以下属性：

```
set_property(TARGET target_name PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
          "${CMAKE_CURRENT_SOURCE_DIR}/android")
```

或者在 pro 文件中使用 qmake：

```
android: ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
```

注意：使用 Qt Creator 时，如果使用 CMake，则该路径下的文件默认在项目文件下可见。要使用 qmake 实现相同的行为，请使用 DISTFILES 手动将这些文件路径添加到项目中。

构建过程会将模板从项目复制到构建目录 `<build_path>/android-build`，如果项目未设置 QT_ANDROID_PACKAGE_SOURCE_DIR ，则从默认模板复制模板。之后，目录 `<build_path>/android-build` 作为打包目录。应用程序包是使用 Gradle 从那里创建的。

现在，让我们看看扩展默认模板后用户可以使用的不同部分。

##### AndroidManifest.xml

AndroidManifest.xml 文件提供有关应用程序的详细元信息。此信息用于自定义您的应用程序包，设备使用它来决定启用哪些功能，例如应用程序的默认方向。此外，Google Play 商店还使用它来获取有关版本代码、设备支持、包名称等的信息。 Android 清单还用于定义 Android 服务和自定义 Android 活动。

有关 AndroidManifest.xml 的更多信息，请参阅 [Android Manifest file documentation](https://doc.qt.io/qt-6/android-manifest-file-configuration.html) 和  [Qt Creator: Editing Manifest Files](http://doc.qt.io/qtcreator/creator-deploying-android.html#editing-manifest-files)。

##### Gradle Files

[Gradle](http://www.gradle.org/) 用于构建 Android 包。 Qt 包含两组 Gradle 相关文件：

* Gradle 包装器，用于下载 Gradle 本身的特定版本，以及用于调用 Gralde 构建的构建脚本。这些文件随 Qt 一起提供，例如 `~/Qt/<version>/android_<abi>/src/3rdparty/gradle`。

  注意：通常建议使用Qt自带的Gradle版本。但是，如果您希望使用不同的 Gradle 版本，您可以修改 Gradle 包装器 gradle-wrapper.properties 并将其设置为您要使用的 Gradle 版本。

* Gradle配置文件build.gradle，位于 [Android Templates](https://doc.qt.io/qt-6/deployment-android.html#extending-qt-with-android-facilities) 下。 Gradle 需要此文件，可用于自定义构建。它可用于设置构建目标或最小 API 或添加库依赖项。它还可用于设置 [Android Gradle plugin](https://developer.android.com/build/releases/gradle-plugin)，这是构建 Android 应用程序所需的 Gradle 依赖项。一个例子是：

  ```
  buildscript {
      ...
      dependencies {
          classpath 'com.android.tools.build:gradle:7.4.1'
      }
  }
  ```

有关更多信息，请参阅 [Android: Build Configuration Files](https://developer.android.com/studio/build#build-files)。

##### Java/Kotlin Code

要包含任何 Java/Kotlin 代码来定位 Qt 未涵盖的某些 API 或出于其他原因，请将任何代码放置在路径 `<QT_ANDROID_PACKAGE_SOURCE_DIR>/src/` 下。例如，您可以从 Qt 代码中调用 Java 方法。有关示例，请参阅 [Qt Android Notifier Example](https://doc.qt.io/qt-6/qtcore-platform-androidnotifier-example.html)。

##### Resources

Android允许添加图标、图像、字符串、颜色等资源文件。这些资源可以直接从 Java/Kotlin 代码或清单文件引用。任何此类文件都可以添加到您的项目中的 `<QT_ANDROID_PACKAGE_SOURCE_DIR>/res/` 下。例如，可以在 `res/drawable/icon.png` 下添加应用程序图标。

有关更多信息，请参阅 [Android: App resources overview](https://developer.android.com/guide/topics/resources/providing-resources)。

#### Qt 内部资源

默认情况下，Qt 打包了应用程序正常运行所需的一些资源。例如，在 Unix 上，这些资源位于 `~/Qt/<version>/android_<abi>/src/android/templates/res` 下。

##### strings.xml

该文件可以在 `res/values/strings.xml` 中找到。该文件包含 Qt 库用来引用各种语言的消息翻译的字符串。

##### libs.xml

该文件可以在 `res/values/libs.xml` 中找到。它纯粹用于管理Qt库的部署信息，不建议手动修改。

#### Android

有关管理 Android 资源的更多信息，请参阅 [Adding resources](https://doc.qt.io/qt-6/porting-to-android.html)。

有关在 Qt 项目中使用第三方库的更多信息，请参阅 [Including a Library to an Android Project](https://doc.qt.io/qt-6/android-3rdparty-libs.html)。

<br/>

## 参考

* [Deploying Qt Applications | 部署 Qt 应用程序 | Qt 6.7](https://doc.qt.io/qt-6/deployment.html)
* [Localizing Applications | 本地化应用程序 | Qt 6.7](https://doc.qt.io/qt-6/localization.html#deploy-translations)
* [Qt for Linux/X11 - Deployment | Qt 6.7](https://doc.qt.io/qt-6/linux-deployment.html)
* [Qt for Windows - Deployment | Qt 6.7](https://doc.qt.io/qt-6/windows-deployment.html)
* [Qt for macOS - Deployment | Qt 6.7](https://doc.qt.io/qt-6/macos-deployment.html)
* [Deploying an Application on Android | Qt 6.7](https://doc.qt.io/qt-6/deployment-android.html)

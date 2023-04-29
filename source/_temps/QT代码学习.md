---
title: QT代码学习
tags:
---

Code：qtbase-everywhere-src-5.13.2



<!--more-->

<br/>

### 宏语法

```c++
// qglobal.h

#ifdef _MSC_VER
#  define QT_SUPPORTS(FEATURE) (!defined QT_NO_##FEATURE)
#else
#  define QT_SUPPORTS(FEATURE) (!defined(QT_NO_##FEATURE))
#endif

#define QT_CONFIG(feature) (1/QT_FEATURE_##feature == 1)
#define QT_REQUIRE_CONFIG(feature) Q_STATIC_ASSERT_X(QT_FEATURE_##feature == 1, "Required feature " #feature " for file " __FILE__ " not available.")
```

#feature 表示：在参数前后加上“的字符串。

##feature 表示：把宏参数名与宏定义代码序列中的标识符连接在一起，形成一个新的标识符或者数字。



```
// qglobal.h
#  define Q_STATIC_ASSERT_X(Condition, Message) static_assert(bool(Condition), Message)
```

**static_assert**(常量表达式，提示字符串)

如果第一个参数常量表达式的值为false，会产生一条编译错误，第二个参数就是错误提示字符串。

* 使用**static_assert**，我们可以在编译期间发现更多的错误，用编译器来强制保证一些契约，并帮助我们改善编译信息的可读性，尤其是用于模板的时候。

* **static_assert**可以用在全局作用域中，命名空间中，类作用域中，函数作用域中，几乎可以不受限制的使用。

* 编译器在遇到一个**static_assert**语句时，通常立刻将其第一个参数作为常量表达式进行演算，但如果该常量表达式依赖于某些模板参数，则延迟到模板实例化时再进行演算，这就让检查模板参数成为了可能。

* 性能方面，由于是**static_assert**编译期间断言，不生成目标代码，因此**static_assert**不会造成任何运行期性能损失。

* 相对**assert**，**assert**是运行期断言，它用来发现运行期间的错误，不能提前到编译期发现错误，也不具有强制性，也谈不上改善编译信息的可读性，既然是运行期检查，对性能当然是有影响的，所以经常在发行版本中，**assert**都会被关掉。
* 相对**#error**，**#error**仅仅能在预编译时显示一个错误信息，它能做的不多，可以配合#ifdef/ifndef参与预编译的条件检查，由于它无法获得编译信息，没有判断功能。



### 环境检测

```
#include <QtCore/qsystemdetection.h>
#include <QtCore/qprocessordetection.h>
#include <QtCore/qcompilerdetection.h>
```



System Detect:

     DARWIN   - Any Darwin system (macOS, iOS, watchOS, tvOS)
     MACOS    - macOS
     IOS      - iOS
     WATCHOS  - watchOS
     TVOS     - tvOS
     WIN32    - Win32 (Windows 2000/XP/Vista/7 and Windows Server 2003/2008)
     WINRT    - WinRT (Windows Runtime)
     CYGWIN   - Cygwin
     SOLARIS  - Sun Solaris
     HPUX     - HP-UX
     LINUX    - Linux [has variants]
     FREEBSD  - FreeBSD [has variants]
     NETBSD   - NetBSD
     OPENBSD  - OpenBSD
     INTERIX  - Interix
     AIX      - AIX
     HURD     - GNU Hurd
     QNX      - QNX [has variants]
     QNX6     - QNX RTP 6.1
     LYNX     - LynxOS
     BSD4     - Any BSD 4.4 system
     UNIX     - Any UNIX BSD/SYSV system
     ANDROID  - Android platform
     HAIKU    - Haiku


Processor Detect:

```
BIG_ENDIAN/LITTLE_ENDIAN
Size of a pointer
machine register size
the size of the machine's word

处理器系列：
    Alpha family
    ARM family
    AVR32 family
    Blackfin family
    X86 family
    Pentium Pro
    Itanium (IA-64) family
    MIPS family
    Power family
    S390 family
    SuperH family
    SPARC family
```



Compiler Detect:

```
SYM      - Digital Mars C/C++ (used to be Symantec C++)
MSVC     - Microsoft Visual C/C++, Intel C++ for Windows
BOR      - Borland/Turbo C++
WAT      - Watcom C++
GNU      - GNU C++
COMEAU   - Comeau C++
EDG      - Edison Design Group C++
OC       - CenterLine C++
SUN      - Forte Developer, or Sun Studio C++
MIPS     - MIPSpro C++
DEC      - DEC C++
HPACC    - HP aC++
USLC     - SCO OUDK and UDK
CDS      - Reliant C++
KAI      - KAI C++
INTEL    - Intel C++ for Linux, Intel C++ for Windows
HIGHC    - MetaWare High C/C++
PGI      - Portland Group C++
GHS      - Green Hills Optimizing C++ Compilers
RVCT     - ARM Realview Compiler Suite
CLANG    - C++ front-end for the LLVM compiler
```


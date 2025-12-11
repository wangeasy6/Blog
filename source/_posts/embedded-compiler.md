---
title: 嵌入式中的编译器
date: 2024-02-19 15:48:00
toc: true
categories:
  - tech
tags:
  - C
  - C++
  - 嵌入式
---

在嵌入式领域，一般使用 GCC 作为我们的交叉编译工具链。（The GNU Compiler Collection，通常简称 [GCC](https://gcc.gnu.org/) ，是一套由 [GNU](https://www.gnu.org/) 开发的编译器集合。）

基于 GCC，本文涵盖以下几个方面：

* [编译原理](/tech/嵌入式中的编译器/#编译原理)
* [Linux 程序结构](/tech/嵌入式中的编译器/#Linux-可执行程序的结构)
* [Linux 进程在内存中的布局](/tech/嵌入式中的编译器/#Linux-进程在内存中的布局)
* [C标准对应的GCC版本](/tech/嵌入式中的编译器/#C标准对应的GCC版本)
* [C++标准对应的GCC版本](/tech/嵌入式中的编译器/#C-标准对应的GCC版本)
* [升级交叉编译工具链](/tech/嵌入式中的编译器/#升级交叉编译工具链)

<!--more-->

<br/>

## 编译原理

编译器就是将高级语言翻译成机器语言的一个工具。参考：《程序员的自我修养——链接、装载与库》

一般分为 4 步：

1. 预处理（Prepressing），`gcc –E hello.c –o hello.i`
2. 编译（Compilation），`gcc –S hello.i –o hello.s`
3. 汇编（Assembly），`as hello.s –o hello.o`
4. 链接（Linking），`ld -static /usr/lib/crt1.o /usr/lib/crti.o /usr/lib/gcc/i486-linux-gnu/4.1.3/crtbeginT.o -L/usr/lib/gcc/i486-linux-gnu/4.1.3 -L/usr/lib -L/lib hello.o --start-group -lgcc -lgcc_eh -lc --end-group /usr/lib/gcc/i486-linux-gnu/4.1.3/crtend.o /usr/lib/crtn.o`

如下图所示：

![compile](/resources/Compiler/compile.jpg)

### 预处理

* 处理`“#include`”预编译指令，将被包含的文件插入到该预编译指令的位置
* 将所有的`“#define`”删除，并且展开所有的宏定义
* 处理所有条件预编译指令，比如`“#if``”、“#ifdef``”、“#elif``”、“#else``”、“#endif`”
* 删除所有的注释
* 添加行号和文件名标识

### 编译

编译过程就是把预处理完的文件进行一系列词法分析、语法分析、语义分析及优化后生产相应的汇编代码文件。

### 汇编

汇编器是将汇编代码转变成机器可以执行的指令，每一个汇编语句几乎都对应一条机器指令。

### 链接

将编译后的目标文件和库文件链接，最终产生一个可以执行的程序。

### 交叉编译

交叉编译是指在一种操作系统上编译出另一种操作系统下可执行的程序的过程。

通常情况下，我们在 Ubuntu PC 环境中编写和调试程序，然后将程序源代码传输到目标平台（ARM .etc）上进行编译和执行。

交叉编译通常用于嵌入式系统、移动设备和其他特定平台的开发中。

### 编译链中的其它常用工具

- nm - 列出目标文件中的符号
- objcopy： 可用于目标文件格式转换，如 `.elf` 转换成 `.bin`
- objdump - 显示目标文件中的信息
- readelf - 显示任何 ELF 格式目标文件中的信息
- size - 列出对象或归档文件的部分大小
- strings - 列出文件中的可打印字符串

参考：[Binutils - GNU Project - Free Software Foundation](https://www.gnu.org/software/binutils/)

<br/>

## Linux 可执行程序的结构

Linux 中的目标文件格式为 ELF（Executable and Linkable Format），规定了以下 4 种文件类型：

* 可执行文件（Executable File，\*.out）
* 共享目标文件（Shared Object File，\*.so，即动态链接库）
* 可重定位文件（Relocatable File，\*.o，静态链接库为 \*.o 的打包，\*.a）
* 核心转存文件（Core Dump File）

参考：[Executable and Linkable Format - Wikipedia](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)

ELF 文件由以下 4 个部分组成：

1. ELF头部（ELF Header）：包含了文件的基本信息，如魔数、文件类型、机器架构、入口地址等。ELF头部通常位于文件的开头处。
2. 节区表（Section Header Table）：包含了文件中所有节区的信息，如名称、偏移量、大小等。节区表中的每一项对应一个节区。
3. 节区（Section）：包含了文件中的各种数据和代码，如.text节区存储代码段、.data节区存储数据段、.rodata 存储只读数据段等。不同的节区用于存储不同类型的信息。
4. 程序头部（Program Header Table）：只存在于可执行文件和共享库中，用于描述程序在内存中的布局。程序头部包含了加载程序时需要的信息，如节区在内存中的偏移量、大小等。

参考：[elf(5) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man5/elf.5.html)

<br/>

## Linux 进程在内存中的布局

Linux程序在内存中的结构可以分为以下几个部分：

1. 代码段（text segment）：存储程序的可执行指令，通常是只读的。代码段通常是共享的，多个程序可以共享同一个代码段，以节省内存空间。
2. 数据段（data segment）：存储程序的全局变量和静态变量。数据段通常包括初始化的数据和未初始化的数据（BSS段）。
3. 堆（heap）：存储动态分配的内存空间，由程序员手动管理。堆的大小可以在运行时动态调整，通常由malloc、calloc、realloc等函数来分配和释放内存。
4. 栈（stack）：存储函数的局部变量、函数参数、函数返回地址等信息。栈是自动管理的，函数调用时会自动分配一块内存，函数返回时会自动释放这块内存。
5. 共享库（shared libraries）：存储程序所依赖的共享库的代码和数据。共享库通常是动态链接的，多个程序可以共享同一个共享库，以节省内存空间。
6. 内核空间（kernel space）：存储操作系统内核的代码和数据。用户程序无法直接访问内核空间，需要通过系统调用来与内核交互。

布局如下图所示：

![ProgramInMemory](/resources/Compiler/program_in_memory.png)

<br/>

## C标准对应的GCC版本

从 GCC 4.5 开始，C99 基本上完全受支持。

参考：[Status of C99 features in GCC - GNU Project](https://www.gnu.org/software/gcc/c99status.html)

<br/>

## C++标准对应的GCC版本

|                           | C++98        | C++11        | C++14        | C++17        | C++20              |
| ------------------------- | ------------ | ------------ | ------------ | ------------ | ------------------ |
| **available**             |              | 4.3          | 4.8          | 5            | 8                  |
| **feature-complete**      |              | 4.8.1        | 5            | 9            | still experimental |
| **as default**            | GCC< 6.1     |              | 6.1<=GCC<=10 | 11<=GCC      |                    |
| **command-line flag**     | -std=c++98   | -std=c++11   | -std=c++14   | -std=c++17   | -std=c++20         |
| **enable GNU extensions** | -std=gnu++98 | -std=gnu++11 | -std=gnu++14 | -std=gnu++17 | -std=gnu++20       |

参考：

* [C++ Standards Support in GCC - GNU Project](https://www.gnu.org/software/gcc/projects/cxx-status.html)
* [C++11 的编译器支持 - cppreference.com](https://zh.cppreference.com/w/cpp/compiler_support/11)

<br/>

## 升级交叉编译工具链

当拿到的 GCC 版本过老，想要使用的功能不支持时，可以尝试升级交叉编译工具链。有几个渠道可以获取新的交叉编译工具链。

* Ubuntu apt 获取，例如：`sudo apt install gcc-arm-linux-gnueabi`
* Linaro 是一个非营利性技术联盟，为ARM平台提供开源软件和工具: [Linaro Releases](https://releases.linaro.org/components/toolchain/binaries/)
* 适用于 AArch64 和 ARM 32 位 A-Profile 内核的预构建 GNU 交叉工具链的 Arm 官方版本可在 [Arm Developer](https://developer.arm.com/downloads/-/gnu-a) 网站上获取。

以 Linaro 为例，进入 [latest-7](https://releases.linaro.org/components/toolchain/binaries/latest-7/) 文件夹，可以看到以下文件夹：

![gcc_cross_list](/resources/Compiler/gcc_cross_list.png)

GNU 交叉编译器的命名规则：

```
arch [-vendor] [-os] [-(gnu)abi]
```

- arch：体系架构，如 arm 、 aarch64 或者 mips 等；

- vendor：工具链提供商，没有 vendor 时，可用 none 代替；
- os：目标操作系统，没有 os 支持时，可用 none 代替；
- abi：应用二进制接口（Application Binary Interface），对于嵌入式平台命名为 eabi ， e 表示 Embedded；

**gnueabi 和 gnueabihf 的区别：**

`gnueabi` 和 `gnueabihf` 是两个针对 ARM 架构的不同的交叉编译器，它们的主要区别在于浮点运算的处理方式。以下是它们之间的主要区别：

1. **架构支持**：
   - `gnueabi`：对应于 `armel` 架构，即 ARM EABI Little-endian（小端）架构。
   - `gnueabihf`：对应于 `armhf` 架构，即 ARM Hard Float（硬浮点）架构。
2. **浮点运算处理**：
   - `gnueabi`：默认使用 `softfp` 模式，这意味着即使 ARM 处理器有浮点运算单元（FPU），编译器也会通过普通寄存器传递浮点参数，这样可以减少中断时需要保存的寄存器数量，降低中断负荷，但需要在计算前将参数从普通寄存器转换为浮点格式。
   - `gnueabihf`：默认使用 `hard` 模式，这意味着 ARM 处理器会使用 FPU 进行浮点计算，并且浮点参数也通过 FPU 的浮点寄存器传递，这样可以避免参数转换，提高性能，但会增加中断时的负荷。
3. **兼容性**：
   - `gnueabi` 与 `softfp` 和 `hard` 模式都是兼容的，但 `gnueabihf` 和 `softfp` 模式之间不兼容。

总结来说，`gnueabi` 和 `gnueabihf` 的选择取决于目标硬件是否支持硬浮点运算以及对性能和资源消耗的具体需求。如果目标设备有 FPU 并且对性能要求较高，可以选择 `gnueabihf`；如果对资源消耗有限制或者目标设备不支持硬浮点，可以选择 `gnueabi`。

**如何查看 CPU 有没有 FPU：**

使用命令`cat /proc/cpuinfo`，以 Exynos 4412 为例：

```sh
[root@iTOP-4412]# cat /proc/cpuinfo 
processor	: 0
model name	: ARMv7 Processor rev 0 (v7l)
BogoMIPS	: 67.20
Features	: half thumb fastmult vfp edsp neon vfpv3 tls vfpd32 
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x3
CPU part	: 0xc09
CPU revision	: 0
```

CPU 特性列表中：**VFP (Vector Floating Point)**：这是ARM为其Cortex-A系列处理器设计的SIMD技术，用于提高多媒体和信号处理的速度和效率。如果CPU支持VFP，那么它具有浮点运算单元（FPU）。`vfp`、`vfpv3` 和 `vfpd32` 表示CPU具有不同版本的VFP支持。`half` 表明CPU支持半精度浮点数，这也是FPU功能的一部分。`fastmult` 表明CPU具有快速乘法能力，这也是FPU的一部分。

### 常用编译器

**arm-none-eabi**：（ARM architecture，novendor，nottarget an operating system，complies with the ARM EABI）用于编译 ARM 架构的裸机系统，包括 ARM Linux 的 boot、kernel，不适用编译 Linux 应用。适用于 ARM7/Cortex-M/Cortex-R，不支持跟操作系统关系密切的函数，如 fork 等，可调用 newlib 库。

**arm-none-linux-gnueabi**：(ARM architecture, no vendor, creates binaries that run on the Linux operating system,and uses the GNU EABI)主要用于基于ARM架构的Linux系统，包括 ARM 架构的 boot、kernel、linux 应用。适用于 ARM9/ARM11/Cortex-A，支持系统函数，可调用 glibc 库。

**arm-eabi**： 用于安卓程序编译。

### 常用开发板

**树莓派（Raspberry Pi）**：arm-linux-gnueabihf

**香橙派（Orange Pi） 5B**：aarch64-linux-gnu

**iTop4412（SAMSUNG EXYNOS 4412）**：arm-linux-gnueabihf

参考：[GCC 编译器套件说明_arm-linux-gnueabihf-gcc-CSDN博客](https://blog.csdn.net/qq_42992084/article/details/129190911)

### 不同版本编译器产生的库能用吗？

通常情况下，不同版本的编译器产生的库是不兼容的。因为不同版本的编译器可能会对库的生成方式、函数接口、数据结构等方面有所改变，导致无法直接在不同版本的编译器中使用。在使用库时，最好确保使用的库是与当前编译器版本兼容的，以避免出现不可预料的问题。如果需要在不同版本的编译器中使用同一个库，可能需要重新编译该库以确保与目标编译器兼容。

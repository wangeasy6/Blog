---
title: Linux BootLoader
date: 2018-08-18 22:26:44
toc: true
categories:
  - tech
tags:
  - Linux
  - 待续
---

### 学习环境

[4412开发板 POP/1G DDR](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.802a2e8dPjyDS3&id=38712193806&_u=i1nungt958cc)

### 基础前提

* bash
* Makefile

### 简介

* 引导加载程序是系统上电后运行的第一段代码。
* 在 Windows 主机上，引导程序由 BIOS 和位于硬盘 MBR 中的 OS BootLoader（LILO、GRUB等）组成。BIOS 完成硬件检测和资源分配后，将硬盘 MBR 中的 OS BootLoader 读到系统的 RAM 中，然后将控制权交到 BootLoader。BootLoader 的主要任务就是将内核镜像从硬盘上读到 RAM 中，然后跳转到内核入口，开始启动操作系统。
* 在 Linux 上，通常没有 BIOS 这样的固件程序，因此整个系统的加载启动任务就完全由 BootLoader 来完成了。

* 通常 Linux BootLoader 包含以下任务：
  * 初始化硬件设备
  * 建立内存空间的映射

* 严格来说，“BootLoader” 只是引导设备并且执行主程序的固件；而有的还提供了更多的命令行接口，可以进行调试、读写内存、烧写 Flash、配置环境变量等，提供了更好的调试功能，这种我们称为"Monitor"，这些都统称为 BootLoader。

<!--more-->
### BootLoader 的种类
* 除了按照处理器体系结构不同划分以外，还有功能复杂程度的不同。

* 下表列出了 Linux 的开源引导程序及其支持的体系结构：

| BootLoader | Monitor | 描述                            | X86  | ARM  | PowerPC |
| ---------- | ------- | ------------------------------- | ---- | ---- | ------- |
| LILO       | 否      | Linux 磁盘引导程序               | 是   | 否   | 否      |
| GRUB       | 否      | GNU 的 LILO 替代程序             | 是   | 否   | 否      |
| Loadlin    | 否      | 从 DOS 引导 Linux                | 是   | 否   | 否      |
| ROLO       | 否      | 从 ROM 引导 Linux 而不需要 BIOS   | 是   | 否   | 否      |
| EtherBoot  | 否      | 通过以太网卡启动 Linux 系统       | 是   | 否   | 否      |
| LinuxBIOS  | 否      | 完全替代 BUIS 的 Linux 引导程序   | 是   | 否   | 否      |
| BLOB       | 否      | LART 等硬件平台的引导程序         | 否   | 是   | 否      |
| U-Boot     | 是      | 通用引导程序                      | 是   | 是   | 是      |
| RedBoot    | 是      | 基于 eCos 的引导程序              | 是   | 是   | 是      |

  在嵌入式系统中，我们最常见和使用的就是 U-Boot。



### U-Boot
#### 简介
* Das U-Boot 是一套在GNU通用公共许可证之下发布的自由软件，可以支持多种不同的计算机系统结构，包括PPC、ARM、AVR32、MIPS、x86、68k、Nios与MicroBlaze。支持的文件系统包括： Cramfs、 ext2、 FAT、 FDOS、 JFFS2、 ReiserFS、 UBIFS、 YAFFS2。
* Das U-Boot可以在x86计算机上建构，但这部x86计算机必须安装有可支持特定平台结构的交互发展GNU工具链，例如crosstool、Embedded Linux Development Kit (ELDK)或OSELAS.Toolchain。
* 在线资源：
  * [HomePage](http://www.denx.de/wiki/U-Boot/WebHome)
  * [GitHub](https://github.com/u-boot/u-boot)

#### 移植

* 从 GitHub 上下载 u-boot 工程
* 文件目录说明：--
* 更改
  * build_uboot.sh —— 针对Samsung 4412 进行引导编译和封装的一个脚本
  * config.mk
  * MakeFile


### 参考书籍

《iTOP-4412系列开发板实战教程》
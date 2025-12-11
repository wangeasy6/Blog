---
title: iTOP-4412 总览
date: 2025-08-27 13:30:00
toc: true
categories:
  - tech
tags:
  - 嵌入式
---

![overview](/resources/itop4412-porting/overview.svg)

讯为的 iTOP-4412 POP 1G Elite（精英板） 开发板，其核心如下所示：

* **SoC:** Exynos 4412
* **ARM core:** Cortex-A9 based Quad CPU
* **ARM architecture:** ARMv7A
* **CPU Chip:** SMDKC210 (s5pc220)
* **EMMC:** 8GB (KLM8G1WEPD-B031)

是一块主要用于学习 Linux 系统开法的开发板。在此基础，总结一下一个典型的 Linux 软件系统的组成，及其关键节点的执行、操作过程。

试验环境：

* **Cross Compiler:** [gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf](https://releases.linaro.org/components/toolchain/binaries/latest-7/arm-linux-gnueabihf/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar.xz)
* **Build Environment:** Ubuntu 20.04.6 x86_64

<!--more-->

<br/>

## 软件系统组成

一个基础的 Linux 软件系统由3个部分组成：**引导程序（Boot Loader）**、**操作系统（Linux Kernel）**、**根文件系统（Root File System）**。

**Boot Loader** 在操作系统内核运行之前运行，负责初始化硬件设备、建立内存空间映射图，从而将系统的软硬件环境带到一个合适状态，以便为最终调用操作系统内核准备好正确的环境。

**操作系统** 是一个用来和硬件打交道并为用户程序提供一个有限服务集的低级支撑软件，在Linux的术语中被称为——内核（Kernel）。Linux 内核的主要模块（或组件）分以下几个部分：存储管理、CPU和进程管理、文件系统、设备管理和驱动、网络通信，以及系统的初始化（引导）、系统调用等。

**根文件系统** 是Linux内核启动时挂载的首个文件系统，包含引导系统及挂载其他文件系统所需的必要文件和目录，系统引导过程在挂载后会将初始化脚本和服务加载到内存运行。

本篇所使用的软件版本如下：

* **Boot Loader:** [iTop4412_uboot_20151119.tar.gz](https://github.com/TOPEET-Develop/iTop4412_uboot_public_merge), Based on u-boot-2010.03.
* **Kernel:** [linux-4.19.296](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/snapshot/linux-4.19.296.tar.gz)
* **Root FS 1:** [BusyBox 1.36.1](https://busybox.net/downloads/busybox-1.36.1.tar.bz2)
* **Root FS 2:** [Ubuntu 16](https://busybox.net/downloads/busybox-1.36.1.tar.bz2)

<br/>

## 准备 SD 卡

### 区域划分

以 2G(1.92 GiB, 2034237440 bytes, 3973120 sectors, Sector size : 512 bytes) SD 卡为例：

| block（10进制）    | 名称            | 预留大小   | 镜像大小                               |
| ------------------ | --------------- | ---------- | -------------------------------------- |
| 0                  | MBR             | 512B       |                                        |
| [1, 2048)          | uboot           | 1MB - 512B | 444KB (u-boot-iTOP-4412.bin)           |
| [2048, 4096)       | dtb             | 1MB        | 54KB (exynos4412-itop-pop1g-elite.dtb) |
| [4096, 34816)      | kernel          | 15MB       | 3.9MB (zImage@iTop4412_Kernel_3.0)     |
|                    |                 |            | 6.5MB (zImage@linux-4.19.323)          |
| [34965, 2132864]   | 分区 2（ext3）  | 1024MB     |                                        |
| [2132865, 2750579] | 分区 3（ext3）  | 301.6M     |                                        |
| [2750580 3368294]  | 分区 4（ext3）  | 301.6M     |                                        |
| [3368295 3947159]  | 分区 1（FAT32） | 282.7M     |                                        |

将 SD 卡分为 原始区 + 4 个分区，分区 1 为 FAT32  格式，分区 2、3、4 为 ext3 格式。

* 原始区即为未分区区域，预留 17 MB。以下按照 block 为单位，1 block = 512 bytes。
  * block 1 默认存放 MBR (Master Boot Record) 分区表。
  * 1 - 2048，约 1MB 的区域，一般用于放置 UBoot。
  * 2048 - 34816，16MB 的区域，用于放置 zImage、dtb。
* 分区 1 用于放置镜像文件。当在 uboot 中使用 sdfuse 命令向 EMMC 写入数据时，会自动读取此分区下的文件。
* 分区 2 用于写入根文件系统，一般来说比较大。
* 分区 3、4 可用于挂载为 home、temp 等目录。

#### 16进制表示的 block 的实际大小

| 0x(block) | 10(block) | 大小  |
| --------- | --------- | ----- |
| 0x80      | 128       | 64KB  |
| 0x400     | 1024      | 512KB |
| 0x800     | 2048      | 1MB   |
| 0x2000    | 8192      | 4MB   |
| 0x3800    | 14336     | 7MB   |
| 0x5000    | 20480     | 10MB  |

16进制表示的 block 大小，常常在 `dd`、`mmc read` 等命令中使用。

### 在 uboot 中

需要先把 uboot 写入 EMMC，从 EMMC 启动开发板。

以 2G TF 卡为例，依次执行：

```bash
# Log 显示检测到 TF卡： MMC1:	1940 MB
iTOP-4412 $ fdisk -c 1 300 300 300
iTOP-4412 $ fatformat mmc 1:1
iTOP-4412 $ ext3format mmc 1:2
iTOP-4412 $ ext3format mmc 1:3
iTOP-4412 $ ext3format mmc 1:4

# 查看分区情况
iTOP-4412 $ fdisk -p 1

partion #    size(MB)     block start #    block count    partition_Id 
   1           282          3368295          578865          0x0C 
   2          1024            34965         2097900          0x83 
   3           301          2132865          617715          0x83 
   4           301          2750580          617715          0x83
```

此时如果在 PC 上查看这张卡，会看到如下信息：

```bash
Ubuntu 20 $ sudo fdisk -l /dev/sdb

Disk /dev/sdb: 1.92 GiB, 2034237440 bytes, 3973120 sectors
Disk model: Storage Device  
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x24052147

Device     Boot   Start     End Sectors   Size Id Type
/dev/sdb1       3368295 3947159  578865 282.7M  c W95 FAT32 (LBA)
/dev/sdb2         34965 2132864 2097900     1G 83 Linux
/dev/sdb3       2132865 2750579  617715 301.6M 83 Linux
/dev/sdb4       2750580 3368294  617715 301.6M 83 Linux
```



### 在 PC 中

```bash
# 图形界面下分区：
Ubuntu 20 $ sudo apt-get install gparted
Ubuntu 20 $ sudo gparted
```

使用 gparted，在图形界面中，进行相同的分区。



### 新建 sdupdate

分区完成后，将 TF 卡插到 Windows，新建 sdupdate 文件夹，用于后续拷入：`u-boot-iTOP-4412.bin zImage system.img ramdisk-uboot.img`

也可以在 PC Ubunt 系统中，直接挂载 /dev/sdb1 后，进行同样的操作。

<br/>

## OM

### 启动选择操作

Exynos 4412 芯片支持从以下设备中启动：

* General NAND flash memory
* SD/MMC memory card
* eMMC memory
* USB device (for system debugging and flash reprogramming, not for normal booting)

iTop 4412 开发板支持使用拨码开关来选择从以下设备启动：

| 启动位置 | 拨码 1  | 拨码 2  |
| -------- | ------- | ------- |
| eMMC     | 0       | 1（ON） |
| SD card  | 1（ON） | 0       |

无论从 eMMC 加载还是从 SD 卡加载，都需要先向对应的设备烧写好 Boot Loader。

### 启动过程

![booting-time-operation](/resources/itop4412-porting/booting-time-operation.png)

设备启动或从低电模式唤醒时，程序从 iROM 开始执行。

① iROM 是一片芯片内部的 64K ROM，其中储存的是芯片出厂时烧写的固件，它会初始化 SoC 基本功能，如 时钟、堆栈。

② 而后，根据 运行模式引脚（Operating Mode (OM) pins），从选定的启动设备加载 BL1 到 256K 的内部 SRAM。根据 secure boot 值，iROM 可能会对 BL1 进行完整性检查。

此处的 BL1 即是 Boot Loader。

<br/>

## Boot Loader

Boot Loader 其实是一个统称，所有可以完成系统引导的初级系统都是 Boot Loader。而在 ARM Linux 系统中，uboot 是最常使用的。

在这里，我们使用的具体如下： [iTop4412_uboot_20151119.tar.gz](https://github.com/TOPEET-Develop/iTop4412_uboot_public_merge), Based on u-boot-2010.03。

基于此软件，总结了 2 份单独的文档，所以此篇仅略讲。（[iTOP-4412 UBoot 启动过程梳理 | Easy Wang](https://www.easywang.site/tech/itop4412-uboot-startup-procedure/)、[iTOP-4412 UBoot 使用笔记 | Easy Wang](https://www.easywang.site/tech/itop4412-uboot-usage/)）

### 编译

在 PC 中执行：

```bash
Ubuntu 20 $ cd iTop4412_uboot
./build_uboot.sh POP_1GDDR # For Linux or Android
# ./build_uboot.sh POP_1GDDR_Ubuntu # For Ubuntu
# 生成 u-boot-iTOP-4412.bin

# After all
Ubuntu 20 $ make distclean
# To clean temporary files.
```

将烧写完成的 SD 卡插入开发板。设置拨码从 SD card 启动，即可进入 uboot。

### 写入 SD 卡

在 PC 中执行：

```bash
# 如果插入 SD 卡后，显示为 /dev/sdx（如果不指定路径，默认为 /dev/sdb）：
Ubuntu 20 $ ./mkuboot.sh /dev/sdx
# 脚本分为 2 步：
# 1. 检查 u-boot-iTOP-4412.bin 文件是否存在
# 2. 执行命令： sudo dd iflag=dsync oflag=dsync if=u-boot-iTOP-4412.bin of=/dev/sdx seek=1
```



### 写入 EMMC

先在 PC 中将 u-boot-iTOP-4412.bin 放入 FAT32 的磁盘的 /sdupdate 目录下。

先从 SD 卡进入 uboot，执行以下命令可以将 uboot 烧写到 eMMC：

```bash
# Boot on SD
iTOP-4412 $ sdfuse flash bootloader u-boot-iTOP-4412.bin
```

<br/>

## Kernel 3.0.15

### 编译

在 PC 中执行：

```shell
Ubuntu 20 $ cd iTop4412_Kernel_3.0
# Set the compiler path in line 197 of the Makefile
Ubuntu 20 $ cp config_for_linux_pop_elite .config
Ubuntu 20 $ make zImage
# 生成 arch/arm/boot/zImage
```

```
Can't use 'defined(@array)' (Maybe you should just omit the defined()?) at kernel/timeconst.pl line 373.
make[1]: *** [/home/easy/Code/4412_test/itop4412-pop-1g-elite-org/iTop4412_Kernel_3.0/kernel/Makefile:140: kernel/timeconst.h] Error 255
```

解决：

```
// timeconst.pl @ 373
if (!defined(@val)) { ---> if (!@val) {
```

### 写入 SD 卡

```bash
# 如果插入 SD 卡后，显示为 /dev/sdb
Ubuntu 20 $ sudo dd oflag=dsync if=./arch/arm/boot/zImage of=/dev/sdb seek=2M status=progress
Ubuntu 20 $ sudo sync
Ubuntu 20 $ sudo eject /dev/sdb
```

### 从 SD 加载

```bash
# 进入 uboot 后，如果不在 3s 内输入，会在初始化完成后自动执行 bootcmd
# 使用如下命令查看 uboot 环境变量：
iTOP-4412 $ printenv
# 读 Kernel 到 0x40008000，dtb 到 0x43000000
bootcmd=mmc read 1 40008000 1000 5000;mmc read 1 43000000 800 80;bootm 40008000 42000000 43000000
# 1. 从 mmc 1 的 block 0x1000(4096) 处开始读取 0x5000(20480) block 到内存地址 0x40008000
# 2. 从 mmc 1 的 block 0x800(2048) 处开始读取 0x80(128) block 到内存地址 0x43000000

# 设置并保存环境变量：
iTOP-4412 $ setenv bootcmd 'mmc read 1 0x40008000 0x1000 0x5000;mmc read 1 0x43000000 0x800 0x80;bootm 0x40008000 0x42000000 0x43000000'
iTOP-4412 $ saveenv
```



### 写入 EMMC

先在 PC 中将 zImage 放入 FAT32 分区的 /sdupdate 目录下。

先从 SD 卡进入 uboot，执行以下命令可以将 zImage 烧写到 eMMC：

```bash
# Boot on SD
iTOP-4412 $ sdfuse flash kernel zImage
```

写入日志：

```sh
SD sclk_mmc is 400K HZ
SD sclk_mmc is 50000K HZ
SD sclk_mmc is 50000K HZ
[Fusing Image from SD Card.]
[Partition table on MoviNAND]
ptn 0 name='bootloader' start=0x0 len=N/A (use hard-coded info. (cmd: movi))
ptn 1 name='kernel' start=N/A len=N/A (use hard-coded info. (cmd: movi))
ptn 2 name='ramdisk' start=N/A len=0x300000(~3072KB) (use hard-coded info. (cmd: movi))
ptn 3 name='Recovery' start=N/A len=0x600000(~6144KB) (use hard-coded info. (cmd: movi))
ptn 4 name='system' start=0x15DDA00 len=0x403B0600(~1052353KB) 
ptn 5 name='userdata' start=0x4198E000 len=0x403B0600(~1052353KB) 
ptn 6 name='cache' start=0x81D3E600 len=0x1321EC00(~313467KB) 
ptn 7 name='fat' start=0x94F5D200 len=0x3BAFB600(~977901KB) 
Partition: kernel, File: /sdupdate/zImage
Partition1: Start Address(0x1ccf6e), Size(0x1f6b2a)
reading /sdupdate/zImage
3968904 (0x003c8f88) bytes read
flashing 'kernel'
writing kernel.. 1120, 12288 
MMC write: dev # 0, block # 1120, count 12288. 12288 blocks write finish
12288 blocks written: OK
completed
partition 'kernel' flashed
```

sdfuse 默认将 kernel 写入 1120（0x460）开始的位置。

载入与从 SD 加载基本一致，略。

### 加载根文件系统

在 `.config` 文件中，设置：

```makefile
CONFIG_CMDLINE="root=/dev/mmcblk1p2 console=ttySAC2,115200 init=/linuxrc rootfstype=ext3 rootwait rw"
```

`/dev/mmcblk1p2` 即为根文件系统所在分区。rootwait 表示等待 mmc 设备初始化完成后再挂在，rw 表示根文件系统是可读写的。

<br/>

## Kernel 4.19.296

### 编译

在 PC 中，配置好交叉编译器的环境变量后执行：

```bash
Ubuntu 20 $ cd linux-4.19.323
Ubuntu 20 $ make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- itop4412_pop_defconfig
Ubuntu 20 $ make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage
# 生成 arch/arm/boot/zImage
Ubuntu 20 $ make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- dtbs
# 生成 arch/arm/boot/dts/exynos4412-itop-pop1g-elite.dtb
```

### 写入 SD 卡

```bash
# 以插入 SD 卡后，显示为 /dev/sdb 为例：
Ubuntu 20 $ sudo dd oflag=dsync if=./arch/arm/boot/zImage of=/dev/sdb seek=2M status=progress
Ubuntu 20 $ sudo dd oflag=dsync if=./arch/arm/boot/dts/exynos4412-itop-pop1g-elite.dtb of=/dev/sdb seek=1M status=progress
Ubuntu 20 $ sudo sync
Ubuntu 20 $ sudo eject /dev/sdb
```

余下同 Kernel 3.0.15 类似，略。

### 加载根文件系统

在 `exynos4412-itop-pop1g-elite.dts` 中设置：

```dtd
bootargs = "root=/dev/mmcblk0p2 rw console=ttySAC2,115200 init=/linuxrc rootfstype=ext3 rootwait rw";
```

`/dev/mmcblk0p2` 即为根文件系统所在分区。

<br/>

## 根文件系统 BusyBox 1.36.1

### 编译

```shell
Ubuntu 20 $ mkdir rootfs
Ubuntu 20 $ cd busybox-1.22.1
Ubuntu 20 $ make menuconfig
```

修改：

```
Busybox Settings  --->
	Build Options  --->
		(arm-linux-gnueabihf-) Cross Compiler prefix
	Installation Options ("make install" behavior)  --->
		(../rootfs) BusyBox installation prefix
```

编译：

```shell
Ubuntu 20 $ make
Ubuntu 20 $ make install
Ubuntu 20 $ cd ..
Ubuntu 20 $ cp -ru rootfs_patch/* rootfs/
```



### 写入 SD 卡

将 SD 卡的第2分区 mount 到 ubuntu 中，然后将 rootfs 文件夹中的内容直接拷贝到第2分区中。



### 写入 EMMC

#### 制作 system.img

使用开发板资料中的 make_ext4fs：

```bash
# sudo apt install lib32stdc++6
Ubuntu 20 $ ./make_ext4fs -s -l 314572800 -a root -L Linux ./Release/system.img rootfs
# ./make_ext4fs -s -l 1073741824 -a root -L Linux ./Release/system4.img rootfs4
```

或者使用当前可安装的工具：

```shell
# sudo apt-get install e2fsprogs (https://command-not-found.com/mkfs.ext4)
# Old version: make_ext4fs -s -l 314572800 -a root -L Linux system.img system
Ubuntu 20 $ dd if=/dev/zero of=./Release/system.img bs=4K count=4096
# File size = 4K * 4096 = 16MB
Ubuntu 20 $ mkfs.ext4 -F -L Linux ./Release/system.img

Ubuntu 20 $ mkdir mnt_rootfs
Ubuntu 20 $ sudo mount ./Release/system.img ./mnt_rootfs
Ubuntu 20 $ sudo cp -r ./rootfs/* mnt_rootfs/
Ubuntu 20 $ sudo umount mnt_rootfs
```

下载 system.img 到 SD 卡的 sdupdate/


```bash
iTOP-4412 $ sdfuse flash system system.img
SD sclk_mmc is 400K HZ
SD sclk_mmc is 50000K HZ
SD sclk_mmc is 50000K HZ
[Fusing Image from SD Card.]
[Partition table on MoviNAND]
ptn 0 name='bootloader' start=0x0 len=N/A (use hard-coded info. (cmd: movi))
ptn 1 name='kernel' start=N/A len=N/A (use hard-coded info. (cmd: movi))
ptn 2 name='ramdisk' start=N/A len=0x300000(~3072KB) (use hard-coded info. (cmd: movi))
ptn 3 name='Recovery' start=N/A len=0x600000(~6144KB) (use hard-coded info. (cmd: movi))
ptn 4 name='system' start=0x15DDA00 len=0x403B0600(~1052353KB) 
ptn 5 name='userdata' start=0x4198E000 len=0x403B0600(~1052353KB) 
ptn 6 name='cache' start=0x81D3E600 len=0x1321EC00(~313467KB) 
ptn 7 name='fat' start=0x94F5D200 len=0x3BAFB600(~977901KB) 
Partition: system, File: /sdupdate/system.img
Partition1: Start Address(0x1ccf6e), Size(0x1f6b2a)
reading /sdupdate/system.img
15561800 (0x00ed7448) bytes read
flashing 'system'
Compressed ext3 image

MMC write: dev # 0, block # 44781, count 8. 8 blocks write finish
8 blocks verify1: OK

...

MMC write: dev # 0, block # 569069, count 16. 16 blocks write finish
16 blocks verify1: OK
partition 'system' flashed
```



<br/>

## 根文件系统 Ubuntu 16

### 编译

无需编译，直接在 Ubuntu 编译好的基础包上进行配置，具体操作见 [系统移植 iTOP-4412 Ubuntu](/tech/itop4412-ubuntu/)。

### 写入 SD 卡

将 SD 卡的第2分区 mount 到 ubuntu 中，然后将 rootfs 文件夹中的内容直接拷贝到第2分区中。



<br/>

## 参考

* 《iTOP-4412系列开发板实战教程》
* Chapter 5. Booting Sequence @ 《SEC_Exynos4412_Users Manual_Ver.0.30.00_Preliminary.pdf》
* [各类板子 bring up_芯片bring up-CSDN博客](https://blog.csdn.net/lyndon_li/article/details/129807113)
* [2020.09.12 - xhr4412 移植 u-boot-2020.07 & linux-5.8.5 & BusyBox-1.31.1 到 iTOP-4412 汇总_linux5.8.12系统移植-CSDN博客](https://blog.csdn.net/u012849539/article/details/108553847)
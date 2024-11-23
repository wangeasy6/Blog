---
title: iTOP-4412 UBoot 使用笔记
date: 2024-11-20 22:42:00
toc: true
categories:
  - Technology
tags:
  - 嵌入式
---

基于讯为开发板 - iTOP-4412 POP 1G Elite（精英板）的 UBoot 使用笔记。

* UBoot version: [iTop4412_uboot_20151119.tar.gz](https://github.com/TOPEET-Develop/iTop4412_uboot_public_merge), Based on u-boot-2010.03.
* Linux version1: [iTop4412_Kernel_3.0_20160811.tar.gz](https://github.com/TOPEET-Develop/iTop4412_kernel_public_merge), Based on v3.0.15.
* Linux version2: [linux-4.19.323](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/snapshot/linux-4.19.323.tar.gz)

<!--more-->

<br/>

## 启动日志

```
OK

U-Boot 2010.03-dirty (Nov 18 2024 - 22:28:36) for iTOP-4412 Android


CPU:	SMDK4412-AP1.1 [e4412211]
	APLL = 1000MHz, MPLL = 800MHz
	ARM_CLOCK = 1000MHz
PMIC:	S5M8767(VER5.0)
Board:	iTOP-4412Quad
POP type: POP for C220
DRAM:	1023 MB
MMC:   max_emmc_clock:40 MHZ
Set CLK to 400 KHz
EMMC CLOCK OUTPUT:: 400KHz -[div:50]
response timeout error : 00000104 cmd 8
response timeout error : 00000104 cmd 55
max_emmc_clock:40 MHZ
Input CLK [ 50 MHz] is higher than limit [40 MHZ]
Set CLK to 40000 KHz
EMMC clock output: 40000 KHz
max_emmc_clock:40 MHZ
Input CLK [ 50 MHz] is higher than limit [40 MHZ]
Set CLK to 40000 KHz
EMMC clock output: 40000 KHz
[mmc_init] err:0
MMC0:	7456 MB
SD sclk_mmc is 400K HZ
SD sclk_mmc is 50000K HZ
SD sclk_mmc is 50000K HZ
[mmc_init] err:0
MMC1:	1940 MB
*** Warning - using default environment

In:    serial
Out:   serial
Err:   serial


Checking Boot Mode ... SDMMC
SYSTEM ENTER NORMAL BOOT MODE
Hit any key to stop autoboot:  0 
iTOP-4412 $ 
```

<br/>

# 存储情况

## 板载 EMMC

```sh
iTOP-4412 $ mmcinfo 0
Device: S5P_MSHC4
Manufacturer ID: 15
OEM: 100
Name: 8WPD3 
Tran Speed: 0
Rd Block Len: 512
MMC version 4.0
High Capacity: Yes
Size: 7456MB (block: 15269888)
Bus Width: 2-bit

iTOP-4412 $ fdisk -p 0

partion #    size(MB)     block start #    block count    partition_Id 
   1          5050          4881129        10344411          0x0C 
   2          1027            44781         2104707          0x83 
   3          1027          2149488         2104707          0x83 
   4           306          4254195          626934          0x83
```

## TF 卡

在 Ubuntu 上使用更完整的 `fdisk`(fdisk from util-linux 2.34) 进行查看：

```sh
iTOP-4412 $ mmcinfo 1 
Device: S3C_HSMMC2
Manufacturer ID: 12
OEM: 3456
Name: SD    
Tran Speed: 0
Rd Block Len: 512
SD version 1.10
High Capacity: No
Size: 1940MB (block: 3973120)
Bus Width: 2-bit

Ubuntu $ fdisk /dev/sdb
Command (m for help): p
Disk /dev/sdb: 1.92 GiB, 2034237440 bytes, 3973120 sectors
Disk model: Storage Device  
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x00000000

Device     Boot   Start     End Sectors   Size Id Type
/dev/sdb1       3368295 3947159  578865 282.7M  c W95 FAT32 (LBA)
/dev/sdb2         34965 2132864 2097900     1G 83 Linux
/dev/sdb3       2132865 2750579  617715 301.6M 83 Linux
/dev/sdb4       2750580 3368294  617715 301.6M 83 Linux


Command (m for help): F
Unpartitioned space /dev/sdb: 28.7 MiB, 29436416 bytes, 57493 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes

  Start     End Sectors  Size
   2048   34964   32917 16.1M
3948544 3973119   24576   12M
```

除了正常的分区以外，2048  - 34964 还预留了约 16MB 的未分区区域，用于原始数据的读写（dd@Ubuntu, mmc@UBoot）。

除此以外，1 - 2048 这约 1MB 的区域，同样可以用于原始数据的读写，一般用于放置 UBoot，因为除了第一个 block 默认用于放置 mbr 分区表，加载程序会从此开始读指令。

<br/>

# U-Boot 常用命令

## ?/help

要使用完整的 help，需要修复一个Bug：

```diff
--- a/iTop4412_uboot/common/cmd_help.c
+++ b/iTop4412_uboot/common/cmd_help.c
@@ -27,7 +27,7 @@
 int do_help(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
 {
        return _do_help(&__u_boot_cmd_start,
-                       ((cmd_tbl_t *)(&__u_boot_cmd_end) - (cmd_tbl_t *)(&__u_boot_cmd_start))/sizeof(cmd_tbl_t *),
+                       &__u_boot_cmd_end - &__u_boot_cmd_start,
                        cmdtp, flag, argc, argv);
 }
```

获取全部的简要帮助信息：

```sh
iTOP-4412 $ help 
?       - alias for 'help'
base    - print or set address offset
bdinfo  - print Board Info structure
boot    - boot default, i.e., run 'bootcmd'
bootd   - boot default, i.e., run 'bootcmd'
bootelf - Boot from an ELF image in memory
bootm   - boot application image from memory
bootvx  - Boot vxWorks from an ELF image
cmp     - memory compare
coninfo - print console devices and information
cp      - memory copy
crc32   - checksum calculation
dnw     - dnw     - initialize USB device and ready to receive for Windows server (specific)

echo    - echo args to console
editenv - edit environment variable
emmc    - Open/Close eMMC boot Partition
ext2format- ext2format - disk format by ext2

ext2load- load binary file from a Ext2 filesystem
ext2ls  - list files in a directory (default /)
ext3format- ext3format - disk format bt ext3

fastboot- fastboot- use USB Fastboot protocol

fatformat- fatformat - disk format by FAT32

fatinfo - fatinfo - print information about filesystem
fatload - fatload - load binary file from a dos filesystem

fatls   - list files in a directory (default /)
fdisk   - fdisk	- fdisk for sd/mmc.

fdt     - flattened device tree utility commands
go      - start application at address 'addr'
help    - print command description/usage
iminfo  - print header information for application image
imxtract- extract a part of a multi-image
itest   - return true/false on integer compare
loadb   - load binary file over serial line (kermit mode)
loads   - load S-Record file over serial line
loady   - load binary file over serial line (ymodem mode)
loop    - infinite loop on address range
max8997 - max8997     - Test the MAX8997 (I2C Interface.)

md      - memory display
mdmup   - mdmup - modem upgrade support

mm      - memory modify (auto-incrementing address)
mmc     - MMC sub system
mmcinfo - mmcinfo <dev num>-- display MMC info
movi    - movi	- sd/mmc r/w sub system for SMDK board

mtest   - simple RAM read/write test
mw      - memory write (fill)
nm      - memory modify (constant address)
printenv- print environment variables
reset   - Perform RESET of the CPU
run     - run commands in an environment variable
saveenv - save environment variables to persistent storage
sdfuse  - sdfuse  - read images from FAT partition of SD card and write them to booting device.

setenv  - set environment variables
sleep   - delay execution for some time
source  - run script from memory
version - print monitor version
```

获取特定指令的详细信息：

```sh
iTOP-4412 $ help base
base - print or set address offset

Usage:
base 
    - print address offset for memory commands
base off
    - set address offset for memory commands to 'off'
```

<br/>



## 环境变量相关

### bdinfo

```sh
# bdinfo - print Board Info structure
iTOP-4412 $ bdinfo
arch_number = 0x00000B16
env_t       = 0x00000000
boot_params = 0x40000100
DRAM bank   = 0x00000000
-> start    = 0x40000000
-> size     = 0x10000000
DRAM bank   = 0x00000001
-> start    = 0x50000000
-> size     = 0x10000000
DRAM bank   = 0x00000002
-> start    = 0x60000000
-> size     = 0x10000000
DRAM bank   = 0x00000003
-> start    = 0x70000000
-> size     = 0x0FF00000
DRAM bank   = 0x00000004
-> start    = 0x00000000
-> size     = 0x00000000
DRAM bank   = 0x00000005
-> start    = 0x00000000
-> size     = 0x00000000
DRAM bank   = 0x00000006
-> start    = 0x00000000
-> size     = 0x00000000
DRAM bank   = 0x00000007
-> start    = 0x00000000
-> size     = 0x00000000
baudrate    = 115200 bps
```

arch_number 在 `include/configs/itop_4412_android.h` 的 `MACH_TYPE` 定义。

### printenv

```sh
iTOP-4412 $ help printenv
printenv - print environment variables

Usage:
printenv 
    - print values of all environment variables
printenv name ...
    - print value of environment variable 'name'
iTOP-4412 $ printenv
bootdelay=2
baudrate=115200
stdin=serial
stdout=serial
stderr=serial
bootcmd=movi read kernel 40008000;bootm 40008000 40d00000

Environment size: 127/16380 bytes
```

从代码中直接修改 `bootcmd` 的方法：

```diff
--- a/iTop4412_uboot/board/samsung/smdkc210/smdkc210.c
+++ b/iTop4412_uboot/board/samsung/smdkc210/smdkc210.c
@@ -273,7 +273,8 @@ int board_late_init (void)
 #ifdef SMDK4412_SUPPORT_UBUNTU
                sprintf(boot_cmd, "movi read kernel 40008000;bootm 40008000 40d00000");
 #else
-               sprintf(boot_cmd, "movi read kernel 40008000;movi read rootfs 40df0000 100000;bootm 40008000 40df0000");
+               // sprintf(boot_cmd, "movi read kernel 40008000;movi read rootfs 40df0000 100000;bootm 40008000 40df0000");
+               sprintf(boot_cmd, "mmc read 1 40008000 1000 5000;mmc read 1 43000000 800 80;bootm 40008000 42000000 43000000");
```

### setenv

```sh
iTOP-4412 $ help setenv
setenv - set environment variables

Usage:
setenv name value ...
    - set environment variable 'name' to 'value ...'
setenv name
    - delete environment variable 'name'

# Example
iTOP-4412 $setenv bootcmd 'mmc read 1 0x40008000 0x1000 0x5000;mmc read 1 0x43000000 0x800 0x80;bootm 0x40008000 0x42000000 0x43000000'
```



### saveenv

将环境变量保存到持久存储。

<br/>

## 给 mmc 分区

```sh
# 2GB TF card test example.
iTOP-4412 $ fdisk -c 1 300 300 300
iTOP-4412 $ fdisk -p 1

partion #    size(MB)     block start #    block count    partition_Id 
   1          1005          1888110         2059050          0x0C 
   2           301            34965          617715          0x83 
   3           301           652680          617715          0x83 
   4           301          1270395          617715          0x83
iTOP-4412 $ fatformat mmc 1:1
iTOP-4412 $ ext3format mmc 1:2
iTOP-4412 $ ext3format mmc 1:3
iTOP-4412 $ ext3format mmc 1:4
```

### fdisk

```sh
iTOP-4412 $ help fdisk
fdisk - fdisk	- fdisk for sd/mmc.

Usage:
fdisk -c <device_num> [<sys. part size(MB)> <user part size> <cache part size>]	- create partition.
fdisk -p <device_num>	- print partition information
```

后 3 个参数，分别指代 sys，user，cache 的大小，如果不指定：

* 对于 device 0：分别默认为 1G，1G，300M，分区标志为 0x83。
* 对于 device 1，分别默认为 1G，300M，300M，分区标志为 0x83。

多余的存储空间，将作自动分为一个盘，分区标志为 0x0C。

需要注意的是，多余的空间默认是 p1，然后才是顺序的参数指定的大小。

分区标志（partition_Id）：

* 0x05：扩展分区
* 0x07：HPFS/NTFS分区
* 0x83：Linux分区
* 0x0B：FAT32分区
* 0x0C：FAT32分区
* 0x0F：扩展分区
* 0x1C：隐藏的FAT32分区
* 0xEE：GPT分区

### fatformat

```sh
iTOP-4412 $ help fatformat
fatformat - fatformat - disk format by FAT32

Usage:
fatformat <interface(only support mmc)> <dev:partition num>
	- format by FAT32 on 'interface'
```

### ext3format

```sh
iTOP-4412 $ help ext3format
ext3format - ext3format - disk format bt ext3

Usage:
ext3format 	- format by ext3 on 'interface'
```

在 Ubuntu 系统下，可以使用 mkfs.ext3 来实现同样的功能。

```
mkfs.ext3 /dev/sdxp             # x 代表 device_num, p 代表 partition_num
mkfs.ext3 -b 4096 /dev/sdxp     # -b 选项用于指定块大小，单位为字节。4096 是一个常见的块大小。
mkfs.ext3 -L mylabel /dev/sdxp	# -L 选项用于设置文件系统的卷标（label）。
mkfs.ext3 -F /dev/sdxp			# -F 选项用于强制格式化，即使设备或分区已经挂载。
```

mkfs 命令簇包含以下格式的文件系统格式化处理：

```
mkfs.bfs       mkfs.exfat     mkfs.ext4      mkfs.minix     mkfs.reiserfs  
mkfs.btrfs     mkfs.ext2      mkfs.f2fs      mkfs.msdos     mkfs.vfat      
mkfs.cramfs    mkfs.ext3      mkfs.fat       mkfs.ntfs      mkfs.xfs
```

<br/>

## 读写存储

### mmc

```sh
iTOP-4412 $ help mmc
mmc - MMC sub system

Usage:
mmc read <device num> addr blk# cnt
mmc write <device num> addr blk# cnt
mmc rescan <device num>
mmc erase <boot | user> <device num> <start block> <block count>
mmc list - lists available devices

# mmc read example:
# ######### mmc read/write device mem_addr   block_start block_count
iTOP-4412 $ mmc read       1      0x43000000 0x800       0x80
```

block_size = 512 Byte

**read**：从 mmc 的 block_start 位置读 count 个 block 到 内存地址。

**write**：从内存地址读 count 个 block，写入 mmc 的 block_start 位置。

mem_addr, block_start, block_count 用 16 进制数表示。



### fatload

从 TF 卡加载，并启动内核：

```sh
iTOP-4412 $ help fatload
fatload - fatload - load binary file from a dos filesystem

Usage:
fatload <interface> <dev[:part]>  <addr> <filename> [bytes]
    - load binary file 'filename' from 'dev' on 'interface'
      to address 'addr' from dos filesystem

iTOP-4412 $ fatload mmc 1:1 40008000 /sdupdate/zImage
```



### sdfuse

快捷地将 TF 卡的文件写入 MMC0（板载MMC）的工具。将对应的文件放入到 TF 卡的 FAT 磁盘的 `/sdupdate` 文件夹下，就可以开始烧写了。

```sh
iTOP-4412 $ sdfuse flashall
# flashall 包含以下单项

iTOP-4412 $ sdfuse flash kernel zImage
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
Partition1: Start Address(0x336567), Size(0x8d531)
reading /sdupdate/zImage
6774472 (0x00675ec8) bytes read
flashing 'kernel'
writing kernel.. 1120, 20480 
MMC write: dev # 1, block # 1120, count 20480. 20480 blocks write finish
20480 blocks written: OK
completed
partition 'kernel' flashed

# sdfuse flash kernel 其实是下面 2 条命令的结合，实现从 SD 卡读指定文件到内存，再从内存写入 MMC0 的 kernel 位置
# fatload mmc 1:1 40008000 /sdupdate/zImage
# mmc write 0 40008000 0x460 0x5000
# (0x460 == 1120, 0x5000 == 20480)
# 下同

iTOP-4412 $ sdfuse flash bootloader u-boot-iTOP-4412.bin
iTOP-4412 $ sdfuse flash ramdisk ramdisk-uboot.img
iTOP-4412 $ sdfuse flash system system.img
```

默认使用哪个mmc启动，就写入哪个mmc。

详情：

```sh
iTOP-4412 $ sdfuse flash ramdisk ramdisk-uboot.img
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
Partition: ramdisk, File: /sdupdate/ramdisk-uboot.img
Partition1: Start Address(0x1ccf6e), Size(0x1f6b2a)
reading /sdupdate/ramdisk-uboot.img
651926 (0x0009f296) bytes read
flashing 'ramdisk'
writing RFS.. 21600, 1274 
MMC write: dev # 1, block # 21600, count 1274. 1274 blocks write finish
1274 blocks verify1: OK
completed
partition 'ramdisk' flashed
```

### movi

```sh
iTOP-4412 $ help movi
movi - sd/mmc r/w sub system for SMDK board

Usage:
movi init - Initialize moviNAND and show card info
movi read  {u-boot | kernel} {addr} - Read data from sd/mmc
movi write {fwbl1 | u-boot | kernel} {addr} - Write data to sd/mmc
movi read  rootfs {addr} [bytes(hex)] - Read rootfs data from sd/mmc by size
movi write rootfs {addr} [bytes(hex)] - Write rootfs data to sd/mmc by size
movi read  {sector#} {bytes(hex)} {addr} - instead of this, you can use "mmc read"
movi write {sector#} {bytes(hex)} {addr} - instead of this, you can use "mmc write"
```

从最后 2 条可以看出，movi 最终调用的是 mmc，比 mmc 提供了更便捷的读取 u-boot、kernel、rootfs 的方法。

<br/>

## 启动命令

### boot/bootd

boot default, run 'bootcmd'.



### bootm

```sh
iTOP-4412 $ help bootm
bootm - boot application image from memory

Usage:
bootm [addr [arg ...]]
    - boot application image stored in memory
	passing arguments 'arg ...'; when booting a Linux kernel,
	'arg' can be the address of an initrd image
	When booting a Linux kernel which requires a flat device-tree
	a third argument is required which is the address of the
	device-tree blob. To boot that kernel without an initrd image,
	use a '-' for the second argument. If you do not pass a third
	a bd_info struct will be passed instead

Sub-commands to do part of the bootm sequence.  The sub-commands must be
issued in the order below (it's ok to not issue all sub-commands):
	start [addr [arg ...]]
	loados  - load OS image
	fdt     - relocate flat device tree
	cmdline - OS specific command line processing/setup
	bdt     - OS specific bd_t processing
	prep    - OS specific prep before relocation or go
	go      - start OS
```



### go

一般用于调试裸机程序。

```sh
iTOP-4412 $ help go
go - start application at address 'addr'

Usage:
go addr [arg ...]
    - start application at address 'addr'
      passing 'arg' as arguments
```

<br/>

# 启动 Kernel

## v3.0.15

将编译好的 zImage、system.img 放到 TF 卡的 /sdupdate。

```sh
iTOP-4412 $ sdfuse flash kernel zImage
iTOP-4412 $ sdfuse flash system system.img

# boot from EMMC
# bootcmd=movi read kernel 40008000;bootm 40008000
iTOP-4412 $ movi read kernel 40008000
reading kernel.. 1120, 20480 
MMC read: dev # 0, block # 1120, count 20480 ...20480 blocks read: OK
completed
iTOP-4412 $ bootm 40008000
Boot with zImage
Wrong Ramdisk Image Format
[err] boot_get_ramdisk
## Transferring control to Linux (at address 40008000) ...

Starting kernel ...


Starting with bi_boot_params.
Uncompressing Linux... done, booting the kernel.
[    0.000000] Initializing cgroup subsys cpu
...
```

## v4.19.323

```sh
# @Ubuntu, TF card display as /dev/sdb.
ubuntu $ sudo dd iflag=dsync oflag=dsync if=arch/arm/boot/zImage of=/dev/sdb seek=4096
# @ arch/arm/boot/dts/exynos4412-itop-elite.dts
# bootargs = "root=/dev/mmcblk0p2 rw console=ttySAC2,115200 init=/linuxrc rootwait";
ubuntu $ sudo dd iflag=dsync oflag=dsync if=arch/arm/boot/dts/exynos4412-itop-elite.dtb of=/dev/sdb seek=2048
ubuntu $ mkdir tf_system
ubuntu $ sudo mount /dev/sdb2 tf_system
ubuntu $ sudo cp -r rootfs/* tf_system
ubuntu $ sudo umount tf_system

# boot from TF card
# bootcmd=mmc read 1 0x40008000 0x1000 0x5000;mmc read 1 0x43000000 0x800 0x80;bootm 0x40008000 0x42000000 0x43000000

iTOP-4412 $ mmc read 1 0x40008000 0x1000 0x5000;mmc read 1 0x43000000 0x800 0x80;bootm 0x40008000 0x42000000 0x43000000

MMC read: dev # 1, block # 4096, count 20480 ...20480 blocks read: OK

MMC read: dev # 1, block # 2048, count 128 ...128 blocks read: OK
Boot with zImage
Wrong Ramdisk Image Format
[err] boot_get_ramdisk
## Transferring control to Linux (at address 40008000) ...

Starting kernel ...


Uncompressing Linux... done, booting the kernel.
[    0.000000] Booting Linux on physical CPU 0xa00
...
```

由于使用了设备树文件作为启动项，为了支持传入 dtb，需要修改以下代码：

（主要作用是把 ramdisk 屏蔽，把 dtb 的地址传给 Kernel。由于 ramdisk 被屏蔽了，所以地址 0x42000000 其实只起一个占位作用。）

```diff
--- a/iTop4412_uboot/include/configs/itop_4412_android.h
+++ b/iTop4412_uboot/include/configs/itop_4412_android.h
@@ -31,6 +31,7 @@
 #define CONFIG_S5PC210      1       /* in a SAMSUNG S5PC210 SoC */
 #define CONFIG_SMDKC210     1
 #define CONFIG_DEVICE_STRING    "iTOP-4412"
+#define CONFIG_OF_LIBFDT 1


--- a/iTop4412_uboot/common/cmd_bootm.c
+++ b/iTop4412_uboot/common/cmd_bootm.c
@@ -294,6 +294,8 @@ int bootm_start(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
        if (((images.os.type == IH_TYPE_KERNEL) ||
             (images.os.type == IH_TYPE_MULTI)) &&
            (images.os.os == IH_OS_LINUX)) {
+
+#if defined(CONFIG_OF_RAMDISK)
                /* find ramdisk */
                ret = boot_get_ramdisk (argc, argv, &images, IH_INITRD_ARCH,
                                &images.rd_start, &images.rd_end);
@@ -301,9 +303,10 @@ int bootm_start(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
                        puts ("Ramdisk image is corrupt or invalid\n");
                        return 1;
                }
+#endif
 
 #if defined(CONFIG_OF_LIBFDT)
-#if defined(CONFIG_PPC) || defined(CONFIG_M68K) || defined(CONFIG_SPARC)
+// #if defined(CONFIG_PPC) || defined(CONFIG_M68K) || defined(CONFIG_SPARC)
                /* find flattened device tree */
                ret = boot_get_fdt (flag, argc, argv, &images,
                                    &images.ft_addr, &images.ft_len);
@@ -313,7 +316,7 @@ int bootm_start(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
                }
 
                set_working_fdt_addr(images.ft_addr);
-#endif
+// #endif
 #endif
        }

--- a/iTop4412_uboot/common/image.c
+++ b/iTop4412_uboot/common/image.c
@@ -1176,6 +1176,7 @@ static int fit_check_fdt (const void *fit, int fdt_noffset, int verify)
  *      0 - success
  *      1 - failure
  */
+#if defined(CONFIG_SYS_BOOTMAPSZ)
 int boot_relocate_fdt (struct lmb *lmb, ulong bootmap_base,
                char **of_flat_tree, ulong *of_size)
 {
@@ -1255,6 +1256,7 @@ int boot_relocate_fdt (struct lmb *lmb, ulong bootmap_base,
 error:
        return 1;
 }
+#endif /* CONFIG_SYS_BOOTMAPSZ */


--- a/iTop4412_uboot/lib_arm/bootm.c
+++ b/iTop4412_uboot/lib_arm/bootm.c
@@ -88,6 +88,8 @@ int do_bootm_linux(int flag, int argc, char *argv[], bootm_headers_t *images)
 
        debug ("## Transferring control to Linux (at address %08lx) ...\n",
               (ulong) theKernel);
+       printf ("## Transferring control to Linux (at address %08lx) ...\n",
+              (ulong) theKernel);
 
 #if defined (CONFIG_SETUP_MEMORY_TAGS) || \
     defined (CONFIG_CMDLINE_TAG) || \
@@ -131,7 +133,17 @@ int do_bootm_linux(int flag, int argc, char *argv[], bootm_headers_t *images)
 
        cleanup_before_linux ();
 
-       theKernel (0, machid, bd->bi_boot_params);
+       if(argc > 3)
+       {
+               unsigned long fdt_addr = simple_strtoul(argv[3], NULL, 16);
+               printf ("\nStarting with fdt-0x%08lx\n", fdt_addr);
+               theKernel (0, machid, fdt_addr);
+       }
+       else
+       {
+               printf ("\nStarting with bi_boot_params.\n");
+               theKernel (0, machid, bd->bi_boot_params);
+       }
        /* does not return */
```


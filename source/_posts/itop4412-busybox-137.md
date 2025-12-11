---
title: 系统移植 iTOP-4412 BusyBox
toc: true
categories:
  - tech
tags:
  - 嵌入式
---

[Busybox](www.busybox.net)，是一个未完全实现的Unix系统，可以裁剪为最小的一个运行系统，可以包含一些简单的命令。

**制作环境：**

* **Board:** iTOP-4412 POP 1G 精英板
* **Root FS:** [BusyBox 1.37.0](https://busybox.net/downloads/busybox-1.37.0.tar.bz2)
* **Compiler:** [gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf](https://releases.linaro.org/components/toolchain/binaries/latest-7/arm-linux-gnueabihf/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar.xz)

<!--more-->

<br/>

## 依赖

curses：`sudo apt-get install libncurses5-dev libncursesw5-dev`

<br/>

## 编译

```bash
make defconfig
make menuconfig
```

设置：

```
 Busybox Settings  --->   
    Build Options  --->             
         (arm-linux-gnueabihf-) Cross Compiler prefix  
    Installation Options ("make install" behavior)  ---> 
         (../bb137) Destination path for 'make install' (NEW)  
```

错误1：

```
libbb/hash_md5_sha.c: In function ‘sha1_end’:
libbb/hash_md5_sha.c:1316:28: error: ‘sha1_process_block64_shaNI’ undeclared (first use in this function); did you mean ‘sha1_process_block64’?
   || ctx->process_block == sha1_process_block64_shaNI
```

解决：

```diff
 libbb/hash_md5_sha.c | 2 ++
 1 file changed, 2 insertions(+)

diff --git a/libbb/hash_md5_sha.c b/libbb/hash_md5_sha.c
index 57a801459..75a61c32c 100644
--- a/libbb/hash_md5_sha.c
+++ b/libbb/hash_md5_sha.c
@@ -1313,7 +1313,9 @@ unsigned FAST_FUNC sha1_end(sha1_ctx_t *ctx, void *resbuf)
 	hash_size = 8;
 	if (ctx->process_block == sha1_process_block64
 #if ENABLE_SHA1_HWACCEL
+# if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
 	 || ctx->process_block == sha1_process_block64_shaNI
+# endif
 #endif
 	) {
 		hash_size = 5;
```

错误2：

```bash
networking/libiproute/iplink.c: In function ‘do_set_can’:
networking/libiproute/iplink.c:454:35: error: ‘IFLA_CAN_TERMINATION’ undeclared (first use in this function); did you mean ‘IFLA_HSR_VERSION’?
    addattr_l(&req.n, sizeof(req), IFLA_CAN_TERMINATION, &val, sizeof(val));
```

解决：

```diff
--- a/networking/libiproute/iplink.c      2024-04-16 15:23:50.000000000 +0800
+++ b/networking/libiproute/iplink.c       2025-02-21 16:55:56.196723363 +0800
@@ -18,6 +18,10 @@
 #include "rt_names.h"
 #include "utils.h"
 
+#ifdef IFLA_CAN_TERMINATION
+#include <linux/can.h>
+#endif
+
 #undef  ETH_P_8021AD
 #define ETH_P_8021AD            0x88A8
 #undef  VLAN_FLAG_REORDER_HDR
@@ -451,7 +455,12 @@
                        uint16_t val;
                        NEXT_ARG();
                        val = get_u16(*argv, keyword);
+            #ifdef IFLA_CAN_TERMINATION
                        addattr_l(&req.n, sizeof(req), IFLA_CAN_TERMINATION, &val, sizeof(val));
+            #else
+            // Handle the case where IFLA_CAN_TERMINATION is not defined
+            fprintf(stderr, "IFLA_CAN_TERMINATION is not supported on this system.\n");
+            #endif
                        break;
                }
                default:
```

编译：

```sh
make
make install
```



<br/>

## 添加其它文件

1. 创建所需文件夹（根据FHS标准）
   `mkdir dev etc home lib media mnt opt proc sys tmp var root`

2. 添加相应的库

   从编译busybox所使用的交叉编译工具里拷贝，使用 `arm-linux-gnueabihf-readelf -d bin/busybox` 查询到：

   ```
     Tag        Type                         Name/Value
    0x00000001 (NEEDED)                     Shared library: [libm.so.6]
    0x00000001 (NEEDED)                     Shared library: [libresolv.so.2]
    0x00000001 (NEEDED)                     Shared library: [libc.so.6]
   ```

   将这 3 个库文件拷贝到 lib 文件夹下。

   一般把正常需要的库都拷贝到/lib下，这样也不会太大，包括：
   ld-linux:动态链接库，必需
   libc: 标准c函数库，必需
   libm: 数学库，一般需要
   libdl: 用于动态装载共享库，较少用到
   libcrypt: 加密附加库，需要认证的程序用到，较少用
   libpthread: POSIX线程库，一般需要

3. 添加配置文件

​	vim etc/inittab

```
::sysinit:/etc/init.d/rcS
console::askfirst:-/bin/sh
::once:/usr/sbin/telnetd -l /bin/login
::ctrlaltdel:/sbin/reboot
::shutdown:/bin/umount -a -r
```

​	mkdir etc/init.d; vim etc/init.d/rcS

```sh
#!/bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin
export PATH

#
#       Trap CTRL-C &c only in this shell so we can interrupt subprocesses.
#

mount -a
mkdir -p /dev/pts
mount -t devpts devpts /dev/pts
echo /sbin/mdev > /proc/sys/kernel/hotplug
mdev -s
mkdir -p /var/lock

hwclock -s
feed_wdg &

ifconfig lo 127.0.0.1
ifconfig eth0 192.168.0.99

/bin/hostname -F /etc/HOSTNAME
```

​	vim etc/HOSTNAME

```
Easy4412
```

​	vim fstab

```
#device         mount-point     type    options         dump    fsck order
proc            /proc           proc    defaults                0       0
tmpfs           /tmp            tmpfs   defaults                0       0
sysfs           /sys            sysfs   defaults                0       0
tmpfs           /dev            tmpfs   defaults                0       0
var             /dev            tmpfs   defaults                0       0
ramfs           /dev            ramfs   defaults                0       0
```

​	vim etc/profile

```
USER="`id -un`"
LOGNAME=$USER
PS1='[\u@\h \W]# '      # 这个显示的是命令行下的主机名和用户名格式，如：[root@Blue etc]#
PATH=$PATH

HOSTNAME=`/bin/hostname`

export USER LOGNAME PS1 PATH
```

​	vim etc/group

```
root:x:0:root
```

​	vim etc/passwd

```
root::0:0:root:/:/bin/sh
```

## 制作镜像文件

```sh
dd if=/dev/zero of=rootfs.ext4 bs=1M count=1024
```

这个命令会创建一个名为image.img的文件，大小为1GB（Ext4默认保留5%的空间）。这个文件的内容全部是零，因为我们使用的输入文件（if）是/dev/zero，这是一个特殊的文件，读取它会得到无限的零。

```sh
mkfs.ext4 rootfs.ext4
```

这个命令会将image.img文件格式化为ext4文件系统。

```
sudo mkdir /mnt/myimage
sudo mount -o loop rootfs.ext4 /mnt/myimage
```

这个命令会将image.img文件挂载到/mnt/myimage目录。注意，我们需要使用-o loop选项，因为我们是挂载一个文件，而不是一个设备。

操作镜像文件：现在，我们可以在/mnt/myimage目录下创建文件、写入数据等，所有的操作都会被写入到image.img文件中。

操作完成后，我们需要卸载这个镜像文件`sudo umount /mnt/myimage`。

```sh
sudo apt install android-sdk-ext4-utils
make_ext4fs -s -l 996147200 -a root -L linux rootfs.ext4 MiniFS
```



## 参考

* [linux根文件系统制作之busybox编译和系统构建-枫露清愁-ChinaUnix博客](http://blog.chinaunix.net/uid-29401328-id-5019660.html)
* [ext4文件系统镜像制作教程 - 小蓝博客 (8kiz.cn)](https://www.8kiz.cn/archives/6149.html)
* [[PATCH\] libbb/sha: add missing sha-NI guard](https://lists.uclibc.org/pipermail/busybox/2024-September/090899.html)
* [Linux qemu-arm的使用_qemu-system-arm-CSDN博客](https://blog.csdn.net/heyan131/article/details/145829681)

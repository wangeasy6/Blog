---
title: iTOP-4412 第三方软件移植-Openssh
date: 2024-11-08 00:06:00
toc: true
categories:
  - tech
tags:
  - 嵌入式
---

想要使用 scp 方便地通过网络下载文件到开发板，想要通过 ssh 网络登录到开发板，就需要编译和部署 openssh，但是 busybox 默认是不带 openssh 的，这时候就需要我们手动编译了。

试验环境：

* **Board:** iTOP-4412 POP 1G Elite（精英板）
* **Compiler:** arm-linux-gnueabi-gcc version 4.4.1 (Sourcery G++ Lite 2009q3-67)
* **Kernel:** linux-3.0.15
* **Root FS:** busybox-1.22.1

编译版本：

* zlib-1.2.8（dep）
* openssl-1.1.0l（dep）
* openssh-9.3p1

<!--more-->

<br/>

默认已将交叉工具编译链在环境变量中配好。

## zlib

```shell
$ mkdir output/zlib # mkdir output first

$ wget https://www.zlib.net/fossils/zlib-1.2.8.tar.gz
$ tar xzf zlib-1.2.8.tar.gz
$ cd zlib-1.2.8/

$ export CC=arm-none-linux-gnueabi-gcc
$ ./configure --prefix=../output/zlib
$ make && make install
```

（此步骤非部署 openssh 的必要操作）因为 zlib 是很多软件的依赖，所以把它放到编译器的默认文件夹也是一个省事的做法。

```shell
$ tree output/zlib/
output/zlib/
├── include
│   ├── zconf.h
│   └── zlib.h
├── lib
│   ├── libz.a
│   ├── libz.so -> libz.so.1.2.8
│   ├── libz.so.1 -> libz.so.1.2.8
│   ├── libz.so.1.2.8
│   └── pkgconfig
│       └── zlib.pc
└── share
    └── man
        └── man3
            └── zlib.3

$ cp output/zlib/include/* /home/easy/Code/4412_test/arm-2009q3/arm-none-linux-gnueabi/libc/usr/include/
$ cp output/zlib/lib/libz* /home/easy/Code/4412_test/arm-2009q3/arm-none-linux-gnueabi/libc/usr/lib/
```

<br/>

## openssl

```shell
$ mkdir output/openssl # mkdir output first

$ wget https://www.openssl.org/source/old/1.1.0/openssl-1.1.0l.tar.gz
$ tar xzf openssl-1.1.0l.tar.gz
$ cd openssl-1.1.0l/

$ export CC= # clear zlib CC set, use --cross-compile-prefix
$ ./Configure linux-armv4 no-async  shared --cross-compile-prefix=arm-none-linux-gnueabi- --prefix=$PWD/../output/openssl/
$ make && make install
```

- \-\-cross-compile-prefix：编译器前缀。如果此时环境变量中存在正确的 CC 配置（在编译 zlib 时配置的），则不要再设置此项，会产生叠加而导致此项设置有误，或者如使用上述命令先清除 CC 配置。
- \-\-prefix：安装地址，需要使用绝对路径。

<br/>

## openssh

依赖于 zlib、openssl。

编译：

```shell
$ mkdir output/openssh # mkdir output first

$ wget https://cdn.openbsd.org/pub/OpenBSD/OpenSSH/portable/openssh-9.3p1.tar.gz
$ tar xzf openssh-9.3p1.tar.gz
$ cd openssh-9.3p1/

# 进行 strip 处理
$ ./configure CC=arm-none-linux-gnueabi-gcc --host=arm-linux-gnueabi --prefix="" --with-zlib=$PWD/../output/zlib --with-ssl-dir=$PWD/../output/openssl --disable-strip

$ make -j 4
$ make install-nokeys DESTDIR=$PWD/../output/openssh STRIP_OPT="-s --strip-program=arm-none-linux-gnueabi-strip"
```

- CC：编译器。
- \-\-host：目标板的设备属性。
- \-\-prefix：目标安装地址。scp 传输文件时，默认使用 `$(prefix)/bin/ssh` 来传输文件，因为使用的是 busybox，所以此处置空。不指定时，默认为 `/usr/local`。
- \-\-with-zlib：zlib 的安装地址。
- \-\-with-ssl-dir：ssl 的安装地址，同 openssl 的安装地址。
- DESTDIR：install 导出文件的位置。
- STRIP_OPT：`strip` 程序用于从二进制文件（如可执行文件、库文件等）中移除不必要的信息。这些信息通常包括符号、重定位表和其他调试信息。处理后的程序可以显著减少二进制文件的大小、略微提高程序的启动速度、增大逆向难度。总结：在软件调试过程中，保留信息有利于定位问题；在嵌入式程序发布中，**建议进行 strip 处理**。如不要进行 strip 处理，不填此项即可

<br/>

## 自动编译脚本

和压缩包放置在同一目录下：

```
.
├── build_ssh.sh
├── Makefile
├── openssh-9.3p1.tar.gz
├── openssl-1.1.0l.tar.gz
└── zlib-1.2.8.tar.g
```

build_ssh.sh：

```bash
#!/bin/sh

_ARCH=arm
_HOST=arm-linux-gnueabihf
_CROSS_COMPILE=arm-linux-gnueabihf-
_CC=${_CROSS_COMPILE}gcc
_STRIP=${_CROSS_COMPILE}strip
_OUTDIR=gnueabihf_output

echo "~~~~ build ssh confure ~~~~"
echo "ARCH          = $_ARCH"
echo "HOST          = $_HOST"
echo "CROSS_COMPILE = $_CROSS_COMPILE"
echo "CC            = $_CC"
echo "STRIP         = $_STRIP"
echo "OUTDIR        = $_OUTDIR"
echo "~~~~ build ssh confure ~~~~\r\n"


if test ! -d "$_OUTDIR"; then
    mkdir $_OUTDIR
    echo "mkdir $_OUTDIR"
fi


# zlib
if test ! -e zlib-1.2.8.tar.gz; then
    echo "zlib-1.2.8.tar.gz not found."
    exit 1
fi

if test ! -d "zlib-1.2.8"; then
    tar xzf zlib-1.2.8.tar.gz
    echo "Decompression zlib-1.2.8.tar.gz"
fi

cd zlib-1.2.8/
export CC=$_CC

if test ! -d "../$_OUTDIR/zlib"; then
    mkdir ../$_OUTDIR/zlib
    echo "mkdir ../$_OUTDIR/zlib"
fi

if test -e "libz.so"; then
    echo "Skip zlib compilation."
else
    ./configure --prefix=../$_OUTDIR/zlib
    make -j4
fi
if test -e "../$_OUTDIR/zlib/lib/libz.so"; then
    echo "Skip zlib install."
else
    make install
fi
cd ..
echo "zlib done."


# openssl
if test ! -e openssl-1.1.0l.tar.gz; then
    echo "openssl-1.1.0l.tar.gz not found."
    exit 1
fi

if test ! -d "openssl-1.1.0l"; then
    tar xzf openssl-1.1.0l.tar.gz
    echo "Decompression openssl-1.1.0l.tar.gz"
fi

cd openssl-1.1.0l

if test ! -d "../$_OUTDIR/openssl"; then
    mkdir ../$_OUTDIR/openssl
    echo "mkdir ../$_OUTDIR/openssl"
fi

export CC=

if test -e "libssl.so"; then
    echo "Skip openssl compilation."
else
    ./Configure linux-armv4 no-async  shared --cross-compile-prefix=${_CROSS_COMPILE} --prefix=$PWD/../$_OUTDIR/openssl/
    make -j4
fi
if test -e "../$_OUTDIR/openssl/lib/libssl.so"; then
    echo "Skip openssl install."
else
    make install
fi
cd ..
echo "openssl done."


# openssh
if test ! -e openssh-9.3p1.tar.gz; then
    echo "openssh-9.3p1.tar.gz not found."
    exit 1
fi

if test ! -d "openssh-9.3p1"; then
    tar xzf openssh-9.3p1.tar.gz
    echo "Decompression openssh-9.3p1.tar.gz"
fi

cd openssh-9.3p1

if test ! -d "../$_OUTDIR/openssh"; then
    mkdir ../$_OUTDIR/openssh
    echo "mkdir ../$_OUTDIR/openssh"
fi

if test -e "scp"; then
    echo "Skip openssh compilation."
else
    ./configure CC=${_CC} --host=$_HOST --prefix="" --with-zlib=$PWD/../$_OUTDIR/zlib --with-ssl-dir=$PWD/../$_OUTDIR/openssl --disable-strip
    make -j4
fi
if test -e "../$_OUTDIR/openssh/bin/scp"; then
    echo "Skip openssh install."
else
    make install-nokeys DESTDIR=$PWD/../$_OUTDIR/openssh STRIP_OPT="-s --strip-program=$_STRIP"
fi
cd ..
echo "openssh done."
```

自动清理脚本：

```makefile
# Makefile
idir?=gnueabihf_output
odir?=../rootfs_patch
mode?=rn	# -n:do not overwrite an existing file -u:copy only when the SOURCE file is newer

all:usage

usage:
	@echo "make deploy <mode=rn/ru> <idir=gnueabihf_output> <odir=../rootfs_patch>"
	@echo "make clean"

deploy:
	cp -$(mode) $(idir)/zlib/lib/libz.* $(odir)/lib/
	cp -$(mode) $(idir)/openssl/bin/* $(odir)/bin/
	cp -$(mode) $(idir)/openssl/lib/* $(odir)/lib/
	cp -$(mode) $(idir)/openssl/ssl $(odir)/etc/

	-mkdir $(odir)/sbin $(odir)/bin $(odir)/.ssh
	cp -$(mode) $(idir)/openssh/bin/* $(odir)/bin/
	cp -$(mode) $(idir)/openssh/etc/* $(odir)/etc/
	cp -$(mode) $(idir)/openssh/libexec $(odir)/
	cp -$(mode) $(idir)/openssh/sbin/* $(odir)/sbin/

	rm -rf $(odir)/lib/pkgconfig
	rm $(odir)/lib/*.a

clean:
	rm -rf zlib-1.2.8 openssl-1.1.0l openssh-9.3p1
	rm -rf $(idir)
```

<br/>

## 部署

### 部署 scp

```shell
cp -ru output/zlib/lib/libz.* ../rootfs_patch/lib/

cp -ru output/openssl/bin/* ../rootfs_patch/bin/
cp -ru output/openssl/lib/* ../rootfs_patch/lib/
cp -ru output/openssl/ssl ../rootfs_patch/etc/

mkdir ../rootfs_patch/sbin ../rootfs_patch/bin ../rootfs_patch/.ssh
cp -ru output/openssh/bin/* ../rootfs_patch/bin/
cp -ru output/openssh/etc/* ../rootfs_patch/etc/
cp -ru output/openssh/libexec ../rootfs_patch/
cp -ru output/openssh/sbin/* ../rootfs_patch/sbin/

rm -rf ../rootfs_patch/lib/pkgconfig
rm ../rootfs_patch/lib/*.a
```

../rootfs_patch 相当于根文件夹，对应放入到开发板的 / 目录下，即完成部署。

```shell
[root@iTOP-4412]$ mkdir ~/.ssh
[root@iTOP-4412]$ scp easy@192.168.0.111:~/test .
[10009.789940] [dm96-TxRound].4 for LEN_ODD tail_room +1, rslt add 1
The authenticity of host '192.168.0.111 (192.168.0.111)' can't be established.
ED25519 key fingerprint is SHA256:xxxxx/xxxxxxxxxxxxxxxx/xxxxxxx.
This key is not known by any other names.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added '192.168.0.111' (ED25519) to the list of known hosts.
easy@192.168.0.111's password:
test                                       0%    0     0.0K
test                                     100% 1100   168.6KB/s   00:00
```

<br/>

### 部署 sshd

**编辑 /etc/passwd**，增加一行：`sshd:x:74:74:Privilege-separated SSH:/var/empty/sshd:/sbin/nologin`。否则报错：Privilege separation user sshd does not exist。

**修改 sshd_config**：

````
HostKey /etc/ssh/ssh_host_rsa_key
PasswordAuthentication yes
PermitRootLogin yes
Subsystem       sftp    /libexec/sftp-server
````

否则无法使用 root 账户登录。

**生成 HostKey**：

```shell
[root@iTOP-4412]$ ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key -N ""
```

否则会提示：sshd: no hostkeys available -- exiting

**设置 root 密码**：

```shell
[root@iTOP-4412]$ passwd root
# 输入 2 遍密码
```

**启动 sshd**：

```shell
[root@iTOP-4412]$ /sbin/sshd
# 需要使用绝对路径启动，否则报错：sshd re-exec requires execution with an absolute path
```

**登录测试**：

使用另外一台可以使用 ssh 的电脑，或者使用 XShell 等 ssh 工具连接。

```shell
$ ssh root@192.168.0.200
# 输入密码
```

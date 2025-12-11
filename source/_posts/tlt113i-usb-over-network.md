---
title: TLT113i USB over Network
toc: true
categories:
  - tech
tags:
  - 嵌入式
date: 2025-11-06 22:31:00
---

[上一篇](/tech/tlt113i-usb-wifi-rtl8811cu/)解决了无线连接的问题，图穷匕首现，用这个板子做 USB over Network 的主控，解决我远程使用 3D 打印机、HP 打印机的问题。经测试，USB Camera、U盘也可以使用。

USB/IP 是一种基于网络的设备共享机制，可将电脑A（Server端）连接的USB设备通过网络共享给远程电脑B（Client端）。

试验环境：

* Server端：软件：linux-kernel-tools-usbip；运行环境：[T113-i-MiniEVM | Tronlong](https://www.tronlong.com/Product/show/248.html)；编译环境：x86_64 Linux Ubuntu。
* Client端：软件：[vadimgrn/usbip-win2: USB/IP Client for Windows](https://github.com/vadimgrn/usbip-win2)；运行环境：Win11。

<!--more-->

<br/>

## Server 端

### 修改 Kernel 支持

```bash
$ ./build.sh menuconfig
Device Drivers  --->
	[*] USB support  --->
        <*>   USB/IP support
        <*>     VHCI hcd
        (8)       Number of ports per USB/IP virtual host controller (NEW)
        (1)       Number of USB/IP virtual host controllers (NEW)
        <*>     Host driver
        <*>     VUDC driver
```

重新编译 Kernel 并烧写。

如果使用 M 选项编译，则需在使用时手动加载：`modprobe -a usbip-core usbip-host vhci_hcd`。

### 从源码编译 usbip tool

如果是 Ubuntu 系统或者树莓派，可以从 apt 直接获取。但我手上的是 busybox 系统，需要从 kernel 中编译。

```bash
$ cd kernel/linux-5.4/tools/usb/usbip
$ ./autogen.sh
$ mkdir tools-usbip
$ ./configure --prefix=/home/easy/Code/TLT113-MiniEVM/tina5.0_v1.0/kernel/linux-5.4/tools/usb/usbip/tools-usbip/ --host=arm-linux-gnueabi CC=arm-linux-gnueabi-gcc CFLAGS=-I'/home/easy/Code/TLT113-MiniEVM/tina5.0_v1.0/out/t113_i/tlt113-minievm-nand/buildroot/buildroot/host/arm-buildroot-linux-gnueabi/sysroot/usr/include/' LDFLAGS=-L'/home/easy/Code/TLT113-MiniEVM/tina5.0_v1.0/out/t113_i/tlt113-minievm-nand/buildroot/buildroot/target/usr/lib/'
$ make
$ make install
$ tree tools-usbip/
tools-usbip/
├── include
│   └── usbip
│       ├── list.h
│       ├── sysfs_utils.h
│       ├── usbip_common.h
│       ├── usbip_host_common.h
│       ├── usbip_host_driver.h
│       └── vhci_driver.h
├── lib
│   ├── libusbip.a
│   ├── libusbip.la
│   ├── libusbip.so -> libusbip.so.0.0.1
│   ├── libusbip.so.0 -> libusbip.so.0.0.1
│   └── libusbip.so.0.0.1
├── sbin
│   ├── usbip
│   └── usbipd
└── share
    └── man
        └── man8
            ├── usbip.8
            └── usbipd.8
```

* 将 lib、sbin 文件夹下的文件放到开发板对应目录。

#### 配置 usb.ids

usb.ids 是 USB 设备数据库，用于在执行 usbip 命令时显示设备相关信息。

```bash
$ mkdir -p /usr/share/hwdata
$ wget http://www.linux-usb.org/usb.ids -O /usr/share/hwdata/usb.ids
```

#### 使用命令

Server 端可使用的命令：

* 列出当前系统识别的所有 USB 设备：`lsusb`。

* 列出当前系统所有可导出 USB 设备：`usbip list -l`，得到 busid。示例：

  ```bash
  $ usbip list -l
   - busid 1-1.1 (0bda:c811)
     Realtek Semiconductor Corp. : unknown product (0bda:c811)
  
   - busid 1-1.2 (1a86:7523)
     QinHeng Electronics : CH340 serial converter (1a86:7523)
  ```

* 绑定 USB/IP：`usbip bind -b <busid>`，被绑定的设备即可被 usbip 客户端导入。

* 解绑 USB/IP：`usbip unbind -b <busid>`。

* 启动 usbipd 守护进程：`usbipd -D`。


#### 自启动脚本

```bash
$ vim /etc/init.d/S45usbipd
$ chmod +x /etc/init.d/S45usbipd
```

`S45usbipd`：

```shell
usbipd -D
busid=$(usbip list -l | grep busid | grep -v 0bda | awk '{print $3}')
while IFS= read -r line; do
	usbip bind -b $line
	echo "usbip bind -b $line"
done <<< "$busid"
```

* 启动 usbipd 为守护进程。
* 将除了 WiFi 发射器（0bda）的其它所有 usb 设备绑定。

## Client 端（Win11）

软件：[vadimgrn/usbip-win2: USB/IP Client for Windows](https://github.com/vadimgrn/usbip-win2)

![usbip-win.png](/resources/usbip/usbip-win.png)

* 在 Server 栏填写 IP 和 Port（默认 3240），点击 `Add devices` 就会显示可用的设备。
* 右键单击具体的设备，点击 `Attach`，就可以在 设备管理器 中查看到对应的设备被加载了。

## Client 端（Linux）


* 查询 Server 端的设备：`usbip list -r <usbip server ip>`。

  ```
  Exportable USB devices
  ======================
   - 192.168.1.9
          3-2: unknown vendor : unknown product (1005:b113)
             : /sys/devices/pci0000:00/0000:00:14.0/usb3/3-2
             : (Defined at Interface level) (00/00/00)
  ```

* 使用 busid 连接 Server 端的设备：`usbip attach -r <usbip server ip> -b 3-2`。

  ```
  successfully attached to port 1
  ```

* 解除连接：`usbip detach -p 1`。

  ```
  port 1 is successfully detached
  ```


## 参考

* [局域网中USB远程共享：USB/IP_usbip-CSDN博客](https://blog.csdn.net/OTZ_2333/article/details/124073337)

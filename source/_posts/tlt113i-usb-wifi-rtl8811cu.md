---
title: TLT113i USB WiFi(RLT8811CU) 移植
toc: true
categories:
  - tech
tags:
  - 嵌入式
date: 2025-10-29 20:45:00
---

绿联的USB无限网卡，在 Windows 上用是比较简单的：插上网卡，会作为存储盘弹出，其中包含了驱动软件安装包，驱动软件安装后，系统会将网卡自动设备为WiFi设备，就直接可以使用系统软件连接 WiFi 了。

想在 TLT113 这个 ARM 板上使用这张网卡，就比较麻烦了。需要在 kernel、rootfs、驱动 这3个层面进行适配，使用也需要使用命令行工具。

试验环境：

* 工业评估板：[T113-i-MiniEVM | Tronlong](https://www.tronlong.com/Product/show/248.html)
  * 软件：[全志 Linux Tina-SDK](https://tina.100ask.net/)
* [绿联 CM448 USB无线网卡](https://www.lulian.cn/product/848.html)
  * 软件：[绿联USB2.0无线网卡RTL8811CU芯片驱动（Windows+Linux系统）](https://www.lulian.cn/download/60.html)

<!--more-->

<br/>

## kernel 修改

```bash
# @ Tina 根目录
$ ./build.sh menuconfig
Device Drivers  --->
	[*] Network device support  --->
		<*>   USB Network Adapters  --->
			<*>   Multi-purpose USB Networking Framework
			<*>     ASIX AX88xxx Based USB 2.0 Ethernet Adapters
			<*>     ASIX AX88179/178A USB 3.0/2.0 to Gigabit Ethernet 
			-*-     CDC Ethernet support (smart devices such as cable modems)
			<*>   Simple USB Network Links (CDC Ethernet subset)
		[*]   Wireless LAN  --->
			<*>     IEEE 802.11 for Host AP (Prism2/2.5/3 and WEP/TKIP/CCMP)
			[*]   Realtek devices
			<*>     Realtek rtlwifi family of devices  --->
			<*>     RTL8723AU/RTL8188[CR]U/RTL819[12]CU (mac80211) support
			<*>   Realtek RTL8192CU/RTL8188CU USB Wireless Network Adapter
			<*>     Realtek 802.11ac wireless chips support  ----
	[*] USB support  --->
		<*>   USB Wireless Device Management support

[*] Networking support  --->
	-*-   Wireless  --->
		<*>   cfg80211 - wireless configuration API
		[*]     nl80211 testmode command
		[*]     enable powersave by default
		[*]     support CRDA
		[*]     cfg80211 wireless extensions compatibility
		<*>   Generic IEEE 802.11 Networking Stack (mac80211)
		[*]   Minstrel
```

<br/>

## buildroot_fs 定制

```bash
# @ Tina 根目录
$ ./build.sh buildroot_menuconfig
Target packages  --->
	Hardware handling  --->
        -*- usb_modeswitch
        [*] usb_modeswitch_data
        [*] usbutils
	Networking applications  --->
		[*] hostapd
		[*]   Enable hostap driver
		[*]   Enable nl80211 driver
		[*]   Enable ACS
		[*]   Enable EAP
		[*]   Enable WPS
		[*]   Enable VLAN support
		[*]     Enable dynamic VLAN support
		[*]     Use netlink-based API for VLAN operations 
		[*] wpa_supplicant
		[*]   Enable nl80211 support
		[*]   Enable AP mode
		[*]     Enable Wi-Fi Display
		[*]     Enable mesh networking
		[*]   Enable autoscan
		[*]   Enable EAP
		[*]   Enable HS20
		[*]   Enable syslog support
		[*]   Enable WPS
		[*]   Install wpa_cli binary
		[*]   Install wpa_passphrase binary
	allwinner platform private package select  --->
		wireless  --->
            [*] wifimanager-v2.0
            [*]   wifimanager-v2.0-lib
            [*]     Tina wifimanager-v2.0-demo
            -*- wireless_common
            
$ ./build.sh
$ ./build.sh pack
```

获得 t113_i_linux_tlt113-minievm-nand_uart0.img，下载到开发板。

<br/>

## 驱动编译

从 [绿联USB2.0无线网卡RTL8811CU芯片驱动（Windows+Linux系统）](https://www.lulian.cn/download/60.html) 页面下载驱动软件：https://download.lulian.cn/2025-drive/CM448-20204_CM496-90334_Drive_V1.1.zip

将 `RTL8811CU_ARM` 解压出来。

### 修改 Makefile

```diff
$ diff RTL8811CU_ORG/Makefile RTL8811CU_ARM/Makefile 
124c124
< CONFIG_PLATFORM_I386_PC = y
---
> CONFIG_PLATFORM_I386_PC = n
160c160
< CONFIG_PLATFORM_ARM_SUNxI = n
---
> CONFIG_PLATFORM_ARM_SUNxI = y
1775c1775
< _PLATFORM_FILES += platform/platform_ARM_SUNxI_usb.o
---
> # _PLATFORM_FILES += platform/platform_ARM_SUNxI_usb.o
1784,1788c1784,1788
< CROSS_COMPILE=/home/android_sdk/Allwinner/a10/android-jb42/lichee-jb42/buildroot/output/external-toolchain/bin/arm-none-linux-gnueabi-
< KVER  := 3.0.8
< KSRC=/home/android_sdk/Allwinner/a10/android-jb42/lichee-jb42/linux-3.0
---
> CROSS_COMPILE := arm-linux-gnueabi-
> KVER  := 5.4.61
> KSRC:= /home/easy/Code/TLT113-MiniEVM/tina5.0_v1.0/out/kernel/build
```

### 修复报错

```diff
$ diff RTL8811CU_ORG/os_dep/linux/usb_intf.c RTL8811CU_ARM/os_dep/linux/usb_intf.c
1668c1668
< 	ret = platform_wifi_power_on();
---
> 	ret = 0;//platform_wifi_power_on();
1708c1708
< 	platform_wifi_power_off();
---
> 	//platform_wifi_power_off();
$ diff RTL8811CU_ORG/os_dep/linux/os_intfs.c RTL8811CU_ARM/os_dep/linux/os_intfs.c
22a23
> MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);
```

### 编译

```bash
$ make
```

获得 8821cu.ko，下载到开发板。

<br/>

## 配置+测试

设置 WiFi 密码，`vim /etc/wpa_supplicant.conf`：

```ini
ctrl_interface=/var/run/wpa_supplicant
update_config=1

network={
  ssid="Net Name"
  key_mgmt=WPA-PSK
  psk="password"
}
```

安装模块：

```bash
$ insmod 8821cu.ko
```

切换USB网卡从存储模式到 WLAN 模式：

```bash
$ lsusb -v
Bus 001 Device 002: ID 0bda:1a2b
idVendor           0x0bda
idProduct          0x1a2b
$ usb_modeswitch -KQ -v 0bda -p 1a2b
```

启动 wpa：

```bash
$ ifconfig -a
# 多了 wlan0 和 wlan1，经测试，wlan0 是 5G，wlan1 是 2.4G。
$ wpa_supplicant -B -i wlan1 -c /etc/wpa_supplicant.conf

# 查看连接状态
$ wpa_cli -i wlan1 status
# wpa_state=COMPLETED 即为连接成功
```

设置静态IP：

```bash
$ ifconfig wlan1 192.168.0.200 netmask 255.255.255.0 broadcast 192.168.0.255 up
$ route add default gw 192.168.0.1 wlan1
$ ping 192.168.0.1 -c 4
PING 192.168.0.1 (192.168.0.1): 56 data bytes
64 bytes from 192.168.0.1: seq=0 ttl=64 time=2.891 ms
64 bytes from 192.168.0.1: seq=1 ttl=64 time=1.975 ms
64 bytes from 192.168.0.1: seq=2 ttl=64 time=4.090 ms
64 bytes from 192.168.0.1: seq=3 ttl=64 time=1.098 ms

--- 192.168.0.1 ping statistics ---
4 packets transmitted, 4 packets received, 0% packet loss
```

### 开机自动配置脚本

```bash
$ vim /etc/init.d/S41rtl8811cu

insmod /lib/modules/5.4.61-rt37/8821cu.ko
output=$(lsusb | grep 0bda:1a2b 2>/dev/null)
if [[ -n "$output" ]]; then
	usb_modeswitch -KQ -v 0bda -p 1a2b
fi
# Waiting for wlan1, Check 3 times.
MAX_ATTEMPTS=3
attempt=1
while true; do
	if ip link show | grep -q "wlan1"; then
		echo "Finded wlan1 in $attempt times."
		break
	else
		if [ $attempt -gt $MAX_ATTEMPTS ]; then
			echo "Not find wlan1, exit."
			exit 0
		fi
		sleep 1
		attempt=$((attempt + 1))
	fi
done
wpa_supplicant -B -i wlan1 -c /etc/wpa_supplicant.conf
ifconfig wlan1 192.168.0.200 netmask 255.255.255.0 broadcast 192.168.0.255 up
route add default gw 192.168.0.1 wlan1

$ chmod +x S41rtl8811cu
```

* 修改自己对应的 0bda:1a2b、网络参数。

<br/>

## 参考

* [Tina中移植RTL8188FTV | 全志在线开发者论坛](https://bbs.aw-ol.com/topic/5724/tina中移植rtl8188ftv)
* [usb_modeswitch — Cloud Atlas: Discovery beta 文档](https://cloud-atlas.readthedocs.io/zh-cn/latest/linux/gentoo_linux/usb_modeswitch.html)

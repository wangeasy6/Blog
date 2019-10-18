---
title: 树莓派
date: 2018-07-28 00:08:00
toc: true
categories:
  - tools
tags:
  - 待续
---

### 主题
* [ 开机更新 ](/2018/07/28/raspberryPi/#开机更新)
* [ 常用软件 ](/2018/07/28/raspberryPi/#常用软件)
* [ 更换源地址到国内镜像 ](/2018/07/28/raspberryPi/#更换源地址到国内镜像)
* [ 更换pip源到国内镜像 ](/2018/07/28/raspberryPi/#更换pip源到国内镜像)
* [ 开启Root权限 ](/2018/07/28/raspberryPi/#开启Root权限)
* [ 命令行配置无线网络连接 ](/2018/07/28/raspberryPi/#命令行配置无线网络连接)
* [ 配置网口为固定IP ](/2018/07/28/raspberryPi/#配置网口为固定IP)
* [ 树莓派3B使用UART ](/2018/07/28/raspberryPi/#树莓派3B使用UART)
* [ 树莓派开启SSH ](/2018/07/28/raspberryPi/#树莓派开启SSH)
* [ 树莓派开启Samba ](/2018/07/28/raspberryPi/#树莓派开启Samba)
* [ 树莓派横竖屏转换 ](/2018/07/28/raspberryPi/#树莓派横竖屏转换)
* [中文输入](/2018/07/28/raspberryPi/#树莓派中文输入)
* [软键盘](/2018/07/28/raspberryPi/#树莓派软键盘)
* [ rar解压缩 ](/2018/07/28/raspberryPi/#rar解压缩)
* [Windows 下远程开发 QT 环境搭建]()

<!--more-->
<br/>

### 开机更新
`sudo rpi-update`         // 固件更新
`sudo apt-get update`     // 软件源更新
`sudo apt-get upgrade`    // 软件更新

<br/>
### 常用软件
`sudo apt-get install vim`

<br/>

### 更换源地址到国内镜像

1.lsb_release -a 查看自己的版本类型

2.`sudo vim /etc/apt/sources.list`

   （1）将文件里的默认的官方软件源用# 注释掉
   （2）添加下面的软件源（中国科技大学的软件源 ） （手动添加注意空格）

- stretch版本
  deb http://mirrors.ustc.edu.cn/raspbian/raspbian/ stretch main contrib non-free rpi

- jessis版本
  deb http://mirrors.ustc.edu.cn/raspbian/raspbian/ jessis main contrib non-free rpi

<br/>

### 更换pip源到国内镜像
**pip国内的一些镜像：**

  阿里云 http://mirrors.aliyun.com/pypi/simple/
  中国科技大学 https://pypi.mirrors.ustc.edu.cn/simple/
  豆瓣(douban) http://pypi.douban.com/simple/
  清华大学 https://pypi.tuna.tsinghua.edu.cn/simple/
  中国科学技术大学 http://pypi.mirrors.ustc.edu.cn/simple/

**临时使用：**

  使用 -i 指定源：
  例： sudo pip3 install cython -i  https://pypi.mirrors.ustc.edu.cn/simple/

**永久修改：**

  linux:
  修改 ~/.pip/pip.conf (没有就创建一个)， 内容如下：
  ```
  [global]
  index-url = https://pypi.tuna.tsinghua.edu.cn/simple
  ```

  windows:
  直接在user目录中创建一个pip目录，如：C:\Users\xx\pip，新建文件pip.ini，内容如下
  ```
  [global]
  index-url = https://pypi.tuna.tsinghua.edu.cn/simple
  ```

<br/>

### 开启Root权限

1.`sudo passwd root`
  输入两遍密码

2.`sudo vim /etc/ssh/sshd_config`
  修改 PermitRootLogin without-password 为 PermitRootLogin yes

3.`sudo reboot`

<br/>

### 命令行配置无线网络连接

使用如下命令可以扫描附件的无线网：

`sudo iwlist wlan0 scan`

ESSID 是无线网名称，IEEE 是协议版本和加密方式。



添加有密码的WiFi网络：

`sudo vim /etc/wpa_supplicant/wpa_supplicant.conf`

在文件末尾出添加一下代码，并替换掉ssid_name以及password即可：

```
network={
        ssid="ssid_name"
        psk="password"
        key_mgmt=WPA-PSK
}
```

如果要连接的是隐藏的网络，需要在配置文件中添加`scan_ssid`连接。需要添加的内容如下：

```
network={
    ssid="Hidden_ssid"
    psk="password"
    scan_ssid=1
}
```

如果您有两个网络的范围，可以添加优先级选项来在它们之间进行选择。具有最高优先级的范围内的网络将是连接的网络：

```bash
network={
    ssid="ssid_name1"
    psk="password1"
    priority=1
    id_str="homeOne"
}

network={
    ssid="ssid_name2"
    psk="password2"
    priority=2
    id_str="homeTwo"
}
```

<br/>

### 配置网口为固定IP

`vi /etc/dhcpcd.conf`

修改以下内容：

```
#网线
interface eth0
static ip_address=192.168.1.15/24
static routers=192.168.1.1
static domian_name_servers=114.114.114.114
static domian_name_servers=114.114.115.115
#无线
interface wlan0
static ip_address=192.168.2.15/24
static routers=192.168.2.1
static domian_name_servers=114.114.114.114
static domian_name_servers=114.114.115.115
```

修改完成后，按esc键后输入 :wq 保存。

重启网络：`sudo service network restart`

或重启树莓派就生效了：`sudo reboot`

#### Check一下：

使用 `route -n` 检查网关是否设置成功。

如没有，使用 `route add default gw 192.168.1.1` 添加网关。

* PS：电脑直连树莓派出现 Windows能ping通树莓派，但树莓派无法ping通Windows 的情况

  将防火墙关闭即可。

<br/>

### 树莓派3B使用UART

参考：https://blog.csdn.net/asukasmallriver/article/details/76851375

1.前言

树莓派CPU内部有两个串口，一个是硬件串口(官方称为PL011 UART)，一个是迷你串口(官方成为mini-uart)。

在树莓派2B/B+这些老版树莓派上，官方设计时都是将“硬件串口”分配给GPIO中的UART(GPIO14&GPIO15)，因此可以独立调整串口的速率和模式。

而树莓派3的设计上，官方在设计时将硬件串口分配给了新增的蓝牙模块上，而将一个没有时钟源，必须由内核提供时钟参考源的“迷你串口”分配给了GPIO的串口，这样以来由于内核的频率本身是变化的，就会导致“迷你串口”的速率不稳定，这样就出现了无法正常使用的情况。

目前解决方法就是，关闭蓝牙对硬件串口的使用，将硬件串口重新恢复给GPIO的串口使用，也就意味着树莓派3的板载蓝牙和串口，只能二选一使用。

树莓派GPIO中串口的管脚在 p8、p10、GND： 

![Pin.png](/images/raspberryPi/Pin.png)

2.配置设备树

下载 [pi3-miniuart-bt-overlay.dtb](/images/raspberryPi/pi3-miniuart-bt-overlay.dtb) 文件，并将文件拷贝到 /boot/overlays/ 目录下。

编辑 /boot/config.txt：`sudo vim /boot/config.txt`

添加：

```bash
dtoverlay=pi3-miniuart-bt-overlay
```

3.关闭板载蓝牙：

```shell
sudo systemctl disable hciuart
sudo vim /lib/systemd/system/hciuart.service
```

将 "ttyAMA0" 修改为 "ttyS0"

```bash
[Unit]
Description=Configure Bluetooth Modems connected by UART
ConditionPathIsDirectory=/proc/device-tree/soc/gpio@7e200000/bt_pins
Requires=dev-serial1.device
After=dev-serial1.device

[Service]
Type=forking
#ExecStart=/usr/bin/btuart
ExecStart=/usr/bin/hciattach /dev/ttyAMA0 bcm43xx 115200 noflow -

[Install]
WantedBy=multi-user.target
```

4.修改cmdline.txt

`sudo vim /boot/cmdline.txt`

删除以下部分：`console=ttyAMA0,115200 kgdboc=ttyAMA0,115200`

最终变成：

```bash
dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait
```

5.禁用串口的控制台功能

```shell
sudo systemctl stop serial-getty@ttyAMA0.service
sudo systemctl disable serial-getty@ttyAMA0.service
```

6.重启树莓派

使修改生效：`sudo reboot`

<br/>

### 树莓派开启SSH
1.SSH服务安装

`sudo apt-get install openssh-server`

2.检查树莓派SSH服务是否开启

`ps -e | grep ssh`
ssh-agent为客户端，sshd为服务器端服务，只有ssh-agent没有sshd表明SSH服务还没有开启。

3.SSH服务开启

`sudo /etc/init.d/ssh start`

4.SSH服务配置

openssh-server配置文件为“/etc/ssh/sshd_config”，可以配置SSH服务的各项参数，如端口配置，默认端口为22，可以配置为其他端口，配置后重启生效。

5.树莓派SSH服务开机自动启动

在终端命令行中启动SSH服务后，如果系统重启或关机后启动，SSH服务默认是关闭的，依然需要手动启动，为了方便可以设置SSH服务开机自动启动，打开/etc/rc.local文件，在语句exit 0之前加入：

`/etc/init.d/ssh start`

<br/>

### 树莓派横竖屏转换

编辑/boot/config.txt，根据想旋转的角度
对于触摸屏添加如下内容：

```
lcd_rotate = 0 //不旋转
lcd_rotate = 1 //旋转90度
lcd_rotate = 2 //旋转180
lcd_rotate = 3 //旋转270
```

对于HDMI显示输出，添加：

```display_rotate = 0
display_rotate = 0
display_rotate = 1
display_rotate = 2
display_rotate = 3
```

对于触摸定位：

**1.安装xinput**
`sudo apt-get install xinput`

**2.列出所有输入设备信息**
`xinput --list`

如果远程操作记得在命令前加DISPLAY=:0，`DISPLAY=:0 xinput --list`

得到以下信息：

```
NTGAGE:~ $ DISPLAY=:0 xinput --list
⎡ Virtual core pointer                          id=2    [master pointer  (3)]
⎜   ↳ Virtual core XTEST pointer                id=4    [slave  pointer  (2)]
⎜   ↳ Logitech USB Optical Mouse                id=6    [slave  pointer  (2)]
⎜   ↳ WaveShare WaveShare Touchscreen           id=7    [slave  pointer  (2)]
⎣ Virtual core keyboard                         id=3    [master keyboard (2)]
    ↳ Virtual core XTEST keyboard               id=5    [slave  keyboard (3)]
    ↳ Logitech USB Keyboard                     id=8    [slave  keyboard (3)]
    ↳ Logitech USB Keyboard                     id=9    [slave  keyboard (3)]
```

**3.列出目标设备属性**

`DISPLAY=:0 xinput --list-props 7`

得到以下信息：

```
NTGAGE:~ $ DISPLAY=:0 xinput --list-props 7
Device 'WaveShare WaveShare Touchscreen':
        Device Enabled (115):   1
        Coordinate Transformation Matrix (116): 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000
        Device Accel Profile (240):     0
        Device Accel Constant Deceleration (241):       1.000000
        Device Accel Adaptive Deceleration (242):       1.000000
        Device Accel Velocity Scaling (243):    10.000000
        Device Product ID (244):        3823, 5
        Device Node (245):      "/dev/input/event3"
        Evdev Axis Inversion (246):     0, 0
        Evdev Axis Calibration (247):   <no items>
        Evdev Axes Swap (248):  0
        Axis Labels (249):      "Abs MT Position X" (267), "Abs MT Position Y" (268), "Abs MT Pressure" (269), "None" (0), "None" (0), "None" (0)
        Button Labels (250):    "Button Unknown" (233), "Button Unknown" (233), "Button Unknown" (233), "Button Wheel Up" (121), "Button Wheel Down" (122)
        Evdev Scrolling Distance (251): 0, 0, 0
        Evdev Middle Button Emulation (252):    0
        Evdev Middle Button Timeout (253):      50
        Evdev Third Button Emulation (254):     0
        Evdev Third Button Emulation Timeout (255):     1000
        Evdev Third Button Emulation Button (256):      3
        Evdev Third Button Emulation Threshold (257):   20
        Evdev Wheel Emulation (258):    0
        Evdev Wheel Emulation Axes (259):       0, 0, 4, 5
        Evdev Wheel Emulation Inertia (260):    10
        Evdev Wheel Emulation Timeout (261):    200
        Evdev Wheel Emulation Button (262):     4
        Evdev Drag Lock Buttons (263):  0
```

如果执行到这一步，发现并没有以上的Evdev等属性项，请跳转到步骤4。

如果如上所示，则执行修改：

如屏幕显示为翻转90度。/boot/config.txt设置为display_rotate=1
上述信息中Evdev Axis Inversion 项是每条轴的旋转设置项，后面第一个参数是x,第二个参数是y.
Evdev Axes Swap项对应的是两条轴的交换。0为不翻转，1为翻转 
例如，x轴原本是朝向右的，当把Evdev Axis Inversion的第一个参数设置为1，即x轴朝向左。

现在目的是要触摸旋转90度，从坐标轴理解：即目的x轴正向为初始y轴的反向。目的y轴的正方向为初始x轴的正向。
1）所以先交换x、y轴
`DISPLAY=:0 xinput --set-prop '7' 'Evdev Axes Swap' 1`
2）然后反转y轴
`DISPLAY=:0 xinput --set-prop '7' 'Evdev Axis Inversion' 0 1`
这样即可完成触摸旋转90度。若要旋转其他角度，推理一下即可。
显示旋转修改之后需要重启。而触摸旋转不需要重启。

**4.无Evdev操作**

如果查询到的信息如下：

```
DISPLAY=:0 xinput --list-props 6
Device 'WaveShare WaveShare Touchscreen':
        Device Enabled (114):   1
        Coordinate Transformation Matrix (115): 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000
        libinput Calibration Matrix (246):      0.000000, 1.000000, 0.000000, -1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000
        libinput Calibration Matrix Default (247):      1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000
        libinput Send Events Modes Available (248):     1, 0
        libinput Send Events Mode Enabled (249):        0, 0
        libinput Send Events Mode Enabled Default (250):        0, 0
        Device Node (251):      "/dev/input/event0"
        Device Product ID (252):        3823, 5
```

可以看到该驱动方式采用的是libinput。

查看/usr/share/X11/xorg.conf.d/目录下是否有40-libinput.conf这个文件。
无 则需要安装，`sudo apt-get install xserver-xorg-input-libinput`
复制该文件到/etc/X11/xorg.conf.d/目录下。一开始xorg.conf.d这个目录在/etc/X11可能没有，需要自己创建。
修改40-libinput.conf 文件，`sudo vim /etc/X11/xorg.conf.d/40-libinput.conf`

```
Section "InputClass"
        Identifier "libinput touchscreen catchall"
        MatchIsTouchscreen "on"
        MatchDevicePath "/dev/input/event*"
        Driver "libinput"
EndSection
添加一行  Option "CalibrationMatrix" "0 1 0 -1 0 1 0 0 1
结果为
Section "InputClass"
        Identifier "libinput touchscreen catchall"
        Option "CalibrationMatrix" "0 1 0 -1 0 1 0 0 1
        MatchIsTouchscreen "on"
        MatchDevicePath "/dev/input/event*"
        Driver "libinput"
EndSection
```

重启生效。

上述修改以90度为例，如果需要修改为其他角度，参考libinput的算法，对应的值是：

```
90 度旋转： Option "CalibrationMatrix" "0 1 0 -1 0 1 0 0 1"
180度旋转： Option "CalibrationMatrix" "-1 0 1 0 -1 1 0 0 1"
270度旋转： Option "CalibrationMatrix" "0 -1 1 1 0 0 0 0 1"
```

<br/>

### 树莓派中文输入

1、基于Fcitx输入法框架的google拼音输入法，在命令行中输入以下命令即可安装。

安装命令：`sudo apt-get install fcitx fcitx-googlepinyin fcitx-module-cloudpinyin fcitx-sunpinyin`



2、SCIM支持多国语言的输入法，默认支持英语键盘、智能拼音、五笔字型、自然码、二笔等。

安装命令：`sudo apt-get install scim-pinyin`



3、Ibus输入法是一个基于Python开发的全新输入法框架，支持多种语言。

安装命令：`sudo apt-get install ibus ibus-pinyin`



PS：重启生效。

<br/>

### 树莓派软键盘

安装中文包：`sudo apt-get install ttf-wqy-zenhei`

安装Matchbox-keyboard：`sudo apt-get install matchbox-keyboard`

<br/>

### 树莓派开启Samba
1.Samba软件安装

`sudo apt-get install samba`

2.Samba配置

`sudo vim /etc/samba/smb.conf`

在smb.conf文件的最后加上如下内容：
```
[share]
  path = /home/pi/
  public = yes
  writable = yes
```
3.启动Samba服务
`sudo /etc/init.d/samba start`
ps 看到samba进程smbd

4.在Window下打开
在 "我的电脑" 下，鼠标右键添加一个网络位置，在网络位置输入：
`\\ip\share`

5.添加samba共享账户
此项是可选项，设置一个用户和密码
`sudo smbpasswd -a pi`
输入password

在 /etc/samba/smb.conf 中的[share]下加入：
`  valid users = pi`

重启服务
`sudo /etc/init.d/samba restart`

<br/>
### rar解压缩

**解决方案：裝上unrar-nonfree版本**

1.刪除unrar-free (2014年的Raspbian预设沒有安裝，因此不用再刪除)
  `sudo apt-get remove unrar-free`

2.修改apt的source list
   `sudo vim /etc/apt/sources.list`
   deb-src http://archive.raspbian.org/raspbian wheezy main contrib non-free rpi

3.更新apt源库
  `sudo apt-get update`

4.准备unrar-nonfree的文件夹，我們要从源码构建
  `mkdir unrar-nonfree`
  `cd unrar-nonfree`

5.安裝相依套件
  `sudo apt-get build-dep unrar-nonfree`

6.下载 unrar-nonfree 的源代码并编译成 deb
  `sudo apt-get source -b unrar-nonfree`

7.安裝 unrar-nonfree 的deb套件
  `sudo dpkg -i unrar*.deb`

8.unrar 解压命令
  `unrar x xxx.part1.rar`
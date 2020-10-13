---
title: Raspberry Pi 手记
date: 2020-02-11 10:08:00
toc: true
categories:
  - Technology
tags:
  - 树莓派
---

### 主题
* [ 开机更新 ](/Technology/RaspberryPi/#开机更新)
* [ 更换源地址到国内镜像 ](/Technology/RaspberryPi/#更换源地址到国内镜像)
* [ 更换pip源到国内镜像 ](/Technology/RaspberryPi/#更换pip源到国内镜像)
* [ 开启Root权限 ](/Technology/RaspberryPi/#开启Root权限)
* [ 命令行配置无线网络连接 ](/Technology/RaspberryPi/#命令行配置无线网络连接)
* [ 配置网口为固定IP ](/Technology/RaspberryPi/#配置网口为固定IP)
* [ 树莓派3B使用UART ](/Technology/RaspberryPi/#树莓派3B使用UART)
* [配置固定USB端口名称](/Technology/RaspberryPi/#配置固定USB端口名称)
* [ 树莓派开启SSH ](/Technology/RaspberryPi/#树莓派开启SSH)
* [开启远程桌面](/Technology/RaspberryPi/#开启远程桌面)
* [ 树莓派开启Samba ](/Technology/RaspberryPi/#树莓派开启Samba)
* [切换屏幕分辨率](/Technology/RaspberryPi/#切换屏幕分辨率)
* [ 树莓派横竖屏转换 ](/Technology/RaspberryPi/#树莓派横竖屏转换)
* [截屏](/Technology/RaspberryPi/#截屏)
* [中文输入](/Technology/RaspberryPi/#树莓派中文输入)
* [软键盘](/Technology/RaspberryPi/#树莓派软键盘)
* [ rar解压缩 ](/Technology/RaspberryPi/#rar解压缩)

硬件环境：**Raspberry Pi 3B+**

<!--more-->
<br/>

### 开机更新
`sudo rpi-update`         // 固件更新
`sudo apt-get update`     // 软件源更新
`sudo apt-get upgrade`    // 软件更新

<br/>

### GPIO

#### wiringPi包括一套gpio控制命令

* 查看GPIO图：`gpio readall`
* 读：`gpio read <pin>`
* 设置高低：`gpio mode <pin> <in/out/pwm/clock/up/down/tri`
* `gpio write <pin> <1/0>`

<br/>

### 更换源地址到国内镜像

1.lsb_release -a 查看自己的版本类型

2.选择以下任意一个国内镜像进行替换

* [清华大学](https://mirror.tuna.tsinghua.edu.cn/help/raspbian/)
* [中国科学技术大学](http://mirrors.ustc.edu.cn/help/raspbian.html)

<br/>

### 更换pip源到国内镜像
**pip国内的一些镜像：**

* 阿里云 http://mirrors.aliyun.com/pypi/simple/
* 中国科技大学 https://pypi.mirrors.ustc.edu.cn/simple/
* 清华大学 https://pypi.tuna.tsinghua.edu.cn/simple/
* 中国科学技术大学 http://pypi.mirrors.ustc.edu.cn/simple/

**临时使用：**

  使用 -i 指定源，例： sudo pip3 install cython -i  https://pypi.mirrors.ustc.edu.cn/simple/

**永久修改：**

  linux：修改 ~/.pip/pip.conf (没有就创建一个)， 内容如下：

  ```
  [global]
  index-url = https://pypi.tuna.tsinghua.edu.cn/simple
  ```

  windows：直接在user目录中创建一个pip目录，如：C:\Users\xx\pip，新建文件pip.ini，内容如下

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

![Pin.png](/images/RaspberryPi/Pin.png)

2.配置设备树

下载 [pi3-miniuart-bt-overlay.dtb](/images/RaspberryPi/pi3-miniuart-bt-overlay.dtb) 文件，并将文件拷贝到 /boot/overlays/ 目录下。

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

### 配置固定USB端口名称

树莓派的板集上只有一个UART，经常会遇到不够用的情况，那么这个时候就只能使用USB转串口线了。

正常使用USB端口接USB转串口线来发送数据时，会根据拔插的先后顺序为串口分配`ttyUSB*`，这样在程序上就有一个麻烦，需要根据当前的串口名称改动程序或者配置。

为了解决USB端口号的对应问题，我们可以固定端口号设备名。例如，右上角的USB端口插入USB转串口线时，系统会固定命名为`tty_RightUp`，使用`ls /dev`命令，原来会显示`/dev/ttyUSB*`，现在则多了一个固定名称`/dev/tty_RightUp`，在写程序调用open传参的时候就不用更改了。相当于将这个端口和这个端口默认连接的设备做了一一对应，再也不用担心端口号变动了。

#### 实现原理&方法：

Raspberry Pi 使用的设备文件框架是 `udev`，`udev`允许通过配置文件来进行一些规则配置，配置文件放置在`/etc/udev/rules.d/`下，以`.rules`结尾。

`99-com.rules`文件就是对于串行通讯端口（COM口）配置的文件，`99`表示系统应用该规则的顺序。

在这个文件中，我们可以添加此规则来对设备名称新增一个固定的软链接：

`KERNEL=="ttyUSB*",KERNELS=="1-1.3",MODE:="0x777", SYMLINK+="tty_RightUp"`

| 键      | 含义                                                         |
| ------- | ------------------------------------------------------------ |
| KERNEL  | 在内核里看到的设备名字，`ttyUSB*`是树莓派默认给USB转串的名称 |
| KERNELS | 使用`udevadm`命令得到的关于对应端口的绝对信息                |
| MODE    | 设备文件的权限                                               |
| SYMLINK | 添加软链接                                                   |

##### udevadm

在编写`udev`规则时，常常使用`udevadm`命令来查看设备属性。当获取到设备特定属性之后，我们便能编写对于特定设备或端口的特殊规则。

在树莓派中，对于同一USB端口插入不同的USB转串口设备，设备的`KERNELS`的部分参数始终不变；而插入不同USB端口，`KERNELS`的参数又都不同，所以在这里我们使用`KERNELS`参数来识别对应的USB端口。

使用：`udevadm info --attribute-walk --name=/dev/ttyUSB0 | grep KERNELS`

得到：

```
KERNELS=="ttyUSB0"
KERNELS=="1-1.3:1.0"
KERNELS=="1-1.3"
KERNELS=="1-1"
KERNELS=="usb1"
KERNELS=="3f980000.usb"
KERNELS=="soc"
KERNELS=="platform"
```

其中的数字部分是层层递进的：

```
KERNELS=="1-1.3:1.0"
KERNELS=="1-1.3"
KERNELS=="1-1"
```

选取不带`:`的最长的那行数据便是此端口的特定参属性，填入规则中，则最终规则为：

`KERNEL=="ttyUSB*",KERNELS=="1-1.3",MODE:="0x777", SYMLINK+="tty_RightUp"`

重启服务生效：`sudo /etc/init.d/udev restart`

#### 参考：

[树莓派绑定USB串口id](https://blog.csdn.net/qq_26800875/article/details/83302546)

[Linux┊详解udev](https://www.cnblogs.com/sopost/archive/2013/01/09/2853200.html)

[udev wiki][link]

[link]:https://wiki.archlinux.org/index.php/Udev_(简体中文)	"udev wiki"



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

### 开启远程桌面

#### VNC远程桌面连接

树莓派**3B+**自带vncserver，所以只要开启就可以进行远程连接。

##### 启动树莓派VNC服务

Enable VNC：`sudo raspi-config` -> `Interfacing Options` -> `VNC` -> `<是>` -> `<确定>` -> `<Finish>`

启动服务：`vncserver`

到这儿在树莓派上的vnc服务就已经启动了，启动服务会给出连接地址，就在“New desktop is ...”后面的括号里。

##### 使用PC连接

1. 下载对应版本的[VNC Viewer](https://www.realvnc.com/en/connect/download/viewer/windows/)，安装好；
2. 在输入框中输入连接地址，回车，输入树莓派的账号密码，点击连接。

#### RDP远程桌面连接

RDP是“Windows远程桌面连接”所使用的协议，xrdp是一个开源的RDP Server软件。

##### 启动树莓派RDP服务

安装xrdp：`sudoapt-get install xrdp`

##### 使用PC连接

1. 打开Windows “远程桌面连接”；
2. 输入树莓派IP，点击连接；
3. session 选择 Xorg，输入用户密码，Ok。

#### VNC/RDP对比

1. VNC可以记住密码，RDP需要每次登录都输入用户名密码；
2. 都可以在退出后重新连接，原有的工作界面不变；
3. RDP画质相对好一些。
4. VNC改变窗口大小，会自己适配；RDP如果显示窗口小于原画，则只会显示部分，会在左边或下边显示拖动条。

<br/>

### 禁止休眠/屏幕常亮

Raspbian使用了轻量桌面显示管理器即lightdm,通过该管理器可对xserver桌面会话交互、显示等进行设置.

**1.打开lightdm.conf**

```
sudo vi /etc/lightdm/lightdm.conf
```

**2.修改lightdm.conf**

找到[SeatDefaults]段下的’xserver-command’,取消注释,修改为如下:

```
#xserver-command=X
```

修改为

```
xserver-command=X -s 0 -dpms
```

- -s  设置屏幕保护不启用
- -dpms 关闭电源节能管理

**3.重启**

```
 sudo reboot
```

<br/>

### 切换屏幕分辨率

命令：`sudo raspi-config` -> `Advanced Options` -> `Resolution`

树莓派**3B+**提供了两种输出模式，**CEA**和**DMT**。

我的显示器分辨率是2560×1440，如果选择**CEA**模式的分辨率，会居中显示在2560×1440的屏幕上，四周会有黑边；如果选择**DMT**模式，无论选择什么分辨率，都会自动拉伸，覆盖到整个屏幕。

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

### 截屏

1.首先在终端中用下面的命令安装名叫**scrot**的截屏工具。
   `sudo apt-get install scrot`

2.截取全屏幕执行：
   `sudo scrot`

3.用鼠标选区屏幕区域截取执行：
   `sudo scrot -s`

4.20秒后截取，参数可以自定义：
   `sudo scrot -d 20`

<br/>

### 树莓派中文输入

1.基于Fcitx输入法框架的google拼音输入法，在命令行中输入以下命令即可安装。

安装命令：`sudo apt-get install fcitx fcitx-googlepinyin fcitx-module-cloudpinyin fcitx-sunpinyin`



2.SCIM支持多国语言的输入法，默认支持英语键盘、智能拼音、五笔字型、自然码、二笔等。

安装命令：`sudo apt-get install scim-pinyin`



3.Ibus输入法是一个基于Python开发的全新输入法框架，支持多种语言。

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

<br/>

### 其它

* [HDMI-VGA转换器黑屏解决方法](https://blog.csdn.net/mdq11111/article/details/50281891)
* [使用overlayfs打造一个只读的不怕意外关机的树莓派Raspberry Pi](https://blog.csdn.net/zhufu86/article/details/78906046)


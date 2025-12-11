---
title: 系统移植 iTOP-4412 Ubuntu
date: 2025-08-28 13:30:00
toc: true
categories:
  - tech
tags:
  - 嵌入式
---

以 iTOP-4412 POP 1G 主板为基础，总结一下 Ubuntu 文件系统的移植。

试验版本：ubuntu-base-16.04.6

<!--more-->

<br/>

## 获取 ubuntu-base

根据： [在ARM板上跑Ubuntu系统和黑客帝国，牛！-腾讯云开发者社区-腾讯云](https://cloud.tencent.com/developer/article/2217947)，下载了 [ubuntu-base-16.04.6-base-armhf.tar.gz](https://mirror.nju.edu.cn/ubuntu-cdimage/ubuntu-base/releases/16.04.6/release/ubuntu-base-16.04.6-base-armhf.tar.gz)。

[Index of /ubuntu-base/releases](https://cdimage.ubuntu.com/ubuntu-base/releases/)

## 配置 Ubuntu

参考： [米尔瑞米派Remi Pi Ubuntu系统移植指南-腾讯云开发者社区-腾讯云](https://cloud.tencent.com/developer/article/2420771)

### 准备chroot环境

```sh
$ mkdir ubuntu_rootfs
$ sudo tar xzf ubuntu-base-16.04.6-base-armhf.tar.gz -C ubuntu_rootfs
# sudo for "tar: dev/dsp: Cannot mknod: Operation not permitted"
$ sudo apt-get install qemu-user-static
$ sudo cp /etc/resolv.conf ./ubuntu_rootfs/etc/resolv.conf
$ sudo cp /usr/bin/qemu-arm-static ./ubuntu_rootfs/usr/bin/

# 此时系统所占大小
$ sudo du -h -d 1 ubuntu-base-16
104M	ubuntu-base-16
```

### 制作挂载脚本

将下列脚本代码拷贝到**mount_ubuntu_fs.sh**文件中,并改变权限(777)为可执行。

```bash
#!/bin/bash
function mnt() {
	echo "MOUNTING"
	sudo mount -t proc /proc ${2}proc
	sudo mount -t sysfs /sys ${2}sys
	sudo mount -o bind /dev ${2}dev
	sudo mount -o bind /dev/pts ${2}dev/pts        
	sudo chroot ${2}
}
function umnt(){
	echo "UNMOUNTING"
	sudo umount ${2}proc
	sudo umount ${2}sys
	sudo umount ${2}dev/pts
	sudo umount ${2}dev
}
if [ "$1" == "-m" ] && [ -n "$2" ] ;
then
	mnt $1 $2
elif [ "$1" == "-u" ] && [ -n "$2" ];
then
	umnt $1 $2
else
echo ""
echo "Either 1'st, 2'nd or bothparameters were missing"
echo ""
echo "1'st parameter can be one ofthese: -m(mount) OR -u(umount)"
echo "2'nd parameter is the full pathof rootfs directory(with trailing '/')"
echo ""
echo "For example: ./ch-mount.sh -m ./ubuntu_rootfs/"
echo "For example: ./ch-mount.sh -u ./ubuntu_rootfs/"
echo ""
echo 1st parameter : ${1}
echo 2nd parameter : ${2}
fi
```

### 配置 Ubuntu

#### 挂载

```sh
$ ./mount_ubuntu_fs.sh -m ./dfs/
MOUNTING
[sudo] password for easy: 
root@ubuntu:/# ls
bin   core  etc   lib      lost+found  mnt  proc  run   srv  tmp  var
boot  dev   home  media    opt  root  sbin  sys  usr
```

#### 基础包安装

```sh
root@ubuntu:/# chmod 777 /tmp         （避免update时失败）
root@ubuntu:/# apt update
root@ubuntu:/# apt-get install language-pack-zh-hant language-pack-zh-hans
root@ubuntu:/# apt install language-pack-en-base
root@ubuntu:/# apt install dialog rsyslog
root@ubuntu:/# apt install systemd avahi-daemon avahi-utils udhcpc ssh（必备安装）
root@ubuntu:/# apt install sudo
root@ubuntu:/# apt install vim
root@ubuntu:/# apt install net-tools
root@ubuntu:/# apt install ethtool
root@ubuntu:/# apt install ifupdown
root@ubuntu:/# apt install iputils-ping
root@ubuntu:/# apt install htop
root@ubuntu:/# apt install lrzsz
root@ubuntu:/# apt install gpiod
root@ubuntu:/# apt install wpasupplicant
root@ubuntu:/# apt install kmod
root@ubuntu:/# apt install iw
root@ubuntu:/# apt install usbutils
root@ubuntu:/# apt install memtester
root@ubuntu:/# apt install alsa-utils
root@ubuntu:/# apt install ufw
root@ubuntu:/# apt install psmisc

# ping: icmp open socket: Operation not permitted
root@ubuntu:/# chmod u+s $( which ping );
```

#### 添加log

```sh
root@ubuntu:/# touch /var/log/rsyslog
root@ubuntu:/# chown syslog:adm /var/log/rsyslog
root@ubuntu:/# chmod 666 /var/log/rsyslog
root@ubuntu:/# systemctl unmask rsyslog
root@ubuntu:/# systemctl enable rsyslog
```

#### 安装网络和语言包支持

```sh
root@ubuntu:/# apt-get install synaptic
root@ubuntu:/# apt-get install rfkill
root@ubuntu:/# apt-get install network-manager
root@ubuntu:/# apt install -y --force-yes --no-install-recommends fonts-wqy-microhei
root@ubuntu:/# apt install -y --force-yes --no-install-recommends ttf-wqy-zenhei
```

#### LXDE桌面系统安装

```sh
root@ubuntu:/# apt-get install xinit
root@ubuntu:/# apt-get install lxde
```

#### 浏览器和音频安装

```sh
root@ubuntu:/# sudo apt install epiphany-browser
root@ubuntu:/# sudo apt install xine-ui
```

#### 设置root密码

```sh
root@ubuntu:/# passwd root
Enter new UNIX password:root
Retype new UNIX password:root
passwd: password updated successfully
```

#### 创建新用户

```sh
root@ubuntu:/# adduser easy
perl: warning: Setting locale failed.
perl: warning: Please check that your locale settings:
LANGUAGE = (unset),
LC_ALL = (unset),
LC_TIME = "zh_CN.UTF-8",
LC_IDENTIFICATION = "zh_CN.UTF-8",
LC_TELEPHONE = "zh_CN.UTF-8",
LC_NUMERIC = "zh_CN.UTF-8",
LC_ADDRESS = "zh_CN.UTF-8",
LC_NAME = "zh_CN.UTF-8",
LC_MONETARY = "zh_CN.UTF-8",
LC_PAPER = "zh_CN.UTF-8",
LC_MEASUREMENT = "zh_CN.UTF-8",
LANG = "zh_CN.UTF-8"
are supported and installed on your system.
perl: warning: Falling back to the standard locale ("C").
Adding user `myir' ...
Adding new group `myir' (1000) ...
Adding new user `myir' (1000) with group `myir' ...
Creating home directory `/home/myir' ...
Copying files from `/etc/skel' ...
Enter new UNIX password:e
Retype new UNIX password:e
passwd: password updated successfully
Changing the user information for myir
Enter the new value, or press ENTER for the default
Full Name []: Easy  
Room Number []: 
Work Phone []:
Home Phone []:
Other []:
Is the information correct? [Y/n] Y
```

添加权限：

```sh
chmod 660 /etc/sudoers
vim /etc/sudoers
root ALL=(ALL:ALL) ALL
easy ALL=(ALL:ALL) ALL
chmod 440 /etc/sudoers
```

#### 设置hosts和主机名称

```sh
vim /etc/hostname
# 主机名：清空，写入 iTop4412_Ubuntu16
```

#### 配置网卡接口

```sh
root@ubuntu:/# vim /etc/network/interfaces
auto eth0
iface eth0 inet dhcp
```

#### 卸载系统

```sh
root@ubuntu:/# exit
exit
PC@system1:~$
PC@system1:~$ ./mount_ubuntu_fs.sh -u ./ubuntu_rootfs/
UNMOUNTING

# umount ubuntu_rootfs/dev/pts: target is busy
$ fuser -m dfs/dev/pts
dfs/dev/pts:  1418  1424  1432  1434  1436m  1446  1530  1535  1541  1548  1564m  1582  1585  1587  1590  1593  1605  1609  1612  1620  1628  1631  1641  1645  1662  1676  1686  1687  1688  1690  1696  1697  1704  1705  1706  1708  1710  1719  1731  1734  1738  1739  1740  1741  1802  1806  1829  1834  1848  1930  1934  2153
sudo kill -9 1418
```

至此ubuntu 文件系统已经配置完成。

#### 设置终端类型

在 `.bashrc` 末尾追加一行 `export TERM=xterm`。



## 烧写系统

配置好的 ubuntu_rootfs，需要烧写到 SD 卡。

在 dts 中设置了 `bootargs = "root=/dev/mmcblk0p2 rw console=ttySAC2,115200 init=/linuxrc rootwait";`

所以，复制系统文件到 p2。

```bash
mkdir tf_card
sudo mount /dev/sdb2 tf_card/
sudo cp -r ubuntu_rootfs/* tf_card/
sudo sync
sudo umount tf_card
rm -rf tf_card
```

也可以在 tf_card 目录中直接启动模拟器。

```bash
./mount_ubuntu_fs.sh -m tf_card/
```



## 待解决

### startup

```
[FAILED] Failed to start Set console keymap
See 'systemctl status keyboard-setup.service' for details.
easy@iTop4412_Ubuntu16:~$ systemctl status keyboard-setup.service
● keyboard-setup.service - Set console keymap
   Loaded: loaded (/lib/systemd/system/keyboard-setup.service; disabled; vendor 
preset: enabled)
   Active: failed (Result: exit-code) since Fri 2021-04-02 02:51:59 U
TC; 3 years 7 months ago
  Process: 116 ExecStart=/usr/bin/loadkeys /etc/console-setup/cached.kmap.gz 
(code=exited, status=203/EXEC)
 Main PID: 116 (code=exited, status=203/EXEC)

[FAILED] Failed to start Set console font and keymap.
See 'systemctl status console-setup.service' for details.
easy@iTop4412_Ubuntu16:~$ systemctl status console-setup.service
● console-setup.service - Set console font and keymap
   Loaded: loaded (/lib/systemd/system/console-setup.service; static; vendor pre
set: enabled)
   Active: failed (Result: exit-code) since Fri 2021-04-02 02:52:00 U
TC; 3 years 7 months ago
     Docs: man:loadkeys(1)
  Process: 154 ExecStart=/bin/setupcon --save (code=exited, status=203/E
XEC)
 Main PID: 154 (code=exited, status=203/EXEC)

[FAILED] Failed to start Light Display Manager.
See 'systemctl status lightdm.service' for details.
easy@iTop4412_Ubuntu16:~$ systemctl status lightdm.service
● lightdm.service - Light Display Manager
   Loaded: loaded (/lib/systemd/system/lightdm.service; enabled; vendor preset: 
enabled)
   Active: failed (Result: start-limit-hit) since Tue 2024-11-26 07:0
2:19 UTC; 8min ago
     Docs: man:lightdm(1)
  Process: 476 ExecStart=/usr/sbin/lightdm (code=exited, status=1/FAILUR
E)
  Process: 471 ExecStartPre=/bin/sh -c [ "$(basename $(cat /etc/X11/default-disp
lay-manager 2>/dev/null))" = "lightdm" ] (code=exited, status=0/SUCCESS)
 Main PID: 476 (code=exited, status=1/FAILURE)
```



### reboot

```sh
easy@iTop4412_Ubuntu16:~$ reboot
Failed to set wall message, ignoring: Interactive authentication required.
Failed to reboot system via logind: Interactive authentication required.
Failed to start reboot.target: Interactive authentication required.
See system logs and 'systemctl status reboot.target' for details.
Failed to open /dev/initctl: Permission denied
Failed to talk to init daemon.
easy@iTop4412_Ubuntu16:~$ poweroff 
Failed to set wall message, ignoring: Interactive authentication required.
Failed to power off system via logind: Interactive authentication required.
Failed to start poweroff.target: Interactive authentication required.
See system logs and 'systemctl status poweroff.target' for details.
Failed to open /dev/initctl: Permission denied
Failed to talk to init daemon.
```



### watchdog: BUG

```
easy@iTop4412_Ubuntu16:~$ [  596.033398] watchdog: BUG: soft lockup - CPU#0 stuck for 23s! [swapper/0:0]
[  599.782711] rcu: INFO: rcu_preempt detected stalls on CPUs/tasks:
[  599.788652] rcu: 	0-....: (18 ticks this GP) idle=71e/0/0x3 softirq=6547/6564 fqs=1298 
[  599.797995] rcu: 	(detected by 1, t=2602 jiffies, g=4889, q=21)
[  610.317193] cpu cpu0: _set_opp_voltage: failed to set voltage (1187500 1187500 1187500 mV): -5
[  610.822172] cpufreq: __target_index: Failed to change cpu frequency: -5
```



### vim 显示问题

只显示一半，设置了 lines 之后，如何设置刚好是终端的行数？



## 参考

* [ubuntu20.04LTS终端没有颜色——给黑底白字的终端染色 – 朝露碎梦](https://aibofan.com/ubuntu20-04lts-terminal-has-no-color-dye-terminal-with-white-text-on-black-background/)

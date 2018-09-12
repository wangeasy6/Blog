---
title: 树莓派
date: 2018-07-28 00:08:00
categories:
  - tools
toc: true
---

### 主题
* 常用软件
* 树莓派开启SSH
* 更换源地址到国内镜像
* 更换pip源到国内镜像
* 开启Root权限
* rar解压缩

<!--more-->
<br/>
### 常用软件
`sudo apt-get install vim`

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
* pip国内的一些镜像：

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
### 树莓派（debian）下解压缩rar

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
### 中文输入法
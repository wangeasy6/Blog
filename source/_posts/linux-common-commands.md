---
title: Linux常用命令
date: 2023-07-31 17:31:00
toc: true
categories:
  - tech
tag:
  - Tools
---

![linux-wallpaper.jpg](/resources/Cover/linux-wallpaper.jpg)

<!--more-->

## 环境信息获取

### 查看系统基本信息

```
$ uname -a
Linux ubuntu 5.15.0-76-generic #83~20.04.1-Ubuntu SMP Wed Jun 21 20:23:31 UTC 2023 x86_64 x86_64 x86_64 GNU/Linux
$ uname --kernel-name 
Linux
```

Linux : **kernel name**

ubuntu : **network node hostname**

5.15.0-76-generic : **kernel release**

\#83~20.04.1-Ubuntu SMP Wed Jun 21 20:23:31 UTC 2023 : **kernel version**

x86_64 : **machine hardware name**

x86_64 : **processor type**

x86_64 : **hardware platform**

GNU/Linux : **operating system**

### 查看 Linux 内核版本

```
$ cat /proc/version
Linux version 5.15.0-76-generic (buildd@lcy02-amd64-019) (gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0, GNU ld (GNU Binutils for Ubuntu) 2.34) #83~20.04.1-Ubuntu SMP Wed Jun 21 20:23:31 UTC 2023
```

还包含了 gcc 编译版本、编译时间。

### 查看 Ubuntu 系统版本

```
$ lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 20.04.6 LTS
Release:	20.04
Codename:	focal
```

### 查看 CentOS 系统版本

`$ cat /etc/redhat-release`

### 查看 CPU 信息

```
$ lscpu
Architecture:                    x86_64
CPU op-mode(s):                  32-bit, 64-bit
Byte Order:                      Little Endian
Address sizes:                   45 bits physical, 48 bits virtual
CPU(s):                          2
On-line CPU(s) list:             0,1
Thread(s) per core:              1
Core(s) per socket:              1
Socket(s):                       2
NUMA node(s):                    1
Vendor ID:                       GenuineIntel
CPU family:                      6
Model:                           165
Model name:                      Intel(R) Core(TM) i5-10400F CPU @ 2.90GHz
Stepping:                        3
CPU MHz:                         2904.008
BogoMIPS:                        5808.01
Hypervisor vendor:               VMware
Virtualization type:             full
L1d cache:                       64 KiB
L1i cache:                       64 KiB
L2 cache:                        512 KiB
L3 cache:                        24 MiB
NUMA node0 CPU(s):               0,1
Vulnerability Itlb multihit:     Not affected
Vulnerability L1tf:              Mitigation; PTE Inversion
Vulnerability Mds:               Vulnerable: Clear CPU buffers attempted, no microcode; SMT 
                                 Host state unknown
Vulnerability Meltdown:          Mitigation; PTI
Vulnerability Mmio stale data:   Vulnerable: Clear CPU buffers attempted, no microcode; SMT 
                                 Host state unknown
Vulnerability Retbleed:          Mitigation; IBRS
Vulnerability Spec store bypass: Mitigation; Speculative Store Bypass disabled via prctl and
                                  seccomp
Vulnerability Spectre v1:        Mitigation; usercopy/swapgs barriers and __user pointer san
                                 itization
Vulnerability Spectre v2:        Mitigation; IBRS, IBPB conditional, STIBP disabled, RSB fil
                                 ling, PBRSB-eIBRS Not affected
Vulnerability Srbds:             Unknown: Dependent on hypervisor status
Vulnerability Tsx async abort:   Not affected
Flags:                           fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cm
                                 ov pat pse36 clflush mmx fxsr sse sse2 ss syscall nx pdpe1g
                                 b rdtscp lm constant_tsc arch_perfmon nopl xtopology tsc_re
                                 liable nonstop_tsc cpuid tsc_known_freq pni pclmulqdq ssse3
                                  fma cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt aes xsave 
                                 avx f16c rdrand hypervisor lahf_lm abm 3dnowprefetch invpci
                                 d_single pti ssbd ibrs ibpb stibp fsgsbase tsc_adjust bmi1 
                                 avx2 smep bmi2 invpcid rdseed adx smap clflushopt xsaveopt 
                                 xsavec xgetbv1 xsaves arat flush_l1d arch_capabilities
```

适用平台：包含此命令的所有平台

```
$ cat /proc/cpuinfo
processor	: 0
model name	: ARMv7 Processor rev 5 (v7l)
BogoMIPS	: 100.00
Features	: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm 
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x0
CPU part	: 0xc07
CPU revision	: 5

Hardware	: Generic DT based system
Revision	: 0000
Serial		: 0000000000000000
```

适用平台：全平台

### 查看硬盘使用情况

```
$ df -hT
Filesystem           Type            Size      Used Available Use% Mounted on
/dev/root            jffs2          13.0M      9.4M      3.6M  72% /
devtmpfs             devtmpfs       18.4M         0     18.4M   0% /dev
tmpfs                tmpfs          18.4M      4.0K     18.4M   0% /tmp
/dev/mmcblk0p1       vfat           29.1G     27.4M     29.1G   0% /sdcard
```

-h	Human readable (e.g. 1K 243M 2G)

-T	Print filesystem type

### 查看内存情况

```
$ free -m
              total        used        free      shared  buff/cache   available
Mem:           3877         765         183           9        2928        2821
Swap:          2047         201        1846
```

`-m` 意为单位为 MB；还可选 `-b/-k/-g`。

### 查看内存详情

```
$ cat /proc/meminfo
MemTotal:          37768 kB
MemFree:            4068 kB
MemAvailable:      20104 kB
Buffers:            3936 kB
Cached:            13264 kB
SwapCached:            0 kB
Active:            10704 kB
Inactive:           8900 kB
Active(anon):       2408 kB
Inactive(anon):        0 kB
Active(file):       8296 kB
Inactive(file):     8900 kB
Unevictable:           0 kB
Mlocked:               0 kB
SwapTotal:             0 kB
SwapFree:              0 kB
Dirty:                 0 kB
Writeback:             0 kB
AnonPages:          2420 kB
Mapped:             2436 kB
Shmem:                 4 kB
Slab:               5664 kB
SReclaimable:       1920 kB
SUnreclaim:         3744 kB
KernelStack:         464 kB
PageTables:          188 kB
NFS_Unstable:          0 kB
Bounce:                0 kB
WritebackTmp:          0 kB
CommitLimit:       18884 kB
Committed_AS:       4336 kB
VmallocTotal:     991232 kB
VmallocUsed:           0 kB
VmallocChunk:          0 kB
```

### 查看系统时间

```
$ date
Fri 21 Jul 2023 01:44:38 AM PDT
```

### 实时 Mem/CUP

```
$ top
Mem: 33652K used, 4116K free, 4K shrd, 3952K buff, 13264K cached
CPU:  0.0% usr  0.0% sys  0.0% nic  100% idle  0.0% io  0.0% irq  0.0% sirq
Load average: 0.00 0.00 0.00 1/58 1593
  PID  PPID USER     STAT   VSZ %VSZ CPU %CPU COMMAND
 1593  1461 root     R     2332  6.1   0  0.0 top
 1459  1031 root     S     4992 13.1   0  0.0 sshd: root@pts/0
 1486  1031 root     S     4992 13.1   0  0.0 sshd: root@pts/1
 1031     1 root     S     4688 12.3   0  0.0 sshd: /usr/sbin/sshd -f /root/sshd_config -h /
 1461  1459 root     S     2384  6.2   0  0.0 -sh
 1488  1486 root     S     2356  6.2   0  0.0 -sh
 1022     1 root     S     2352  6.2   0  0.0 -sh
    1     0 root     S     2328  6.1   0  0.0 init
  882     1 root     S     2324  6.1   0  0.0 telnetd
  619     1 root     S     1208  3.1   0  0.0 udevd --daemon
  583     2 root     SW       0  0.0   0  0.0 [kworker/0:2]
```

默认 3s 刷新一次，按 q 退出。

## 添加环境变量

在Ubuntu系统中，添加路径到环境变量通常指的是将路径添加到`PATH`环境变量中，这样你就可以在任何目录下通过命令行执行该路径下的可执行文件。以下是一些基本的步骤来添加路径到环境变量：

### 临时添加到环境变量

1. **打开终端**：你可以使用快捷键`Ctrl + Alt + T`打开终端。

2. **使用`export`命令**：
   
   ```sh
   export PATH=$PATH:/path/to/your/directory
   ```
   这将把`/path/to/your/directory`添加到当前会话的`PATH`环境变量中。注意替换`/path/to/your/directory`为你实际想要添加的目录路径。

### 永久添加到环境变量

如果你希望添加的路径在每次打开终端时都包含在`PATH`中，你需要将路径添加到你的shell配置文件中。

1. **打开配置文件**：根据你使用的shell，配置文件可能是`.bashrc`、`.zshrc`等。使用文本编辑器打开它，例如：
   
   ```sh
   nano ~/.bashrc
   ```
   
2. **添加路径**：在文件的末尾添加以下行：
   ```sh
   export PATH=$PATH:/path/to/your/directory
   ```
   同样，替换`/path/to/your/directory`为你的实际路径。

3. **保存并关闭文件**：如果你是使用`nano`，可以按`Ctrl + O`保存更改，然后按`Ctrl + X`退出。

4. **使更改生效**：为了让更改立即生效，你可以运行以下命令：
   
   ```sh
   source ~/.bashrc
   ```
   或者，你也可以简单地关闭并重新打开终端。

### 系统范围内的环境变量更改

对于系统范围内的环境变量更改，你可以编辑`/etc/profile`或`/etc/environment`文件。但是，通常推荐使用用户的shell配置文件，因为`/etc/profile`和`/etc/environment`会影响到所有用户，而且需要管理员权限来编辑。

1. **打开`/etc/profile`**：
   
   ```sh
   sudo nano /etc/profile
   ```
   
2. **添加路径**：在文件的末尾添加`export`命令：
   ```sh
   export PATH=$PATH:/path/to/your/directory
   ```

3. **保存并关闭文件**：使用`sudo nano`时，保存和关闭方法与普通`nano`相同。

4. **使更改生效**：对于系统范围内的更改，你可能需要重启系统，或者至少注销并重新登录。

请记住，永久添加路径到环境变量时要特别小心，因为错误的路径或语法错误可能会导致系统命令无法找到或执行错误。

## 命令行工具

### cp

**排除某些文件夹：**

```bash
rsync -avr --exclude='path1/to/exclude' --exclude='path2/to/exclude' source destination
```

### grep

在当前目录下查找：`grep -re "find_str"`

排除某类文件：`grep -re "find_str" --exclude=*.dts`

只在某类文件中查找：`grep -re "find_str" --include=*.c`

不匹配二进制文件：`grep -re "find_str" . --binary-files=without-match`

如果你想要搜索文件名而不是文件内容，可以使用 `--name` 选项：`grep "search_pattern" --name /path/to/directory`

如果你想要排除整个目录，可以使用 `--exclude-dir` 选项：`grep "search_pattern" --exclude-dir="/path/to/exclude" /path/to/directory`

### scp

#### 单文件传输

**上传：**

`scp local_file user@192.168.1.100:/romte_path/`

**下载：**

`scp user@192.168.1.100:/romte_path/remote_file .`

#### 多文件传输

**上传：**

`scp file1 file2 user@192.168.1.100:/remote_path`

**下载：**

`scp user@192.168.1.100:/romte_path/\{path1/file1,path2/file2\} .`

#### 复制整个文件夹

**上传：**
`scp -vr file_path user@192.168.1.100:/remote_path`

**下载：**
`scp -r user@192.168.1.100:/remote_path .`

#### 在两个远程主机之间复制文件

`scp user@192.168.1.100:/romte_path/remote_file user@192.168.1.101:/romte_path/`

#### 使用压缩来加快传输

在文件传输的过程中，我们可以使用压缩文件来加快文件传输.

我们可以使用 C 选项来启用压缩功能，该文件在传输前被压缩，在目的主机上被解压缩。

`scp -C user@192.168.1.100:/romte_path/remote_file .`

### tftp

windows 上免费的 tftp server：[TFTP Server TFTPDWIN](https://download.cnet.com/TFTP-Server-TFTPDWIN/3001-2085_4-10417389.html)

一、上传到指定文件夹

```bash
tftp -p -l file_or_path target_ip
```

二、下载到当前目录

```bash
tftp -g -r file_or_path source_ip
```

## 启动服务

### tftpd

```sh
sudo apt-get install tftpd-hpa
sudo vim /etc/default/tftpd-hpa
# 修改 TFTP_DIRECTORY
# 重启 TFTP 服务
sudo service tftpd-hpa restart
# 查看服务运行状态
sudo service tftpd-hpa status
```



### nfs

```sh
sudo apt-get install nfs-kernel-server
sudo vim /etc/exports
# add line 
# /home/easy/Code/nfsd *(rw,sync,no_subtree_check,no_root_squash)
# /home/easy/Code/nfsd/rootfs *(rw,sync,no_subtree_check,no_root_squash)
# 重启 nfs 服务
sudo service nfs-kernel-server restart
# 查看服务运行状态
sudo service nfs-kernel-server status
```

支持V2：

```sh
$ sudo cat /proc/fs/nfsd/versions
-2 +3 +4 +4.1 +4.2 # 表示不支持 V2
$ sudo vim /etc/default/nfs-kernel-server
# RPCNFSDCOUNT="-V 2 8"
# RPCMOUNTDOPTS="-V 2 --manage-gids"
# RPCSVCGSSDOPTS="--nfs-version 2,3,4 --debug --syslog"
$ sudo /etc/init.d/nfs-kernel-server restart
$ sudo cat /proc/fs/nfsd/versions 
+2 +3 +4 +4.1 +4.2
```


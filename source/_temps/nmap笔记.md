---
title: nmap笔记
date: 2019-02-18 20:21:02
toc: true
categories:
  - tools
---

nmap，也就是Network Mapper，最早是Linux下的网络扫描和嗅探工具包。 

其基本功能有三个，一是探测一组主机是否在线；其次是扫描 主机端口，嗅探所提供的网络服务；还可以推断主机所用的操作系统 。  

<!--more-->

**测试版本为 Nmap 7.70**

### ping 扫描

进行ping扫描，打印出对扫描做出响应的主机，指令：

`nmap -sP 192.168.1.0/24` / `nmap -sn 192.168.1.0/24`

结果示例：

```
Nmap scan report for Hiwifi.lan (192.168.1.1)
Host is up (0.0040s latency).
MAC Address: D4:EE:07:5E:BF:6A (Hiwifi)
```

主机信息可能包含在 report for之后 或者 MAC Address 括号后面。

### 开放端口扫描

`nmap -PS 192.168.1.0/24` / `nmap -PS 192.168.1.1`

指令可扫描整个网段，也可以扫描单个地址。

```
Nmap scan report for Hiwifi.lan (192.168.1.1)
Host is up (0.0064s latency).
Not shown: 993 closed ports
PORT     STATE SERVICE
53/tcp   open  domain
80/tcp   open  http
81/tcp   open  hosts2-ns
82/tcp   open  xfer
83/tcp   open  mit-ml-dev
443/tcp  open  https
5000/tcp open  upnp
MAC Address: D4:EE:07:5E:BF:6A (Hiwifi)
```


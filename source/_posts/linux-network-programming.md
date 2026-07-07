---
title: Linux 网络编程
date: 2026-06-13 23:29:00
toc: true
categories:
  - tech
tags:
  - C
---

Linux 的套接字（Socket）接口提供了一套标准化的网络通信机制。它的核心操作涵盖了从创建、绑定、监听到连接与数据传输的完整通信流程。

<!--more-->

<br/>

## 函数一览

### 核心操作：创建、绑定、监听与连接
这部分是套接字编程的基础，完整定义了一个通信端点的生命周期。

| 函数                | 函数原型                                                     | 作用简述                                                     |
| :------------------ | :----------------------------------------------------------- | :----------------------------------------------------------- |
| [socket](#socket)   | `int socket(int domain, int type, int protocol);`            | 创建一个新的通信端点（套接字），成功时返回一个文件描述符。   |
| [bind](#bind)       | `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);` | 将套接字与一个本地地址（通常是 IP 和端口）绑定，主要供服务器端使用。 |
| [listen](#listen)   | `int listen(int sockfd, int backlog);`                       | 将套接字置于监听模式，使其能接受客户端的连接请求。`backlog` 定义了连接队列的最大长度。 |
| [accept](#accept)   | `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);` | 在监听套接字上接受一个传入的连接请求，并返回一个新的套接字描述符，专用于与该客户端通信。 |
| [connect](#connect) | `int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);` | 客户端向指定地址的服务器发起连接请求。                       |
| [close](#close)     | `int close(int fd);`                                         | 关闭一个套接字，终止连接并释放相关资源。                     |

### 数据发送与接收

完成连接后，这些接口用于实际的网络数据交换，提供了不同复杂度的功能选择。

| 函数                  | 函数原型                                                     | 作用简述                                                     |
| :-------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| [send](#sendmsg)      | `ssize_t send(int sockfd, const void *buf, size_t len, int flags);` | 在已连接的套接字上发送数据，是 TCP 通信的常用发送函数。      |
| [recv](#recvmsg)      | `ssize_t recv(int sockfd, void *buf, size_t len, int flags);` | 在已连接的套接字上接收数据，是 TCP 通信的常用接收函数。      |
| [sendto](#sendmsg)    | `ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);` | 发送数据，用于无连接的协议（如 UDP），需指定目标地址。       |
| [recvfrom](#recvmsg)  | `ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);` | 接收数据，用于无连接的协议（如 UDP），可获取发送方的地址。   |
| [sendmsg](#sendmsg)   | `ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);` | 更通用的发送/接收函数，允许控制信息（如辅助数据）的传递，功能最为强大，可将多个 I/O 操作聚合在一起。 |
| [recvmsg](#recvmsg)   | `ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);` | 更通用的接收函数。                                           |
| [sendmmsg](#sendmmsg) | `int sendmmsg(int sockfd, struct mmsghdr *msgvec, unsigned int vlen, int flags);` | 在套接字上批量发送多个消息。Linux 特有的。                   |
| [recvmmsg](#recvmmsg) | `int recvmmsg(int sockfd, struct mmsghdr *msgvec, unsigned int vlen, int flags, struct timespec *timeout);` | 批量接受套接字上的多个消息。Linux 特有的。                   |

### 地址转换与主机名解析

为了处理用户友好的 IP 地址和域名，Linux 提供了一系列转换接口。

| 函数                         | 函数原型                                                     | 作用简述                                                     |
| :--------------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| [inet_pton](#inet_pton)      | `int inet_pton(int af, const char *src, void *dst);`         | 将点分十进制（IPv4）或十六进制字符串（IPv6）的 IP 地址转换为网络字节序的二进制数据。`af` 可以是 `AF_INET` 或 `AF_INET6`。 |
| [inet_ntop](#inet_ntop)      | `const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);` | 将网络字节序的二进制 IPv4/IPv6 地址转换为点分十进制或十六进制的字符串形式。 |
| [htons/htonl](#htons)        | `uint16_t htons(uint16_t hostshort);`                        | 将 16/32 位无符号整数从主机字节序转换为网络字节序（大端序）。对应的反函数是 `ntohs` 和 `ntohl`。 |
| [getaddrinfo](#getaddrinfo)  | `int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);` | 将主机名和服务名转换为套接字地址结构，支持 IPv4 和 IPv6，是推荐使用的现代化解析函数。 |
| [freeaddrinfo](#getaddrinfo) | `void freeaddrinfo(struct addrinfo *res);`                   | 释放由 `getaddrinfo` 动态分配并返回的地址信息链表。          |
| [gai_strerror](#getaddrinfo) | `const char *gai_strerror(int errcode);`                     | 将 `getaddrinfo` 和 `getnameinfo` 返回的错误代码转换为可读的错误信息字符串。 |

### 套接字选项与高级控制

| 函数                      | 函数原型                                                     | 作用简述                                                     |
| :------------------------ | :----------------------------------------------------------- | :----------------------------------------------------------- |
| [getsockopt](#getsockopt) | `int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);` | 获取指定套接字的选项值。                                     |
| [setsockopt](#getsockopt) | `int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);` | 设置套接字选项，以控制套接字的行为（如地址重用、接收/发送缓冲区大小等）。 |
| [fcntl](#fcntl)           | `int fcntl(int fd, int cmd, ... /* arg */);`                 | 对套接字描述符执行多种控制操作，如设置 `O_NONBLOCK` 标志以切换到非阻塞模式。 |
| [ioctl](#ioctl)           | `int ioctl(int fd, unsigned long request, ...);`             | 一个多功能的系统调用，对设备进行特定的控制操作。在网络编程中，常用于**获取或设置网络接口信息**，如 IP 地址、MTU 等。 |
| [socketpair](#socketpair) | `int socketpair(int domain, int type, int protocol, int sv[2]);` | 创建一对相互连接的、匿名的套接字，主要用于**同一台主机上父子进程或相关进程间的通信**（IPC），创建的一对套接字是全双工的。 |

### 高级零拷贝接口
这些是 Linux 特有的系统调用，用于在内核空间中直接在两个文件描述符之间传递数据，避免了数据在内核空间和用户空间之间的拷贝，从而**极大提升大数据传输的性能**。

| 函数                  | 函数原型                                                     | 作用简述                                                     |
| :-------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| [sendfile](#sendfile) | `ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);` | 在两个文件描述符之间高效地传输数据，常用于**网络服务器发送文件**。 |
| [splice](#splice)     | `ssize_t splice(int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags);` | 在两个文件描述符之间移动数据，无需将数据拷贝到用户空间。     |

#### `sendfile` vs. `splice`
*   `sendfile` 是一种简化的、专门用于高效文件传输的接口。
*   `splice` 更为通用，可以在任意两个文件描述符之间移动数据，并且支持更多的控制标志。

<br/>

## 函数详情

基于 Linux man-pages 项目 5.09 版本。

### socket

#### 参考

[socket(2) - Linux manual page](https://man7.org/linux/man-pages/man2/socket.2.html) / [man.cx manual pages](https://man.cx/socket%282%29)

#### 原型

```c
#include <sys/types.h> 
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

`socket()` 创建一个用于通信的终端，并返回一个指向此终端的文件描述符。成功调用，返回的文件描述符将是当前进程中未打开的最小数字的文件描述符。

#### 参数

`domain` 参数指定一个通信域：用于通信的协议族。这些协议族定义在 `<sys/socket.h>` 中。Linux 内核当前理解的常见的格式：

- `AF_UNIX` / `AF_LOCAL`：本地通信
- `AF_INET`：IPv4 网络协议
- `AF_INET6`：IPv6 网络协议
- `AF_IPX`：IPX - Novell 协议
- `AF_NETLINK`：内核用户接口设备
- `AF_X25`：ITU-T X.25 / ISO-8208 协议
- `AF_AX25`：业余无线电台 AX.25 协议
- `AF_ATMPVC`：访问原始 ATM PVC
- `AF_APPLETALK`：Appletalk
- `AF_PACKET`：底层数据包接口
- `AF_LLC`：逻辑链路控制（IEEE 802.2 LLC）协议
- `AF_CAN`：控制器区域网（Controller Area Network）汽车总线协议
- `AF_BLUETOOTH`：蓝牙低级套接协议
- `AF_KCM`：KCM（kernel connection multiplexer，内核连接复用器）接口

关于上述地址族以及其他一些地址族的更多详细信息，可以在 [address_families(7)](https://man7.org/linux/man-pages/man7/address_families.7.html) 中找到。

套接字具有指定的 **type**，它定义了通信语义。当前定义的类型有：

| 类型                 | 描述                                                         |
| :------------------- | :----------------------------------------------------------- |
| **`SOCK_STREAM`**    | 提供有序的、可靠的、双向的、基于连接的字节流。可能支持带外数据传输机制。 |
| **`SOCK_DGRAM`**     | 支持数据报（无连接、不可靠的固定最大长度消息）。             |
| **`SOCK_SEQPACKET`** | 为固定最大长度的数据报提供有序的、可靠的、双向的、基于连接的数据传输路径；使用者在每次输入系统调用时需读取整个数据包。 |
| **`SOCK_RAW`**       | 提供原始的网络协议访问。                                     |
| **`SOCK_RDM`**       | 提供一个不保证顺序的可靠数据报层。                           |
| **`SOCK_PACKET`**    | 已废弃，不应在新程序中使用；请参阅 [packet(7)](https://man7.org/linux/man-pages/man7/packet.7.html)。 |

某些类型可能并非在所有协议族中都实现。从 Linux 2.6.27 开始，**type** 参数还有第二个用途：除了指定套接字类型外，它可以包含以下任何值的按位或，以修改 `socket()` 的行为：

| 标志                | 描述                                                         |
| :------------------ | :----------------------------------------------------------- |
| **`SOCK_NONBLOCK`** | 在新文件描述符引用的打开文件描述上设置 `O_NONBLOCK` 文件状态标志（参见 [open(2)](https://man.cx/open%282%29)）。使用此标志可以省去额外调用 [fcntl()](#fcntl) 来实现相同结果。 |
| **`SOCK_CLOEXEC`**  | 在新文件描述符上设置 close-on-exec（`FD_CLOEXEC`）标志。有关其用途，请参阅 [open(2)](https://man.cx/open%282%29) 中 `O_CLOEXEC` 标志的描述。 |

**protocol** 指定与套接字一起使用的特定协议。通常，在给定协议族中，只有一个协议支持特定的套接字类型，此时 protocol 可以指定为 0。但是，可能存在多个协议，在这种情况下，必须以这种方式指定特定的协议。使用的协议编号特定于通信发生的“通信域”；请参阅 [protocols(5)](https://man.cx/protocols%285%29)。关于如何将协议名称字符串映射到协议编号，请参阅 [getprotoent(3)](https://man.cx/getprotoent%283%29)。

##### 不同类型套接字的细节

- **`SOCK_STREAM`** 套接字是全双工字节流。它们不保留记录边界。流套接字在发送或接收任何数据之前必须处于连接状态。与另一个套接字的连接通过  [connect()](#connect) 调用创建。连接后，可以使用 [read(2)](https://man.cx/read%282%29) 和 [write(2)](https://man.cx/write%282%29) 调用或 [send()](#sendmsg) 和 [recv()](#recvmsg) 调用的某些变体来传输数据。会话完成后，可以执行 [close()](#close)。带外数据（Out-of-band data：TCP 紧急数据，使用 MSG_OOB flag 实现插队处理）也可以使用 [send()](#sendmsg) / [recv()](#recvmsg) 进行收发。实现 `SOCK_STREAM` 的通信协议可确保数据不会丢失或重复。如果一段数据由于对端协议缓冲区空间不足而无法在合理的时间内成功传输，则认为连接已断开。当在套接字上启用 `SO_KEEPALIVE` 时，协议会以特定于协议的方式检查另一端是否仍然存在。如果进程在损坏的流上发送或接收数据，则会引发 `SIGPIPE` 信号（其默认动作是终止进程），这会导致没有显式处理该信号的进程退出。
- **`SOCK_SEQPACKET`** 套接字使用与 `SOCK_STREAM` 套接字相同的系统调用。唯一的区别是 [read(2)](https://man.cx/read%282%29) 调用将只返回请求的数据量，到达的数据包中剩余的任何数据都将被丢弃。此外，传入数据报中的所有消息边界都将被保留。
- **`SOCK_DGRAM`** 和 **`SOCK_RAW`** 套接字允许向 [sendto()](#sendmsg) 调用中指定的目标发送数据报。数据报通常使用 [recvfrom()](#recvmsg) 接收，它返回下一个数据报及其发送者的地址。
- **`SOCK_PACKET`** 是一个过时的套接字类型，用于直接从设备驱动程序接收原始数据包。请改用 [packet(7)](https://man.cx/packet%287%29) 。

可以使用 [fcntl()](#fcntl) 的 `F_SETOWN` 操作来指定一个进程或进程组，以便在带外数据到达时接收 `SIGURG` 信号，或者在 `SOCK_STREAM` 连接意外断开时接收 `SIGPIPE` 信号。此操作也可用于设置接收 I/O 和通过 `SIGIO` 接收 I/O 事件异步通知的进程或进程组。使用 `F_SETOWN` 等效于使用 `FIOSETOWN` 或 `SIOCSPGRP` 参数调用  [ioctl()](#ioctl) 。

当网络向协议模块发出错误信号时（例如，对 IP 使用 ICMP 消息），会为套接字设置待处理错误标志。对此套接字的下一个操作将返回待处理错误的错误代码。对于某些协议，可以启用每个套接字的错误队列来检索有关错误的详细信息；请参阅 [ip(7)](https://man.cx/ip%287%29) 中的 `IP_RECVERR`。

套接字的操作由套接字级别选项控制。这些选项在 `<sys/socket.h>` 中定义。函数 [setsockopt()](#getsockopt) 和 [getsockopt()](#getsockopt) 用于设置和获取选项。

#### 返回值

成功时，返回新套接字的文件描述符。出错时，返回 -1，并设置相应的 `errno`。

##### 错误码

| 错误                      | 描述                                               |
| :------------------------ | :------------------------------------------------- |
| **`EACCES`**              | 无权创建指定类型和/或协议的套接字。                |
| **`EAFNOSUPPORT`**        | 实现不支持指定的地址族。                           |
| **`EINVAL`**              | 未知协议，或协议族不可用。                         |
| **`EINVAL`**              | type 中包含无效标志。                              |
| **`EMFILE`**              | 已达到每个进程对打开文件描述符数量的限制。         |
| **`ENFILE`**              | 已达到系统范围内打开文件总数的限制。               |
| **`ENOBUFS` 或 `ENOMEM`** | 可用内存不足。在释放足够的资源之前无法创建套接字。 |
| **`EPROTONOSUPPORT`**     | 此域中不支持协议类型或指定的协议。                 |

其他错误可能由底层协议模块生成。

#### 遵循标准

POSIX.1-2001, POSIX.1-2008, 4.4BSD。

`SOCK_NONBLOCK` 和 `SOCK_CLOEXEC` 标志是 Linux 特有的。

`socket()` 出现在 4.2BSD 中。它通常可移植到支持 BSD 套接层克隆的非 BSD 系统（包括 System V 变体）。

#### 注意

POSIX.1 并不要求包含 `<sys/types.h>`文件，而且在 Linux 上也不需要这个头文件。不过，某些历史（BSD）实现版本曾要求包含此头文件，而编写可移植的应用程序时或许明智的做法是将其包含在内。

在 4.x BSD 版本中用于协议家族的显式常量有 `PF_UNIX`、`PF_INET`  等等，而 `AF_UNX`、`AF_INET` 等等则用于地址家族。然而，早在 BSD 手册中就已承诺：“协议族通常与地址族相同”，后续的规范则处处使用 `AF_*`。

<br/>

### bind

#### 参考

[bind(2) - Linux manual page](https://man7.org/linux/man-pages/man2/bind.2.html) / [man.cx manual pages](https://man.cx/bind%282%29)

#### 原型

```c
#include <sys/types.h> 
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

#### 描述

当使用 [socket()](#socket) 创建一个套接字时，它存在于一个命名空间（地址族）中，但尚未被分配地址。`bind()` 会将 `addr` 指定的地址分配给文件描述符 `sockfd` 所引用的套接字。`addrlen` 指定了 `addr` 所指向地址结构的大小（以字节为单位）。

传统上，这个操作被称为“给一个套接字分配名字”。对于一个 `SOCK_STREAM` 套接字来说，在它能接收连接（[accept()](#accept)）之前，通常必须使用 `bind()` 来分配一个本地地址。

名称绑定的规则在不同的地址族之间有所不同。详细信息请参见第 7 节手册中的相应条目：对于 `AF_INET`，参见 [ip(7)](https://man.cx/ip%287%29)；对于 `AF_INET6`，参见 [ipv6(7)](https://man.cx/ipv6%287%29)；对于 `AF_UNIX`，参见 [unix(7)](https://man.cx/unix%287%29)；对于 `AF_APPLETALK`，参见 [ddp(7)](https://man.cx/ddp%287%29)；对于 `AF_PACKET`，参见 [packet(7)](https://man.cx/packet%287%29)；对于 `AF_X25`，参见 [x25(7)](https://man.cx/x25%287%29)；对于 `AF_NETLINK`，参见 [netlink(7)](https://man.cx/netlink%287%29)。

实际传递给 `addr` 参数的结构体取决于地址族。`sockaddr` 结构体的定义类似于：

```c
struct sockaddr {
    sa_family_t sa_family;
    char sa_data[14];
}
```

这个结构体的唯一用途是强制转换传递给 `addr` 的结构体指针，以避免编译器警告。

#### 返回值

成功时，返回 0。出错时，返回 -1，并设置相应的 `errno`。

##### 错误码

| 错误             | 描述                                                         |
| :--------------- | :----------------------------------------------------------- |
| **`EACCES`**     | 地址受到保护，且用户不是超级用户。                           |
| **`EADDRINUSE`** | 给定的地址已被使用。                                         |
| **`EADDRINUSE`** | （Internet domain套接字）套接字地址结构中的端口号被指定为0，但在尝试绑定到一个临时端口时，发现临时端口范围内的所有端口当前都被使用。参见 [ip(7)](https://man.cx/ip%287%29) 中对关于 `/proc/sys/net/ipv4/ip_local_port_range` 的讨论。 |
| **`EBADF`**      | `sockfd` 不是一个有效的文件描述符。                          |
| **`EINVAL`**     | 套接字已经绑定到了一个地址。                                 |
| **`EINVAL`**     | `addrlen` 不正确，或者 `addr` 对于这个套接字的域来说不是一个有效的地址。 |
| **`ENOTSOCK`**   | 文件描述符 `sockfd` 不指向一个套接字。                       |

以下错误是 UNIX 域 (`AF_UNIX`) 套接字特有的：

| 错误                | 描述                                                         |
| :------------------ | :----------------------------------------------------------- |
| **`EACCES`**        | 对路径前缀的某个组件的搜索权限被拒绝。（也参见 [path_resolution(7)](https://man.cx/path_resolution%287%29)） |
| **`EADDRNOTAVAIL`** | 请求的接口不存在，或者请求的地址不是本地的。                 |
| **`EFAULT`**        | `addr` 指向了用户可访问地址空间之外。                        |
| **`ELOOP`**         | 解析 `addr` 时遇到了太多的符号链接。                         |
| **`ENAMETOOLONG`**  | `addr` 过长。                                                |
| **`ENOENT`**        | 套接字路径名的目录前缀中的某个组件不存在。                   |
| **`ENOMEM`**        | 可用的内核内存不足。                                         |
| **`ENOTDIR`**       | 路径前缀的某个组件不是一个目录。                             |
| **`EROFS`**         | 套接字的 inode 将位于一个只读文件系统上。                    |

#### 遵循标准

POSIX.1-2001, POSIX.1-2008, SVr4, 4.4BSD （`bind()` 首次出现在 4.2BSD 中）。

#### 注意

POSIX.1 不要求包含 `<sys/types.h>`，并且在 Linux 上也不需要这个头文件。然而，一些历史悠久的（BSD）实现需要这个头文件，为了可移植性，包含它可能是明智的。

有关 `socklen_t` 类型的背景信息，参见 [accept()](#accept)。

<br/>

### listen

#### 参考

[listen(2) - Linux manual page](https://man7.org/linux/man-pages/man2/listen.2.html) / [man.cx manual pages](https://man.cx/listen%282%29)

#### 原型

```c
#include <sys/types.h>
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```

监听套接字上的连接。

`listen()` 将 `sockfd` 引用的套接字标记为被动套接字，即这个套接字将用于使用 [accept()](#accept) 接受传入的连接请求。

#### 参数

* `sockfd`：一个指向 `SOCK_STREAM` 或 `SOCK_SEQPACKET` 类型的套接字的文件描述符。

* `backlog`：定义了套接字文件描述符（sockfd）的挂起连接队列可增长的最大长度。如果队列已满时仍有连接请求到达，客户端可能会收到 `ECONNREFUSED` 错误；或者，如果底层协议支持重传，则可能会忽略该请求，以便稍后重新尝试连接时能够成功。

  自 Linux 2.2 版本起，TCP 套接字上 `backlog` 参数的行为发生了变化。如今，此参数指的是已经建立了连接（completely established）的等待接受（accept）的队列长度，而不是未完成建立连接请求（incomplete connection requests）的数量。未完成套接字的队列最大长度可以通过 `/proc/sys/net/ipv4/tcp_max_syn_backlog` 设置。当启用 `syncookies` 时，则该队列在逻辑上将不存在最大长度限制，此时上述设置也会被忽略。

  如果 `backlog` 大于 `/proc/sys/net/core/somaxconn`，自动设置为此值。自 Linux 5.4，此文件默认为 4096，在更早的版本，默认为 128。

#### 返回值

成功时返回零。出错时返回 -1，并设置相应的 `errno`。

##### 错误码

*   **`EADDRINUSE`**：另一个套接字已经在同一端口上监听。
*   **`EADDRINUSE`**（Internet 域套接字）：`sockfd` 引用的套接字之前未绑定到地址，并且在尝试将其绑定到临时端口时，发现临时端口范围内的所有端口号当前都在使用中。请参阅 [ip(7)](https://man.cx/ip%287%29) 中关于 `/proc/sys/net/ipv4/ip_local_port_range` 的讨论。
*   **`EBADF`**：参数 `sockfd` 不是有效的文件描述符。
*   **`ENOTSOCK`**：文件描述符 `sockfd` 不指向套接字。
*   **`EOPNOTSUPP`**：套接字不是支持 `listen()` 操作的类型。

#### 遵循标准
POSIX.1-2001, POSIX.1-2008, 4.4BSD（`listen()` 首次出现在 4.2BSD 中）。

#### 注意
要接受连接，请执行以下步骤：
1.  使用 [socket()](#socket) 创建套接字。
2.  使用 [bind()](#bind) 将套接字绑定到本地地址，以便其他套接字可以 [connect()](#connect) 到该套接字。
3.  使用 `listen()` 指定接受传入连接的意愿和传入连接的队列限制。
4.  使用 [accept()](#accept) 接受连接。

POSIX.1 不要求包含头文件，Linux 上也不需要此头文件。然而，某些历史悠久的（BSD）实现需要此头文件，可移植的应用程序最好包含它。

`backlog` 参数在 TCP 套接字上的行为在 Linux 2.2 中发生了改变。现在它指定了完全建立连接、等待接受的队列长度，而不是不完全的连接请求的数量。不完全套接字的队列最大长度可以使用 `/proc/sys/net/ipv4/tcp_max_syn_backlog` 设置。当启用 syncookies 时，没有逻辑上的最大长度，此设置将被忽略。有关更多信息，请参见 [tcp(7)](https://man.cx/tcp%287%29)。

如果 `backlog` 参数大于 `/proc/sys/net/core/somaxconn` 中的值，则该值将被静默地截断为该值。自 Linux 5.4 起，此文件中的默认值为 4096；在早期内核中，默认值为 128。在 2.4.25 之前的内核中，此限制是一个硬编码值 `SOMAXCONN`，值为 128。

<br/>

### accept

#### 参考

[accept(2) - Linux manual page](https://man7.org/linux/man-pages/man2/accept.2.html) / [man.cx manual pages](https://man.cx/accept%282%29)

#### 原型

```c
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *_Nullable restrict addr,
          socklen_t *_Nullable restrict addrlen);
// POSIX.1-2001, SVr4, 4.4BSD (accept() first appeared in 4.2BSD).

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sys/socket.h>

int accept4(int sockfd, struct sockaddr *_Nullable restrict addr,
          socklen_t *_Nullable restrict addrlen, int flags);
// POSIX.1-2024.  Linux 2.6.28, glibc 2.10.
```

`accept()` 系统调用用于基于连接的套接字类型（`SOCK_STREAM`、`SOCK_SEQPACKET`）。它从监听套接字 `sockfd` 的挂起连接队列中提取第一个连接请求，创建一个新的已连接套接字，并返回一个指向该套接字的新文件描述符。新创建的套接字并不处于监听状态。原始套接字 `sockfd` 不受此调用影响。

如果队列中没有挂起的连接，并且套接字未被标记为非阻塞，则`accept()`会阻塞调用者，直到出现连接。如果套接字被标记为非阻塞，且队列中没有挂起的连接，则`accept()`会失败，并返回`EAGAIN`或`EWOULDBLOCK`错误。

在 Linux 上，由 `accept()` 返回的新套接字不会从监听套接字继承诸如 `O_NONBLOCK` 和 `O_ASYNC` 之类的文件状态标志。这一行为与典型的 BSD 套接字实现有所不同。为了保证可移植性，程序不应依赖于文件状态标志的继承或非继承特性，而应始终在由 `accept()` 返回的套接字上显式设置所有所需的标志。

#### 参数

* `sockfd`：一个已通过 [socket()](#socket) 创建、已通过 [bind()](#bind) 绑定到本地地址，并在 [listen()](#listen) 之后正在监听连接的套接字。
* `addr`：一个指向 `sockaddr` 结构的指针。该结构中填充的是通信层所知的对方套接字的地址。`addr` 返回的地址的确切格式由套接字的地址族决定。当 `addr` 为 `NULL` 时，表示未填充任何内容，在这种情况下，`addrlen` 未使用，也应为 `NULL`。
* `addrlen`：调用时必须将其初始化为包含addr所指向结构的大小（以字节为单位）；返回时，它将包含对方地址的实际大小。如果提供的缓冲区太小，返回的地址将被截断；在这种情况下，`addrlen` 返回的值将大于调用时提供的值。
* `flag`：如果值为 0，则 `accept4()` 与 `accept()` 相同。可以按位或以下值以获得不同的行为：
  * `SOCK_NONBLOCK`：在新文件描述符所引用的打开文件描述（参见 [open(2)](https://man7.org/linux/man-pages/man2/open.2.html) 手册页）上设置`O_NONBLOCK`文件状态标志。使用此标志可避免多次调用 [fcntl(2)](https://man7.org/linux/man-pages/man2/fcntl.2.html) 函数来达到相同的效果。
  * `SOCK_CLOEXEC`：在新文件描述符上设置 close-on-exec（`FD_CLOEXEC`）标志。有关此标志可能有用之处的原因，请参阅 [open(2)](https://man7.org/linux/man-pages/man2/open.2.html) 手册页中`O_CLOEXEC`标志的描述。

#### 返回值

* **非负整数**：调用成功，返回所接受套接字的文件描述符。

* **-1**：发生错误，并设置相应的 `errno`，同时 `addrlen` 保持不变。

**错误处理**：Linux 的 `accept()`（以及 `accept4()`）会将新套接字上处于待处理状态的网络错误，作为调用的返回值错误码直接返回。这一行为与其他 BSD 套接字实现有所不同。为了确保操作的可靠性，应用程序在调用 `accept()` 之后，应当检测针对特定协议所定义的网络错误；一旦检测到此类错误，应将其视作 `EAGAIN` 错误进行处理，即尝试重试。对于 TCP/IP 协议而言，此类错误包括：`ENETDOWN`、`EPROTO`、`ENOPROTOOPT`、`EHOSTDOWN`、`ENONET`、`EHOSTUNREACH`、`EOPNOTSUPP` 以及 `ENETUNREACH`。

##### 错误码

* **EAGAIN** 或 **EWOULDBLOCK**：套接字已被标记为非阻塞模式，但当前没有可供 `accept` 的连接。POSIX.1-2001 和 POSIX.1-2008 标准允许在此情况下返回上述任一错误，且不要求这两个常量具有相同的值；因此，为了保证应用程序的可移植性，应当同时检查这两种可能的错误。

* **EBADF**：`sockfd` 不是一个有效的已打开文件描述符。

* **ECONNABORTED**：连接已被中止。

* **EFAULT**：`addr` 参数指向的地址位于用户地址空间的不可写区域。

* **EINTR**：系统调用在接收到有效连接之前，被捕获到的信号所中断；详见 [signal(7)](https://man7.org/linux/man-pages/man7/signal.7.html)。

* **EINVAL**：套接字未处于监听（listening）状态，或者 `addrlen` 参数无效（例如，其值为负数）。

* **EINVAL** ：`accept4()` 的  `flags` 参数中包含无效值。

* **EMFILE**：已达到当前进程允许打开的文件描述符数量的上限。

* **ENFILE**：已达到系统范围内允许打开的文件总数的上限。

* **ENOBUFS** 或 **ENOMEM**：可用内存不足。这通常意味着内存分配受限于套接字缓冲区的大小限制，而非受限于系统的整体内存容量。

* **ENOTSOCK**：文件描述符 `sockfd` 并非指向一个套接字。

* **EOPNOTSUPP**：所引用的套接字并非 `SOCK_STREAM` 类型。

* **EPERM**：防火墙规则禁止建立此连接。

* **EPROTO**：协议错误。

  此外，系统还可能返回与新创建的套接字相关、或由具体协议所定义的网络错误。不同版本的 Linux 内核可能会返回其他类型的错误，例如 `ENOSR`、`ESOCKTNOSUPPORT`、`EPROTONOSUPPORT` 或 `ETIMEDOUT`。在进行系统调用追踪（trace）时，可能会看到 `ERESTARTSYS`。

#### 遵循标准

- `accept()`：POSIX.1-2001、POSIX.1-2008、SVr4、4.4BSD（`accept()` 首次出现在 4.2BSD）。
- `accept4()` 是 Linux 特有的非标准扩展。

在 Linux 上，`accept()` 返回的新套接字不会继承监听套接字的文件状态标志（如 `O_NONBLOCK` 和 `O_ASYNC`）。此行为与标准的 BSD 套接字实现不同。可移植程序不应依赖文件状态标志的继承或非继承性，而应始终在从 `accept()` 返回的套接字上显式设置所有必需的标志。

#### 注意

POSIX.1-2001 不要求包含 `<sys/types.h>`，并且在 Linux 上也不需要此头文件。然而，一些历史悠久的（BSD）实现需要此头文件，可移植的应用程序最好包含它。

在传递了 `SIGIO` 或 [select(2)](https://man.cx/select%282%29)、[poll(2)](https://man.cx/poll%282%29) 或 [epoll(7)](https://man.cx/epoll%287%29) 返回可读事件之后，可能并不总是有连接在等待，因为连接可能由于异步网络错误或在调用 `accept()` 之前被另一个线程移除了。如果发生这种情况，则调用将阻塞以等待下一个连接到来。为确保 `accept()` 永不阻塞，传递的套接字 `sockfd` 需要设置 `O_NONBLOCK` 标志（参见 [socket(7)](https://man.cx/socket%287%29)）。

对于某些需要显式确认的协议（例如 DECnet）而言，`accept()` 函数仅可被视为从队列中取出下一个连接请求，而并不意味着已对该连接进行了确认。对新文件描述符执行常规的读或写操作，即可隐式地表示确认；而关闭该新套接字，则隐式地表示拒绝。目前在 Linux 系统中，仅有 DECnet 协议采用了这种语义。

关于 `socklen_t` 类型：在最初的 BSD 套接字实现（以及其他较旧的系统）中，`accept()` 函数的第三个参数被声明为 `int *` 类型。一份 POSIX.1g 标准草案曾提议将其更改为 `size_t *` 类型；而在随后的 POSIX 标准及 glibc 2.x 版本中，该参数最终被定为 `socklen_t *` 类型。

<br/>

### connect

#### 参考

[connect(2) - Linux manual page ](https://man7.org/linux/man-pages/man2/connect.2.html) / [man.cx manual pages](https://man.cx/connect%282%29)

#### 原型

```c
#include <sys/types.h>
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr,
           socklen_t addrlen);
```

#### 描述
`connect()` 系统调用将文件描述符 `sockfd` 所引用的套接字连接到 `addr` 指定的地址。`addrlen` 参数指定了 `addr` 的大小。`addr` 中地址的格式由套接字 `sockfd` 的地址空间决定；更多细节请参见 [socket()](#socket)。

如果套接字 `sockfd` 的类型是 `SOCK_DGRAM`，那么 `addr` 是默认发送数据报的目标地址，也是唯一接收数据报的源地址。如果套接字的类型是 `SOCK_STREAM` 或 `SOCK_SEQPACKET`，此调用会尝试与绑定到 `addr` 所指定地址的套接字建立连接。

某些协议套接字（例如 UNIX 域流式套接字）只能成功 `connect()` 一次。某些协议套接字（例如 UNIX 域和 Internet 域中的数据报套接字）可以多次使用 `connect()` 来改变它们的关联。某些协议套接字（例如 TCP 套接字，以及 UNIX 域和 Internet 域中的数据报套接字）可以通过连接到一个 `sockaddr` 中 `sa_family` 成员设置为 `AF_UNSPEC` 的地址来解除关联；此后，该套接字可以连接到另一个地址。（自 Linux 2.2 内核起支持 `AF_UNSPEC`。）

#### 返回值
如果连接或绑定成功，返回零。出错时，返回 -1，并设置相应的 `errno`。

##### 错误码

以下仅是一般性的套接字错误。可能还存在其他特定于域的错误代码。

| 错误                   | 描述                                                         |
| :--------------------- | :----------------------------------------------------------- |
| **`EACCES`**           | 对于通过路径名标识的 UNIX 域套接字：套接字文件的写权限被拒绝，或者路径前缀中某个目录的搜索权限被拒绝。（另请参见 [path_resolution(7)](https://man.cx/path_resolution%287%29)。） |
| **`EACCES`， `EPERM`** | 用户尝试连接广播地址但没有启用套接字的广播标志，或者由于本地防火墙规则导致连接请求失败。如果 SELinux 策略拒绝连接，也可能返回 `EACCES`（例如，策略规定 HTTP 代理只能连接到与 HTTP 服务器相关的端口，而代理尝试连接到不同的端口）。 |
| **`EADDRINUSE`**       | 本地地址已被使用。                                           |
| **`EADDRNOTAVAIL`**    | （Internet 域套接字）`sockfd` 所引用的套接字之前没有绑定到地址，在尝试将其绑定到临时端口时，发现临时端口范围内的所有端口号当前都在使用中。参见 [ip(7)](https://man.cx/ip%287%29) 中关于 `/proc/sys/net/ipv4/ip_local_port_range` 的讨论。 |
| **`EAFNOSUPPORT`**     | 传入地址的 `sa_family` 字段中的地址族不正确。                |
| **`EAGAIN`**           | 对于非阻塞的 UNIX 域套接字：套接字是非阻塞的，且无法立即完成连接。对于其他套接字族：路由缓存中的条目不足。 |
| **`EALREADY`**         | 套接字是非阻塞的，且之前的连接尝试尚未完成。                 |
| **`EBADF`**            | `sockfd` 不是一个有效的打开文件描述符。                      |
| **`ECONNREFUSED`**     | 在流式套接字上调用 `connect()`，发现在远程地址上没有进程在监听。 |
| **`EFAULT`**           | 套接字结构地址位于用户地址空间之外。                         |
| **`EINPROGRESS`**      | 套接字是非阻塞的，且无法立即完成连接。（UNIX 域套接字会返回 `EAGAIN` 而不是此错误。）可以通过 [select(2)](https://man.cx/select%282%29) 或 [poll(2)](https://man.cx/poll%282%29) 选择套接字以进行写入，以等待连接完成。在 [select(2)](https://man.cx/select%282%29) 指示可写之后，使用 [getsockopt()](#getsockopt) 读取 `SOL_SOCKET` 级别的 `SO_ERROR` 选项，以确定 `connect()` 是否成功完成（`SO_ERROR` 为零）或未成功（`SO_ERROR` 是此处列出的常见错误代码之一，解释了失败的原因）。 |
| **`EINTR`**            | 系统调用被捕获的信号中断；请参见 [signal(7)](https://man.cx/signal%287%29)。 |
| **`EISCONN`**          | 套接字已连接。                                               |
| **`ENETUNREACH`**      | 网络不可达。                                                 |
| **`ENOTSOCK`**         | 文件描述符 `sockfd` 不指向套接字。                           |
| **`EPROTOTYPE`**       | 套接字类型不支持请求的通信协议。例如，尝试将 UNIX 域数据报套接字连接到流式套接字时可能会发生此错误。 |
| **`ETIMEDOUT`**        | 尝试连接时超时。服务器可能太忙而无法接受新连接。请注意，对于 IP 套接字，当服务器启用了 syncookies 时，超时时间可能非常长。 |

#### 遵循标准
POSIX.1-2001, POSIX.1-2008, SVr4, 4.4BSD（`connect()` 首次出现在 4.2BSD 中）。

#### 注意
POSIX.1 不要求包含 `<sys/types.h>`，并且在 Linux 上也不需要此头文件。然而，一些历史悠久的（BSD）实现需要此头文件，可移植的应用程序最好包含它。有关 `socklen_t` 类型的背景信息，请参见 [accept()](#accept)。

如果 `connect()` 失败，套接字的状态视为未指定。可移植的应用程序应关闭该套接字并创建一个新套接字以重新连接。

<br/>

### close

#### 参考
[close(2) - Linux manual page](https://man7.org/linux/man-pages/man2/close.2.html) / [man.cx manual pages](https://man.cx/close%282%29)

#### 原型
```c
#include <unistd.h>

int close(int fd);
```

#### 描述
`close()` 会关闭一个文件描述符，使其不再引用任何文件，并且可以被重用。该文件描述符之前所关联的文件上的任何记录锁（参见 [fcntl()](#fcntl)），只要是该进程所持有的，都会被移除（无论当初用于获取该锁的是哪一个文件描述符）。如果 `fd` 是引用底层打开文件描述（参见 [open(2)](https://man.cx/open%282%29)）的最后一个文件描述符，则与该打开文件描述相关联的资源会被释放；如果该文件描述符是对一个已被 [unlink(2)](https://man.cx/unlink%282%29) 删除的文件的最后一个引用，则该文件会被删除。

#### 返回值
成功时返回 0。出错时返回 -1，并设置相应的 `errno`。

##### 错误码

| 错误                    | 描述                                                         |
| :---------------------- | :----------------------------------------------------------- |
| **`EBADF`**             | `fd` 不是一个有效的打开文件描述符。                          |
| **`EINTR`**             | `close()` 调用被信号中断；参见 [signal(7)](https://man.cx/signal%287%29)。 |
| **`EIO`**               | 发生了 I/O 错误。                                            |
| **`ENOSPC`， `EDQUOT`** | 在 NFS 上，这些错误通常不会在第一次超过可用存储空间的写入时报告，而是会在随后的 [write(2)](https://man.cx/write%282%29)、[fsync(2)](https://man.cx/fsync%282%29) 或 `close()` 上报告。关于为什么在出错后不应重试 `close()` 的讨论，请参见注意部分。 |

#### 遵循标准
POSIX.1-2001, POSIX.1-2008, SVr4, 4.3BSD。

#### 注意
一次成功的 `close()` 并不能保证数据已成功保存到磁盘，因为内核会使用缓冲区缓存来延迟写入。通常情况下，文件系统在文件关闭时不会刷新缓冲区。如果你需要确保数据已物理存储到底层磁盘上，应使用 [fsync(2)](https://man.cx/fsync%282%29)。（这一步的结果将取决于磁盘硬件。）

可以使用 close-on-exec 文件描述符标志来确保文件描述符在成功的 [execve(2)](https://man.cx/execve%282%29) 时被自动关闭；详情请参见 [fcntl()](#fcntl)。

**多线程进程与 `close()`**：在文件描述符可能被同一进程中的其他线程正在使用的系统调用中时关闭它们，可能并不明智。由于文件描述符可能会被重用，因此存在一些隐蔽的竞争条件，可能导致意外的副作用。此外，考虑以下两个线程对同一个文件描述符执行操作的场景：

1.  一个线程被阻塞在该文件描述符上的 I/O 系统调用中。例如，它正试图向一个已满的管道 [write(2)](https://man.cx/write%282%29)，或者正试图从一个当前没有可用数据的流套接字 [read(2)](https://man.cx/read%282%29)。
2.  另一个线程关闭了该文件描述符。

这种情况下的行为因系统而异。在某些系统上，当文件描述符被关闭时，被阻塞的系统调用会立即返回并报错。在 Linux（可能还有其他一些系统）上，行为则有所不同：被阻塞的 I/O 系统调用会持有对底层打开文件描述的引用，这个引用会保持该文件描述处于打开状态，直到 I/O 系统调用完成。（关于打开文件描述的讨论，请参见 [open(2)](https://man.cx/open%282%29)。）因此，第一个线程中被阻塞的系统调用可能会在第二个线程中的 `close()` 之后成功完成。

**处理 `close()` 的错误返回**：细心的程序员会检查 `close()` 的返回值，因为之前 [write(2)](https://man.cx/write%282%29) 操作中出现的错误，完全有可能仅在最终释放打开文件描述的那次 `close()` 上才被报告出来。如果在关闭文件时未能检查返回值，可能会导致数据的静默丢失。这种情况在 NFS 和磁盘配额限制下尤为常见。

不过需要注意的是，失败的返回值只应用于诊断目的（即向应用程序发出警告：可能仍有 I/O 待处理或可能发生过 I/O 失败）或补救目的（例如，再次写入文件或创建备份）。在失败返回后重试 `close()` 是错误的做法，因为这可能导致来自另一个线程的重用的文件描述符被关闭。

之所以会发生这种情况，是因为 Linux 内核总是在关闭操作的早期就释放文件描述符，以便它能够被重用；而那些可能返回错误的步骤（例如将数据刷新到文件系统或设备）则会在关闭操作的后期才执行。许多其他实现也同样会关闭文件描述符（除了 `EBADF` 的情况，即文件描述符无效），即使它们随后在 `close()` 返回时报告了一个错误。POSIX.1 目前对此保持沉默，但计划在下一个主要版本的该标准中强制执行此行为。

想要获知 I/O 错误的细心的程序员可以在 `close()` 之前调用 [fsync(2)](https://man.cx/fsync%282%29)。

`EINTR` 错误是一个比较特殊的案例。关于 `EINTR` 错误，POSIX.1-2008 规定：如果 `close()` 被一个要捕获的信号中断，它应返回 -1 并将 `errno` 设置为 `EINTR`，而 `fildes` 的状态则未指定。这允许了 Linux 及许多其他实现上的行为：与 `close()` 可能报告的其他错误一样，文件描述符被保证会关闭。然而，它也允许另一种可能性：实现返回一个 `EINTR` 错误，但保持文件描述符处于打开状态。（根据其文档，HP-UX 的 `close()` 就是这样的。）此时调用者必须再次使用 `close()` 来关闭文件描述符，以避免文件描述符泄漏。

这种实现行为上的分歧给可移植的应用程序带来了很大的困扰，因为在许多实现上，在收到 `EINTR` 错误后不能再调用 `close()`，而至少在一个实现上，又必须再次调用 `close()`。下一个主要版本的 POSIX.1 标准已有计划来解决这个难题。

<br/>

### sendmsg

#### 参考

[send(2) - Linux manual page](https://man7.org/linux/man-pages/man2/sendmsg.2.html) / [man.cx manual pages](https://man.cx/sendmsg%282%29)

#### 原型

```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags);

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
        const struct sockaddr *dest_addr, socklen_t addrlen);

ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
```

#### 描述
系统调用 `send()`、`sendto()` 和 `sendmsg()` 用于将消息发送到另一个套接字。`send()` 调用只能在套接字处于连接状态时使用（以便知晓预期的接收者）。`send()` 和 [write(2)](https://man.cx/write%282%29) 的唯一区别在于 `flags` 参数的存在。当 `flags` 参数为零时，`send()` 等同于 [write(2)](https://man.cx/write%282%29)。此外，以下调用：

```c
send(sockfd, buf, len, flags);
```

等同于：

```c
sendto(sockfd, buf, len, flags, NULL, 0);
```

参数 `sockfd` 是发送套接字的文件描述符。

如果在面向连接的套接字（`SOCK_STREAM`，`SOCK_SEQPACKET`）上使用 `sendto()`，则参数 `dest_addr` 和 `addrlen` 将被忽略（若它们不为 NULL 和 0，可能会返回错误 `EISCONN`），并且如果套接字实际上并未连接，将返回错误 `ENOTCONN`。否则，目标地址由 `dest_addr` 给出，其大小由 `addrlen` 指定。

对于 `sendmsg()`，目标地址由 `msg.msg_name` 给出，其大小由 `msg.msg_namelen` 指定。

对于 `send()` 和 `sendto()`，消息位于 `buf` 中，长度为 `len`。对于 `sendmsg()`，消息由数组 `msg.msg_iov` 中的元素指向。`sendmsg()` 调用还允许发送辅助数据（也称为控制信息）。

如果消息太长，无法通过底层协议原子性地传递，则返回错误 `EMSGSIZE`，并且消息不会被发送。`send()` 函数本身并不隐含传递失败的信息。本地检测到的错误通过返回值 -1 指示。

当消息无法装入套接字的发送缓冲区时，`send()` 通常会阻塞，除非套接字已被设置为非阻塞 I/O 模式。在非阻塞模式下，它将失败并返回错误 `EAGAIN` 或 `EWOULDBLOCK`。可以使用 [select(2)](https://man.cx/select%282%29) 调用来确定何时可以发送更多数据。

#### flags 参数
`flags` 参数是零个或多个以下标志的按位或。

| 标志                                    | 描述                                                         |
| :-------------------------------------- | :----------------------------------------------------------- |
| **`MSG_CONFIRM`**（自 Linux 2.3.15 起） | 通知链路层传输已取得进展：你已从对端收到成功回复。如果链路层没有收到此信息，它将定期重新探测邻居。仅对 `SOCK_DGRAM` 和 `SOCK_RAW` 套接字有效，且目前仅针对 IPv4 和 IPv6 实现。详情请参见 [arp(7)](https://man.cx/arp%287%29)。 |
| **`MSG_DONTROUTE`**                     | 不要使用网关发送数据包，仅发送给直接连接的网络上的主机。这通常仅由诊断或路由程序使用。该标志仅为支持路由的协议族定义；数据包套接字不支持。 |
| **`MSG_DONTWAIT`**（自 Linux 2.2 起）   | 启用非阻塞操作；如果操作会阻塞，则返回 `EAGAIN` 或 `EWOULDBLOCK`。这提供了与设置 `O_NONBLOCK` 标志类似的行为（通过 [fcntl()](#fcntl) 的 `F_SETFL` 操作），不同之处在于 `MSG_DONTWAIT` 是每次调用选项，而 `O_NONBLOCK` 是对打开文件描述的设置（参见 [open(2)](https://man.cx/open%282%29)），这将影响调用进程中的所有线程以及引用同一打开文件描述的其他进程。 |
| **`MSG_EOR`**（自 Linux 2.2 起）        | 终止一条记录（仅在支持此概念的套接字上，如 `SOCK_SEQPACKET` 类型的套接字）。 |
| **`MSG_MORE`**（自 Linux 2.4.4 起）     | 调用方有更多数据要发送。此标志用于 TCP 套接字，以获得与 `TCP_CORK` 套接字选项相同的效果（参见 [tcp(7)](https://man.cx/tcp%287%29)），不同之处在于该标志可按每次调用设置。自 Linux 2.6 起，此标志也支持 UDP 套接字，并通知内核将设置了该标志的所有调用中发送的数据打包成一个数据报，仅在未指定此标志的调用执行时才发送。（另请参见 [udp(7)](https://man.cx/udp%287%29) 中描述的 `UDP_CORK` 套接字选项。） |
| **`MSG_NOSIGNAL`**（自 Linux 2.2 起）   | 如果面向流的套接字上的对端已关闭连接，则不生成 `SIGPIPE` 信号。仍返回 `EPIPE` 错误。这提供了与使用 [sigaction(2)](https://man.cx/sigaction%282%29) 忽略 `SIGPIPE` 类似的行为，但 `MSG_NOSIGNAL` 是每次调用的特性，而忽略 `SIGPIPE` 会设置影响进程中所有线程的进程属性。 |
| **`MSG_OOB`**                           | 在支持此概念的套接字（例如 `SOCK_STREAM` 类型）上发送带外数据；底层协议也必须支持带外数据。 |

#### sendmsg()
`sendmsg()` 使用的 `msghdr` 结构定义如下：

```c
struct msghdr {
    void         *msg_name;       /* 可选地址 */
    socklen_t     msg_namelen;    /* 地址大小   */
    struct iovec *msg_iov;        /* 分散/聚集数组 */
    size_t        msg_iovlen;     /* msg_iov 中的元素数 */
    void         *msg_control;    /* 辅助数据，见下文 */
    size_t        msg_controllen; /* 辅助数据缓冲区长度 */
    int           msg_flags;      /* 标志（未使用） */
};
```

`msg_name` 字段用于未连接的套接字，以指定数据报的目标地址。它指向一个包含地址的缓冲区；`msg_namelen` 字段应设置为地址的大小。对于已连接的套接字，这些字段应分别指定为 NULL 和 0。

`msg_iov` 和 `msg_iovlen` 字段指定分散-聚集位置，如 [writev(2)](https://man.cx/writev%282%29) 所述。

你可以使用 `msg_control` 和 `msg_controllen` 成员发送控制信息（辅助数据）。内核可以处理的最大控制缓冲区长度由 `/proc/sys/net/core/optmem_max` 中的值限制；请参见 [socket(7)](https://man.cx/socket%287%29)。有关在各种套接字域中使用辅助数据的更多信息，请参见 [unix(7)](https://man.cx/unix%287%29) 和 [ip(7)](https://man.cx/ip%287%29)。

`msg_flags` 字段被忽略。

#### 返回值
成功时，这些调用返回发送的字节数。出错时，返回 -1，并设置相应的 `errno`。

##### 错误码

以下是一些由套接字层生成的标准错误。底层协议模块还可能生成并返回其他错误（请参阅它们各自的手册页）。

| 错误                          | 描述                                                         |
| :---------------------------- | :----------------------------------------------------------- |
| **`EACCES`**                  | （对于通过路径名标识的 UNIX 域套接字）对目标套接字文件的写权限被拒绝，或者路径前缀的某个组件的搜索权限被拒绝。（请参阅 [path_resolution(7)](https://man.cx/path_resolution%287%29)。）（对于 UDP 套接字）试图向网络/广播地址发送数据，但如同它是单播地址一样。 |
| **`EAGAIN` 或 `EWOULDBLOCK`** | 套接字被标记为非阻塞，并且请求的操作会阻塞。POSIX.1-2001 允许在这种情况下返回这两个错误中的任何一个，并且不要求这些常量具有相同的值，因此可移植的应用程序应同时检查这两种可能性。 |
| **`EAGAIN`**                  | （Internet 域数据报套接字）`sockfd` 引用的套接字之前未绑定到地址，并且在尝试将其绑定到临时端口时，发现临时端口范围内的所有端口号当前都在使用中。参见 [ip(7)](https://man.cx/ip%287%29) 中关于 `/proc/sys/net/ipv4/ip_local_port_range` 的讨论。 |
| **`EALREADY`**                | 另一个 Fast Open 正在进行中。                                |
| **`EBADF`**                   | `sockfd` 不是有效的打开文件描述符。                          |
| **`ECONNRESET`**              | 连接被对端重置。                                             |
| **`EDESTADDRREQ`**            | 套接字不是连接模式，并且未设置对端地址。                     |
| **`EFAULT`**                  | 为参数指定的用户空间地址无效。                               |
| **`EINTR`**                   | 在传输任何数据之前发生了信号；请参见 [signal(7)](https://man.cx/signal%287%29)。 |
| **`EINVAL`**                  | 传递了无效参数。                                             |
| **`EISCONN`**                 | 连接模式的套接字已连接，但指定了接收者。（现在要么返回此错误，要么忽略接收者规范。） |
| **`EMSGSIZE`**                | 套接字类型要求消息原子性地发送，而要发送的消息的大小使得这不可能实现。 |
| **`ENOBUFS`**                 | 网络接口的输出队列已满。这通常表示接口已停止发送，但也可能由瞬时拥塞引起。（通常，这不会在 Linux 中发生。当设备队列溢出时，数据包只是被静默丢弃。） |
| **`ENOMEM`**                  | 没有可用内存。                                               |
| **`ENOTCONN`**                | 套接字未连接，且未指定目标。                                 |
| **`ENOTSOCK`**                | 文件描述符 `sockfd` 不指向套接字。                           |
| **`EOPNOTSUPP`**              | `flags` 参数中的某些位不适用于该套接字类型。                 |
| **`EPIPE`**                   | 面向连接的套接字的本地端已被关闭。在这种情况下，除非设置了 `MSG_NOSIGNAL`，否则进程还将收到 `SIGPIPE` 信号。 |

#### 遵循标准
4.4BSD，SVr4，POSIX.1-2001。这些接口首次出现在 4.2BSD 中。POSIX.1-2001 仅描述了 `MSG_OOB` 和 `MSG_EOR` 标志。POSIX.1-2008 增加了对 `MSG_NOSIGNAL` 的规范。`MSG_CONFIRM` 标志是一个 Linux 扩展。

#### 注意
根据 POSIX.1-2001，`msghdr` 结构的 `msg_controllen` 字段的类型应为 `socklen_t`，`msg_iovlen` 字段的类型应为 `int`，但 glibc 目前将两者都类型化为 `size_t`。关于一个 Linux 特有的系统调用（可用于在单个调用中传输多个数据报）的信息，请参见 [sendmmsg()](#sendmmsg)。

<br/>

### recvmsg

#### 参考

[recv(2) - Linux manual page](https://man7.org/linux/man-pages/man2/recvmsg.2.html) / [man.cx manual pages](https://man.cx/recvmsg%282%29)

#### 原型

```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t recv(int sockfd, void *buf, size_t len, int flags);

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
        struct sockaddr *src_addr, socklen_t *addrlen);

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
```

#### 描述

`recv()`、`recvfrom()` 和 `recvmsg()` 调用用于从套接字接收消息。它们可用于在无连接和面向连接的套接字上接收数据。

`recv()` 和 [read(2)](https://man.cx/read%282%29) 的唯一区别在于 `flags` 参数的存在。当 `flags` 参数为零时，`recv()` 通常等同于 [read(2)](https://man.cx/read%282%29)（但请参见注意部分）。此外，以下调用：

```c
recv(sockfd, buf, len, flags);
```

等同于：

```c
recvfrom(sockfd, buf, len, flags, NULL, NULL);
```

所有三个调用在成功完成时返回消息的长度。如果消息过长而无法放入提供的缓冲区，则根据接收消息的套接字类型，多余的字节可能被丢弃。如果套接字上没有可用的消息，则接收调用会等待消息到达，除非套接字是非阻塞的（参见 [fcntl()](#fcntl)），在这种情况下返回值 -1，并将外部变量 `errno` 设置为 `EAGAIN` 或 `EWOULDBLOCK`。

接收调用通常返回任何可用的数据，最多不超过请求的数量，而不是等待接收完整个请求的数量。应用程序可以使用 [select(2)](https://man.cx/select%282%29)、[poll(2)](https://man.cx/poll%282%29) 或 [epoll(7)](https://man.cx/epoll%287%29) 来确定何时有更多数据到达套接字。

#### flags 参数

`flags` 参数通过将以下一个或多个值进行按位或运算形成：

| 标志                                                         | 描述                                                         |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| **`MSG_CMSG_CLOEXEC`**（仅限 `recvmsg()`；自 Linux 2.6.23 起） | 为通过 UNIX 域文件描述符使用 `SCM_RIGHTS` 操作（在 [unix(7)](https://man.cx/unix%287%29) 中描述）接收到的文件描述符设置 close-on-exec 标志。此标志的用途与 [open(2)](https://man.cx/open%282%29) 的 `O_CLOEXEC` 标志相同。 |
| **`MSG_DONTWAIT`**（自 Linux 2.2 起）                        | 启用非阻塞操作；如果操作会阻塞，则调用失败并返回错误 `EAGAIN` 或 `EWOULDBLOCK`。这提供了与设置 `O_NONBLOCK` 标志（通过 [fcntl()](#fcntl) 的 `F_SETFL` 操作）类似的行为，但区别在于 `MSG_DONTWAIT` 是一个每次调用的选项，而 `O_NONBLOCK` 是打开文件描述的设置（参见 [open(2)](https://man.cx/open%282%29)），它将影响调用进程中的所有线程以及持有引用同一打开文件描述的文件描述符的其他进程。 |
| **`MSG_ERRQUEUE`**（自 Linux 2.2 起）                        | 此标志指定应从套接字错误队列中接收已排队的错误。错误在一个辅助消息中传递，其类型依赖于协议（对于 IPv4 为 `IP_RECVERR`）。用户应提供一个足够大的缓冲区。更多信息请参见 [cmsg(3)](https://man.cx/cmsg%283%29) 和 [ip(7)](https://man.cx/ip%287%29)。导致错误的原始数据包的有效载荷通过 `msg_iovec` 作为正常数据传递。导致错误的数据报的原始目标地址通过 `msg_name` 提供。错误在 `sock_extended_err` 结构中提供：<br><br>`#define SO_EE_ORIGIN_NONE 0`<br>`#define SO_EE_ORIGIN_LOCAL 1`<br>`#define SO_EE_ORIGIN_ICMP 2`<br>`#define SO_EE_ORIGIN_ICMP6 3`<br><br>`struct sock_extended_err {`<br>`    uint32_t ee_errno;   /* 错误号 */`<br>`    uint8_t  ee_origin;  /* 错误来源 */`<br>`    uint8_t  ee_type;    /* 类型 */`<br>`    uint8_t  ee_code;    /* 代码 */`<br>`    uint8_t  ee_pad;     /* 填充 */`<br>`    uint32_t ee_info;    /* 附加信息 */`<br>`    uint32_t ee_data;    /* 其他数据 */`<br>`    /* 后面可能还有更多数据 */`<br>`};`<br><br>`struct sockaddr *SO_EE_OFFENDER(struct sock_extended_err *);`<br><br>`ee_errno` 包含排队错误的 errno 编号。`ee_origin` 是错误来源的代码。其他字段是协议特定的。宏 `SO_EE_OFFENDER` 返回一个指针，指向产生错误的网络对象的地址，该指针基于指向辅助消息的指针。如果此地址未知，则 `sockaddr` 的 `sa_family` 成员包含 `AF_UNSPEC`，并且 `sockaddr` 的其他字段未定义。导致错误的原始数据包的有效载荷作为正常数据传递。对于本地错误，不传递地址（可以通过 `cmsghdr` 的 `cmsg_len` 成员进行检查）。对于错误接收，在 `msghdr` 中设置 `MSG_ERRQUEUE` 标志。在错误被传递后，套接字待处理错误将根据下一个排队错误重新生成，并将在下一个套接字操作时传递。 |
| **`MSG_OOB`**                                                | 此标志请求接收带外数据，这些数据通常不会在正常数据流中接收到。某些协议将加速数据放在正常数据队列的开头，因此此标志不能用于此类协议。 |
| **`MSG_PEEK`**                                               | 此标志使接收操作从接收队列的开头返回数据，但不从队列中移除该数据。因此，后续的接收调用将返回相同的数据。 |
| **`MSG_TRUNC`**（自 Linux 2.2 起）                           | 对于原始（`AF_PACKET`）、Internet 数据报（自 Linux 2.4.27/2.6.8 起）、netlink（自 Linux 2.6.22 起）和 UNIX 数据报（自 Linux 3.4 起）套接字：返回数据包或数据报的实际长度，即使它比传递的缓冲区长。对于与 Internet 流套接字一起使用，请参阅 [tcp(7)](https://man.cx/tcp%287%29)。 |
| **`MSG_WAITALL`**（自 Linux 2.2 起）                         | 此标志请求操作阻塞，直到满足整个请求。但是，如果捕获到信号、发生错误或断开连接，或者要接收的下一个数据与返回的数据类型不同，则调用仍可能返回少于请求的数据。此标志对数据报套接字无效。 |

#### recvfrom()

`recvfrom()` 将接收到的消息放入缓冲区 `buf` 中。调用者必须指定缓冲区的大小 `len`。如果 `src_addr` 不为 `NULL`，并且底层协议提供消息的源地址，则该源地址将被放入 `src_addr` 指向的缓冲区中。在这种情况下，`addrlen` 是一个值-结果参数。在调用之前，应将其初始化为与 `src_addr` 关联的缓冲区的大小。返回时，`addrlen` 将更新为包含源地址的实际大小。如果提供的缓冲区太小，返回的地址将被截断；在这种情况下，`addrlen` 将返回一个大于调用时提供的值。如果调用者不关心源地址，则应将 `src_addr` 和 `addrlen` 指定为 `NULL`。

#### recv()

`recv()` 调用通常仅用于已连接的套接字（参见 [connect()](#connect)）。它等价于以下调用：

```c
recvfrom(fd, buf, len, flags, NULL, 0);
```

#### recvmsg()

`recvmsg()` 调用使用 `msghdr` 结构来最小化直接提供的参数数量。该结构定义如下（在 `<sys/socket.h>` 中）：

```c
struct iovec {                    /* 分散/聚集数组项 */
    void  *iov_base;              /* 起始地址 */
    size_t iov_len;               /* 要传输的字节数 */
};

struct msghdr {
    void         *msg_name;       /* 可选地址 */
    socklen_t     msg_namelen;    /* 地址大小 */
    struct iovec *msg_iov;        /* 分散/聚集数组 */
    size_t        msg_iovlen;     /* msg_iov 中的元素数 */
    void         *msg_control;    /* 辅助数据，见下文 */
    size_t        msg_controllen; /* 辅助数据缓冲区长度 */
    int           msg_flags;      /* 接收到的消息的标志 */
};
```

`msg_name` 字段指向一个调用者分配的缓冲区，用于在套接字未连接时返回源地址。调用者应在本次调用之前将此缓冲区的大小设置为 `msg_namelen`；在成功调用返回时，`msg_namelen` 将包含返回的地址的长度。如果应用程序不需要知道源地址，则可以将 `msg_name` 指定为 `NULL`。

`msg_iov` 和 `msg_iovlen` 字段描述了分散-聚集位置，如 [readv(2)](https://man.cx/readv%282%29) 中所述。

`msg_control` 字段（长度为 `msg_controllen`）指向一个缓冲区，用于其他与协议控制相关的消息或杂项辅助数据。当调用 `recvmsg()` 时，`msg_controllen` 应包含 `msg_control` 中可用缓冲区的长度；在成功调用返回时，它将包含控制消息序列的长度。消息的形式如下：

```c
struct cmsghdr {
    size_t cmsg_len;     /* 数据字节计数，包括头部（在 POSIX 中类型为 socklen_t） */
    int    cmsg_level;   /* 始发协议 */
    int    cmsg_type;    /* 协议特定类型 */
    /* 后面跟着 unsigned char cmsg_data[]; */
};
```

辅助数据应仅通过 [cmsg(3)](https://man.cx/cmsg%283%29) 中定义的宏来访问。例如，Linux 使用此辅助数据机制来传递扩展错误、IP 选项或 UNIX 域套接字上的文件描述符。有关在各种套接字域中使用辅助数据的更多信息，请参见 [unix(7)](https://man.cx/unix%287%29) 和 [ip(7)](https://man.cx/ip%287%29)。

在 `recvmsg()` 返回时，`msghdr` 中的 `msg_flags` 字段会被设置。它可以包含多个标志：

| 标志               | 描述                                                         |
| :----------------- | :----------------------------------------------------------- |
| **`MSG_EOR`**      | 指示记录结束；返回的数据完成了一条记录（通常用于 `SOCK_SEQPACKET` 类型的套接字）。 |
| **`MSG_TRUNC`**    | 指示数据报的尾部被丢弃，因为数据报大于提供的缓冲区。         |
| **`MSG_CTRUNC`**   | 指示由于辅助数据缓冲区空间不足，某些控制数据被丢弃。         |
| **`MSG_OOB`**      | 返回以指示接收到加速或带外数据。                             |
| **`MSG_ERRQUEUE`** | 指示未接收到数据，但收到了来自套接字错误队列的扩展错误。     |

#### 返回值

这些调用返回接收到的字节数，如果发生错误则返回 -1。在发生错误时，`errno` 被设置为指示错误。当流套接字对端执行了有序关闭时，返回值将为 0（传统的“文件结束”返回）。各种域（例如 UNIX 和 Internet 域）中的数据报套接字允许零长度数据报。当接收到此类数据报时，返回值为 0。如果从流套接字请求接收的字节数为 0，也可能返回值 0。

##### 错误码

以下是一些由套接字层生成的标准错误。底层协议模块可能生成并返回其他错误（请参阅它们的手册页）。

| 错误                          | 描述                                                         |
| :---------------------------- | :----------------------------------------------------------- |
| **`EAGAIN` 或 `EWOULDBLOCK`** | 套接字被标记为非阻塞并且接收操作会阻塞，或者已设置接收超时并且在接收到数据之前超时已过期。POSIX.1 允许在这种情况下返回任一错误，并且不要求这些常量具有相同的值，因此可移植的应用程序应同时检查两种可能性。 |
| **`EBADF`**                   | 参数 `sockfd` 是一个无效的文件描述符。                       |
| **`ECONNREFUSED`**            | 远程主机拒绝允许网络连接（通常是因为它没有运行请求的服务）。 |
| **`EFAULT`**                  | 接收缓冲区指针指向进程地址空间之外。                         |
| **`EINTR`**                   | 在任何数据可用之前，接收被信号的传递中断；请参见 [signal(7)](https://man.cx/signal%287%29)。 |
| **`EINVAL`**                  | 传递了无效参数。                                             |
| **`ENOMEM`**                  | 无法为 `recvmsg()` 分配内存。                                |
| **`ENOTCONN`**                | 套接字与面向连接的协议关联，并且尚未连接（请参见 [connect()](#connect) 和 [accept()](#accept)）。 |
| **`ENOTSOCK`**                | 文件描述符 `sockfd` 不指向套接字。                           |

#### 遵循标准

POSIX.1-2001, POSIX.1-2008, 4.4BSD（这些接口首次出现在 4.2BSD 中）。POSIX.1 仅描述了 `MSG_OOB`、`MSG_PEEK` 和 `MSG_WAITALL` 标志。

#### 注意

如果有一个零长度数据报待处理，则 [read(2)](https://man.cx/read%282%29) 和 `flags` 参数为零的 `recv()` 提供不同的行为。在这种情况下，[read(2)](https://man.cx/read%282%29) 无效（数据报仍然待处理），而 `recv()` 会消耗待处理的数据报。

`socklen_t` 类型由 POSIX 发明。另请参见 [accept()](#accept)。

根据 POSIX.1，`msghdr` 结构的 `msg_controllen` 字段应类型化为 `socklen_t`，`msg_iovlen` 字段应类型化为 `int`，但 glibc 目前将两者都类型化为 `size_t`。

有关可用于在单个调用中接收多个数据报的 Linux 特有系统调用的信息，请参见 [recvmmsg()](#recvmmsg)。

<br/>

### sendmmsg

#### 参考

[sendmmsg(2) - Linux manual page](https://man7.org/linux/man-pages/man2/sendmmsg.2.html) / [man.cx manual pages](https://man.cx/sendmmsg%282%29)

#### 原型

```c
#define _GNU_SOURCE /* See feature_test_macros(7) */
#include <sys/socket.h>

int sendmmsg(int sockfd, struct mmsghdr *msgvec, unsigned int vlen, int flags);
```

#### 描述

`sendmmsg()` 系统调用是 [sendmsg()](#sendmsg) 的扩展，允许调用者使用一个系统调用在套接字上发送多个消息。（这对某些应用程序具有性能优势。）

参数 `sockfd` 是用于发送数据的套接字的文件描述符。

参数 `msgvec` 是一个指向 `mmsghdr` 结构体数组的指针。该数组的大小由 `vlen` 指定。

`mmsghdr` 结构体的定义如下：

```c
struct mmsghdr {
    struct msghdr msg_hdr;  /* 消息头 */
    unsigned int  msg_len;  /* 已传输的字节数 */
};
```

`msg_hdr` 字段是一个 `msghdr` 结构体，其描述参见 [sendmsg()](#sendmsg)。`msg_len` 字段用于返回从 `msg_hdr` 中消息发送的字节数（即与单次 [sendmsg()](#sendmsg) 调用的返回值相同）。

参数 `flags` 包含通过 OR 运算组合的标志。这些标志与 [sendmsg()](#sendmsg) 的相同。

阻塞式的 `sendmmsg()` 调用会一直阻塞，直到发送完 `vlen` 条消息。非阻塞式调用则会尽可能多地发送消息（最多不超过 `vlen` 指定的数量），然后立即返回。

从 `sendmmsg()` 返回时，`msgvec` 中各元素的 `msg_len` 字段会被更新，以包含从相应 `msg_hdr` 传输的字节数。该调用的返回值指示了 `msgvec` 中已被更新的元素数量。

#### 返回值

成功时，`sendmmsg()` 返回 `msgvec` 中已发送的消息数量；如果该值小于 `vlen`，调用者可以再次调用 `sendmmsg()` 重试发送剩余的消息。出错时，返回 -1，并设置相应的 `errno`。

##### 错误码

错误原因与 [sendmsg()](#sendmsg) 相同。仅当没有任何数据报可以发送时，才会返回错误。另请参见下文 **BUGS** 部分。

#### 版本

`sendmmsg()` 系统调用在 Linux 3.0 中添加。glibc 的支持从版本 2.14 开始添加。

#### 遵循标准

`sendmmsg()` 是 Linux 特有的。

#### 备注

`vlen` 中指定的值会被限制为 `UIO_MAXIOV`（即 1024）。

#### BUGS

如果在至少一条消息已发送之后发生错误，该调用仍会成功，并返回已发送的消息数量。此时错误码会丢失。调用者可以重试传输，从第一条失败的消息开始，但无法保证如果再次返回错误，该错误码与先前丢失的错误码相同。

#### 示例

下面的示例使用 `sendmmsg()` 通过一次系统调用在两个不同的 UDP 数据报中发送 `"one two"` 和 `"three"`。第一个数据报的内容来自一对缓冲区。

```c
#define _GNU_SOURCE
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    int sockfd;
    struct sockaddr_in addr;
    struct mmsghdr msg[2];
    struct iovec msg1[2], msg2;
    int retval;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(1234);
    if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("connect()");
        exit(EXIT_FAILURE);
    }

    memset(msg1, 0, sizeof(msg1));
    msg1[0].iov_base = "one";
    msg1[0].iov_len = 3;
    msg1[1].iov_base = "two";
    msg1[1].iov_len = 3;

    memset(&msg2, 0, sizeof(msg2));
    msg2.iov_base = "three";
    msg2.iov_len = 5;

    memset(msg, 0, sizeof(msg));
    msg[0].msg_hdr.msg_iov = msg1;
    msg[0].msg_hdr.msg_iovlen = 2;
    msg[1].msg_hdr.msg_iov = &msg2;
    msg[1].msg_hdr.msg_iovlen = 1;

    retval = sendmmsg(sockfd, msg, 2, 0);
    if (retval == -1)
        perror("sendmmsg()");
    else
        printf("%d messages sent\n", retval);

    exit(0);
}
```

<br/>

### recvmmsg

#### 参考

[recvmmsg(2) - Linux manual page](https://man7.org/linux/man-pages/man2/recvmmsg.2.html) / [man.cx manual pages](https://man.cx/recvmmsg%282%29)

#### 原型

```c
#define _GNU_SOURCE /* See feature_test_macros(7) */
#include <sys/socket.h>

int recvmmsg(int sockfd, struct mmsghdr *msgvec, unsigned int vlen, int flags, struct timespec *timeout);
```

#### 描述
`recvmmsg()` 系统调用是 [recvmsg()](#recvmsg) 的扩展，允许调用者通过一次系统调用从套接字接收多个报文（这对于某些应用程序具有性能优势）。相比于 [recvmsg()](#recvmsg)，进一步的扩展是支持接收操作超时。

参数 `sockfd` 是用于接收数据的套接字的文件描述符。

参数 `msgvec` 是指向 `mmsghdr` 结构体数组的指针。该数组的大小由 `vlen` 指定。

`mmsghdr` 结构体在 `<sys/socket.h>` 中定义为：

```c
struct mmsghdr {
    struct msghdr msg_hdr;      /* 消息头 */
    unsigned int  msg_len;      /* 为此消息接收的字节数 */
};
```

`msg_hdr` 字段是一个 `msghdr` 结构体，详见 [recvmsg()](#recvmsg)。`msg_len` 字段是为该条目的消息返回的字节数，该值与在此消息头上单独调用 [recvmsg()](#recvmsg) 的返回值相同。

参数 `flags` 包含通过 OR 运算组合的标志。标志与 [recvmsg()](#recvmsg) 中记录的相同，并增加了以下项：

*   **`MSG_WAITFORONE`**（自 Linux 2.6.34 起）：接收到第一条消息后开启 `MSG_DONTWAIT`。

参数 `timeout` 指向一个 `struct timespec`（参见 [clock_gettime(2)](https://man.cx/clock_gettime%282%29)），用于定义接收操作的超时时间（秒加纳秒，但请参见 **BUGS**）。（此间隔将向上舍入到系统时钟粒度，且内核调度延迟可能导致阻塞时间略有超出。）如果 `timeout` 为 `NULL`，则操作将无限期阻塞。

阻塞式 `recvmmsg()` 调用会阻塞，直到接收到 `vlen` 条消息或超时到期为止。非阻塞式调用则立即读取尽可能多的消息（最多不超过 `vlen` 指定的限制）。

从 `recvmmsg()` 返回时，`msgvec` 中相继的元素会被更新，以包含关于每条已接收消息的信息：`msg_len` 包含已接收消息的大小；`msg_hdr` 的子字段会如 [recvmsg()](#recvmsg) 所述进行更新。调用的返回值指示了 `msgvec` 中已被更新的元素个数。

#### 返回值
成功时，`recvmmsg()` 返回 `msgvec` 中接收到的消息数量；发生错误时，返回 -1，并设置相应的 `errno`。

##### 错误码

错误类型与 [recvmsg](#recvmsg) 相同。此外还可能发生以下错误：

*   **`EINVAL`**：`timeout` 无效。另请参见 **BUGS**。

#### 版本
`recvmmsg()` 系统调用在 Linux 2.6.33 中加入。glibc 的支持从版本 2.12 开始。

#### 遵循标准
`recvmmsg()` 是 Linux 特有的。

#### 缺陷
`timeout` 参数的行为与预期不符。`timeout` 仅在接收到每个数据报之后才会被检查，因此如果在超时到期前接收了至多 `vlen-1` 个数据报，之后再也没有收到更多数据报，则该调用将永远阻塞。

如果在至少一条消息已被接收之后发生了错误，该调用仍会成功，并返回已接收的消息数量。调用者期望在后续对 `recvmmsg()` 的调用中返回该错误码。然而在当前实现中，在此期间该错误码可能被某个不相关的网络事件（例如一个传入的 ICMP 报文）所覆盖。

#### 示例
以下程序使用 `recvmmsg()` 在一个套接字上接收多个报文并将它们存储到多个缓冲区中。当所有缓冲区填满或指定的超时到期时，调用返回。

以下片段会定期生成一个包含随机数的 UDP 数据报：

```bash
$ while true; do echo $RANDOM > /dev/udp/127.0.0.1/1234; sleep 0.25; done
```

这些数据报由示例应用程序读取，可能产生如下输出：

```
$ ./a.out
5 messages received
1 11782
2 11345
3 304
4 13514
5 28421
```

##### 程序源码

```c
#define _GNU_SOURCE
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int main(void)
{
#define VLEN 10
#define BUFSIZE 200
#define TIMEOUT 1
    int sockfd, retval;
    struct sockaddr_in addr;
    struct mmsghdr msgs[VLEN];
    struct iovec iovecs[VLEN];
    char bufs[VLEN][BUFSIZE+1];
    struct timespec timeout;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(1234);
    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    memset(msgs, 0, sizeof(msgs));
    for (int i = 0; i < VLEN; i++) {
        iovecs[i].iov_base = bufs[i];
        iovecs[i].iov_len = BUFSIZE;
        msgs[i].msg_hdr.msg_iov = &iovecs[i];
        msgs[i].msg_hdr.msg_iovlen = 1;
    }

    timeout.tv_sec = TIMEOUT;
    timeout.tv_nsec = 0;

    retval = recvmmsg(sockfd, msgs, VLEN, 0, &timeout);
    if (retval == -1) {
        perror("recvmmsg()");
        exit(EXIT_FAILURE);
    }

    printf("%d messages received\n", retval);
    for (int i = 0; i < retval; i++) {
        bufs[i][msgs[i].msg_len] = 0;
        printf("%d %s", i+1, bufs[i]);
    }
    exit(EXIT_SUCCESS);
}
```

<br/>

### inet_pton

#### 参考

[inet_pton(3) - Linux manual page](https://man7.org/linux/man-pages/man3/inet_pton.3.html) / [man.cx manual pages](https://man.cx/inet_pton%283%29)

#### 原型

```c
#include <arpa/inet.h>

int inet_pton(int af, const char *src, void *dst);
```

#### 描述

该函数将字符串 `src` 转换为地址族 `af` 下的网络地址结构体，然后将该网络地址结构体复制到 `dst`。参数 `af` 必须是 `AF_INET` 或 `AF_INET6`。`dst` 以网络字节序写入。

当前支持以下地址族：

**AF_INET**：`src` 指向一个包含点分十进制格式 IPv4 网络地址的字符串，格式为 `"ddd.ddd.ddd.ddd"`，其中 `ddd` 是一个最多三位、取值范围 0 到 255 的十进制数。该地址被转换为 `struct in_addr` 类型并复制到 `dst`，`dst` 的长度必须为 `sizeof(struct in_addr)`（4 字节/32 位）。

**AF_INET6**：`src` 指向一个包含 IPv6 网络地址的字符串。该地址被转换为 `struct in6_addr` 类型并复制到 `dst`，`dst` 的长度必须为 `sizeof(struct in6_addr)`（16 字节/128 位）。

IPv6 地址允许的格式遵循以下规则：

1. 首选格式为 `x:x:x:x:x:x:x:x`。这种形式由八个十六进制数组成，每个数表示一个 16 位的值（即每个 `x` 最多可以是 4 个十六进制数字）。
2. 首选格式中连续的一组零可以缩写为 `::`。一个地址中只能出现一个 `::`。例如，环回地址 `0:0:0:0:0:0:0:1` 可以缩写为 `::1`。全零的通配符地址可以写为 `::`。
3. 另一种格式用于表示 IPv4 映射的 IPv6 地址。这种形式写作 `x:x:x:x:x:x:d.d.d.d`，其中前六个 `x` 是十六进制值，定义地址的前 96 位（即六个最高有效的 16 位段），而 `d` 以点分十进制表示法表示地址的最低有效 32 位。此类地址的示例为 `::FFFF:204.152.189.116`。

有关 IPv6 地址表示形式的更多详细信息，请参见 RFC 2373。

#### 返回值

`inet_pton()` 成功时返回 1（网络地址已成功转换）。如果 `src` 不包含指定地址族下的有效网络地址字符串，则返回 0。如果 `af` 不包含有效的地址族，则返回 -1，并将 `errno` 设置为 `EAFNOSUPPORT`。

##### 属性

有关本节所用术语的解释，请参见 [attributes(7)](https://man.cx/attributes%287%29)。

#### 遵循标准

POSIX.1-2001、POSIX.1-2008。

#### 备注

与 [inet_aton(3)](https://man.cx/inet_aton%283%29) 和 [inet_addr(3)](https://man.cx/inet_addr%283%29) 不同，`inet_pton()` 支持 IPv6 地址。另一方面，`inet_pton()` 仅接受点分十进制表示法的 IPv4 地址，而 [inet_aton(3)](https://man.cx/inet_aton%283%29) 和 [inet_addr(3)](https://man.cx/inet_addr%283%29) 允许更通用的数字与点表示法（十六进制和八进制数字格式，以及不需要显式写出全部四个字节的格式）。如需同时处理 IPv6 地址和数字与点表示法的 IPv4 地址的接口，请参见 [getaddrinfo()](#getaddrinfo)。

#### 缺陷

`AF_INET6` 不能识别 IPv4 地址。需要在 `AF_INET6` 族中传递 IPv4 映射的 IPv6 地址的显式格式才能完成转换。

<br/>

### inet_ntop

#### 参考

[inet_ntop(3) - Linux manual page](https://man7.org/linux/man-pages/man3/inet_ntop.3.html) / [man.cx manual pages](https://man.cx/inet_ntop%283%29)

#### 原型

```c
#include <arpa/inet.h>

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
```

#### 描述

此函数将地址族 *af* 中的网络地址结构体 *src* 转换为字符串。结果字符串被复制到 *dst* 指向的缓冲区中，该指针必须非空。调用者通过参数 *size* 指定此缓冲区可用的字节数。

`inet_ntop()` 扩展了 [inet_ntoa(3)](https://man.cx/inet_ntoa%283%29) 函数以支持多种地址族，[inet_ntoa(3)](https://man.cx/inet_ntoa%283%29) 现在已被认为是不推荐使用的，建议改用 `inet_ntop()`。

当前支持以下地址族：

- **AF_INET**
  *src* 指向一个 `struct in_addr`（网络字节序），它将被转换为点分十进制格式的 IPv4 网络地址，格式为 "`ddd.ddd.ddd.ddd`"。缓冲区 *dst* 的长度必须至少为 **INET_ADDRSTRLEN** 字节。
- **AF_INET6**
  *src* 指向一个 `struct in6_addr`（网络字节序），它将被转换为该地址最合适的 IPv6 网络地址表示形式。缓冲区 *dst* 的长度必须至少为 **INET6_ADDRSTRLEN** 字节。

#### 返回值

成功时，`inet_ntop()` 返回一个指向 *dst* 的非空指针。如果发生错误，则返回 NULL，并设置相应的 `errno`。

##### 错误码

- **EAFNOSUPPORT**
  *af* 不是有效的地址族。
- **ENOSPC**
  转换后的地址字符串将超过 *size* 指定的大小。

#### 属性

有关本节所用术语的解释，请参见 [attributes(7)](https://man.cx/attributes%287%29)。

#### 遵循标准

POSIX.1-2001、POSIX.1-2008。请注意，RFC 2553 定义了一个原型，其中最后一个参数 *size* 的类型为 `size_t`。许多系统遵循 RFC 2553。Glibc 2.0 和 2.1 使用 `size_t`，但 2.2 及更高版本使用 `socklen_t`。

#### 缺陷

**AF_INET6** 会将 IPv4 映射的 IPv6 地址转换为 IPv6 格式。

<br/>

### htons

#### 参考

[byteorder(3) - Linux manual page](https://man7.org/linux/man-pages/man3/htons.3.html) / [man.cx manual pages](https://man.cx/htons%283%29)

#### 原型

```c
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong);

uint16_t htons(uint16_t hostshort);

uint32_t ntohl(uint32_t netlong);

uint16_t ntohs(uint16_t netshort);
```

#### 描述

`htonl()` 函数将无符号整数 `hostlong` 从主机字节序转换为网络字节序。

`htons()` 函数将无符号短整数 `hostshort` 从主机字节序转换为网络字节序。

`ntohl()` 函数将无符号整数 `netlong` 从网络字节序转换为主机字节序。

`ntohs()` 函数将无符号短整数 `netshort` 从网络字节序转换为主机字节序。

在 i386 架构中，主机字节序为小端序（最低有效字节在前），而 TCP/IP 协议栈所使用的网络字节序为大端序（最高有效字节在前）。

#### 属性

有关本节所用术语的解释，请参见 [attributes(7)](https://man.cx/attributes%287%29)。

#### 遵循标准

POSIX.1-2001，POSIX.1-2008。某些系统要求包含 `<netinet/in.h>` 而不是 `<arpa/inet.h>`。

<br/>

### getaddrinfo

#### 参考

[getaddrinfo(3) - Linux manual page](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html) / [man.cx manual pages](https://man.cx/getaddrinfo%283%29)

#### 原型

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *node, const char *service,
const struct addrinfo *hints,
struct addrinfo **res);

void freeaddrinfo(struct addrinfo *res);

const char *gai_strerror(int errcode);
```

glibc 的功能测试宏要求（参见 [feature_test_macros(7)](https://man.cx/feature_test_macros%287%29)）：

`getaddrinfo()`、`freeaddrinfo()`、`gai_strerror()`：
- glibc 2.22 及更高版本：`_POSIX_C_SOURCE >= 200112L`
- glibc 2.21 及更早版本：`_POSIX_C_SOURCE`

#### 描述

给定 `node` 和 `service`（分别标识互联网主机和服务），`getaddrinfo()` 返回一个或多个 `addrinfo` 结构体，每个结构体包含一个可在调用 [bind()](#bind) 或 [connect()](#connect) 时使用的互联网地址。`getaddrinfo()` 函数将 [gethostbyname(3)](https://man.cx/gethostbyname%283%29) 和 [getservbyname(3)](https://man.cx/getservbyname%283%29) 函数的功能合并到一个接口中，但与后者不同的是，`getaddrinfo()` 是可重入的，并且允许程序消除 IPv4 与 IPv6 的依赖性。

`getaddrinfo()` 使用的 `addrinfo` 结构体包含以下字段：

```c
struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    socklen_t        ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
};
```

`hints` 参数指向一个 `addrinfo` 结构体，该结构体指定了选择 `res` 所指向列表中返回的套接字地址结构的条件。如果 `hints` 不为 `NULL`，则它指向一个 `addrinfo` 结构体，其 `ai_family`、`ai_socktype` 和 `ai_protocol` 字段指定了限制 `getaddrinfo()` 返回的套接字地址集合的条件，具体如下：

- **`ai_family`**
  此字段指定所需返回的地址族。此字段的有效值包括 `AF_INET` 和 `AF_INET6`。值 `AF_UNSPEC` 表示 `getaddrinfo()` 应返回可与 `node` 和 `service` 一起使用的任何地址族（例如 IPv4 或 IPv6）的套接字地址。

- **`ai_socktype`**
  此字段指定首选的套接字类型，例如 `SOCK_STREAM` 或 `SOCK_DGRAM`。在此字段中指定 0 表示 `getaddrinfo()` 可以返回任何类型的套接字地址。

- **`ai_protocol`**
  此字段指定返回的套接字地址的协议。在此字段中指定 0 表示 `getaddrinfo()` 可以返回任何协议的套接字地址。

- **`ai_flags`**
  此字段指定附加选项，如下所述。多个标志通过按位 OR 组合在一起。`hints` 所指向结构体中的所有其他字段必须包含 0 或空指针（视情况而定）。将 `hints` 指定为 `NULL` 等同于将 `ai_socktype` 和 `ai_protocol` 设为 0；`ai_family` 设为 `AF_UNSPEC`；`ai_flags` 设为 `AI_V4MAPPED | AI_ADDRCONFIG`。（POSIX 为 `ai_flags` 指定了不同的默认值；参见 NOTES。）

`node` 指定一个数字网络地址（对于 IPv4，为 [inet_aton(3)](https://man.cx/inet_aton%283%29) 支持的点分十进制表示法；对于 IPv6，为 [inet_pton()](#inet_pton) 支持的十六进制字符串格式），或一个网络主机名，其网络地址会被查找和解析。

如果 `hints.ai_flags` 包含 `AI_NUMERICHOST` 标志，则 `node` 必须是一个数字网络地址。`AI_NUMERICHOST` 标志会抑制任何可能耗时的网络主机地址查找。

如果在 `hints.ai_flags` 中指定了 `AI_PASSIVE` 标志，并且 `node` 为 `NULL`，则返回的套接字地址将适用于 [bind()](#bind) 一个将 [accept()](#accept) 连接的套接字。返回的套接字地址将包含“通配地址”（对于 IPv4 地址为 `INADDR_ANY`，对于 IPv6 地址为 `IN6ADDR_ANY_INIT`）。通配地址用于期望接受主机任何网络地址连接的应用程序（通常是服务器）。如果 `node` 不为 `NULL`，则忽略 `AI_PASSIVE` 标志。

如果 `hints.ai_flags` 中未设置 `AI_PASSIVE` 标志，则返回的套接字地址将适用于 [connect()](#connect)、[sendto()](#sendmsg) 或 [sendmsg()](#sendmsg)。如果 `node` 为 `NULL`，则网络地址将设置为环回接口地址（对于 IPv4 地址为 `INADDR_LOOPBACK`，对于 IPv6 地址为 `IN6ADDR_LOOPBACK_INIT`）；这用于意图与同一主机上运行的对等方通信的应用程序。

`service` 设置每个返回地址结构中的端口号。如果此参数是服务名称（参见 [services(5)](https://man.cx/services%285%29)），则将其转换为相应的端口号。此参数也可以指定为十进制数字，然后直接转换为二进制。如果 `service` 为 `NULL`，则返回的套接字地址的端口号将保持未初始化状态。

如果在 `hints.ai_flags` 中指定了 `AI_NUMERICSERV` 且 `service` 不为 `NULL`，则 `service` 必须指向包含数字端口号的字符串。此标志用于在已知不需要调用名称解析服务的情况下抑制该调用。`node` 或 `service` 可以有一个为 `NULL`，但不能同时为 `NULL`。

`getaddrinfo()` 函数分配并初始化一个 `addrinfo` 结构体的链表，链表中每个结构体对应于一个与 `node` 和 `service` 匹配的网络地址，并受 `hints` 施加的任何限制，然后将指向链表开头的指针返回到 `res` 中。链表中的项通过 `ai_next` 字段链接。

链表可能有多个 `addrinfo` 结构体的原因有多种，包括：网络主机是多宿主的，可通过多种协议（例如同时支持 `AF_INET` 和 `AF_INET6`）访问；或者同一服务可通过多种套接字类型（例如一个 `SOCK_STREAM` 地址和另一个 `SOCK_DGRAM` 地址）获得。通常，应用程序应尝试按照返回的顺序使用这些地址。`getaddrinfo()` 内部使用的排序函数在 RFC 3484 中定义；对于特定系统，可以通过编辑 `/etc/gai.conf`（自 glibc 2.5 起可用）来调整顺序。

如果 `hints.ai_flags` 包含 `AI_CANONNAME` 标志，则返回列表中第一个 `addrinfo` 结构体的 `ai_canonname` 字段将被设置为指向主机的官方名称。

返回的每个 `addrinfo` 结构体的其余字段初始化如下：

- `ai_family`、`ai_socktype` 和 `ai_protocol` 字段返回套接字创建参数（即，这些字段的含义与 [socket()](#socket) 的相应参数相同）。例如，`ai_family` 可能返回 `AF_INET` 或 `AF_INET6`；`ai_socktype` 可能返回 `SOCK_DGRAM` 或 `SOCK_STREAM`；`ai_protocol` 返回套接字的协议。
- 指向套接字地址的指针放在 `ai_addr` 字段中，套接字地址的长度（以字节为单位）放在 `ai_addrlen` 字段中。

如果 `hints.ai_flags` 包含 `AI_ADDRCONFIG` 标志，则仅当本地系统至少配置了一个 IPv4 地址时，`res` 指向的列表中才会返回 IPv4 地址；仅当本地系统至少配置了一个 IPv6 地址时，才会返回 IPv6 地址。环回地址在这种情况下不被视为有效的已配置地址。此标志在例如纯 IPv4 系统上很有用，可确保 `getaddrinfo()` 不返回那些在 [connect()](#connect) 或 [bind()](#bind) 中总是会失败的 IPv6 套接字地址。

如果 `hints.ai_flags` 指定了 `AI_V4MAPPED` 标志，并且 `hints.ai_family` 被指定为 `AF_INET6`，且找不到匹配的 IPv6 地址，则在 `res` 指向的列表中返回 IPv4 映射的 IPv6 地址。如果在 `hints.ai_flags` 中同时指定了 `AI_V4MAPPED` 和 `AI_ALL`，则在 `res` 指向的列表中同时返回 IPv6 地址和 IPv4 映射的 IPv6 地址。如果未同时指定 `AI_V4MAPPED`，则忽略 `AI_ALL`。

`freeaddrinfo()` 函数释放为动态分配的链表 `res` 所分配的内存。

##### 针对国际化域名的扩展

从 glibc 2.3.4 开始，`getaddrinfo()` 已得到扩展，可选择允许将传入和传出的主机名透明地转换为国际化域名（IDN）格式或从中转换回来（参见 RFC 3490《应用程序中的域名国际化》（IDNA））。定义了四个新标志：

- **`AI_IDN`** 
  如果指定了此标志，则 `node` 中给出的节点名称会在必要时转换为 IDN 格式。源编码是当前语言环境的编码。如果输入名称包含非 ASCII 字符，则使用 IDN 编码。节点名称中包含非 ASCII 字符的部分（以点分隔）将在传递给名称解析函数之前使用 ASCII 兼容编码（ACE）进行编码。

- **`AI_CANONIDN`** 
  在成功进行名称查找后，如果指定了 `AI_CANONNAME` 标志，`getaddrinfo()` 将返回与传回的 `addrinfo` 结构体值对应的节点的规范名称。返回值是名称解析函数返回值的精确副本。如果名称使用 ACE 编码，则名称的一个或多个组成部分将包含 `xn--` 前缀。要这些组成部分转换为可读形式，可以在 `AI_CANONNAME` 之外再传递 `AI_CANONIDN` 标志。结果字符串使用当前语言环境的编码进行编码。

- **`AI_IDN_ALLOW_UNASSIGNED`、`AI_IDN_USE_STD3_ASCII_RULES`** 
  设置这些标志将分别启用 IDNA 处理中使用的 `IDNA_ALLOW_UNASSIGNED`（允许未分配的 Unicode 码位）和 `IDNA_USE_STD3_ASCII_RULES`（检查输出以确保其符合 STD3 规范的主机名）标志。

#### 返回值

`getaddrinfo()` 成功时返回 0，失败时返回 -1，并设置相应的 `errno`。

##### 错误码

- **`EAI_ADDRFAMILY`**：指定的网络主机在所请求的地址族中没有网络地址。
- **`EAI_AGAIN`**：名称服务器返回临时故障指示。请稍后重试。
- **`EAI_BADFLAGS`**：`hints.ai_flags` 包含无效标志；或者 `hints.ai_flags` 包含 `AI_CANONNAME` 但 `name` 为 `NULL`。
- **`EAI_FAIL`**：名称服务器返回永久性故障指示。
- **`EAI_FAMILY`**：请求的地址族不受支持。
- **`EAI_MEMORY`**：内存不足。
- **`EAI_NODATA`**：指定的网络主机存在，但没有定义任何网络地址。
- **`EAI_NONAME`**：`node` 或 `service` 未知；或者 `node` 和 `service` 均为 `NULL`；或者在 `hints.ai_flags` 中指定了 `AI_NUMERICSERV` 但 `service` 不是数字端口号字符串。
- **`EAI_SERVICE`**：请求的服务不适用于所请求的套接字类型。可能通过另一种套接字类型可用。例如，如果 `service` 是 `"shell"`（一种仅在流套接字上可用的服务），而 `hints.ai_protocol` 是 `IPPROTO_UDP`，或者 `hints.ai_socktype` 是 `SOCK_DGRAM`，则可能发生此错误；或者如果 `service` 不为 `NULL`，而 `hints.ai_socktype` 是 `SOCK_RAW`（一种不支持服务概念的套接字类型），也可能发生此错误。
- **`EAI_SOCKTYPE`**：请求的套接字类型不受支持。例如，如果 `hints.ai_socktype` 和 `hints.ai_protocol` 不一致（例如分别是 `SOCK_DGRAM` 和 `IPPROTO_TCP`），则可能发生此错误。
- **`EAI_SYSTEM`**：其他系统错误，请检查 `errno` 获取详细信息。

`gai_strerror()` 函数将这些错误代码转换为人类可读的字符串，适用于错误报告。

#### 文件

- `/etc/gai.conf`

#### 属性

有关本节所用术语的解释，请参见 [attributes(7)](https://man.cx/attributes%287%29)。

#### 遵循标准

POSIX.1-2001，POSIX.1-2008。`getaddrinfo()` 函数在 RFC 2553 中有文档说明。

#### 备注

`getaddrinfo()` 支持 `address%scope-id` 表示法，用于指定 IPv6 范围 ID。

`AI_ADDRCONFIG`、`AI_ALL` 和 `AI_V4MAPPED` 自 glibc 2.3.3 起可用。`AI_NUMERICSERV` 自 glibc 2.3.4 起可用。

根据 POSIX.1，将 `hints` 指定为 `NULL` 应使 `ai_flags` 被假定为 0。GNU C 库在这种情况下假定值为 `(AI_V4MAPPED | AI_ADDRCONFIG)`，因为此值被认为是对规范的改进。

#### 示例

以下程序演示了 `getaddrinfo()`、`gai_strerror()`、`freeaddrinfo()` 和 [getnameinfo(3)](https://man.cx/getnameinfo%283%29) 的使用。这些程序是 UDP 数据报的 echo 服务器和客户端。

##### 服务器程序

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 500

int main(int argc, char *argv[]) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* 允许 IPv4 或 IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* 数据报套接字 */
    hints.ai_flags = AI_PASSIVE;    /* 通配 IP 地址 */
    hints.ai_protocol = 0;          /* 任何协议 */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() 返回一个地址结构体列表。
       尝试每个地址，直到我们成功 [bind()](#bind)。
       如果 [socket()](#socket)（或 [bind()](#bind)）失败，
       我们（关闭套接字并）尝试下一个地址。 */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  /* 成功 */

        close(sfd);
    }

    freeaddrinfo(result);           /* 不再需要 */

    if (rp == NULL) {               /* 没有地址成功 */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    /* 读取数据报并将其回送给发送方 */
    for (;;) {
        peer_addr_len = sizeof(peer_addr);
        nread = recvfrom(sfd, buf, BUF_SIZE, 0,
                         (struct sockaddr *) &peer_addr, &peer_addr_len);
        if (nread == -1)
            continue;               /* 忽略失败的请求 */

        char host[NI_MAXHOST], service[NI_MAXSERV];
        s = getnameinfo((struct sockaddr *) &peer_addr,
                        peer_addr_len, host, NI_MAXHOST,
                        service, NI_MAXSERV, NI_NUMERICSERV);
        if (s == 0)
            printf("Received %zd bytes from %s:%s\n", nread, host, service);
        else
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));

        if (sendto(sfd, buf, nread, 0,
                   (struct sockaddr *) &peer_addr, peer_addr_len) != nread)
            fprintf(stderr, "Error sending response\n");
    }
}
```

##### 客户端程序

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 500

int main(int argc, char *argv[]) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* 获取匹配 host/port 的地址 */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* 允许 IPv4 或 IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* 数据报套接字 */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* 任何协议 */

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() 返回一个地址结构体列表。
       尝试每个地址，直到我们成功 [connect()](#connect)。
       如果 [socket()](#socket)（或 [connect()](#connect)）失败，
       我们（关闭套接字并）尝试下一个地址。 */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* 成功 */

        close(sfd);
    }

    freeaddrinfo(result);           /* 不再需要 */

    if (rp == NULL) {               /* 没有地址成功 */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }

    /* 将剩余的命令行参数作为单独的数据报发送，并从服务器读取响应 */
    for (int j = 3; j < argc; j++) {
        len = strlen(argv[j]) + 1;  /* +1 用于终止空字节 */
        if (len > BUF_SIZE) {
            fprintf(stderr, "Ignoring long message in argument %d\n", j);
            continue;
        }

        if (write(sfd, argv[j], len) != len) {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }

        nread = read(sfd, buf, BUF_SIZE);
        if (nread == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("Received %zd bytes: %s\n", nread, buf);
    }

    exit(EXIT_SUCCESS);
}
```

#### 另请参阅

[getnameinfo(3)](https://man.cx/getnameinfo%283%29)

<br/>

### getsockopt

#### 参考

[getsockopt(2) - Linux manual page](https://man7.org/linux/man-pages/man2/setsockopt.2.html) / [man.cx manual pages](https://man.cx/setsockopt%282%29)

#### 原型

```c
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>

int getsockopt(int sockfd, int level, int optname,
        void *optval, socklen_t *optlen);
int setsockopt(int sockfd, int level, int optname,
        const void *optval, socklen_t optlen);
```

#### 描述

`getsockopt()` 和 `setsockopt()` 用于操纵 `sockfd` 文件描述符所引用的套接字上的选项。选项可能存在于多个协议层次上，它们总是在最顶层的套接字层次上存在。

当操纵套接字选项时，必须指定选项所在的层次（level）以及选项的名称（optname）。要在套接字 API 层次上操纵选项，`level` 应指定为 `SOL_SOCKET`。要操纵任何其他层次的选项，则应提供控制该选项的相应协议的协议号。例如，要指明某个选项应由 TCP 协议解释，`level` 应设置为 TCP 的协议号；参见 [getprotoent(3)](https://man.cx/getprotoent%283%29)。

参数 `optval` 和 `optlen` 用于访问 `setsockopt()` 的选项值。对于 `getsockopt()`，它们标识一个缓冲区，用于返回所请求选项的值。对于 `getsockopt()`，`optlen` 是一个值-结果参数，初始时包含 `optval` 所指向缓冲区的大小，返回时被修改为指示实际返回的值的大小。如果不需要提供或返回选项值，`optval` 可以为 `NULL`。

`optname` 和指定的选项被原封不动地传递给相应的协议模块进行解释。头文件 `<sys/socket.h>` 包含了套接字层次选项的定义，这些选项描述如下。其他协议层次的选项在格式和名称上有所不同；请查阅手册第 4 节中的相应条目。

大多数套接字层次的选项使用 `int` 类型的参数作为 `optval`。对于 `setsockopt()`，参数应为非零值以启用一个布尔选项，若需禁用该选项则设置为零。

有关可用套接字选项的详细描述，请参阅 [socket(7)](https://man.cx/socket%287%29) 以及相应协议的手册页。

#### 返回值

成功时，对于标准选项返回零。对于 Netfilter 允许程序员定义带有相应处理器的自定义套接字选项，此类选项成功时的返回值是处理器返回的值。出错时，返回 -1，并设置相应的 `errno`。

##### 错误码

| 错误              | 描述                                                         |
| :---------------- | :----------------------------------------------------------- |
| **`EBADF`**       | 参数 `sockfd` 不是一个有效的文件描述符。                     |
| **`EFAULT`**      | `optval` 指向的地址不在进程地址空间的有效部分。对于 `getsockopt()`，如果 `optlen` 不在进程地址空间的有效部分，也可能返回此错误。 |
| **`EINVAL`**      | 在 `setsockopt()` 中 `optlen` 无效。在某些情况下，如果 `optval` 中的值无效（例如，对于 [ip(7)](https://man.cx/ip%287%29) 中描述的 `IP_ADD_MEMBERSHIP` 选项），也可能触发此错误。 |
| **`ENOPROTOOPT`** | 在指定的层次上，该选项未知。                                 |
| **`ENOTSOCK`**    | 文件描述符 `sockfd` 不指向套接字。                           |

#### 遵循标准

POSIX.1-2001, POSIX.1-2008, SVr4, 4.4BSD（这些系统调用首次出现在 4.2BSD 中）。

#### 注意

POSIX.1 不要求包含 `<sys/types.h>`，且 Linux 上也不需要此头文件。然而，某些历史悠久的（BSD）实现需要此头文件，可移植的应用程序最好包含它。

关于 `socklen_t` 类型的背景信息，请参见 [accept()](#accept)。

<br/>

翻译一下这个网页：https://man.cx/setsockopt(2)，对于其中的超链接，改成markdown 格式，如：[socket()](#socket)

<br/>

### fcntl

#### 参考

[fcntl(2) - Linux manual page](https://man7.org/linux/man-pages/man2/fcntl.2.html) / [man.cx manual pages](https://man.cx/fcntl%282%29)

#### 原型

```c
#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int cmd, ... /* arg */ );
```

#### 描述

`fcntl()` 在已打开的文件描述符 `fd` 上执行以下描述的某项操作。操作由 `cmd` 决定。

`fcntl()` 可以接受一个可选的第三个参数。是否需要该参数由 `cmd` 决定。所需参数的类型在每个 `cmd` 名称后的括号中指明（多数情况下，所需类型为 `int`，我们使用名称 `arg` 来标识该参数），如果不需要参数，则指定为 `void`。

下面某些操作仅在特定的 Linux 内核版本后才被支持。检查主机内核是否支持特定操作的首选方法是，使用所需的 `cmd` 值调用 `fcntl()`，然后测试调用是否因 `EINVAL` 而失败，这表示内核无法识别该值。

##### 复制一个文件描述符

*   **F_DUPFD (int)**：复制文件描述符 `fd`，使用大于或等于 `arg` 的最小可用文件描述符。这与 [dup2(2)](https://man.cx/dup2%282%29) 不同，后者使用精确指定的文件描述符。成功时返回新的文件描述符。更多详细信息请参见 [dup(2)](https://man.cx/dup%282%29)。

*   **F_DUPFD_CLOEXEC (int; since Linux 2.6.24)**：行为类似 `F_DUPFD`，但会额外为新文件描述符设置执行时关闭标志（`close-on-exec`）。使用此标志可避免程序再执行一次 `fcntl()` `F_SETFD` 操作来设置 `FD_CLOEXEC` 标志。关于该标志为何有用的解释，请参见 [open(2)](https://man.cx/open%282%29) 中对 `O_CLOEXEC` 的描述。

##### 文件描述符标志

以下命令操作与文件描述符关联的标志。目前只定义了一个标志：`FD_CLOEXEC`，即 close-on-exec 标志。如果 `FD_CLOEXEC` 位被设置，文件描述符将在 [execve(2)](https://man.cx/execve%282%29) 成功时自动关闭（如果 [execve(2)](https://man.cx/execve(2)) 失败，文件描述符保持打开）。如果未设置 `FD_CLOEXEC` 位，文件描述符将在 [execve(2)](https://man.cx/execve(2)) 期间保持打开。

*   **F_GETFD (void)**：返回文件描述符标志；`arg` 被忽略。
*   **F_SETFD (int)**：将文件描述符标志设置为 `arg` 指定的值。

在多线程程序中，当一个线程执行 [fork(2)](https://man.cx/fork(2)) 加 [execve(2)](https://man.cx/execve(2)) 的同时，使用 `fcntl()` `F_SETFD` 设置 close-on-exec 标志容易受到竞争条件的影响，这可能会无意中将文件描述符泄露给子进程中执行的程序。有关详情和问题补救措施，请参见 [open(2)](https://man.cx/open(2)) 中对 `O_CLOEXEC` 标志的讨论。

##### 文件状态标志

每个打开的文件描述都有关联的状态标志，由 [open(2)](https://man.cx/open%282%29) 初始化，并可能被 `fcntl()` 修改。复制的文件描述符（由 [dup(2)](https://man.cx/dup%282%29)、`fcntl(F_DUPFD)`、[fork(2)](https://man.cx/fork%282%29) 等创建）引用同一个打开的文件描述，因此共享相同的文件状态标志。这些标志及其语义详见 [open(2)](https://man.cx/open%282%29)。

*   **F_GETFL (void)**：返回（作为函数结果）文件访问模式和文件状态标志；`arg` 被忽略。
*   **F_SETFL (int)**：将文件状态标志设置为 `arg` 指定的值。`arg` 中的文件访问模式（`O_RDONLY`, `O_WRONLY`, `O_RDWR`）和文件创建标志（即 `O_CREAT`, `O_EXCL`, `O_NOCTTY`, `O_TRUNC`）将被忽略。在 Linux 上，此命令只能修改 `O_APPEND`、`O_ASYNC`、`O_DIRECT`、`O_NOATIME` 和 `O_NONBLOCK` 标志。无法修改 `O_DSYNC` 和 `O_SYNC` 标志；详见下文 BUGS。

##### 建议性记录锁

Linux 实现了传统的（“与进程关联的”）UNIX 记录锁，由 POSIX 标准化。对于语义更优的 Linux 特有替代方案，请参阅下文关于打开文件描述锁的讨论。

`F_SETLK`, `F_SETLKW`, 和 `F_GETLK` 用于获取、释放和测试记录锁（也称为字节范围锁、文件段锁或文件区域锁）的存在。第三个参数 `lock` 是一个指向至少包含以下字段（顺序未指定）的结构体的指针：

```c
struct flock {
    ...
    short l_type;    /* 锁类型: F_RDLCK, F_WRLCK, F_UNLCK */
    short l_whence;  /* 解释 l_start 的方式: SEEK_SET, SEEK_CUR, SEEK_END */
    off_t l_start;   /* 锁的起始偏移量 */
    off_t l_len;     /* 要锁定的字节数 */
    pid_t l_pid;     /* 阻塞我们锁的进程的 PID (由 F_GETLK 和 F_OFD_GETLK 设置) */
    ...
};
```

该结构体的 `l_whence`、`l_start` 和 `l_len` 字段指定了想要锁定的字节范围。可以锁定文件结尾之后的字节，但不能锁定文件开始之前的字节。
*   `l_start` 是锁的起始偏移量，其解释方式取决于 `l_whence`：
    *   `SEEK_SET`：相对于文件开头。
    *   `SEEK_CUR`：相对于当前文件偏移量。
    *   `SEEK_END`：相对于文件末尾。
    在后两种情况下，只要偏移量不位于文件开始之前，`l_start` 可以是负数。
*   `l_len` 指定要锁定的字节数。如果 `l_len` 为正数，则锁定的范围覆盖从 `l_start` 到 `l_start+l_len-1`（含）的字节。`l_len` 为 0 具有特殊含义：锁定从 `l_whence` 和 `l_start` 指定的位置开始直到文件末尾的所有字节，无论文件增长到多大。
POSIX.1-2001 允许（但不要求）实现支持负的 `l_len` 值；如果 `l_len` 为负数，则锁定的区间覆盖从 `l_start+l_len` 到 `l_start-1`（含）的字节。Linux 从内核版本 2.4.21 和 2.5.49 开始支持此特性。

`l_type` 字段可用于对文件施加读锁 (`F_RDLCK`) 或写锁 (`F_WRLCK`)。任意数量的进程可以持有一个文件区域的读锁（共享锁），但只有一个进程可以持有写锁（排他锁）。排他锁会排斥所有其他锁，无论是共享锁还是排他锁。单个进程在一个文件区域上只能持有一种锁；如果对一个已锁定的区域施加新的锁，则现有的锁会转换为新的锁类型。（如果新锁指定的字节范围与现有锁的范围不完全一致，这种转换可能涉及锁的分裂、缩小或合并。）

*   **F_SETLK (struct flock \*)**：获取（当 `l_type` 为 `F_RDLCK` 或 `F_WRLCK` 时）或释放（当 `l_type` 为 `F_UNLCK` 时）由 `lock` 的 `l_whence`、`l_start` 和 `l_len` 字段指定的字节上的锁。如果另一个进程持有冲突的锁，此调用返回 -1 并将 `errno` 设置为 `EACCES` 或 `EAGAIN`。（不同实现中返回的错误不同，因此 POSIX 要求可移植的应用程序检查这两种错误。）。

*   **F_SETLKW (struct flock \*)**：与 `F_SETLK` 类似，但如果文件上持有冲突的锁，则等待该锁被释放。如果在等待时捕获到信号，则调用会被中断，并在信号处理程序返回后立即返回（返回值为 -1，`errno` 设置为 `EINTR`；参见 [signal(7)](https://man.cx/signal%287%29)）。

*   **F_GETLK (struct flock \*)**：在此调用中，输入 `lock` 描述了我们想要对文件施加的锁。如果可以放置该锁，`fcntl()` 不会实际放置它，而是将 `lock` 的 `l_type` 字段返回 `F_UNLCK`，并保持结构体的其他字段不变。如果一个或多个不兼容的锁会阻止此锁的放置，则 `fcntl()` 会在 `lock` 的 `l_type`、`l_whence`、`l_start` 和 `l_len` 字段中返回其中一个锁的详细信息。如果冲突的锁是传统的（与进程关联的）记录锁，则 `l_pid` 字段被设置为持有该锁的进程的 PID。如果冲突的锁是打开文件描述锁，则 `l_pid` 被设置为 -1。注意，返回的信息在调用者检查时可能已经过时。

要放置读锁，`fd` 必须为读取而打开。要放置写锁，`fd` 必须为写入而打开。要同时放置两种锁，请以读写方式打开文件。

使用 `F_SETLKW` 放置锁时，内核会检测死锁。例如，进程 A 持有文件中第 100 字节的写锁，进程 B 持有第 200 字节的写锁。如果每个进程随后都尝试使用 `F_SETLKW` 锁定已经被对方锁定的字节，那么如果没有死锁检测，两个进程都将无限期阻塞。当内核检测到这种死锁时，它会使得其中一个阻塞的锁请求立即失败，并返回错误 `EDEADLK`。遇到此错误的应用程序应释放其部分锁，以便其他应用程序继续运行，然后再尝试重新获取所需的锁。涉及超过两个进程的循环死锁也会被检测到。但请注意，内核的死锁检测算法存在限制；参见 BUGS。

除了通过显式的 `F_UNLCK` 移除外，记录锁在进程终止时也会自动释放。记录锁不会被通过 [fork(2)](https://man.cx/fork%282%29) 创建的子进程继承，但会在 [execve(2)](https://man.cx/execve%282%29) 后保留。。

由于 [stdio(3)](https://man.cx/stdio%283%29) 库执行缓冲，应避免在该库中使用记录锁例程；应改用 [read(2)](https://man.cx/read%282%29) 和 [write(2)](https://man.cx/write%282%29)。

上述记录锁与进程相关联（与下文所述的打开文件描述锁不同）。这带来了一些不良后果：
*   如果一个进程关闭了任何引用某个文件的文件描述符，那么该进程在该文件上的所有锁都会被释放，无论这些锁是通过哪个文件描述符获得的。这意味着，当某个库函数出于某种原因打开、读取并关闭了像 `/etc/passwd` 这样的文件时，进程可能会丢失其在该文件上的锁。
*   进程内的线程共享锁。换句话说，多线程程序不能使用记录锁来确保线程不会同时访问文件的同一区域。

打开文件描述锁解决了这两个问题。

##### 打开文件描述锁（非 POSIX）

打开文件描述锁是建议性的字节范围锁，其操作在大多数方面与传统记录锁相同。此锁类型是 Linux 特有的，自 Linux 3.15 起可用。（有人已向奥斯汀工作组提议将此锁类型包含在下一版 POSIX.1 中。）。关于打开文件描述的解释，请参阅 [open(2)](https://man.cx/open%282%29)。

这两种锁类型的主要区别在于，传统的记录锁与进程关联，而打开文件描述锁与获取锁时打开的文件描述关联，类似于 [flock(2)](https://man.cx/flock%282%29) 获取的锁。因此（与传统建议性记录锁不同），打开文件描述锁会跨 [fork(2)](https://man.cx/fork%282%29)（以及带 `CLONE_FILES` 标志的 [clone(2)](https://man.cx/clone%282%29)）继承，并且仅在打开文件描述的最后一次关闭时自动释放，而不是在文件的任何关闭时释放。

冲突锁组合（即一个读锁和一个写锁，或两个写锁），如果一个锁是打开文件描述锁，另一个是传统记录锁，即使它们由同一个进程通过同一个文件描述符获得，也会冲突。

通过同一个打开文件描述（即，通过同一个文件描述符，或通过由 `fork(2)`、[dup(2)](https://man.cx/dup%282%29)、`fcntl()` `F_DUPFD` 等创建的文件描述符副本）放置的打开文件描述锁始终是兼容的：如果对已锁定的区域放置新锁，则现有锁会转换为新锁类型。（如上所述，这种转换可能导致锁的分裂、缩小或与现有锁合并。）

另一方面，通过不同打开文件描述获取的打开文件描述锁之间可能会相互冲突。因此，多线程程序中的线程可以通过各自对文件执行 `open(2)` 操作，并通过生成的文件描述符应用锁，来使用打开文件描述锁同步对文件区域的访问。

与传统建议性锁一样，`fcntl()` 的第三个参数 `lock` 是一个指向 `flock` 结构体的指针。与传统记录锁不同，在使用下述命令时，该结构体的 `l_pid` 字段必须设置为 0。

用于操作打开文件描述锁的命令与传统锁的命令类似：

*   **F_OFD_SETLK (struct flock \*)**：获取（`l_type` 为 `F_RDLCK` 或 `F_WRLCK` 时）或释放（`l_type` 为 `F_UNLCK` 时）由 `lock` 的 `l_whence`、`l_start` 和 `l_len` 字段指定的字节上的打开文件描述锁。如果另一个进程持有冲突的锁，此调用返回 -1 并将 `errno` 设置为 `EAGAIN`。

*   **F_OFD_SETLKW (struct flock \*)**：与 `F_OFD_SETLK` 类似，但如果文件上持有冲突的锁，则等待该锁被释放。如果在等待时捕获到信号，则调用会被中断，并在信号处理程序返回后立即返回（返回值为 -1，`errno` 设置为 `EINTR`；参见 [signal(7)](https://man.cx/signal%287%29)）。

*   **F_OFD_GETLK (struct flock \*)**：在此调用中，输入 `lock` 描述了想要对文件放置的打开文件描述锁。如果可以放置该锁，`fcntl()` 不会实际放置它，而是将 `lock` 的 `l_type` 字段返回 `F_UNLCK`，并保持结构体的其他字段不变。如果一个或多个不兼容的锁会阻止此锁的放置，则如上文对 `F_GETLK` 的描述，会通过 `lock` 返回其中一个锁的详细信息。

在当前实现中，内核不会对打开文件描述锁执行死锁检测（这与进程关联的记录锁形成对比）。

##### 强制锁

**警告**：Linux 对强制锁的实现不可靠。详见下文 BUGS。由于这些错误，并且认为该功能很少使用，自 Linux 4.5 起，强制锁已成为一个可选功能，由配置选项 (`CONFIG_MANDATORY_FILE_LOCKING`) 控制。这是完全移除该功能的第一步。

默认情况下，传统（与进程关联的）记录锁和打开文件描述锁都是建议性的。建议性锁不会强制执行，仅在协作进程之间有用。这两种锁类型也可以设置为强制锁。强制锁对所有进程强制执行。

如果一个进程试图对具有不兼容强制锁的文件区域执行不兼容的访问（例如 [read(2)](https://man.cx/read%282%29) 或 [write(2)](https://man.cx/write%282%29)），结果取决于其打开文件描述是否启用了 `O_NONBLOCK` 标志。如果未启用，则系统调用会阻塞，直到锁被移除或转换为与访问兼容的模式。如果启用了 `O_NONBLOCK`，则系统调用会失败并返回错误 `EAGAIN`。

要使用强制锁，必须在包含要锁定文件的文件系统上启用强制锁，并且文件本身也必须启用强制锁。在文件系统上，通过向 [mount(8)](https://man.cx/mount%288%29) 提供 `-o mand` 选项或向 [mount(2)](https://man.cx/mount%282%29) 提供 `MS_MANDLOCK` 标志来启用强制锁。在文件上，通过禁用文件的组执行权限并启用设置组 ID 权限位来启用强制锁（参见 [chmod(1)](https://man.cx/chmod%281%29) 和 [chmod(2)](https://man.cx/chmod%282%29)）。

POSIX 未规定强制锁。其他一些系统也支持强制锁，尽管启用方式各不相同。

##### 锁丢失

在网络文件系统（如 NFS）上获取建议性锁时，锁可能会丢失。这可能是由于服务器上的管理操作，或由于网络分区（即与服务器的网络连接丢失）持续时间足以让服务器假定客户端不再起作用。当文件系统确定锁已丢失时，未来的 [read(2)](https://man.cx/read(2)) 或 [write(2)](https://man.cx/write(2)) 请求可能会失败并返回错误 `EIO`。此错误将持续存在，直到锁被移除或文件描述符被关闭。自 Linux 3.12 起，至少对于 NFSv4（包括所有次要版本）会发生这种情况。某些 UNIX 版本在这种情况下会发送信号（`SIGLOST`）。Linux 没有定义此信号，也不提供丢失锁的异步通知。

##### 管理信号

`F_GETOWN`、`F_SETOWN`、`F_GETOWN_EX`、`F_SETOWN_EX`、`F_GETSIG` 和 `F_SETSIG` 用于管理 I/O 可用性信号。

*   **F_GETOWN (void)**：返回当前接收文件描述符 `fd` 上事件的 `SIGIO` 和 `SIGURG` 信号的进程 ID 或进程组 ID。进程 ID 以正值返回；进程组 ID 以负值返回（但参见下文 BUGS）。`arg` 被忽略。

* **F_SETOWN (int)**：设置将接收文件描述符 `fd` 上事件的 `SIGIO` 和 `SIGURG` 信号的进程 ID 或进程组 ID。目标进程或进程组 ID 在 `arg` 中指定。进程 ID 以正值指定；进程组 ID 以负值指定。最常见的是，调用进程将自己指定为所有者（即 `arg` 指定为 [getpid(2)](https://man.cx/getpid%282%29)）。

  除了设置文件描述符所有者外，还必须启用文件描述符上的信号生成。这是通过使用 `fcntl()` `F_SETFL` 命令在文件描述符上设置 `O_ASYNC` 文件状态标志来完成的。随后，每当文件描述符上的输入或输出变为可能时，就会发送 `SIGIO` 信号。`fcntl()` `F_SETSIG` 命令可用于获取除 `SIGIO` 之外的信号投递。

  `F_SETOWN` 指定的所有者进程（组）发送信号需经过与 [kill(2)](https://man.cx/kill%282%29) 相同的权限检查，其中发送进程是使用 `F_SETOWN` 的进程（但参见下文 BUGS）。如果权限检查失败，信号将被静默丢弃。注意：`F_SETOWN` 操作会记录调用 `fcntl()` 时调用者的凭据，并使用这些保存的凭据进行权限检查。

  如果文件描述符 `fd` 引用一个套接字，`F_SETOWN` 还会选择当该套接字上有带外数据到达时接收 `SIGURG` 信号的接收者。（`SIGURG` 在 [select(2)](https://man.cx/select%282%29) 报告套接字具有“异常条件”的任何情况下发送。）

  在 2.6.x 内核中（直至并包括内核 2.6.11），如果在一个运行支持线程组（例如 NPTL）的线程库的多线程进程中给 `F_SETSIG` 一个非零值，那么给 `F_SETOWN` 一个正值会有不同的含义：它不再是一个标识整个进程的进程 ID，而是一个标识进程内特定线程的线程 ID。因此，当使用 `F_SETSIG` 时，可能需要向 `F_SETOWN` 传递 [gettid(2)](https://man.cx/gettid%282%29) 的结果，而不是 `getpid(2)`，才能获得合理的结果。（在当前 Linux 线程实现中，主线程的线程 ID 与其进程 ID 相同。这意味着在此场景中，单线程程序可以同样使用 `gettid(2)` 或 `getpid(2)`。）但是，此段中的陈述不适用于为套接字上的带外数据生成的 `SIGURG` 信号：此信号始终发送到进程或进程组，具体取决于给 `F_SETOWN` 的值。上述行为在 Linux 2.6.12 中被意外删除，且不会恢复。从 Linux 2.6.32 开始，请使用 `F_SETOWN_EX` 将 `SIGIO` 和 `SIGURG` 信号定向到特定线程。


*   **F_GETOWN_EX (struct f_owner_ex \*)** (since Linux 2.6.32)：返回由先前 `F_SETOWN_EX` 操作定义的当前文件描述符所有者设置。信息返回到 `arg` 指向的具有以下形式的结构体中：
    
    ```c
    struct f_owner_ex {
        int   type;
        pid_t pid;
    };
    ```
    `type` 字段将为 `F_OWNER_TID`、`F_OWNER_PID` 或 `F_OWNER_PGRP` 之一。`pid` 字段是一个表示线程 ID、进程 ID 或进程组 ID 的正整数。更多详细信息参见 `F_SETOWN_EX`。
    
*   **F_SETOWN_EX (struct f_owner_ex \*)** (since Linux 2.6.32)：此操作执行与 `F_SETOWN` 类似的任务。它允许调用者将 I/O 可用性信号定向到特定的线程、进程或进程组。调用者通过 `arg` 指定信号的目标，`arg` 是一个指向 `f_owner_ex` 结构体的指针。`type` 字段具有以下值之一，用于定义如何解释 `pid`：
    *   `F_OWNER_TID`：将信号发送给 `pid` 指定的线程 ID（[clone(2)](https://man.cx/clone%282%29) 或 [gettid(2)](https://man.cx/gettid%282%29) 返回的值）的线程。
    *   `F_OWNER_PID`：将信号发送给 `pid` 指定的进程。
    *   `F_OWNER_PGRP`：将信号发送给 `pid` 指定的进程组。（注意，与 `F_SETOWN` 不同，这里的进程组 ID 以正值指定。）。

*   **F_GETSIG (void)**：返回当输入或输出变为可能时发送的信号。值为 0 表示发送 `SIGIO`。任何其他值（包括 `SIGIO`）表示改为发送该信号，并且在此情况下，如果信号处理程序使用 `SA_SIGINFO` 安装，则信号处理程序可获得额外信息。`arg` 被忽略。

* **F_SETSIG (int)**：将当输入或输出变为可能时发送的信号设置为 `arg` 中的值。值为 0 表示发送默认的 `SIGIO` 信号。任何其他值（包括 `SIGIO`）表示改为发送该信号，并且在此情况下，如果信号处理程序使用 `SA_SIGINFO` 安装，则信号处理程序可获得额外信息。

  通过将 `F_SETSIG` 与非零值一起使用，并为信号处理程序设置 `SA_SIGINFO`（参见 [sigaction(2)](https://man.cx/sigaction%282%29)），会将有关 I/O 事件的额外信息传递给处理程序中的 `siginfo_t` 结构体。如果 `si_code` 字段指示来源是 `SI_SIGIO`，则 `si_fd` 字段给出与事件关联的文件描述符。否则，没有指示哪些文件描述符处于待处理状态，应使用常规机制（[select(2)](https://man.cx/select%282%29)、[poll(2)](https://man.cx/poll%282%29)、设置了 `O_NONBLOCK` 的 [read(2)](https://man.cx/read%282%29) 等）来确定哪些文件描述符可用于 I/O。

  通过选择实时信号（值 >= `SIGRTMIN`），可以使用相同的信号号对多个 I/O 事件进行排队。（排队取决于可用内存。）如上所述，如果为信号处理程序设置了 `SA_SIGINFO`，则可获得额外信息。请注意，Linux 对可以排队到进程的实时信号数量施加了限制（参见 [getrlimit(2)](https://man.cx/getrlimit%282%29) 和 [signal(7)](https://man.cx/signal%287%29)），如果达到此限制，内核将恢复为投递 `SIGIO`，并且此信号会投递给整个进程，而不是特定线程。


使用这些机制，程序可以在大部分时间实现完全异步 I/O，而无需使用 [select(2)](https://man.cx/select(2)) 或 [poll(2)](https://man.cx/poll(2))。

`O_ASYNC` 的使用是 BSD 和 Linux 特有的。POSIX.1 中规定的 `F_GETOWN` 和 `F_SETOWN` 的唯一用途是与套接字上的 `SIGURG` 信号结合使用。（POSIX 未规定 `SIGIO` 信号。）`F_GETOWN_EX`、`F_SETOWN_EX`、`F_GETSIG` 和 `F_SETSIG` 是 Linux 特有的。POSIX 有异步 I/O 和 `aio_sigevent` 结构来实现类似的功能；这些也在 Linux 中作为 GNU C 库的一部分提供。

##### 租赁协议

`F_SETLEASE` 和 `F_GETLEASE`（Linux 2.4 及更高版本）用于在文件描述符 `fd` 引用的打开文件描述上建立新租约或检索当前租约。文件租约提供了一种机制，当另一个进程（“租约破坏者”）试图 [open(2)](https://man.cx/open%282%29) 或 [truncate(2)](https://man.cx/truncate%282%29) 该文件描述符引用的文件时，持有租约的进程（“租约持有者”）会收到通知（通过信号投递）。

*   **F_SETLEASE (int)**：根据整型 `arg` 中指定的以下值之一设置或移除文件租约：
    *   `F_RDLCK`：获取一个读租约。当文件为写入而打开或被截断时，这将导致调用进程收到通知。读租约只能放在以只读方式打开的文件描述符上。
    *   `F_WRLCK`：获取一个写租约。当文件为读取或写入而打开或被截断时，这将导致调用者收到通知。只有当文件没有其他打开的文件描述符时，才能放置写租约。
    *   `F_UNLCK`：从文件中移除我们的租约。。

租约与一个打开的文件描述相关联（参见 [open(2)](https://man.cx/open%282%29)）。这意味着重复的文件描述符（例如由 [fork(2)](https://man.cx/fork%282%29) 或 [dup(2)](https://man.cx/dup%282%29) 创建的）引用同一个租约，并且可以使用其中任何一个描述符修改或释放此租约。此外，通过对任何这些重复文件描述符执行显式的 `F_UNLCK` 操作，或者当所有这些文件描述符都已关闭时，租约将被释放。租约只能对常规文件获取。非特权进程只能在其 UID（所有者）与进程的文件系统 UID 匹配的文件上获取租约。具有 `CAP_LEASE` 能力的进程可以在任意文件上获取租约。

*   **F_GETLEASE (void)**：通过返回 `F_RDLCK`、`F_WRLCK` 或 `F_UNLCK` 来指示与文件描述符 `fd` 关联的租约类型，分别表示读租约、写租约或无租约。`arg` 被忽略。

当一个进程（“租约破坏者”）执行与通过 `F_SETLEASE` 建立的租约相冲突的 [open(2)](https://man.cx/open%282%29) 或 [truncate(2)](https://man.cx/truncate%282%29) 时，系统调用会被内核阻塞，并且内核通过向租约持有者发送信号（默认为 `SIGIO`）来通知它。租约持有者收到此信号后，应进行必要的清理工作，为另一个进程访问文件做准备（例如刷新缓存缓冲区），然后移除或降级其租约。通过执行指定 `arg` 为 `F_UNLCK` 的 `F_SETLEASE` 命令来移除租约。如果租约持有者当前持有文件的写租约，而租约破坏者正在为读取而打开文件，则租约持有者将租约降级为读租约就足够了。这是通过执行指定 `arg` 为 `F_RDLCK` 的 `F_SETLEASE` 命令来完成的。

如果租约持有者在 `/proc/sys/fs/lease-break-time` 中指定的秒数内未能降级或移出租约，则内核会强制移出或降级租约持有者的租约。一旦租约中断启动，`F_GETLEASE` 将返回目标租约类型（`F_RDLCK` 或 `F_UNLCK`，取决于哪个与租约破坏者兼容），直到租约持有者自愿降级或移出租约，或者内核在租约中断计时器到期后强制执行。一旦租约被自愿或强制移出或降级，并且假设租约破坏者尚未解除对其系统调用的阻塞，内核将允许租约破坏者的系统调用继续。如果租约破坏者被阻塞的 [open(2)](https://man.cx/open(2)) 或 [truncate(2)](https://man.cx/truncate(2)) 被信号处理程序中断，则系统调用失败并返回错误 `EINTR`，但其他步骤仍如上所述发生。如果租约破坏者在 [open(2)](https://man.cx/open(2)) 或 [truncate(2)](https://man.cx/truncate(2)) 中被阻塞时被信号杀死，则其他步骤仍如上所述发生。如果租约破坏者在调用 [open(2)](https://man.cx/open(2)) 时指定了 `O_NONBLOCK` 标志，则该调用立即失败并返回错误 `EWOULDBLOCK`，但其他步骤仍如上所述发生。

用于通知租约持有者的默认信号是 `SIGIO`，但可以使用 `fcntl()` 的 `F_SETSIG` 命令进行更改。如果执行了 `F_SETSIG` 命令（即使指定了 `SIGIO`），并且使用 `SA_SIGINFO` 建立了信号处理程序，则处理程序将收到一个 `siginfo_t` 结构作为其第二个参数，并且此参数的 `si_fd` 字段将包含已被另一个进程访问的租约文件的文件描述符。（如果调用者持有多个文件的租约，这很有用。）

##### 文件和目录变更通知 (dnotify)

**F_NOTIFY** (*int*)

(Linux 2.4 及之后) 当由 *fd* 引用的目录或其包含的任何文件发生变更时提供通知。需要通知的事件在 *arg* 中指定，*arg* 是由以下一个或多个比特位通过 OR 运算构成的掩码：

**DN_ACCESS**

文件被访问（read(2)、[pread(2)](https://man.cx/pread(2))、[readv(2)](https://man.cx/readv(2)) 等）。

**DN_MODIFY**

文件被修改（write(2)、[pwrite(2)](https://man.cx/pwrite(2))、[writev(2)](https://man.cx/writev(2))、[truncate(2)](https://man.cx/truncate(2))、[ftruncate(2)](https://man.cx/ftruncate(2)) 等）。

**DN_CREATE**

文件被创建（open(2)、[creat(2)](https://man.cx/creat(2))、[mknod(2)](https://man.cx/mknod(2))、[mkdir(2)](https://man.cx/mkdir(2))、[link(2)](https://man.cx/link(2))、[symlink(2)](https://man.cx/symlink(2))、[rename(2)](https://man.cx/rename(2)) 到本目录中）。

**DN_DELETE**

文件被删除（unlink(2)、[rename(2)](https://man.cx/rename(2)) 到另一个目录、[rmdir(2)](https://man.cx/rmdir(2))）。

**DN_RENAME**

文件在本目录内被重命名（rename(2)）。

**DN_ATTRIB**

文件的属性被更改（chown(2)、[chmod(2)](https://man.cx/chmod(2))、[utime(2)](https://man.cx/utime(2))、[utimensat(2)](https://man.cx/utimensat(2)) 等）。

（为了获得这些定义，必须在包含*任何*头文件之前定义 **_GNU_SOURCE** 功能测试宏。）

目录通知通常是“一次性”的，应用程序必须重新注册才能接收后续通知。或者，如果在 *arg* 中包含 **DN_MULTISHOT**，则通知将一直有效，直到被显式移除。

一系列 **F_NOTIFY** 请求是累积的，*arg* 中的事件会被添加到已监控的集合中。要禁用所有事件的通知，请使用 *arg* 为 0 调用 **F_NOTIFY**。

通知通过发送信号来传递。默认信号是 **SIGIO**，但可以通过 **fcntl**() 的 **F_SETSIG** 命令进行更改。（注意 **SIGIO** 是非队列化的标准信号之一；改用实时信号意味着多个通知可以排队到进程。）在后一种情况下，信号处理程序会收到一个 *siginfo_t* 结构体作为其第二个参数（如果处理程序是使用 **SA_SIGINFO** 建立的），该结构体的 *si_fd* 字段包含了产生通知的文件描述符（当在多个目录上建立通知时很有用）。

尤其在使用 **DN_MULTISHOT** 时，应使用实时信号进行通知，以便多个通知可以排队。

**注意：** 新应用程序应使用 *inotify* 接口（自内核 2.6.13 起可用），它提供了更好的文件系统事件通知接口。参见 [inotify(7)](https://man.cx/inotify(7))。

##### 变更管道容量

**F_SETPIPE_SZ** (*int*；自 Linux 2.6.35 起)

将由 *fd* 引用的管道容量至少更改为 *arg* 字节。非特权进程可以将管道容量调整为系统页大小与 */proc/sys/fs/pipe-max-size*（参见 [proc(5)](https://man.cx/proc(5))）中定义的限制之间的任意值。尝试将管道容量设置为低于页大小会被静默地向上取整到页大小。非特权进程尝试将管道容量设置为超过 */proc/sys/fs/pipe-max-size* 中的限制会返回错误 **EPERM**；特权进程（**CAP_SYS_RESOURCE**）可以覆盖该限制。

当为管道分配缓冲区时，内核可能会使用比 *arg* 更大的容量（如果这样便于实现）。（在当前实现中，分配的是请求大小按页大小向上取整到下一个 2 的幂次。）设置的的实际容量（以字节为单位）作为函数结果返回。

尝试将管道容量设置为小于当前已用于存储数据的缓冲区空间量会返回错误 **EBUSY**。

请注意，由于向管道写入数据时使用管道缓冲区页面的方式，实际可写入的字节数可能小于名义大小，具体取决于写入的大小。

**F_GETPIPE_SZ** (*void*；自 Linux 2.6.35 起)

返回（作为函数结果）由 *fd* 引用的管道的容量。

##### 文件封印

文件封印限制对给定文件允许的操作集合。对于文件上设置的每个封印，从此以后对该文件执行的一组特定操作将失败并返回 **EPERM**。该文件被称为已封印。默认的封印集取决于底层文件和文件系统的类型。有关文件封印的概述、其用途讨论以及一些代码示例，请参见 [memfd_create(2)](https://man.cx/memfd_create(2))。

目前，文件封印只能应用于由 [memfd_create(2)](https://man.cx/memfd_create(2)) 返回的文件描述符（如果使用了 **MFD_ALLOW_SEALING**）。在其他文件系统上，所有对封印进行操作的 **fcntl**() 操作都将返回 **EINVAL**。

封印是 inode 的属性。因此，引用同一 inode 的所有打开的文件描述符共享同一组封印。此外，封印永远不能被移除，只能被添加。

**F_ADD_SEALS** (*int*；自 Linux 3.17 起)

将位掩码参数 *arg* 中给出的封印添加到由文件描述符 *fd* 引用的 inode 的封印集中。封印一旦添加便无法移除。一旦此调用成功，内核将立即强制执行这些封印。如果当前的封印集已包含 **F_SEAL_SEAL**（见下文），则此调用将被拒绝并返回 **EPERM**。在 **F_SEAL_SEAL** 尚未设置的情况下，添加已设置的封印是空操作。要添加封印，文件描述符 *fd* 必须是可写的。

**F_GET_SEALS** (*void*；自 Linux 3.17 起)

返回（作为函数结果）由 *fd* 引用的 inode 的当前封印集。如果没有设置封印，则返回 0。如果文件不支持封印，则返回 -1，并将 *errno* 设置为 **EINVAL**。

以下封印可用：

**F_SEAL_SEAL**

如果设置了此封印，则任何后续带有 **F_ADD_SEALS** 的 **fcntl**() 调用都将失败并返回 **EPERM**。因此，此封印可防止对封印集本身进行任何修改。如果文件的初始封印集包含 **F_SEAL_SEAL**，则实际上会使封印集固定不变且被锁定。

**F_SEAL_SHRINK**

如果设置了此封印，则相关文件无法缩小尺寸。这会影响带有 **O_TRUNC** 标志的 [open(2)](https://man.cx/open(2)) 以及 [truncate(2)](https://man.cx/truncate(2)) 和 [ftruncate(2)](https://man.cx/ftruncate(2))。如果您尝试缩小相关文件，这些调用将失败并返回 **EPERM**。增加文件大小仍然是可能的。

**F_SEAL_GROW**

如果设置了此封印，则相关文件的尺寸无法增加。这会影响超出文件末尾的 [write(2)](https://man.cx/write(2))、[truncate(2)](https://man.cx/truncate(2))、[ftruncate(2)](https://man.cx/ftruncate(2)) 以及 [fallocate(2)](https://man.cx/fallocate(2))。如果您使用这些调用来增加文件大小，它们将失败并返回 **EPERM**。如果保持大小不变或缩小文件，这些调用仍然可以正常工作。

**F_SEAL_WRITE**

如果设置了此封印，则无法修改文件的内容。请注意，缩小或增加文件大小仍然是允许的。因此，此封印通常与其他封印之一结合使用。此封印影响 [write(2)](https://man.cx/write(2)) 和 [fallocate(2)](https://man.cx/fallocate(2))（仅在结合 **FALLOC_FL_PUNCH_HOLE** 标志时）。如果设置了此封印，这些调用将失败并返回 **EPERM**。此外，尝试通过 [mmap(2)](https://man.cx/mmap(2)) 创建新的共享、可写内存映射也将失败并返回 **EPERM**。

使用 **F_ADD_SEALS** 操作设置 **F_SEAL_WRITE** 封印时，如果存在任何可写的共享映射，则会失败并返回 **EBUSY**。必须先解除这些映射，然后才能添加此封印。此外，如果文件上存在待处理的异步 I/O 操作（io_submit(2)），则所有未完成的写入将被丢弃。

**F_SEAL_FUTURE_WRITE**（自 Linux 5.1 起）

此封印的效果类似于 **F_SEAL_WRITE**，但文件内容仍然可以通过在设置封印之前创建的共享可写映射进行修改。任何通过 [mmap(2)](https://man.cx/mmap(2)) 在文件上创建新的可写映射的尝试都将失败并返回 **EPERM**。同样，通过 [write(2)](https://man.cx/write(2)) 写入文件的尝试也将失败并返回 **EPERM**。

使用此封印，一个进程可以创建一个内存缓冲区，它可以在继续修改该缓冲区的同时，以“只读”的方式与其他进程共享该缓冲区。

##### 文件读/写提示

写入寿命提示可用于告知内核关于给定 inode 或特定打开的文件描述（参见 [open(2)](https://man.cx/open(2)) 了解打开的文件描述的解释）上写入的相对预期寿命。在此上下文中，“写入寿命”是指数据在被覆盖或擦除之前预期在介质上存在的时间。

应用程序可以使用下面列出的不同提示值将写入分为不同的写入类别，以便在单个存储后端上运行的多个用户或应用程序能够以一致的方式聚合其 I/O 模式。但是，这些标志没有隐含的功能语义，只要提示一致使用，不同的 I/O 类可以任意方式使用写入寿命提示。

以下操作可以应用于文件描述符 *fd*：

**F_GET_RW_HINT** (*uint64_t *；自 Linux 4.13 起)

返回与 *fd* 引用的底层 inode 关联的读/写提示的值。

**F_SET_RW_HINT** (*uint64_t *；自 Linux 4.13 起)

设置与 *fd* 引用的底层 inode 关联的读/写提示值。此提示将一直有效，直到被显式修改或底层文件系统被卸载。

**F_GET_FILE_RW_HINT** (*uint64_t *；自 Linux 4.13 起)

返回与 *fd* 引用的打开的文件描述关联的读/写提示的值。

**F_SET_FILE_RW_HINT** (*uint64_t *；自 Linux 4.13 起)

设置与 *fd* 引用的打开的文件描述关联的读/写提示值。

如果打开的文件描述尚未分配读/写提示，则它将使用分配给 inode 的值（如果有）。

自 Linux 4.13 起，以下读/写提示有效：

**RWH_WRITE_LIFE_NOT_SET**

未设置具体提示。这是默认值。

**RWH_WRITE_LIFE_NONE**

没有与此文件或 inode 关联的特定写入寿命。

**RWH_WRITE_LIFE_SHORT**

写入此 inode 或通过此打开的文件描述写入的数据预期寿命较短。

**RWH_WRITE_LIFE_MEDIUM**

写入此 inode 或通过此打开的文件描述写入的数据预期寿命比使用 **RWH_WRITE_LIFE_SHORT** 写入的数据更长。

**RWH_WRITE_LIFE_LONG**

写入此 inode 或通过此打开的文件描述写入的数据预期寿命比使用 **RWH_WRITE_LIFE_MEDIUM** 写入的数据更长。

**RWH_WRITE_LIFE_EXTREME**

写入此 inode 或通过此打开的文件描述写入的数据预期寿命比使用 **RWH_WRITE_LIFE_LONG** 写入的数据更长。

所有特定于写入的提示都是彼此相对的，不应赋予它们任何绝对的个别含义。

#### 返回值

对于成功的调用，返回值取决于操作：

* **F_DUPFD**：新的文件描述符。
* **F_GETFD**：文件描述符标志的值。
* **F_GETFL**：文件状态标志的值。
* **F_GETLEASE**：文件描述符上持有的租约类型。
* **F_GETOWN**：文件描述符所有者的值。
* **F_GETSIG**：当读或写变为可能时发送的信号的值，或者对于传统的 **SIGIO** 行为为零。
* **F_GETPIPE_SZ**、**F_SETPIPE_SZ**：管道容量。
* **F_GET_SEALS**：为 *fd* 引用的 inode 设置的封印的位掩码。

所有其他命令：0。

出错时，返回 -1，并设置相应的 `errno`。

##### 错误码

**EACCES** 或 **EAGAIN**

操作被其他进程持有的锁禁止。

| **EAGAIN** | 操作被禁止，因为文件已被另一个进程内存映射。 |
| --- | --- |
| **EBADF** | *fd* 不是打开的文件描述符。 |
| **EBADF** | *cmd* 是 **F_SETLK** 或 **F_SETLKW**，并且文件描述符的打开模式与请求的锁类型不匹配。 |
| **EBUSY** | *cmd* 是 **F_SETPIPE_SZ**，并且 *arg* 中指定的新管道容量小于管道中当前用于存储数据的缓冲区空间量。 |
| **EBUSY** | *cmd* 是 **F_ADD_SEALS**，*arg* 包含 **F_SEAL_WRITE**，并且 *fd* 引用的文件上存在可写的共享映射。 |

**EDEADLK**

检测到指定的 **F_SETLKW** 命令会导致死锁。

| **EFAULT** | *lock* 位于您可访问的地址空间之外。 |
| --- | --- |
| **EINTR** | *cmd* 是 **F_SETLKW** 或 **F_OFD_SETLKW**，并且操作被信号中断；参见 [signal(7)](https://man.cx/signal(7))。 |
| **EINTR** | *cmd* 是 **F_GETLK**、**F_SETLK**、**F_OFD_GETLK** 或 **F_OFD_SETLK**，并且操作在检查或获取锁之前被信号中断。最可能发生在锁定远程文件时（例如通过 NFS 锁定），但有时也可能在本地发生。 |
| **EINVAL** | *cmd* 中指定的值未被此内核识别。 |
| **EINVAL** | *cmd* 是 **F_ADD_SEALS**，并且 *arg* 包含未识别的封印位。 |
| **EINVAL** | *cmd* 是 **F_ADD_SEALS** 或 **F_GET_SEALS**，并且包含 *fd* 引用的 inode 的文件系统不支持封印。 |
| **EINVAL** | *cmd* 是 **F_DUPFD**，并且 *arg* 为负数或大于最大允许值（参见 [getrlimit(2)](https://man.cx/getrlimit(2)) 中对 **RLIMIT_NOFILE** 的讨论）。 |
| **EINVAL** | *cmd* 是 **F_SETSIG**，并且 *arg* 不是允许的信号编号。 |
| **EINVAL** | *cmd* 是 **F_OFD_SETLK**、**F_OFD_SETLKW** 或 **F_OFD_GETLK**，并且 *l_pid* 未指定为零。 |
| **EMFILE** | *cmd* 是 **F_DUPFD**，并且已达到每个进程打开文件描述符数量的限制。 |
| **ENOLCK** | 打开了太多段锁，锁表已满，或者远程锁定协议失败（例如通过 NFS 锁定）。 |

**ENOTDIR**

在 *cmd* 中指定了 **F_NOTIFY**，但 *fd* 引用的不是目录。

| **EPERM** | *cmd* 是 **F_SETPIPE_SZ**，并且已达到用户管道的软限制或硬限制；参见 [pipe(7)](https://man.cx/pipe(7))。 |
| --- | --- |
| **EPERM** | 尝试清除设置了追加属性文件的 **O_APPEND** 标志。 |
| **EPERM** | *cmd* 是 **F_ADD_SEALS**，但 *fd* 未打开用于写入，或者文件上当前的封印集已包含 **F_SEAL_SEAL**。 |

#### 遵循标准

SVr4, 4.3BSD, POSIX.1-2001。POSIX.1-2001 中仅规定了操作 **F_DUPFD**、**F_GETFD**、**F_SETFD**、**F_GETFL**、**F_SETFL**、**F_GETLK**、**F_SETLK** 和 **F_SETLKW**。

**F_GETOWN** 和 **F_SETOWN** 在 POSIX.1-2001 中规定。（要获得它们的定义，需要定义 **_XOPEN_SOURCE** 的值为 500 或更大，或者定义 **_POSIX_C_SOURCE** 的值为 200809L 或更大。）

**F_DUPFD_CLOEXEC** 在 POSIX.1-2008 中规定。（要获得此定义，需要定义 **_POSIX_C_SOURCE** 的值为 200809L 或更大，或者定义 **_XOPEN_SOURCE** 的值为 700 或更大。）

**F_GETOWN_EX**、**F_SETOWN_EX**、**F_SETPIPE_SZ**、**F_GETPIPE_SZ**、**F_GETSIG**、**F_SETSIG**、**F_NOTIFY**、**F_GETLEASE** 和 **F_SETLEASE** 是 Linux 特有的。（需要定义 **_GNU_SOURCE** 宏来获得这些定义。）

**F_OFD_SETLK**、**F_OFD_SETLKW** 和 **F_OFD_GETLK** 是 Linux 特有的（并且必须定义 **_GNU_SOURCE** 才能获得它们的定义），但正在努力将它们包含在下一版本的 POSIX.1 中。

**F_ADD_SEALS** 和 **F_GET_SEALS** 是 Linux 特有的。

#### 注意事项

[dup2(2)](https://man.cx/dup2(2)) 返回的错误与 **F_DUPFD** 返回的错误不同。

**文件锁定**

原始的 Linux **fcntl**() 系统调用并非为处理大文件偏移量（在 *flock* 结构体中）而设计。因此，在 Linux 2.4 中添加了 **fcntl64**() 系统调用。较新的系统调用对文件锁定使用了不同的结构体 *flock64* 以及相应的命令 **F_GETLK64**、**F_SETLK64** 和 **F_SETLKW64**。但是，使用 glibc 的应用程序可以忽略这些细节，因为其 **fcntl**() 包装函数在可用时会透明地使用较新的系统调用。

**记录锁**

自内核 2.0 起，[flock(2)](https://man.cx/flock(2)) 和 **fcntl**() 放置的锁类型之间没有交互。

一些系统在 *struct flock* 中有更多字段，例如 *l_sysid*（用于标识持有锁的机器）。显然，如果持有锁的进程可能位于不同的机器上，仅靠 *l_pid* 并不是很有用；在 Linux 上，虽然在某些架构（如 MIPS32）上存在此字段，但并未使用。

原始的 Linux **fcntl**() 系统调用并非为处理大文件偏移量（在 *flock* 结构体中）而设计。因此，在 Linux 2.4 中添加了 **fcntl64**() 系统调用。较新的系统调用对文件锁定使用了不同的结构体 *flock64* 以及相应的命令 **F_GETLK64**、**F_SETLK64** 和 **F_SETLKW64**。但是，使用 glibc 的应用程序可以忽略这些细节，因为其 **fcntl**() 包装函数在可用时会透明地使用较新的系统调用。

**记录锁与 NFS**

在 Linux 3.12 之前，如果 NFSv4 客户端与服务器失去联系一段时间（定义为超过 90 秒没有通信），它可能会在毫不知情的情况下失去并重新获得锁。（假设失去联系的时间段称为 NFSv4 租期。在 Linux NFS 服务器上，可以通过查看 `/proc/fs/nfsd/nfsv4leasetime` 来确定，该文件以秒为单位表示该时间段。此文件的默认值为 90。）这种情况可能导致数据损坏的风险，因为另一个进程可能在此期间获得锁并执行文件 I/O。

自 Linux 3.12 起，如果 NFSv4 客户端与服务器失去联系，则认为自己持有锁的进程对文件进行的任何 I/O 都将失败，直到该进程关闭并重新打开文件。可以将内核参数 `nfs.recover_lost_locks` 设置为 1 以获得 3.12 之前的行为，即当与服务器重新建立联系时，客户端将尝试恢复丢失的锁。由于存在数据损坏的风险，此参数默认为 0（禁用）。

#### 缺陷

**F_SETFL**

无法使用 **F_SETFL** 更改 **O_DSYNC** 和 **O_SYNC** 标志的状态。尝试更改这些标志的状态会被静默忽略。

**F_GETOWN**

在某些架构（特别是 i386）上，Linux 系统调用约定的一个限制意味着：如果 **F_GETOWN** 要返回的（负数）进程组 ID 落在 -1 到 -4095 范围内，则 glibc 会错误地将返回值解释为系统调用错误；也就是说，**fcntl**() 的返回值将是 -1，并且 *errno* 将包含（正数）进程组 ID。Linux 特有的 **F_GETOWN_EX** 操作可以避免此问题。从 glibc 2.11 版本开始，glibc 通过使用 **F_GETOWN_EX** 实现 **F_GETOWN**，使得内核的 **F_GETOWN** 问题不可见。

**F_SETOWN**

在 Linux 2.4 及更早版本中，存在一个缺陷：当非特权进程使用 **F_SETOWN** 将套接字文件描述符的所有者指定为调用者之外的进程（组）时，可能会发生。在这种情况下，**fcntl**() 可能返回 -1 并将 *errno* 设置为 **EPERM**，即使调用者有权向其发送信号的所有者进程（组）也是如此。尽管返回了此错误，但文件描述符的所有者仍被设置，并且信号将被发送给所有者。

**死锁检测**

内核在处理 **F_SETLKW** 请求时采用的死锁检测算法可能会产生假阴性（未能检测到死锁，使一组死锁的进程无限期阻塞）和假阳性（**EDEADLK** 错误，而实际上没有死锁）。例如，内核将其依赖项搜索的锁深度限制为 10 步，这意味着超过该大小的循环死锁链将不会被检测到。此外，当使用 [clone(2)](https://man.cx/clone(2)) **CLONE_FILES** 标志创建的两个或多个进程放置的锁在（内核看来）冲突时，内核可能会错误地指示死锁。

**强制锁定**

Linux 强制锁定的实现存在竞态条件，使其不可靠：与锁重叠的 [write(2)](https://man.cx/write(2)) 调用可能会在强制锁获取后修改数据；与锁重叠的 [read(2)](https://man.cx/read(2)) 调用可能会检测到仅在写锁获取后才发生的数据更改。强制锁定和 [mmap(2)](https://man.cx/mmap(2)) 之间也存在类似的竞态。因此，不建议依赖强制锁定。

<br/>

### ioctl

#### 参考

[ioctl(2) - Linux manual page](https://man7.org/linux/man-pages/man2/ioctl.2.html) / [man.cx manual pages](https://man.cx/ioctl%282%29)

#### 库

标准 C 库 (libc, -lc)

#### 原型

```c
#include <sys/ioctl.h>

int ioctl(int fd, unsigned long request, ...);
```

#### 描述

`ioctl()` 系统调用用于操作特殊文件的底层设备参数。特别地，字符特殊文件（例如终端）的许多操作特性都可以通过 `ioctl()` 请求来控制。参数 `fd` 必须是一个已打开的文件描述符。第二个参数是与设备相关的请求代码。第三个参数是一个无类型的指针，指向内存地址。传统上它被定义为 `char *argp`（在 `void *` 成为合法 C 语法之前），因此下文将沿用此命名约定。

一个 `ioctl()` 请求中已编码了参数是输入参数还是输出参数，以及参数 `argp` 的大小（以字节为单位）。用于指定 `ioctl()` 请求的宏和定义位于 `<sys/ioctl.h>` 头文件中。详细信息请参见 [NOTES](#heading7) 部分。

#### 返回值

通常，成功时返回 0。少数 `ioctl()` 请求使用返回值作为输出参数，并在成功时返回一个非负值。出错时，返回 -1，并设置相应的 `errno`。

##### 错误码

*   **EBADF**：`fd` 不是有效的文件描述符。
*   **EFAULT**：`argp` 引用了不可访问的内存区域。
*   **EINVAL**：`request` 或 `argp` 无效。
*   **ENOTTY**：`fd` 未关联到字符特殊设备。
*   **ENOTTY**：指定的 `request` 不适用于文件描述符 `fd` 所引用的对象类型。

#### 遵循标准

没有单一标准。`ioctl()` 的参数、返回值及其语义因具体的设备驱动程序而异（该调用被用作所有无法完美契合 UNIX 流 I/O 模型的操作的“万能”接口）。`ioctl()` 系统调用出现在第七版 AT&T UNIX 中。

#### 注意

为了使用此调用，需要一个已打开的文件描述符。通常，[open(2)](https://man.cx/open%282%29) 调用会产生不必要的副作用，在 Linux 下可以通过为其指定 `O_NONBLOCK` 标志来避免这些副作用。

##### ioctl 结构

Ioctl 命令值是 32 位常量。原则上，这些常量是完全任意的，但人们试图在其中构建一些结构。旧的 Linux 情况主要是 16 位常量，其中最后一个字节是序列号，前面的字节给出了指示驱动程序的类型。有时会使用主编号：`0x03` 用于 `HDIO_*` ioctl，`0x06` 用于 `LP*` ioctl。有时也会使用一个或多个 ASCII 字母。例如，`TCGETS` 的值为 `0x00005401`，其中 `0x54 = 'T'` 表示终端驱动程序；`CYGETTIMEOUT` 的值为 `0x00435906`，其中 `0x43 0x59 = 'C' 'Y'` 表示 cyclades 驱动程序。

后来（0.98p5 版本之后），数字中内建了更多的信息。它包含 2 个方向位（00：无，01：写，10：读，11：读/写），接着是 14 个大小位（给出参数的大小），然后是一个 8 位类型（将 ioctl 按共同用途或共同驱动程序分组），以及一个 8 位序列号。描述此结构的宏位于 `<asm/ioctl.h>` 中，它们是 `_IO(type,nr)`、`_IOR(type,nr,size)`、`_IOW(type,nr,size)` 和 `_IOWR(type,nr,size)`。它们使用 `sizeof(size)`，因此这里 `size` 这个名称有些用词不当：这第三个参数实际上是一个数据类型。

请注意，大小位非常不可靠：在很多情况下它们都是错误的，要么是因为使用了 `sizeof(sizeof(struct))` 的错误宏，要么是因为遗留值。因此，新的结构似乎只带来了缺点：它无助于检查，但会导致不同架构上的值不同。

<br/>

### socketpair

#### 参考

[socketpair(2) - Linux manual page](https://man7.org/linux/man-pages/man2/socketpair.2.html) / [man.cx manual pages](https://man.cx/socketpair%282%29)

#### 原型

```c
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>

int socketpair(int domain, int type, int protocol, int sv[2]);
```

#### 描述
`socketpair()` 调用会在指定的通信域 (`domain`) 中，使用指定的类型 (`type`) 和可选的协议 (`protocol`)，创建一对**未命名的、已连接的套接字**。
用于引用新套接字的文件描述符会返回到 `sv[0]` 和 `sv[1]` 中。这两个套接字是无差别的（即相互对等）。

关于这些参数（`domain`、`type`、`protocol`）的更多细节，请参阅 [socket()](#socket)。

#### 返回值
成功时，返回零。
失败时，返回 -1，并设置相应的 `errno`，并且 `sv` 数组的内容保持不变。在 Linux（及其他系统）上，`socketpair()` 在失败时不会修改 `sv`。POSIX.1-2008 TC2 添加了标准化此行为的要求。

##### 错误码

*   **EAFNOSUPPORT**：本机不支持指定的地址族。
*   **EFAULT**：地址 `sv` 没有指定进程地址空间中的有效部分。
*   **EMFILE**：已达到此进程对打开文件描述符数量的限制。
*   **ENFILE**：已达到系统范围内打开文件总数的限制。
*   **EOPNOTSUPP**：指定的协议不支持创建套接字对。
*   **EPROTONOSUPPORT**：本机不支持指定的协议。

#### 遵循标准
POSIX.1-2001, POSIX.1-2008, 4.4BSD。

`socketpair()` 首次出现在 4.2BSD 中。它通常可以在支持 BSD 套接字层克隆的非 BSD 系统（包括 System V 变体）之间移植。

#### 注意
在 Linux 系统上，此调用支持的域只有 `AF_UNIX`（或其同义词 `AF_LOCAL`）和 `AF_TIPC`（自 Linux 4.12 起）。

从 Linux 2.6.27 开始，`socketpair()` 支持在 `type` 参数中使用 `SOCK_NONBLOCK` 和 `SOCK_CLOEXEC` 标志，如 [socket()](#socket) 中所述。

POSIX.1 不要求包含 `<sys/types.h>`，并且在 Linux 上也不需要此头文件。然而，一些历史（BSD）实现需要此头文件，因此可移植的应用程序最好包含它。

<br/>

### sendfile

#### 参考

[sendfile(2) - Linux manual page](https://man7.org/linux/man-pages/man2/sendfile.2.html) / [man.cx manual pages](https://man.cx/sendfile%282%29)

#### 原型

```c
#include <sys/sendfile.h>

ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
```

#### 函数描述

`sendfile()` 函数在一个文件描述符与另一个文件描述符之间复制数据。由于这种复制操作是在内核内部完成的，因此 `sendfile()` 比结合使用 [read(2)](https://man.cx/read%282%29) 和 [write(2)](https://man.cx/write%282%29) 更为高效，后两种方式需要在用户空间与内核空间之间来回传输数据。

`in_fd` 参数应该是一个以读方式打开的文件描述符，`out_fd` 参数应该是一个以写方式打开的文件描述符。

如果 `offset` 不为 `NULL`，则它指向一个变量，该变量保存了 `sendfile()` 从 `in_fd` 读取数据的起始文件偏移量。当 `sendfile()` 返回时，该变量将被设置为所读取的最后一个字节之后的下一个字节的偏移量。

如果 `offset` 不为 `NULL`，则 `sendfile()` 不会修改 `in_fd` 本身的文件偏移量；否则，文件偏移量会被调整，以反映从 `in_fd` 读取的字节数。

如果 `offset` 为 `NULL`，则数据将从 `in_fd` 当前的**文件偏移量**处开始读取，并且该文件偏移量会在此次调用后被更新。

`count` 参数是要在两个文件描述符之间复制的字节数。

`in_fd` 参数必须对应于一个支持类似 [mmap(2)](https://man.cx/mmap%282%29) 操作的文件（即，它不能是一个套接字）。

在 2.6.33 之前的 Linux 内核版本中，`out_fd` 必须引用一个套接字。自 Linux 2.6.33 起，`out_fd` 可以是任意类型的文件。如果它是一个普通文件，则 `sendfile()` 会适当地更改该文件的偏移量。

#### 返回值

如果传输成功，将返回写入 `out_fd` 的字节数。请注意，一次成功的 `sendfile()` 调用写入的字节数可能少于请求的字节数；如果有未发送的字节，调用者应准备重试调用。另请参见**注意**部分。

如果出错，则返回 -1，并适当设置 `errno`。

##### 错误码

| 错误代码      | 描述                                                         |
| :------------ | :----------------------------------------------------------- |
| **EAGAIN**    | 已通过 `O_NONBLOCK` 选择了非阻塞 I/O，并且写入操作会被阻塞。 |
| **EBADF**     | 输入文件未以读方式打开，或输出文件未以写方式打开。           |
| **EFAULT**    | 地址错误。                                                   |
| **EINVAL**    | 描述符无效或被锁定，或者 `in_fd` 不支持类似 `mmap()` 的操作，或者 `count` 为负数。 |
| **EINVAL**    | `out_fd` 设置了 `O_APPEND` 标志，`sendfile()` 当前不支持此情况。 |
| **EIO**       | 从 `in_fd` 读取时发生未指定的错误。                          |
| **ENOMEM**    | 从 `in_fd` 读取时内存不足。                                  |
| **EOVERFLOW** | `count` 值过大，该操作将导致超出输入文件或输出文件的最大大小。 |
| **ESPIPE**    | `offset` 不为 `NULL`，但输入文件不可定位。                   |

#### 版本

`sendfile()` 首次出现在 Linux 2.2 中。自 glibc 2.1 起，引入了头文件 `<sys/sendfile.h>`。

#### 遵循标准

POSIX.1-2001 或其他标准中均未对其进行规定。其他 UNIX 系统实现了 `sendfile()`，但具有不同的语义和函数原型。它不应用于可移植的程序中。

#### 注意

`sendfile()` 最多传输 `0x7ffff000` (2,147,479,552) 个字节，并返回实际传输的字节数。（这在 32 位和 64 位系统上均适用。）

如果你打算使用 `sendfile()` 将文件发送到 TCP 套接字，但需要在文件内容之前发送一些头部数据，你会发现使用 [tcp(7)](https://man.cx/tcp%287%29) 中描述的 `TCP_CORK` 选项非常有用，可以最大限度地减少数据包数量并调整性能。

在 Linux 2.4 及更早版本中，`out_fd` 也可以是一个普通文件；在 Linux 2.6.x 内核系列中，这种可能性消失了，但在 Linux 2.6.33 中又得以恢复。

最初的 Linux `sendfile()` 系统调用并非为处理大文件偏移量而设计。因此，Linux 2.4 增加了 `sendfile64()`，它为偏移量参数提供了更宽的类型。glibc 的 `sendfile()` 包装函数会透明地处理内核差异。应用程序可能希望在 `sendfile()` 因 `EINVAL` 或 `ENOSYS` 失败时，回退到使用 [read(2)](https://man.cx/read%282%29) 和 [write(2)](https://man.cx/write%282%29)。

如果 `out_fd` 指的是一个支持零拷贝的套接字或管道，调用者必须确保 `in_fd` 所引用的文件中被传输的部分保持未被修改，直到 `out_fd` 另一端的读取者消费了已传输的数据。

Linux 特有的 [splice()](#splice) 调用支持在任意两个文件描述符之间传输数据，前提是其中至少有一个是管道。

<br/>

### splice

#### 参考

[splice(2) - Linux manual page](https://man7.org/linux/man-pages/man2/splice.2.html) / [man.cx manual pages](https://man.cx/splice%282%29)

#### 原型

```c
#define _GNU_SOURCE /* See feature_test_macros(7) */
#include <fcntl.h>

ssize_t splice(int fd_in, loff_t *off_in, int fd_out,
loff_t *off_out, size_t len, unsigned int flags);
```

#### 描述

`splice()` 系统调用在两个文件描述符之间搬移数据，**无需在**内核地址空间和用户地址空间之间进行拷贝。它从文件描述符 `fd_in` 向 `fd_out` 传输最多 `len` 字节的数据，**并且两个文件描述符中必须有一个指向管道**。

以下语义适用于 `fd_in` 和 `off_in`：

*   如果 `fd_in` 指向一个管道，则 `off_in` 必须为 `NULL`。
*   如果 `fd_in` 不指向管道，且 `off_in` 为 `NULL`，则从 `fd_in` 的当前文件偏移量开始读取字节，并且文件偏移量会被适当调整。
*   如果 `fd_in` 不指向管道，且 `off_in` 不为 `NULL`，则 `off_in` 必须指向一个缓冲区，该缓冲区指定了从 `fd_in` 读取数据的起始偏移量；在这种情况下，`fd_in` 的文件偏移量不会改变，但 `off_in` 指向的偏移量会被更新。

类似的规则适用于 `fd_out` 和 `off_out`。

`flags` 参数是一个位掩码，通过 OR 操作组合零个或多个以下值：

*   `SPLICE_F_MOVE`：尝试移动页面而非拷贝。这只是一个提示：如果内核无法从管道移动页面，或管道缓冲区未指向完整页面，则仍可能发生拷贝。此标志的最初实现存在 bug，因此在 Linux 2.6.21 中它被设为空操作（但 `splice()` 调用中仍允许使用）；未来可能会恢复正确的实现。
*   `SPLICE_F_NONBLOCK`：不要阻塞 I/O。这使 `splice` 管道操作变为非阻塞，但 `splice()` 仍可能阻塞，因为其操作的源/目标文件描述符可能会阻塞（除非它们设置了 `O_NONBLOCK` 标志）。
*   `SPLICE_F_MORE`：后续 `splice` 调用将提供更多数据。当 `fd_out` 指向一个套接字时，这是一个有用的提示（另请参见 [send()](#sendmsg) 中对 `MSG_MORE` 的描述，以及 [tcp(7)](https://man.cx/tcp%287%29) 中对 `TCP_CORK` 的描述）。
*   `SPLICE_F_GIFT`：对 `splice()` 无用，请参见 [vmsplice(2)](https://man.cx/vmsplice%282%29)。

#### 返回值

成功完成后，`splice()` 返回搬移到管道或从管道搬移出的字节数。返回 0 表示没有数据可传输（输入结束）。如果 `fd_in` 指向一个管道，这意味着没有数据可传输，且由于没有写入者连接到管道的写入端，阻塞是毫无意义的。出错时，`splice()` 返回 -1，并设置相应的 `errno`。

##### 错误码

| 错误       | 描述                                                         |
| :--------- | :----------------------------------------------------------- |
| **EAGAIN** | 在 `flags` 中指定了 `SPLICE_F_NONBLOCK`，或其中一个文件描述符已被标记为非阻塞（`O_NONBLOCK`），但操作仍会阻塞。 |
| **EBADF**  | 一个或两个文件描述符无效，或者没有正确的读/写模式。          |
| **EINVAL** | 目标文件系统不支持 splice 操作。                             |
| **EINVAL** | 目标文件以追加模式打开。                                     |
| **EINVAL** | 两个文件描述符都不指向管道。                                 |
| **EINVAL** | 为不可定位的设备（例如管道）指定了偏移量。                   |
| **EINVAL** | `fd_in` 和 `fd_out` 指向同一个管道。                         |
| **ENOMEM** | 内存不足。                                                   |
| **ESPIPE** | `off_in` 或 `off_out` 不为 `NULL`，但对应的文件描述符指向一个管道。 |

#### 版本

`splice()` 系统调用首次出现在 Linux 2.6.17 中；glibc 从 2.5 版本开始提供库支持。

#### 遵循标准

此系统调用是 Linux 特有的。

#### 注意

这三个系统调用 `splice()`、[vmsplice(2)](https://man.cx/vmsplice%282%29) 和 [tee(2)](https://man.cx/tee%282%29) 为用户空间程序提供了对一个任意内核缓冲区的完全控制，该缓冲区在内核中使用与管道相同的缓冲区类型实现。概括来说，这些系统调用执行以下任务：

*   `splice()` 将数据从缓冲区移动到任意文件描述符，或反向移动，或从一个缓冲区移动到另一个缓冲区。
*   [tee(2)](https://man.cx/tee%282%29) “拷贝”数据从一个缓冲区到另一个。
*   [vmsplice(2)](https://man.cx/vmsplice%282%29) “拷贝”数据从用户空间到缓冲区。

尽管我们谈论的是拷贝，但实际拷贝通常是被避免的。内核通过将管道缓冲区实现为一组指向内核内存页的引用计数指针来完成这一点。内核通过在输出缓冲区中创建指向这些页面的新指针，并增加页面的引用计数来创建缓冲区中页面的“拷贝”：只有指针被拷贝，而不是缓冲区的页面本身。

在 Linux 2.6.30 及更早版本中，`fd_in` 和 `fd_out` 中必须恰好有一个指向管道。从 Linux 2.6.31 开始，两个参数都可以指向管道。

在 32 位平台上使用非空偏移量时，建议定义 `_FILE_OFFSET_BITS` 为 64 以支持大文件操作。

<br/>

## Echo example (UDP)

### 流程图

![echo-udp](/resources/linux/network-programming/echo-udp.png)

### Server

服务端代码（udp_echo_server.c）

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // 创建 UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 绑定地址和端口
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Echo Server listening on port %d\n", PORT);

    while (1) {
        // 接收客户端消息
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                             (struct sockaddr*)&client_addr, &client_len);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }
        buffer[n] = '\0';
        printf("Received from %s:%d: %s\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

        // 原样发送回去
        sendto(sockfd, buffer, n, 0, (struct sockaddr*)&client_addr, client_len);
    }

    close(sockfd);
    return 0;
}
```



### Client

客户端代码（udp_echo_client.c）

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char *message;

    // 默认消息或从命令行参数获取
    if (argc > 1) {
        message = argv[1];
    } else {
        message = "Hello, UDP Echo!";
    }

    // 创建 UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 发送消息到服务器
    ssize_t sent = sendto(sockfd, message, strlen(message), 0,
                          (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (sent < 0) {
        perror("sendto");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 接收回显
    socklen_t server_len = sizeof(server_addr);
    ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                                (struct sockaddr*)&server_addr, &server_len);
    if (recv_len < 0) {
        perror("recvfrom");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    buffer[recv_len] = '\0';
    printf("Server echo: %s\n", buffer);

    close(sockfd);
    return 0;
}
```

### 编译运行

```bash
# 编译服务端和客户端
gcc -o udp_server udp_echo_server.c
gcc -o udp_client udp_echo_client.c

# 运行服务端（先启动）
./udp_server

# 另开终端运行客户端
./udp_client "Hello, World!"
# 或不带参数使用默认消息
./udp_client
```

<br/>

## Echo example (TCP)

### 流程图

![echo-tcp](/resources/linux/network-programming/echo-tcp.png)

### Server

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int listen_fd, conn_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // 创建监听 socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 绑定地址和端口
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // 监听
    if (listen(listen_fd, 5) < 0) {
        perror("listen");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    printf("TCP Echo Server listening on port %d\n", PORT);

    while (1) {
        // 接受客户端连接
        conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
        if (conn_fd < 0) {
            perror("accept");
            continue;
        }
        printf("New connection from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // 处理客户端数据（echo）
        ssize_t n;
        while ((n = recv(conn_fd, buffer, BUFFER_SIZE, 0)) > 0) {
            send(conn_fd, buffer, n, 0);   // 原样返回
        }
        if (n < 0) {
            perror("recv");
        }
        close(conn_fd);
        printf("Connection closed\n");
    }

    close(listen_fd);
    return 0;
}
```

### Client

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char *message;

    // 默认消息或从命令行参数获取
    if (argc > 1) {
        message = argv[1];
    } else {
        message = "Hello, TCP Echo!";
    }

    // 创建 socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 连接服务器
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 发送消息
    ssize_t sent = send(sock_fd, message, strlen(message), 0);
    if (sent < 0) {
        perror("send");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 接收回显（注意：TCP 流式可能一次收不完，但这里消息短，可以一次读完）
    ssize_t recv_len = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
    if (recv_len < 0) {
        perror("recv");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    buffer[recv_len] = '\0';
    printf("Server echo: %s\n", buffer);

    close(sock_fd);
    return 0;
}
```

### 编译与运行

```bash
# 编译服务端和客户端
gcc -o tcp_server tcp_echo_server.c
gcc -o tcp_client tcp_echo_client.c

# 运行服务端（先启动）
./tcp_server

# 另开终端运行客户端
./tcp_client "Hello, TCP!"
# 或不带参数使用默认消息
./tcp_client
```

<br/>

## 参考

* [Linux man pages online](https://man7.org/linux/man-pages/index.html)
* [Linux Manpages Online - man.cx manual pages](https://man.cx/)

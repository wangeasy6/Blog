---
title: Linux I/O 多路复用之 select
toc: true
date: 2026-05-08 23:00:00
categories:
  - tech
tags:
  - C
  - 嵌入式
---

在 Socket 出现之前，网络编程需要直接处理复杂的 TCP/IP 协议栈细节，不同厂商的 API 也不尽相同。

**1983 年**，加利福尼亚大学伯克利分校在发布的 **4.2BSD**（Berkeley Software Distribution）版本 Unix 系统中，做了一件影响深远的事：**正式引入了 Socket（套接字）接口**。

有了 Socket，但早期的网络程序很快遇到了新问题：如何处理多个连接？最初的模型是“阻塞 I/O”，一个线程只能处理一个连接。当它从一个 socket 读取数据时，如果没有数据，整个程序就会卡在那里，无法响应其他连接。

为了解决这个问题，**select 系统调用** 在 1983 年随着 4.2BSD 一同被创造了出来。

**select 系统调用** 允许程序同时监视多个文件描述符，检测它们的状态变化（如数据可读或可写），从而高效地管理多个 I/O 操作而不需为每个操作创建独立线程或进程。这种能力尤其在网络通信和服务器编程中提高了并发性能。**select 系统调用** 还支持非阻塞 I/O，允许程序在等待 I/O 事件的同时执行其他任务，有效利用 CPU 资源。

自 `select()` 面世以来，在各种 Unix 变体（System V、AIX、SunOS、HP-UX 等）中被广泛实现，但每个系统的实现细节略有差异。**1990** 年，POSIX 标准（POSIX.1-1990）统一了相关规定。

<!--more-->

<br/>

## 函数原型

[Linux manual page - select()](https://man7.org/linux/man-pages/man2/select.2.html)

```c
#include <sys/select.h>

typedef /* ... */ fd_set;

int select(int nfds, fd_set *_Nullable restrict readfds,
          fd_set *_Nullable restrict writefds,
          fd_set *_Nullable restrict exceptfds,
          struct timeval *_Nullable restrict timeout);

void FD_CLR(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
int  FD_ISSET(int fd, const fd_set *set);
void FD_ZERO(fd_set *set);

int pselect(int nfds, fd_set *_Nullable restrict readfds,
          fd_set *_Nullable restrict writefds,
          fd_set *_Nullable restrict exceptfds,
          const struct timespec *_Nullable restrict timeout,
          const sigset_t *_Nullable restrict sigmask);

pselect():
   _POSIX_C_SOURCE >= 200112L
```

* **_Nullable**：用于告诉读者它们可以传递一个 `NULL` 指针，但是头文件中的实际声明是不包含 `_Nullable` 的。
* **restrict**：限制传入参数不能存在其他引用。

### select

参数解释如下：

- **nfds**：是监控的文件描述符集合中最大文件描述符的值加 1（小于 FD_SETSIZE(1024)）。在使用 select() 函数时，必须确保这个参数被正确设置，以便函数能监视所有相关的文件描述符。

- **readfds**、**writefds**、**exceptfds**：分别代表可读、可写和异常监视的文件描述符集合。它们使用 [fd_set](#fd-set) 类型表示。

- **timeout**：一个指向`timeval` 结构的指针，该结构用于设定 `select` 等待 I/O 事件的超时时间。

  `timeout` 的设定有 3 种情况：

  - 当 `timeout` 为 `NULL` 时，`select` 会无限等待，直到至少有一个文件描述符就绪。
  - 当 `timeout` 设置为 0（即 `tv_sec` 和 `tv_usec` 都为 0）时，`select` 会立即返回，用于轮询。
  - 设置具体的时间，`select` 将等待直到该时间过去或者有文件描述符就绪。

  `timeout` 结构定义如下：

  ```c
  struct timeval {
      long tv_sec;  // seconds
      long tv_usec; // microseconds
  };
  ```

- **Return** 有 3 种可能：

  - 大于 0：表示就绪的文件描述符数量，即有多少文件描述符已经准备好进行 I/O 操作；
  - 等于 0：表示超时，没有文件描述符在指定时间内就绪；
  - 小于 0：发生错误。错误发生时，应使用 `perror` 或 `strerror` 函数来获取具体的错误信息。

在使用 `select()` 函数后，通过检查 `readfds`、`writefds` 和 `exceptfds` 集合的变化，可以精确地知道哪些文件描述符已经准备好进行读、写或异常处理。

在 Linux 上，`select()`会修改超时值以反映未休眠的时间量；而大多数其他实现则不会这样做。（POSIX.1允许这两种行为中的任何一种。）所以，在循环中多次使用`select()` 需要重新初始化 `timeout`。

### fd_set

`fd_set` 是一种通过位来管理文件描述符的数据结构。（定义见 `<sys/select.h>`）

以下是对 `fd_set` 操作的常用宏定义：

- `FD_SET(fd, &set)`：将文件描述符（fd）添加到集合（set）中。
- `FD_CLR(fd, &set)`：从集合（set）中移除文件描述符（fd）。
- `FD_ISSET(fd, &set)`：检查文件描述符（fd）是否已被加入集合（set）。
- `FD_ZERO(&set)`：清空集合（set）中的所有文件描述符。

### pselect

`pselect()` 系统调用允许应用程序安全地等待，直到文件描述符就绪或捕获到信号。

`select()` 和 `pselect()` 的操作基本相同，除了以下三个区别：

* `select()` 使用的 `timeout` 参数是一个 `struct timeval` 结构体（包含秒和微秒），而 `pselect()` 使用的 `timeout` 参数是一个 `struct timespec` 结构体（包含秒和纳秒）。
* `select()` 可能会更新 `timeout` 参数以指示剩余的时间。而 `pselect()` 则不会更改此参数。
* `select()` 没有 `sigmask` 参数，其行为与调用 `NULL` `sigmask` 的 `pselect()` 相同。

`sigmask` 是一个指向信号掩码的指针（参见 [sigprocmask(2)](https://man7.org/linux/man-pages/man2/sigprocmask.2.html)）。如果它不为 `NULL`，则 `pselect()` 函数首先会用 `sigmask` 所指向的信号掩码替换当前的信号掩码，然后执行 `select` 函数，最后恢复原始信号掩码。

```c
ready = pselect(nfds, &readfds, &writefds, &exceptfds, timeout, &sigmask);
```

 相当于原子性地执行以下调用：

```c
sigset_t origmask;

pthread_sigmask(SIG_SETMASK, &sigmask, &origmask);
ready = select(nfds, &readfds, &writefds, &exceptfds, timeout);
pthread_sigmask(SIG_SETMASK, &origmask, NULL);
```

需要 `pselect()` 的原因是，如果想要等待信号或文件描述符就绪，那么就需要进行原子测试以防止出现竞争条件。（假设信号处理程序设置了一个全局标志并返回。那么，如果信号在测试之后但在调用 `select()` 之前到达，那么在测试这个全局标志之后调用 `select()` 可能会无限期挂起。相比之下，`pselect()` 允许先阻塞信号，处理已到来的信号，然后使用所需的 `sigmask` 调用 `pselect()`，从而避免竞争。)

<br/>

## 使用示例

以下是一个简单的示例，展示如何使用 `select()` 实现一个仅返回固定页面的 http 服务器。

```c++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8080
#define MAX_CLIENTS 500
#define BUFFER_SIZE 4096
#define BACKLOG_N 1024

int create_tcp_server(){
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    
    // 创建socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // 设置socket选项，允许端口重用
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // 绑定socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // 开始监听
    if (listen(server_fd, BACKLOG_N) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("HTTP服务器已启动，监听端口 %d...\n", PORT);
    return server_fd;
}

void send_http_response(int client_fd) {
    char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "    <title>Simple HTTP server(select)</title>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>Hello from Select HTTP Server!</h1>\n"
        "    <p>This is a simple HTTP server written in C language</p>\n"
        "    <p>Limit on client connections: %d</p>\n"
        "</body>\n"
        "</html>";
    
    char full_response[512];
    int len = snprintf(full_response, sizeof(full_response), response, MAX_CLIENTS);
    
    write(client_fd, full_response, len);
}

int main() {
    int server_fd = create_tcp_server();
    int new_socket, i;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    
    // 初始化客户端socket数组
    fd_set readfds;
    int max_sd, sd, activity, valread;
    int client_sockets[MAX_CLIENTS];
    for (i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = -1;
    }
    
    while (1) {
        // 清空文件描述符集合
        FD_ZERO(&readfds);
        
        // 添加服务器socket到集合
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;
        
        // 添加客户端sockets到集合
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            
            if (sd > max_sd) {
                max_sd = sd;
            }
        }
        
        // 等待活动（使用select）
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        
        if ((activity < 0) && (errno != EINTR)) {
            printf("select error\n");
        }
        
        // 检查是否有新的连接
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                                     (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            
            printf("新连接，socket fd: %d, IP: %s, 端口: %d\n", 
                   new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            
            // 添加新socket到客户端数组
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == -1) {
                    client_sockets[i] = new_socket;
                    printf("添加到位置 %d\n", i);
                    break;
                }
            }
        }
        
        // 处理客户端请求
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            
            if (FD_ISSET(sd, &readfds)) {
                // 检查是否是连接关闭
                if ((valread = read(sd, buffer, BUFFER_SIZE)) == 0) {
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("客户端断开连接，IP: %s, 端口: %d\n", 
                           inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                    
                    close(sd);
                    client_sockets[i] = -1;
                } else {
                    // 处理HTTP请求
                    buffer[valread] = '\0';
                    printf("收到请求:\n%s\n", buffer);
                    
                    // 简单的HTTP响应
                    send_http_response(sd);
                    close(sd);
                    client_sockets[i] = -1;
                }
            }
        }
    }
    
    return 0;
}
```

在这个示例中，利用 `select ` 系统调用实现了对多个客户端连接的**I/O多路复用**。它能够交替处理新的连接请求和已连接客户端的数据收发，而不必为每个客户端创建单独的线程或进程。其主要工作流程如下：

1. **初始化与绑定**：服务器首先创建一个 TCP socket，设置端口重用选项，将其绑定到本地所有地址的 8080 端口，并开始监听连接请求。
2. **主循环与监控**：服务器进入一个无限循环。在每次循环中，它会构建一个需要监控的文件描述符（fd）集合。这个集合始终包含服务器自身的监听 socket，以及所有当前已建立连接的客户端 socket。之后，它调用 `select`函数阻塞等待，直到集合中有任何一个 socket 变为“可读”状态（即有新的连接请求或客户端发来数据）。
3. **处理新连接**：当 `select`返回后，程序首先检查监听 socket 是否有活动（`FD_ISSET`）。如果有，表示有新的客户端尝试连接，服务器会调用 `accept`接受连接，生成一个新的客户端 socket，并将其信息（IP、端口）打印出来，同时将这个新的客户端 socket 描述符存储到一个数组中以便后续管理。
4. **处理客户端数据**：接着，程序遍历所有已记录的客户端 socket。检查每个客户端 socket 是否在本次 `select`返回的可读集合中。 **连接关闭**：如果某个客户端 socket 可读，但调用 `read`读取到的数据长度为 0，则表示客户端已主动关闭连接。服务器随后也会关闭对应的 socket 并清理其在数组中的记录。 **HTTP请求与响应**：如果 `read`读取到了数据，服务器将其视为一个 HTTP 请求。它会将请求内容打印到控制台，然后发送一个预设的、简单的 HTTP 响应报文。这个响应报文体是一个 HTML 页面，其中包含固定的欢迎信息。**在发送响应后，服务器会立即关闭与该客户端的连接**，这是一种短连接（Connection: close）模式。
5. **循环处理**：处理完所有活动 socket 后，程序跳回步骤 2，重新构建监控集合并调用 `select`，等待下一轮网络事件。

![select_drawio](/resources/linux/altio/select_drawio.png)

<br/>

## 实现原理

select的核心实现原理是 **同步轮询**。

**工作流程：**

1. **用户空间到内核的复制**：当调用 `select`时，内核会将用户传入的三个 `fd_set`集合（包含所有待监视的描述符）从用户空间复制到内核空间。
2. **内核轮询**：内核**线性遍历**这 `nfds`个描述符，检查每个描述符的当前状态是否满足用户指定的条件（可读、可写、异常）。这是一个 **O(n)** 复杂度的操作。
3. **修改与返回**：内核会**修改**传入的 `fd_set`集合，将其“清零”，只保留那些**就绪**的描述符。然后将修改后的集合**复制回用户空间**。
4. **用户空间遍历**：用户程序需要**再次线性遍历**初始传入的所有描述符，并使用 `FD_ISSET`宏检查哪些描述符在返回的集合中，以确定哪些描述符真正就绪。

**主要局限性：**

1. **文件描述符数量限制**：`fd_set`是一个固定大小的位图（通常为1024位，由 `FD_SETSIZE`宏定义），意味着进程能监视的描述符数量有上限（通常是1024个）。
2. **效率低下**： **两次复制**：每次调用都需要在用户空间和内核空间之间复制整个描述符集合，开销大。 **两次线性遍历**：内核和用户程序都需要遍历所有描述符。即使只有1个描述符就绪，也要遍历整个集合。
3. **状态非保留**：`select`是“水平触发”的，但每次调用后，内核都会修改并返回新的就绪集合。程序必须**重新设置**所有感兴趣的描述符集合，不能保留之前的监视状态，这增加了编程的复杂性和出错概率。

<br/>

## 参考

* 《Linux/UNIX 系统编程手册》第63章
* [C++ select()函数的用法（附带实例） - C语言中文网](https://c.biancheng.net/view/p0ktaaz.html)

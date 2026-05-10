#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#define PORT 8080
#define MAX_CLIENTS 1000
#define BUFFER_SIZE 4096
#define BACKLOG_N 1024
#define MAX_EVENTS (MAX_CLIENTS+1)

// 客户端连接状态结构体
typedef struct {
    char buffer[BUFFER_SIZE];   // 缓冲区
    int buffer_len;             // 缓冲区中数据长度
} client_t;

int set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return -1;
    }

    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        return -1;
    }

    return 0;
}

int create_tcp_server(){
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // 创建socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == 0) {
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
        "    <title>Simple HTTP server(epoll)</title>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>Hello from Epoll HTTP Server!!</h1>\n"
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
    struct sockaddr_in client_addr;
    struct epoll_event ev, events[MAX_EVENTS];
    socklen_t client_addr_len = sizeof(client_addr);

    // 创建epoll实例
    int epoll_fd;
    if ((epoll_fd = epoll_create1(0)) < 0) {
        perror("epoll_create1 failed");
        exit(EXIT_FAILURE);
    }

    // 添加服务器socket到epoll
    ev.events = EPOLLIN | EPOLLET;  // 边缘触发模式
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) < 0) {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }

    client_t clients;

    // 主循环
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            if (errno == EINTR) {
                //continue;  // 被信号中断，继续
                break;
            }
            perror("epoll_wait failed");
            break;
        }

        for (int i = 0; i < nfds; i++) {
            // 处理新连接
            if (events[i].data.fd == server_fd) {
                while (1) {
                    int client_fd = accept4(server_fd, (struct sockaddr *)&client_addr,
                                           &client_addr_len, SOCK_NONBLOCK);

                    if (client_fd < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            // 没有更多待处理的连接
                            break;
                        }
                        perror("accept4 failed");
                        break;
                    }

                    printf("新连接，socket fd: %d, IP: %s, 端口: %d\n",
                           client_fd, inet_ntoa(client_addr.sin_addr),
                           ntohs(client_addr.sin_port));

                    // 设置非阻塞模式
                    if (set_nonblocking(client_fd) < 0) {
                        close(client_fd);
                        continue;
                    }

                    // 添加客户端到epoll
                    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                    ev.data.fd = client_fd;

                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
                        perror("epoll_ctl: client_fd");
                        close(client_fd);
                        continue;
                    }
                }
            }
            // 处理客户端事件
            else {
                int client_fd = events[i].data.fd;

                // 检查连接是否关闭或出错
                if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                    printf("连接关闭或出错，socket fd: %d, errno: %#x\n", client_fd, events[i].events);

                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                    continue;
                }

                // 处理可读事件
                if (events[i].events & EPOLLIN) {
                    ssize_t bytes_read;

                    // 边缘触发模式下，需要一次性读取所有数据
                    while (1) {
                        bytes_read = read(client_fd,
                                         clients.buffer + clients.buffer_len,
                                         BUFFER_SIZE - clients.buffer_len - 1);

                        if (bytes_read > 0) {
                            clients.buffer_len += bytes_read;

                            // 检查是否收到完整的HTTP请求（简单判断是否有空行）
                            clients.buffer[clients.buffer_len] = '\0';
                            if (strstr(clients.buffer, "\r\n\r\n") != NULL) {
                                printf("收到完整HTTP请求 (socket %d):\n%s\n",
                                       client_fd, clients.buffer);

                                // 发送HTTP响应
                                send_http_response(client_fd);

                                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                                close(client_fd);

                                memset( clients.buffer, '\0', clients.buffer_len );
                                clients.buffer_len = 0;
                                break;
                            }

                            // 如果缓冲区满了，清空缓冲区
                            if (clients.buffer_len >= BUFFER_SIZE - 1) {
                                clients.buffer_len = 0;
                            }
                        }
                        else if (bytes_read == 0) {
                            // 连接关闭
                            printf("客户端关闭连接，socket fd: %d\n", client_fd);

                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                            close(client_fd);
                            memset( clients.buffer, '\0', clients.buffer_len );
                            clients.buffer_len = 0;
                            break;
                        }
                        else {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                                // 没有更多数据可读
                                close(client_fd);
                                break;
                            } else {
                                // 读取错误
                                perror("read failed");

                                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                                close(client_fd);
                                memset( clients.buffer, '\0', clients.buffer_len );
                                clients.buffer_len = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    // 清理资源
    printf("Clear resources.\r\n");
    close(server_fd);
    close(epoll_fd);

    return 0;
}

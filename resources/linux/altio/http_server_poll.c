#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8080
#define MAX_CLIENTS 1000
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
        "    <title>Simple HTTP server(poll)</title>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>Hello from Poll HTTP Server!!!!</h1>\n"
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
    
    // 初始化pollfd数组
    //struct pollfd fds[MAX_CLIENTS + 1];  // +1 给服务器socket
    struct pollfd  *fds = calloc(MAX_CLIENTS + 1, sizeof(struct pollfd));
    if (!fds) {
        perror("calloc failed");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i <= MAX_CLIENTS; i++) {
        fds[i].fd = -1;  // 初始化为无效fd
        fds[i].events = 0;
        fds[i].revents = 0;
    }
    
    // 设置服务器socket
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;  // 监听读事件
    
    while (1) {
        // 调用poll等待事件
        int ready = poll(fds, MAX_CLIENTS + 1, -1);  // -1表示无限等待
        
        if (ready < 0) {
            if (errno == EINTR) {
                continue;  // 被信号中断，继续
            }
            perror("poll failed");
            exit(EXIT_FAILURE);
        }
        
        // 检查是否有新连接
        if (fds[0].revents & POLLIN) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                                     (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                continue;
            }
            
            printf("新连接，socket fd: %d, IP: %s, 端口: %d\n", 
                   new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            
            // 找到空闲位置存储新连接
            for (i = 1; i <= MAX_CLIENTS; i++) {
                if (fds[i].fd == -1) {
                    fds[i].fd = new_socket;
                    fds[i].events = POLLIN;  // 监听读事件
                    printf("添加到位置 %d\n", i - 1);
                    break;
                }
            }
            
            // 如果没有空闲位置，关闭连接
            if (i > MAX_CLIENTS) {
                printf("连接数已达上限，拒绝新连接\n");
                close(new_socket);
            }
            
            // 减少计数，因为处理了一个事件
            ready--;
        }
        
        // 处理客户端请求
        for (i = 1; i <= MAX_CLIENTS && ready > 0; i++) {
            if (fds[i].fd != -1 && (fds[i].revents & (POLLIN | POLLERR | POLLHUP))) {
                ready--;
                
                // 检查连接是否关闭或出错
                if (fds[i].revents & (POLLERR | POLLHUP)) {
                    printf("客户端异常断开，socket fd: %d\n", fds[i].fd);
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    fds[i].events = 0;
                    continue;
                }
                
                // 读取数据
                int valread = read(fds[i].fd, buffer, BUFFER_SIZE - 1);
                
                if (valread == 0) {
                    // 客户端正常关闭连接
                    getpeername(fds[i].fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("客户端断开连接，IP: %s, 端口: %d\n", 
                           inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                    
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    fds[i].events = 0;
                } else if (valread < 0) {
                    // 读取错误
                    if (errno != EWOULDBLOCK && errno != EAGAIN) {
                        perror("read error");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                        fds[i].events = 0;
                    }
                } else {
                    // 成功读取数据
                    buffer[valread] = '\0';
                    printf("收到请求 (socket %d):\n%s\n", fds[i].fd, buffer);
                    
                    // 简单的HTTP响应
                    send_http_response(fds[i].fd);
                    
                    // 发送响应后关闭连接
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    fds[i].events = 0;
                }
            }
        }
    }
    
    close(server_fd);
    free(fds);
    return 0;
}
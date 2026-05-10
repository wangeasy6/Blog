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
        "   <title>Simple HTTP server(select)</title>\n"
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
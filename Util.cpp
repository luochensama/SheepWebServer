//
// Created by luochensama on 2022/3/7.
//

#include "Util.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "unistd.h"
#include "Epoll.h"
#include "netinet/tcp.h"

int socket_bind_listen(int port){
    if(port < 0 || port > 65535) return -1;

    int fd = socket(AF_INET,SOCK_NONBLOCK | SOCK_STREAM | SOCK_CLOEXEC,0);
    if(fd == -1){
        close(fd);
        return -1;
    }
    // 主动关闭连接再打开不会报错。
    int opt = 1;
    if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof opt) == -1){
        close(fd);
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr,0,sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port  = htons(static_cast<uint16_t>(port));
    addr.sin_addr.s_addr = htonl(static_cast<uint32_t>(INADDR_ANY));
    if(bind(fd,reinterpret_cast<sockaddr*>(&addr),sizeof addr) == -1){
        close(fd);
        return -1;
    }

    if(listen(fd,2048) == -1){
        close(fd);
        return -1;
    }

    return fd;
}

void setTcpNoDelay(int fd){
    int opt = 1;
    setsockopt(fd,IPPROTO_TCP,TCP_NODELAY,&opt,sizeof opt);
}

int acceptNewConnection(int listenFd){
    struct sockaddr_in client_addr;
    memset(&client_addr,0,sizeof client_addr);
    socklen_t client_len = sizeof client_addr;
    int fd;
    fd =accept4(listenFd,reinterpret_cast<sockaddr*>(&client_addr),&client_len,SOCK_NONBLOCK | SOCK_CLOEXEC);

    return fd;
}
//
// Created by luochensama on 2022/3/7.
//

#include "Util.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include "unistd.h"
#include "Epoll.h"
#include "netinet/tcp.h"
#include "base/Logging.h"

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
    fd = accept4(listenFd,reinterpret_cast<sockaddr*>(&client_addr),&client_len,SOCK_NONBLOCK | SOCK_CLOEXEC);

    return fd;
}

int64_t transTime(const struct timeval& t){
    return static_cast<int64_t>(t.tv_usec) + static_cast<int64_t>(t.tv_sec%100003)*static_cast<int64_t>(1000000);
}
static const int BufferSize = 4096;
ssize_t readSocket(int fd,std::string& buffer,bool &zero){
    ssize_t total = 0;
    ssize_t now = 0;
    char Buffer[BufferSize];
    while(true){
        now = read(fd, Buffer, BufferSize);
        if(now < 0 || now > BufferSize){
            if(errno == EINTR) continue; // 中断
            else if(errno == EAGAIN){
                return total;
            }
            else{
                perror("read socket error!");
                return -1;
            }
        }else if(now == 0){
            zero = true;
            break;
        }
        total += now;
        buffer += std::string(Buffer,Buffer+now);
    }
    return total;
}

ssize_t writeSocket(int fd,std::string& buffer){
    ssize_t total = buffer.size();
    ssize_t writen = 0;
    ssize_t now = 0;
    const char* ptr = buffer.c_str();
    while(writen < total){
        now = write(fd, ptr+writen, total-writen);
        if(now < 0){
            if(errno == EINTR) continue; // 中断
            else if(errno == EAGAIN) break;
            else{
                perror("write socket error!");
                return -1;
            }
        }else if(now == 0){
            break;
        }
        writen += now;
    }
    if(writen == total) buffer.clear();
    else buffer.substr(writen);

    return writen;
}
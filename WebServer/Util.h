//
// Created by luochensama on 2022/3/7.
//
#pragma once

#include <cstdint>
#include <cstdio>
#include "string"

int socket_bind_listen(int port);
int acceptNewConnection(int fd);
void setTcpNoDelay(int fd);
int64_t transTime(const struct timeval& t);
ssize_t readSocket(int fd,std::string& buffer,bool &zero);
ssize_t writeSocket(int fd,std::string& buffer);
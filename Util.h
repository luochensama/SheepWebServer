//
// Created by luochensama on 2022/3/7.
//
#pragma once

int socket_bind_listen(int port);
int acceptNewConnection(int fd);
void setTcpNoDelay(int fd);
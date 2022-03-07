//
// Created by luochensama on 2022/3/7.
//
#pragma once

#include "base/noncopyable.h"
#include "EventLoop.h"

class Server : noncopyable{
public:
    Server(EventLoop* loop,int threadNum,int port);
    ~Server();
    void start();
    void handleRead();

private:

    EventLoop* loop_;
    bool started_;
    int port_;
    int threadNum_;
    int listenFd_;
    Channel* acceptChannel_;
};

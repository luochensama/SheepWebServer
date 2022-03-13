//
// Created by luochensama on 2022/3/7.
//
#pragma once

#include "base/noncopyable.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"

class Server : noncopyable{
public:
    Server(EventLoop* loop,int threadNum,int port);
    ~Server();
    void start();
    void handleRead();
    void handleConn(){loop_->modChannel(acceptChannel_);};

private:

    EventLoop* loop_;
    EventLoopThreadPool* threadPool_;
    bool started_;
    int port_;
    int threadNum_;
    int listenFd_;
    int fakeFd_; // 用来解决fd耗光的情况。
    std::shared_ptr<Channel> acceptChannel_;
};

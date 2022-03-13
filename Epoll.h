//
// Created by luochensama on 2022/3/6.
//

#pragma once
#include "Channel.h"
#include "vector"
#include "unordered_map"
#include "Timer.h"
#include "sys/epoll.h"

class Epoll : noncopyable{
public:
    Epoll();
    ~Epoll();
    void epoll_add(std::shared_ptr<Channel> request,int timeout);
    void epoll_mod(std::shared_ptr<Channel> request,int timeout);
    void epoll_del(std::shared_ptr<Channel> request);
    std::vector<std::shared_ptr<Channel>> poll();
    std::vector<std::shared_ptr<Channel>> getActiveEvents(int eventNums);
    void handleExpiredEvents(){timer_.handleExpiredEvent();};
    static int getMaxFds(){return MaxFds;};

private:
    static const int MaxFds = 100000;
    int epollFd_;
    std::vector<epoll_event> events_;
    std::shared_ptr<Channel> channels_[MaxFds];
    SP_HttpContext httpContexts_[MaxFds];
    TimerManager timer_;

    void add_timer(std::shared_ptr<Channel> request,int timeout);
};

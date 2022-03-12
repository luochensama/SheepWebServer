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
    void epoll_add(Channel* request,int timeout);
    void epoll_mod(Channel* request,int timeout);
    void epoll_del(Channel* request);
    void updateChannel(Channel* request,int timeout);
    std::vector<Channel*> poll();
    std::vector<Channel*> getActiveEvents(int eventNums);
    void handleExpiredEvents(){timer_.handleExpiredEvent();};
    static int getMaxFds(){return MaxFds;};
    void removeChannel(Channel* channel);

private:
    static const int MaxFds = 100000;
    int epollFd_;
    std::vector<epoll_event> events_;
    Channel* channels_[MaxFds];
    SP_HttpContext httpContexts_[MaxFds];
    TimerManager timer_;

    void add_timer(Channel* request,int timeout);
};

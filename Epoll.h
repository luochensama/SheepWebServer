//
// Created by luochensama on 2022/3/6.
//

#pragma once
#include "Channel.h"
#include "vector"
#include "unordered_map"
#include "sys/epoll.h"

class Epoll : noncopyable{
public:
    Epoll();
    ~Epoll();
    void epoll_add(Channel* request,int timeout);
    void epoll_mod(Channel* request,int timeout);
    void epoll_del(Channel* request);
    void updateChannel(Channel* request,int timeout);
    static int getMAXFDS(){return MAXFDS;};
    std::vector<Channel*> poll();
    std::vector<Channel*> getActiveEvents(int eventNums);

private:
    static const int MAXFDS = 100000;
    int epollFd_;
    std::vector<epoll_event> events_;
    Channel* channels_[MAXFDS];
};

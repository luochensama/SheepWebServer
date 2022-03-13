//
// Created by luochensama on 2022/3/6.
//

#include <cassert>
#include <iostream>
#include "Epoll.h"
#include "base/Logging.h"
const int EVENTSUM = 4096;
const int EPOLLWAIT_TIME = 10000;

Epoll::Epoll() :epollFd_(epoll_create1(EPOLL_CLOEXEC)),events_(EVENTSUM), timer_(){
    assert(epollFd_>0);
}

Epoll::~Epoll() {}

void Epoll::epoll_add(std::shared_ptr<Channel> request, int timeout) {
    int fd = request->getFd();
    if(timeout > 0){
        add_timer(request,timeout);
        httpContexts_[fd] = request->getHolder();
    }
    request->EqualAndUpdateLastEvents();
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();
    channels_[fd] = request;
    if(epoll_ctl(epollFd_,EPOLL_CTL_ADD,fd, &event) < 0){
        LOG << epollFd_;
        perror("epoll_add error");
        channels_[fd] = nullptr;
    }
}

void Epoll::epoll_mod(std::shared_ptr<Channel> request, int timeout) {
    int fd = request->getFd();
    if(timeout > 0){
        add_timer(request,timeout);
    }
    if(request->EqualAndUpdateLastEvents()) return;
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();
    if(epoll_ctl(epollFd_,EPOLL_CTL_MOD,fd,&event) < 0){
        perror("epoll_mod error");
        channels_[fd]= nullptr;
        httpContexts_[fd].reset();
    }
}

void Epoll::epoll_del(std::shared_ptr<Channel> request) {
    int fd = request->getFd();
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getLastEvents();
    if(epoll_ctl(epollFd_,EPOLL_CTL_DEL,fd,&event) < 0){
        perror("epoll_del error");
    }
    channels_[fd] = nullptr;
    httpContexts_[fd].reset();
}

std::vector<std::shared_ptr<Channel>> Epoll::poll() {
    while(true){
        int eventcount = epoll_wait(epollFd_,&*events_.begin(),events_.size(),EPOLLWAIT_TIME);
        if(eventcount < 0) perror("epoll_wait error");
        std::vector<std::shared_ptr<Channel>> req_data = getActiveEvents(eventcount);
        if(!req_data.empty()) return req_data;
    }
}

std::vector<std::shared_ptr<Channel>> Epoll::getActiveEvents(int eventNums) {
    std::vector<std::shared_ptr<Channel>> res;
    for(int i = 0;i < eventNums;++ i){
        int fd = events_[i].data.fd;
        std::shared_ptr<Channel> channel = channels_[fd];
        if(channel) {
            channel->setRevents(events_[i].events);
            res.push_back(channel);
        }
        else
            LOG << "ChannelPtr is invalid";
    }

    return res;
}


void Epoll::add_timer(std::shared_ptr<Channel> request, int timeout) {
    auto rq = request->getHolder();
    if(!rq){
        LOG << "add timer failed";
    } else{
        timer_.addTimer(rq,timeout);
    }
}


//
// Created by luochensama on 2022/3/6.
//

#include <cassert>
#include "Epoll.h"
#include "base/Logging.h"
const int EVENTSUM = 4096;
const int EPOLLWAIT_TIME = 10000;

Epoll::Epoll() :epollFd_(epoll_create1(EPOLL_CLOEXEC)),events_(EVENTSUM){
    assert(epollFd_>0);
}

Epoll::~Epoll() {}

void Epoll::epoll_add(Channel* request, int timeout) {
    int fd = request->getFd();
    if(timeout > 0){

    }
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();
    channels_[fd] = request;
    if(epoll_ctl(epollFd_,EPOLL_CTL_ADD,fd, &event) < 0){
        perror("epoll_add error");
        channels_[fd]= nullptr;
    }
}

void Epoll::epoll_mod(Channel* request, int timeout) {
    int fd = request->getFd();
    if(timeout > 0){

    }

    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();
    if(epoll_ctl(epollFd_,EPOLL_CTL_MOD,fd,&event) < 0){
        perror("epoll_mod error");
        channels_[fd]= nullptr;
    }

}

void Epoll::epoll_del(Channel* request) {
    int fd = request->getFd();

    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();
    if(epoll_ctl(epollFd_,EPOLL_CTL_DEL,fd,&event) < 0){
        perror("epoll_del error");
    }
    channels_[fd]= nullptr;
}

std::vector<Channel*> Epoll::poll() {
    while(true){
        int eventcount = epoll_wait(epollFd_,&*events_.begin(),events_.size(),EPOLLWAIT_TIME);
        if(eventcount < 0) perror("epoll_wait error");
        std::vector<Channel*> req_data = getActiveEvents(eventcount);
        if(!req_data.empty()) return req_data;
    }
}

std::vector<Channel*> Epoll::getActiveEvents(int eventNums) {
    std::vector<Channel*> res;
    for(int i = 0;i < eventNums;++ i){
        int fd = events_[i].data.fd;
        Channel* channel = channels_[fd];
        if(channel) {
            channel->setRevents(events_[i].events);
            res.push_back(channel);
        }
        else
            LOG << "ChannelPtr is invalid";
    }

    return res;
}

void Epoll::updateChannel(Channel* request,int timeout) {
    if(channels_[request->getFd()] == nullptr) epoll_add(request, timeout);
    else epoll_mod(request, timeout);
}

//
// Created by luochensama on 2022/3/6.
//

#include "Channel.h"
#include "EventLoop.h"
#include "base/Logging.h"

void Channel::handleRead() {
    if(readCallBack_) readCallBack_();
}

void Channel::handleWrite() {
    if(writeCallBack_) writeCallBack_();
}

void Channel::handleError() {
    if(errorCallBack_) errorCallBack_();
}

void Channel::handleConn() {
    if(connCallBack_) connCallBack_();
}

void Channel::handleEvents() {
    if((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)){
        return;
    }
    if(revents_ & EPOLLERR){
        handleError();
        return;
    }
    if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)){
        handleRead();
    }
    if(revents_ & EPOLLOUT){
        handleWrite();
    }
    handleConn();
}

Channel::Channel(EventLoop* loop,int fd)
    : loop_(loop), fd_(fd),events_(0), lastEvents_(0), revents_(0){

}

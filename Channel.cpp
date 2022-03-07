//
// Created by luochensama on 2022/3/6.
//

#include "Channel.h"
#include "EventLoop.h"

void Channel::handleRead() {
    if(readCallBack_) readCallBack_();
}

void Channel::handleWrite() {
    if(writeCallBack_) writeCallBack_();
}

void Channel::handleError() {
    if(errorCallBack_) errorCallBack_();
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
}

Channel::Channel(EventLoop* loop,int fd)
    : loop_(loop), fd_(fd),events_(0), lastEvents_(0), revents_(0){

}

void Channel::enableReading() {
    events_ |= (EPOLLIN | EPOLLPRI | EPOLLET);
    loop_->updateChannel(this);
}

void Channel::enableWriting() {
    events_ |= EPOLLOUT;
    loop_->updateChannel(this);
}

void Channel::disableReading() {
    events_ &= ~(EPOLLIN | EPOLLPRI | EPOLLET);
    loop_->updateChannel(this);
}

void Channel::disableWriting() {
    events_ &= ~EPOLLOUT;
    loop_->updateChannel(this);
}

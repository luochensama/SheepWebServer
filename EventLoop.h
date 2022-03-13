//
// Created by luochensama on 2022/3/6.
//
#pragma once

#include <csignal>
#include <cassert>
#include <utility>
#include <iostream>
#include "base/noncopyable.h"
#include "base/Thread.h"
#include "base/CurrentThread.h"
#include "Epoll.h"

class EventLoop :noncopyable{
public:
    typedef std::function<void()> Functor;
    EventLoop();
    ~EventLoop();
    void assertInLoopThread(){
        assert(isInLoopThread());
    }
    bool isInLoopThread() const{
        return threadId_ == CurrentThread::tid();
    }
    void loop();
    void quit();
    void addChannel(std::shared_ptr<Channel> channel,int timeout = 0){
        poll_->epoll_add(channel,timeout);
    }
    void modChannel(std::shared_ptr<Channel> channel,int timeout = 0){
        poll_->epoll_mod(channel,timeout);
    }
    void removeChannel(std::shared_ptr<Channel> channel){
        poll_->epoll_del(channel);
    }
    void handleRead();
    void runInLoop(Functor&& cb){
        if(isInLoopThread())
            cb();
        else
            queueInLoop(std::move(cb));
    }
    void queueInLoop(Functor&& cb);
    void doFunctors();
    void wakeup();

private:
    int wakeupFd_;
    std::shared_ptr<Channel> wakeupChannel_;
    bool looping_;
    pid_t threadId_;
    std::unique_ptr<Epoll> poll_;
    std::vector<std::shared_ptr<Channel>> activeChannel_;
    bool quit_;
    bool callingFunctors_;
    MutexLock mutex_;
    std::vector<Functor> pendingFunctors;
};

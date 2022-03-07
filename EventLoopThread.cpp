//
// Created by luochensama on 2022/3/7.
//

#include <cassert>
#include "EventLoop.h"
#include "EventLoopThread.h"

EventLoopThread::EventLoopThread()
 : loop_(nullptr), mutex_(), cond_(mutex_), thread_(std::bind(&EventLoopThread::threadFunc,this),"EventLoopThread"){

}
EventLoopThread::~EventLoopThread() {
    if(loop_ != nullptr){
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startThread() {
    assert(!thread_.started());

    thread_.start();
    {
        MutexLockGuard lock(mutex_);
        while(loop_ == nullptr)
            cond_.wait();
    }

    return loop_;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;
    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;
        cond_.notify();
    }

    loop.loop();
}
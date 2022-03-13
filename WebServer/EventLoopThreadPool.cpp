//
// Created by luochensama on 2022/3/7.
//

#include <cassert>
#include "EventLoop.h"
#include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop *loop, int threadNums)
: baseLoop_(loop),threadNums_(threadNums),threads_(threadNums),loops_(threadNums),next_(0){
    if (threadNums_ <= 0) {
        LOG << "numThreads_ <= 0";
        abort();
    }
}

void EventLoopThreadPool::start() {
    assert(!started_);
    started_ = true;
    for(int i = 0;i < threadNums_; ++ i){
        threads_[i].reset(new EventLoopThread());
        loops_[i] = threads_[i]->startThread();
    }
}

EventLoop *EventLoopThreadPool::getNextLoop() {
    baseLoop_->assertInLoopThread();
    assert(started_);
    EventLoop* loop = baseLoop_;
    if(!loops_.empty()) {
        loop = loops_[next_];
        next_ = (next_ + 1) % threadNums_;
    }

    return loop;
}

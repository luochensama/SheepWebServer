//
// Created by luochensama on 2022/3/7.
//
#pragma once
#include "base/noncopyable.h"
#include "base/Condition.h"
#include "functional"
#include "base/Thread.h"

class EventLoop;
class EventLoopThread : noncopyable{
public:
    EventLoopThread();
    EventLoop* startThread();

private:
    void threadFunc();

    EventLoop* loop_;
    MutexLock mutex_;
    Condition cond_;
    Thread thread_;
};

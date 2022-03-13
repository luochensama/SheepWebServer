//
// Created by luochensama on 2022/3/1.
//
#pragma once
#include "Condition.h"
#include "noncopyable.h"
#include "Mutex.h"

class CountDownLatch:noncopyable{
public:
    CountDownLatch(int);
    void wait();
    void countDown();


private:
    MutexLock mutex_;
    Condition cond_;
    int count_;

};

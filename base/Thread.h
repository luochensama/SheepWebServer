//
// Created by luochensama on 2022/3/1.
//
#pragma once
#include "noncopyable.h"
#include "pthread.h"
#include "CountDownLatch.h"
#include "functional"

class Thread :noncopyable{
public:
    typedef std::function<void()> ThreadFunc;

    explicit Thread(const ThreadFunc&,const std::string&);

    void start();
    int join();
    bool started() const{return started_;};
    pid_t tid() const {return tid_;};
    const std::string& name() const {return name_;};

private:
    bool started_;
    bool joined_;
    pthread_t pthreadId_; // Pthread函数族需要的ID
    pid_t tid_; // EventLoop实现runInLoop需要的ID
    std::string name_;
    ThreadFunc func_;
    CountDownLatch latch_; // 用于标识线程真正开始.

};

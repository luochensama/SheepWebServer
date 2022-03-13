//
// Created by luochensama on 2022/3/5.
//
#pragma once

#include <cassert>
#include "noncopyable.h"
#include "memory"
#include "LogStream.h"
#include "vector"
#include "CountDownLatch.h"
#include "Thread.h"

class AsyncLogging : noncopyable{
public:
    explicit AsyncLogging(const std::string& basename,int flushInterval=2);
    ~AsyncLogging(){
        if(running_) stop();
    }

    void append(const char* logline,size_t len);
    void start(){
        assert(!running_);
        running_ = true;
        thread_.start();
        latch_.wait();
    }
    void stop(){
        assert(running_);
        running_ = false;
    }

private:
    const int oriSize = 16;
    void threadFunc();
    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::shared_ptr<Buffer> BufferPtr;
    typedef std::vector<BufferPtr> BufferVector;
    const std::string basename_;
    const int flushInterval_;
    Thread thread_;
    bool running_;
    MutexLock mutex_;
    Condition cond_;
    CountDownLatch latch_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;

};

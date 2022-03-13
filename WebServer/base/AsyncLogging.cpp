//
// Created by luochensama on 2022/3/5.
//

#include "AsyncLogging.h"
#include "LogFile.h"

#include <memory>

AsyncLogging::AsyncLogging(const std::string &basename, int flushInterval)
    : basename_(basename),
    flushInterval_(flushInterval),
    running_(false),
    mutex_(),
    cond_(mutex_),
    latch_(1),
    currentBuffer_(new Buffer),
    nextBuffer_(new Buffer),
    buffers_(),
    thread_(std::bind(&AsyncLogging::threadFunc, this), "Logging"){
    assert(basename_.size()>1);
    buffers_.reserve(oriSize);
}

void AsyncLogging::append(const char *logline, size_t len) {
    MutexLockGuard lock(mutex_);
    if(currentBuffer_->avail() > len){
        currentBuffer_->append(logline,len);
    }else{
        buffers_.emplace_back(std::move(currentBuffer_));
        if(nextBuffer_ != nullptr)
            currentBuffer_ = std::move(nextBuffer_);
        else
            currentBuffer_.reset(new Buffer);
        currentBuffer_->append(logline,len);
        cond_.notify();
    }
}


void AsyncLogging::threadFunc() {
    assert(running_==true);

    latch_.countDown();
    LogFile output(basename_);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    BufferVector buffersToWrite;
    buffersToWrite.reserve(oriSize);
    while(running_){
        assert(newBuffer1!=nullptr && newBuffer1->length() == 0);
        assert(newBuffer2!=nullptr && newBuffer2->length() == 0);
        assert(buffersToWrite.empty());

        {
            MutexLockGuard lock(mutex_);
            if(buffers_.empty()){
                cond_.waitForSeconds(flushInterval_);
            }
            buffers_.emplace_back(std::move(currentBuffer_));
            buffersToWrite.swap(buffers_);
            currentBuffer_ = std::move(newBuffer1);
            if(nextBuffer_ == nullptr)
                nextBuffer_ = std::move(newBuffer2);
        }

        assert(!buffersToWrite.empty());
        if(buffersToWrite.size() > 25){ // 如果一次性缓存了那么多很有可能是程序出问题了。
            buffersToWrite.erase(buffersToWrite.begin()+2,buffersToWrite.end());
        }
        for(const auto& x:buffersToWrite){
            output.append(x->data(),x->length());
        }
        if(buffersToWrite.size() > 2)
            buffersToWrite.resize(2);
        if(!newBuffer1){
            assert(!buffersToWrite.empty());
            newBuffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }
        if(!newBuffer2){
            assert(!buffersToWrite.empty());
            newBuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }
        buffersToWrite.clear();
        output.flush();
    }
    output.flush();

}
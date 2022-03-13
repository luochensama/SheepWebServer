//
// Created by luochensama on 2022/3/9.
//

#include <sys/time.h>

#include <memory>
#include <iostream>
#include "Timer.h"
#include "HttpContext.h"
#include "Util.h"
#include "base/Logging.h"

TimerNode::TimerNode(SP_HttpContext httpContext,int64_t timeout)
: deleted_(false),expiredTime_(0),context_(httpContext){
    struct timeval t;
    gettimeofday(&t, nullptr);
    expiredTime_ = transTime(t) + timeout*1000; // 以微秒为单位

}

TimerNode::~TimerNode() {
    if(context_)
        context_->handleClose();
}

void TimerNode::setDelete() {
    deleted_ = true;
}

void TimerNode::deleteTimer() {
    context_.reset();
    setDelete();
}

bool TimerNode::isValid() {
    if(deleted_) return false;
    struct timeval t;
    gettimeofday(&t, nullptr);
    int64_t now = transTime(t);
    if(now > expiredTime_){
        setDelete();
        return false;
    }
    return true;
}

void TimerManager::addTimer(SP_HttpContext httpContext,int64_t timeout) {
    SP_Timer timer(new TimerNode(httpContext,timeout));
    timerQueue_.push(timer);
    httpContext->setTimer(timer);
}

void TimerManager::handleExpiredEvent() {
    while(!timerQueue_.empty()){
        std::shared_ptr<TimerNode> cur = timerQueue_.top();
        if(!cur->isValid())
            timerQueue_.pop();
        else break;
    }
}

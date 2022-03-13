//
// Created by luochensama on 2022/3/9.
//
#pragma once

#include "queue"
#include "memory"
#include "base/noncopyable.h"

class HttpContext;

typedef std::shared_ptr<HttpContext> SP_HttpContext;
class TimerNode : noncopyable{
public:
    explicit TimerNode(SP_HttpContext,int64_t timeout);
    ~TimerNode();
    int64_t getExpiredTime() const {return expiredTime_;}
    void deleteTimer();
    bool isValid();
    void setDelete();
    bool isDeleted() const {return deleted_;};

private:
    bool deleted_;
    int64_t expiredTime_;
    SP_HttpContext context_;

};
typedef std::shared_ptr<TimerNode> SP_Timer;

struct TimerCmp{
    bool operator()(const SP_Timer& a,const SP_Timer& b){
        return a->getExpiredTime() > b->getExpiredTime();
    }
};

class TimerManager {
public:
    void addTimer(SP_HttpContext,int64_t timeout);
    void handleExpiredEvent();

private:
    typedef std::shared_ptr<TimerNode> SP_Timer;
    std::priority_queue<SP_Timer,std::deque<SP_Timer>,TimerCmp> timerQueue_;
};



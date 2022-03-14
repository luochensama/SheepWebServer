//
// Created by luochensama on 2022/3/6.
//

#include "EventLoop.h"
#include "base/Logging.h"
#include "sys/eventfd.h"
#include "unistd.h"

__thread EventLoop* t_loopInThisThread;

int createEventFd(){
    int fd = eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
    if(fd < 0){
        LOG << "Failed in eventfd";
        abort();
    }
    return fd;
}

EventLoop::EventLoop() :
        looping_(false),
        threadId_(CurrentThread::tid()),
        poll_(new Epoll()),
        quit_(false),
        activeChannel_(),
        callingFunctors_(false),
        wakeupFd_(createEventFd()),
        wakeupChannel_(new Channel(this,wakeupFd_)),
        pendingFunctors(){
    if(t_loopInThisThread != nullptr){
        //    LOG << "Another EventLoop" << t_loopInThisThread << " in thread " << threadId_;
    }else{
        t_loopInThisThread = this;
    }
    wakeupChannel_->setReadCallBack(std::bind(&EventLoop::handleRead,this));
    wakeupChannel_->setEvents(EPOLLIN);
    addChannel(wakeupChannel_.get());
}

void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    while(!quit_){
        activeChannel_.clear();
        activeChannel_ = poll_->poll();
        for(auto x:activeChannel_)
            x->handleEvents();
        doFunctors();
        poll_->handleExpiredEvents();
    }

    LOG << "EventLoop" << this << "stop looping";
    looping_ = false;
}

EventLoop::~EventLoop() {
    assert(!looping_);
    t_loopInThisThread = nullptr;
}

void EventLoop::quit() {
    quit_ = true;
    if(!isInLoopThread()){
        wakeup();
    }
}

void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_,reinterpret_cast<char*>(&one),sizeof one);
    if(n != sizeof one){
        LOG << "EventLoop::handleRead() write " << n << " bytes instead of 8";
    }
}

void EventLoop::queueInLoop(EventLoop::Functor&& cb) {
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors.emplace_back(std::move(cb));
    }

    if(!isInLoopThread() || callingFunctors_) wakeup();
}

void EventLoop::doFunctors(){
    callingFunctors_ = true;
    std::vector<Functor> functors;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors);
    }
    for(const auto& x:functors){
        x();
    }

    callingFunctors_ = false;
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_,reinterpret_cast<char*>(&one),sizeof one);
    if(n != sizeof one){
        LOG << "EventLoop::wakeup() write " << n << " bytes instead of 8";
    }
}



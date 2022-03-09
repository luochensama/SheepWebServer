//
// Created by luochensama on 2022/3/6.
//
#pragma once

#include <memory>
#include "base/noncopyable.h"
#include "functional"
#include "sys/epoll.h"

class EventLoop;
class HttpContext;
class Channel : noncopyable{
public:
    typedef std::function<void()> CallBack;

    explicit Channel(EventLoop* loop,int fd);
    void setReadCallBack(const CallBack& cb){readCallBack_ = cb;};
    void setWriteCallBack(const CallBack& cb){writeCallBack_ = cb;};
    void setErrorCallBack(const CallBack& cb){errorCallBack_ = cb;};
    void handleRead();
    void handleWrite();
    void handleError();
    int getFd() const{return fd_;};
    uint32_t getEvents() const{return events_;};
    uint32_t getRevents() const{return revents_;};
    void setRevents(uint32_t rev){ revents_ = rev;};
    void setEvents(uint32_t events){ events_ = events;};
    void enableReading();
    void enableWriting();
    void disableReading();
    void disableWriting();
    std::shared_ptr<HttpContext> getHolder(){return context_.lock();};
    void setHolder(std::shared_ptr<HttpContext> context) {context_ = context;};

    void handleEvents();

private:
    CallBack readCallBack_;
    CallBack writeCallBack_;
    CallBack errorCallBack_;
    EventLoop* loop_;
    std::weak_ptr<HttpContext> context_;
    uint32_t events_;
    uint32_t revents_;
    int lastEvents_;
    int fd_;

};
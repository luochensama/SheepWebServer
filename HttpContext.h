//
// Created by luochensama on 2022/3/9.
//
#pragma once
#include "memory"
class EventLoop;
class Channel;
class HttpContext : public std::enable_shared_from_this<HttpContext>{
public:
    HttpContext(EventLoop* loop,int fd);
    ~HttpContext();

    void handleRead();
    void handleWrite();
    void handleClose();
    Channel* getChannel(){return channel_.get();};
    void newConnection();

private:
    EventLoop* loop_;
    std::unique_ptr<Channel> channel_; // 唯一所有权

};

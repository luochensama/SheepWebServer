//
// Created by luochensama on 2022/3/9.
//

#include <unistd.h>
#include "Channel.h"
#include "HttpContext.h"

HttpContext::HttpContext(EventLoop *loop, int fd) : loop_(loop),channel_(new Channel(loop,fd)){

}

HttpContext::~HttpContext() {
    ::close(channel_->getFd());
}

void HttpContext::newConnection() {

}

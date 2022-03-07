//
// Created by luochensama on 2022/3/7.
//

#include "Server.h"
#include "Epoll.h"
#include "Util.h"
#include "unistd.h"

Server::Server(EventLoop* loop,int threadNum,int port)
 :  loop_(loop),
    threadNum_(threadNum),
    threadPool_(new EventLoopThreadPool(loop_,threadNum)),
    port_(port),
    started_(false),
    listenFd_(socket_bind_listen(port_)),
    acceptChannel_(new Channel(loop_,listenFd_)){
        acceptChannel_->setReadCallBack(std::bind(&Server::handleRead,this));
    }

void Server::start() {
    assert(started_==false);
    threadPool_->start();
    acceptChannel_->enableReading();
    started_ = true;
}

void Server::handleRead() {
    int fd = 0;
    while((fd = acceptNewConnection(listenFd_)) > 0){
        if(fd >= Epoll::getMAXFDS()){
            close(fd);
            continue;
        }
        setTcpNoDelay(fd);

        EventLoop* loop = threadPool_->getNextLoop();
//        loop->runInLoop();
    }
}

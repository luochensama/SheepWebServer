//
// Created by luochensama on 2022/3/7.
//

#include <fcntl.h>
#include "Server.h"
#include "Epoll.h"
#include "Util.h"
#include "unistd.h"
#include "HttpContext.h"

Server::Server(EventLoop* loop,int threadNum,int port)
 :  loop_(loop),
    threadNum_(threadNum),
    threadPool_(new EventLoopThreadPool(loop_,threadNum)),
    port_(port),
    started_(false),
    listenFd_(socket_bind_listen(port_)),
    fakeFd_(::open("/dev/null",O_RDONLY | O_CLOEXEC)),
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
    while(true){
        fd = acceptNewConnection(listenFd_);
        if(fd == 0)
            break;
        else if(fd == -1){
            if(errno == EMFILE){ // 只处理fd不足的错误，其他错误直接退出。
                ::close(fakeFd_);
                fd = acceptNewConnection(listenFd_);
                ::close(fd);
                fakeFd_ = ::open("/dev/null",O_RDONLY | O_CLOEXEC);
                continue;
            }
            else break;
        }
        if(fd > Epoll::getMaxFds()){
            ::close(fd);
            continue;
        }
        setTcpNoDelay(fd);
        EventLoop* loop = threadPool_->getNextLoop();

        std::shared_ptr<HttpContext> newConnection(new HttpContext(loop,fd));
        newConnection->getChannel()->setHolder(newConnection);
        loop->runInLoop(std::bind(&HttpContext::newConnection,newConnection));
    }
}

Server::~Server() {
    ::close(listenFd_);
    ::close(fakeFd_);
}

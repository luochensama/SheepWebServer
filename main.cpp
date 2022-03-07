#include <bits/stdc++.h>
#include <unistd.h>
#include "base/noncopyable.h"
#include "base/Logging.h"
#include "EventLoop.h"
#include "sys/timerfd.h"

EventLoop* loop;
void timeout(){
    printf("timeout\n");
    loop->quit();
}

void print(){
    printf("ok\n");
}

void func(){
    sleep(2);
    loop->runInLoop(print);
}
int main() {
    loop = new EventLoop;
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK | TFD_CLOEXEC);
    Channel channel(loop,timerfd);
    channel.setReadCallBack(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong,sizeof howlong);
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd,0,&howlong, nullptr);
    Thread thread(std::bind(&func),"123");
    thread.start();
    loop->loop();

    return 0;
}

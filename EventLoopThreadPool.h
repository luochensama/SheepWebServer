//
// Created by luochensama on 2022/3/7.
//

#include "base/noncopyable.h"
#include "vector"
#include "EventLoopThread.h"
#include "base/Logging.h"
#include "memory"

class EventLoopThreadPool : noncopyable{
public:
    EventLoopThreadPool(EventLoop* loop,int threadNums);
    ~EventLoopThreadPool() { LOG << "~EventLoopThreadPool()"; }
    void start();

    EventLoop* getNextLoop();
private:

    EventLoop* baseLoop_;
    bool started_;
    int threadNums_;
    int next_;
    std::vector<std::shared_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;

};

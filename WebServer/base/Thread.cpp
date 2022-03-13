//
// Created by luochensama on 2022/3/1.
//

#include "Thread.h"
#include "CurrentThread.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <cassert>
#include "pthread.h"
namespace CurrentThread{
    __thread int t_cacheTid=0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength=6;
    __thread const char* t_threadName="default";
}

pid_t getTid(){return static_cast<pid_t>(::syscall(SYS_gettid));}

void CurrentThread::cacheTid() {
    if(t_cacheTid == 0){
        t_cacheTid = getTid();
        t_tidStringLength = snprintf(t_tidString,sizeof t_tidString,"%5d ",t_cacheTid);
    }
}

struct ThreadData{ // 为了修改 Thread 对象里面的内容，需要一个结构体来存储参数。
    Thread::ThreadFunc func_;
    pid_t* tid_;
    std::string name_;
    CountDownLatch* latch_;
    ThreadData(const Thread::ThreadFunc& func,pid_t* tid,const std::string& name,CountDownLatch* latch):
    func_(func),
    name_(name),
    tid_(tid),
    latch_(latch){};

    void runInThread(){
        *tid_ = CurrentThread::tid();
        tid_ = nullptr;
        latch_->countDown();
        latch_ = nullptr;
        CurrentThread::t_threadName = name_.empty()?"Thread":name_.c_str();
        prctl(PR_SET_NAME, CurrentThread::t_threadName); // 为线程设置名字.

        func_();
        CurrentThread::t_threadName = "finished";
    }
};

void* startThread(void *obj){
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;

    return nullptr;
}

Thread::Thread(const ThreadFunc& func,const std::string &name):
started_(false),
joined_(false),
pthreadId_(0),
tid_(0),
name_(name),
func_(func),
latch_(1){}

void Thread::start() {
    assert(!started_);
    started_ = true;
    ThreadData* data = new ThreadData(func_,&tid_,name_,&latch_);
    if(pthread_create(&pthreadId_, nullptr, &startThread, data)){
        started_ = false;
        delete data;
    }else{
        data->latch_->wait();
        assert(tid_>0);
    }
}

int Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;

    return pthread_join(pthreadId_, nullptr);
}

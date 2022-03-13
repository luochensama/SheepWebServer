//
// Created by luochensama on 2022/3/1.
//
#pragma once
#include "noncopyable.h"
#include "mutex"

class MutexLock:noncopyable{
public:
    MutexLock(){
        pthread_mutex_init(&mutex, nullptr);
    }
    ~MutexLock(){
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }
    void lock() {
        pthread_mutex_lock(&mutex);
    }
    void unlock(){
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_t* get(){return &mutex;}

private:
    pthread_mutex_t mutex;
    friend class Condition;
};

class MutexLockGuard:noncopyable{
public:
    explicit MutexLockGuard(MutexLock &mutex):mutex_(mutex){mutex.lock();}
    ~MutexLockGuard(){mutex_.unlock();}

private:
    MutexLock &mutex_;
};
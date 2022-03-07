//
// Created by luochensama on 2022/3/4.
//

#pragma once
#include "noncopyable.h"
#include "memory"
#include "FileUtil.h"
#include "Mutex.h"

class LogFile : noncopyable{
public:
    LogFile(const std::string& basename,int flushEveryN=1024,bool threadSafe=true);

    void append(const char* logline,size_t len);
    void flush();

private:
    void appendUnlock(const char* logline,size_t len);

    const std::string basename_;
    const int flushEveryN_;
    const bool threadSafe_;
    int count_;
    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<FileWriter> file_;

};

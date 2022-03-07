//
// Created by luochensama on 2022/3/4.
//

#include "LogFile.h"

LogFile::LogFile(const std::string &basename, int flushEveryN, bool threadSafe) : basename_(basename),flushEveryN_(flushEveryN),
    threadSafe_(threadSafe),count_(0),mutex_(new MutexLock()),file_(new FileWriter(basename)){
}

void LogFile::append(const char *logline, size_t len) {
    if(!threadSafe_) MutexLockGuard lock(*mutex_);
    appendUnlock(logline,len);
}

void LogFile::flush() {
    if(!threadSafe_) MutexLockGuard lock(*mutex_);
    file_->flush();
}

void LogFile::appendUnlock(const char *logline, size_t len) {
    ++count_;
    file_->append(logline,len);
    if(count_ == flushEveryN_){
        count_ = 0;
        file_->flush();
    }
}

//
// Created by luochensama on 2022/3/6.
//

#include <sys/time.h>
#include "Logging.h"
#include "AsyncLogging.h"

std::string Logger::filename_ = "/home/luochensama/CLionProjects/webserver/base/WebServer.log";
static pthread_once_t once_control = PTHREAD_ONCE_INIT;
static AsyncLogging* AsyncLogger;

void init_once(){
    AsyncLogger = new AsyncLogging(Logger::getFileName());
    AsyncLogger->start();
}
void output(const char* str,int len){
    pthread_once(&once_control,init_once);
    AsyncLogger->append(str,len);
}

Logger::Impl::Impl(const char *filename, int line):
stream_(),
filename_(filename),
line_(line){

}

void Logger::Impl::formatTime() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    time_t time = tv.tv_sec;
    auto tm = localtime(&time);
    char str[26] = {0};
    size_t len = strftime(str,sizeof str,"%Y-%m-%d %H:%M:%S",tm);
    sprintf(str+len,".%ld\n",tv.tv_usec);
    stream_ << str;
}

Logger::Logger(const char *filename, int line) : impl_(filename,line){

}

Logger::~Logger() {
    impl_.stream_ << " -- " << impl_.filename_ << ":" << impl_.line_ << "\n";
    const LogStream::Buffer& buf(impl_.stream_.buffer());
    output(buf.data(),buf.length());
}

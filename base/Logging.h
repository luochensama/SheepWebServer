//
// Created by luochensama on 2022/3/6.
//
#include "LogStream.h"

class Logger {
public:
    Logger(const char*,int);
    ~Logger();
    LogStream& stream(){return impl_.stream_;};
    void setFileName(const std::string& name){filename_ = name;};
    static std::string getFileName(){return filename_;};

private:
    class Impl{
    public:
        Impl(const char*filename,int line);
        void formatTime();
        LogStream stream_;
        std::string filename_;
        int line_;
    };
    Impl impl_;
    static std::string filename_;
};

#define LOG Logger(__FILE__,__LINE__).stream()


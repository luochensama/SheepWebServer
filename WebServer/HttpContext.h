//
// Created by luochensama on 2022/3/9.
//
#pragma once

#include <unordered_map>
#include "memory"
#include "Timer.h"
class EventLoop;
class Channel;


enum ConnectionState{
    CONNECTION_CONNECTED,
    CONNECTION_DISCONNECTING,
    CONNECTION_DISCONNECTED
};

enum ProcessState{
    STATE_PARSE_URI,
    STATE_PARSE_HEADER,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINISH
};

enum UriState{
    PARSE_URI_AGAIN,
    PARSE_URI_ERROR,
    PARSE_URI_SUCCESS
};

enum HeaderState{
    PARSE_HEADER_AGAIN,
    PARSE_HEADER_ERROR,
    PARSE_HEADER_SUCCESS
};

enum Methods{
    METHOD_GET,
    METHOD_POST,
    METHOD_HEAD
};

enum HttpVersion{
    HTTP_11,
    HTTP_10
};

enum ParseHeaderState{
    HEADER_LINE_READ,
    HEADER_LINE_DONE
};

enum AnalysisState{
    ANALYSIS_SUCCESS,
    ANALYSIS_ERROR
};


class MimeType {
private:
    static void init();
    static std::unordered_map<std::string, std::string> mime;
    MimeType();
    MimeType(const MimeType &m);

public:
    static std::string getMime(const std::string &suffix);

private:
    static pthread_once_t once_control;
};


class HttpContext : public std::enable_shared_from_this<HttpContext>{

public:
    HttpContext(EventLoop* loop,int fd);
    ~HttpContext();

    void handleRead();
    void handleWrite();
    void handleError(int,std::string);
    void handleClose();
    void handleConn();
    Channel* getChannel(){return channel_.get();};
    void newConnection();
    void setTimer(SP_Timer timer){timer_ = timer;};
    void reset();
    void deleteTimer();
    UriState parseURI();
    HeaderState parseHeader();
    AnalysisState analysisRequest();

private:
    EventLoop* loop_;
    std::unique_ptr<Channel> channel_;
    std::weak_ptr<TimerNode> timer_;
    int fd_;
    size_t currentPosition_; // 当前的位置
    size_t stringEndFlag_;
    size_t lineEndPos_; // 一行的结尾处
    ConnectionState connectionState_;
    ProcessState processState_;
    Methods methods_;
    HttpVersion httpVersion_;
    ParseHeaderState parseHeaderState_;
    bool error_;
    std::string inputBuffer_;
    std::string outputBuffer_;
    std::string fileName_;
    std::string line_;
    bool keepAlive_;
    std::unordered_map<std::string,std::string> header_;

    bool isNotFound(size_t pos) const;
    void removePre(size_t end);
    bool receiveOneLine();
};

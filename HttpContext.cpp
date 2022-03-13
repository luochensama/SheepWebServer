//
// Created by luochensama on 2022/3/9.
//

#include <unistd.h>
#include <cstring>
#include <iostream>
#include "Channel.h"
#include "HttpContext.h"
#include "EventLoop.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "Timer.h"
#include "Util.h"
#include "string"
#include "base/Logging.h"

std::unordered_map<std::string, std::string> MimeType::mime;
pthread_once_t MimeType::once_control;
static const int DEFAULT_EXPIRED_TIME = 2000; // ms
static const int DEFAULT_KEEPALIVE_EXPIRED_TIME = 5 * 60 * 1000;

char favicon[555] = {
        '\x89', 'P',    'N',    'G',    '\xD',  '\xA',  '\x1A', '\xA',  '\x0',
        '\x0',  '\x0',  '\xD',  'I',    'H',    'D',    'R',    '\x0',  '\x0',
        '\x0',  '\x10', '\x0',  '\x0',  '\x0',  '\x10', '\x8',  '\x6',  '\x0',
        '\x0',  '\x0',  '\x1F', '\xF3', '\xFF', 'a',    '\x0',  '\x0',  '\x0',
        '\x19', 't',    'E',    'X',    't',    'S',    'o',    'f',    't',
        'w',    'a',    'r',    'e',    '\x0',  'A',    'd',    'o',    'b',
        'e',    '\x20', 'I',    'm',    'a',    'g',    'e',    'R',    'e',
        'a',    'd',    'y',    'q',    '\xC9', 'e',    '\x3C', '\x0',  '\x0',
        '\x1',  '\xCD', 'I',    'D',    'A',    'T',    'x',    '\xDA', '\x94',
        '\x93', '9',    'H',    '\x3',  'A',    '\x14', '\x86', '\xFF', '\x5D',
        'b',    '\xA7', '\x4',  'R',    '\xC4', 'm',    '\x22', '\x1E', '\xA0',
        'F',    '\x24', '\x8',  '\x16', '\x16', 'v',    '\xA',  '6',    '\xBA',
        'J',    '\x9A', '\x80', '\x8',  'A',    '\xB4', 'q',    '\x85', 'X',
        '\x89', 'G',    '\xB0', 'I',    '\xA9', 'Q',    '\x24', '\xCD', '\xA6',
        '\x8',  '\xA4', 'H',    'c',    '\x91', 'B',    '\xB',  '\xAF', 'V',
        '\xC1', 'F',    '\xB4', '\x15', '\xCF', '\x22', 'X',    '\x98', '\xB',
        'T',    'H',    '\x8A', 'd',    '\x93', '\x8D', '\xFB', 'F',    'g',
        '\xC9', '\x1A', '\x14', '\x7D', '\xF0', 'f',    'v',    'f',    '\xDF',
        '\x7C', '\xEF', '\xE7', 'g',    'F',    '\xA8', '\xD5', 'j',    'H',
        '\x24', '\x12', '\x2A', '\x0',  '\x5',  '\xBF', 'G',    '\xD4', '\xEF',
        '\xF7', '\x2F', '6',    '\xEC', '\x12', '\x20', '\x1E', '\x8F', '\xD7',
        '\xAA', '\xD5', '\xEA', '\xAF', 'I',    '5',    'F',    '\xAA', 'T',
        '\x5F', '\x9F', '\x22', 'A',    '\x2A', '\x95', '\xA',  '\x83', '\xE5',
        'r',    '9',    'd',    '\xB3', 'Y',    '\x96', '\x99', 'L',    '\x6',
        '\xE9', 't',    '\x9A', '\x25', '\x85', '\x2C', '\xCB', 'T',    '\xA7',
        '\xC4', 'b',    '1',    '\xB5', '\x5E', '\x0',  '\x3',  'h',    '\x9A',
        '\xC6', '\x16', '\x82', '\x20', 'X',    'R',    '\x14', 'E',    '6',
        'S',    '\x94', '\xCB', 'e',    'x',    '\xBD', '\x5E', '\xAA', 'U',
        'T',    '\x23', 'L',    '\xC0', '\xE0', '\xE2', '\xC1', '\x8F', '\x0',
        '\x9E', '\xBC', '\x9',  'A',    '\x7C', '\x3E', '\x1F', '\x83', 'D',
        '\x22', '\x11', '\xD5', 'T',    '\x40', '\x3F', '8',    '\x80', 'w',
        '\xE5', '3',    '\x7',  '\xB8', '\x5C', '\x2E', 'H',    '\x92', '\x4',
        '\x87', '\xC3', '\x81', '\x40', '\x20', '\x40', 'g',    '\x98', '\xE9',
        '6',    '\x1A', '\xA6', 'g',    '\x15', '\x4',  '\xE3', '\xD7', '\xC8',
        '\xBD', '\x15', '\xE1', 'i',    '\xB7', 'C',    '\xAB', '\xEA', 'x',
        '\x2F', 'j',    'X',    '\x92', '\xBB', '\x18', '\x20', '\x9F', '\xCF',
        '3',    '\xC3', '\xB8', '\xE9', 'N',    '\xA7', '\xD3', 'l',    'J',
        '\x0',  'i',    '6',    '\x7C', '\x8E', '\xE1', '\xFE', 'V',    '\x84',
        '\xE7', '\x3C', '\x9F', 'r',    '\x2B', '\x3A', 'B',    '\x7B', '7',
        'f',    'w',    '\xAE', '\x8E', '\xE',  '\xF3', '\xBD', 'R',    '\xA9',
        'd',    '\x2',  'B',    '\xAF', '\x85', '2',    'f',    'F',    '\xBA',
        '\xC',  '\xD9', '\x9F', '\x1D', '\x9A', 'l',    '\x22', '\xE6', '\xC7',
        '\x3A', '\x2C', '\x80', '\xEF', '\xC1', '\x15', '\x90', '\x7',  '\x93',
        '\xA2', '\x28', '\xA0', 'S',    'j',    '\xB1', '\xB8', '\xDF', '\x29',
        '5',    'C',    '\xE',  '\x3F', 'X',    '\xFC', '\x98', '\xDA', 'y',
        'j',    'P',    '\x40', '\x0',  '\x87', '\xAE', '\x1B', '\x17', 'B',
        '\xB4', '\x3A', '\x3F', '\xBE', 'y',    '\xC7', '\xA',  '\x26', '\xB6',
        '\xEE', '\xD9', '\x9A', '\x60', '\x14', '\x93', '\xDB', '\x8F', '\xD',
        '\xA',  '\x2E', '\xE9', '\x23', '\x95', '\x29', 'X',    '\x0',  '\x27',
        '\xEB', 'n',    'V',    'p',    '\xBC', '\xD6', '\xCB', '\xD6', 'G',
        '\xAB', '\x3D', 'l',    '\x7D', '\xB8', '\xD2', '\xDD', '\xA0', '\x60',
        '\x83', '\xBA', '\xEF', '\x5F', '\xA4', '\xEA', '\xCC', '\x2',  'N',
        '\xAE', '\x5E', 'p',    '\x1A', '\xEC', '\xB3', '\x40', '9',    '\xAC',
        '\xFE', '\xF2', '\x91', '\x89', 'g',    '\x91', '\x85', '\x21', '\xA8',
        '\x87', '\xB7', 'X',    '\x7E', '\x7E', '\x85', '\xBB', '\xCD', 'N',
        'N',    'b',    't',    '\x40', '\xFA', '\x93', '\x89', '\xEC', '\x1E',
        '\xEC', '\x86', '\x2',  'H',    '\x26', '\x93', '\xD0', 'u',    '\x1D',
        '\x7F', '\x9',  '2',    '\x95', '\xBF', '\x1F', '\xDB', '\xD7', 'c',
        '\x8A', '\x1A', '\xF7', '\x5C', '\xC1', '\xFF', '\x22', 'J',    '\xC3',
        '\x87', '\x0',  '\x3',  '\x0',  'K',    '\xBB', '\xF8', '\xD6', '\x2A',
        'v',    '\x98', 'I',    '\x0',  '\x0',  '\x0',  '\x0',  'I',    'E',
        'N',    'D',    '\xAE', 'B',    '\x60', '\x82',
};

void MimeType::init() {
    mime[".html"] = "text/html";
    mime[".avi"] = "video/x-msvideo";
    mime[".bmp"] = "image/bmp";
    mime[".c"] = "text/plain";
    mime[".doc"] = "application/msword";
    mime[".gif"] = "image/gif";
    mime[".gz"] = "application/x-gzip";
    mime[".htm"] = "text/html";
    mime[".ico"] = "image/x-icon";
    mime[".jpg"] = "image/jpeg";
    mime[".png"] = "image/png";
    mime[".txt"] = "text/plain";
    mime[".mp3"] = "audio/mp3";
    mime["default"] = "text/html";
}

std::string MimeType::getMime(const std::string &suffix) {
    pthread_once(&once_control, MimeType::init);
    if (mime.find(suffix) == mime.end())
        return mime["default"];
    else
        return mime[suffix];
}

HttpContext::HttpContext(EventLoop *loop, int fd)
:
        loop_(loop),
        channel_(new Channel(loop,fd)),
        timer_(),
        fd_(fd),
        stringEndFlag_(std::string::npos),
        currentPosition_(0),
        lineEndPos_(0),
        connectionState_(CONNECTION_CONNECTED),
        processState_(STATE_PARSE_URI),
        methods_(METHOD_GET),
        httpVersion_(HTTP_11),
        parseHeaderState_(HEADER_LINE_READ),
        error_(false),
        keepAlive_(false){
    channel_->setReadCallBack(std::bind(&HttpContext::handleRead,this));
    channel_->setWriteCallBack(std::bind(&HttpContext::handleWrite,this));
    channel_->setConnCallBack(std::bind(&HttpContext::handleConn,this));
}
HttpContext::~HttpContext() {
    ::close(channel_->getFd());
}
void HttpContext::reset() {
    fileName_.clear();
    currentPosition_ = 0;
    lineEndPos_ = 0;
    header_.clear();
    processState_ = STATE_PARSE_URI;
    parseHeaderState_ = HEADER_LINE_READ;
}

void HttpContext::newConnection() {
    channel_->setEvents(EPOLLIN | EPOLLET);
    loop_->addChannel(channel_,DEFAULT_EXPIRED_TIME);
}

void HttpContext::handleRead() {
    if(error_ || connectionState_ != CONNECTION_CONNECTED) return;
    bool zero = false;
    ssize_t readBytes = readSocket(fd_,inputBuffer_,zero);
    if(readBytes < 0){ // 如果读取错误返回400.
        error_ = true;
        handleError(400,"Bad Request"); // handleError会关闭连接。
        return;
    }
    if(zero){
        // 读到0认为对方已经关闭连接，这边处理完最后的数据也关闭.
        // 有可能对方会关闭自身读写两端，但服务器只会处理一次，因为如果收到RST
        // 会直接调用channel的errorCallback。
       connectionState_ = CONNECTION_DISCONNECTING;
       if(!readBytes){
           return;
       } // 如果读到0又没读到数据，那么就不继续下面的过程，直接return
    }
    if(processState_ == STATE_PARSE_URI){
        UriState res = parseURI();
        if(res == PARSE_URI_AGAIN){
            return;
        }else if(res == PARSE_URI_ERROR){
            LOG << "FD = " << fd_ << " receive error request" << inputBuffer_ << "!!!!!!";
            error_ = true;
            handleError(400,"Bad Request");
            return;
        }else{
            processState_ = STATE_PARSE_HEADER;
        }
    }
    if(processState_ == STATE_PARSE_HEADER){
        HeaderState res = parseHeader();
        if(res == PARSE_HEADER_AGAIN){
            return;
        }else if(res == PARSE_HEADER_ERROR){
            LOG << "FD = " << fd_ << " receive error header" << inputBuffer_ << "!!!!!!";
            error_ = true;
            handleError(400,"Bad Request");
            return;
        }else{
            if(methods_ == METHOD_POST) processState_ = STATE_RECV_BODY;
            else processState_ = STATE_ANALYSIS;
        }
    }
    if(processState_ == STATE_RECV_BODY){
        int content_length = 0;
        if(header_["Content-length"].empty()){
            error_ = true;
            handleError(400, "Bad Request: Lack of argument (Content-length)");
            return;
        }else{
            content_length = std::stoi(header_["Content-length"]);
        }
        if(content_length > static_cast<int>(inputBuffer_.size())) return;
        processState_ = STATE_ANALYSIS;
    }
    if(processState_ == STATE_ANALYSIS){
        AnalysisState analysisState = analysisRequest();
        if(analysisState == ANALYSIS_SUCCESS){
            processState_ = STATE_FINISH;
        }else{
            return;
        }
    }
    if(processState_ == STATE_FINISH){
        if(!outputBuffer_.empty())
            handleWrite();
        if(!error_){
            reset();
            if(!inputBuffer_.empty()){
                handleRead();
            }
        }
    }
    if(error_) return; // write error
    uint32_t& events = channel_->getEvents();
    if(connectionState_ == CONNECTION_CONNECTED)
        events |= EPOLLIN;
}

void HttpContext::handleWrite() {
    if(error_ || connectionState_ == CONNECTION_DISCONNECTED) return;
    uint32_t& events = channel_->getEvents();
    ssize_t writen = writeSocket(fd_,outputBuffer_);
    if(writen == -1){
        error_ = true;
        events = 0;
        return;
    }
    if(!outputBuffer_.empty()) events |= EPOLLOUT;
}

void HttpContext::handleError(int errornum,std::string short_msg) {
    short_msg = " " + short_msg;
    char send_buff[4096];
    std::string body_buff, header_buff;
    body_buff += "<html><title>哎~出错了</title>";
    body_buff += "<body bgcolor=\"ffffff\">";
    body_buff += std::to_string(errornum) + short_msg;
    body_buff += "<hr><em> LuoChen</em>\n</body></html>";

    header_buff += "HTTP/1.1 " + std::to_string(errornum) + short_msg + "\r\n";
    header_buff += "Content-Type: text/html\r\n";
    header_buff += "Connection: Close\r\n";
    header_buff += "Content-Length: " + std::to_string(body_buff.size()) + "\r\n";
    header_buff += "Server: LuoChen\r\n";

    header_buff += "\r\n";
    // 错误处理不考虑writen不完的情况
    writeSocket(fd_, header_buff);
    writeSocket(fd_, body_buff);
    handleClose();
}

void HttpContext::handleClose() {
    connectionState_ = CONNECTION_DISCONNECTED;
    std::shared_ptr<HttpContext> guard(shared_from_this());
    loop_->removeChannel(channel_);
}

void HttpContext::handleConn() {
    deleteTimer();
    if(error_ || connectionState_ == CONNECTION_DISCONNECTED) return;
    uint32_t events = channel_->getEvents();
    int timeout = DEFAULT_EXPIRED_TIME;
    if(keepAlive_) timeout = DEFAULT_KEEPALIVE_EXPIRED_TIME;
    if(connectionState_ == CONNECTION_DISCONNECTING){
        if(!(events & EPOLLOUT)) {
            handleClose();
            return;
        }else{
            events = 0;
            events |= EPOLLOUT;
        }
    }else{
        if((events & EPOLLOUT) && (events & EPOLLIN)){
            events = 0;
            events |= EPOLLOUT;
        }else if(events == 0) events |= EPOLLIN;
    }
    events |= EPOLLET;
    loop_->modChannel(channel_,timeout);
}
UriState HttpContext::parseURI() {
    // 攒够一行数据再解析
    if(!receiveOneLine()) return PARSE_URI_AGAIN;
    if(!isNotFound(currentPosition_ = line_.find("GET", 0))){
        methods_ = METHOD_GET;
    }else if(!isNotFound(currentPosition_ = line_.find("POST", 0))){
        methods_ = METHOD_POST;
    }else if(!isNotFound(currentPosition_ = line_.find("HEAD", 0))){
        methods_ = METHOD_HEAD;
    }else{
        return PARSE_URI_ERROR;
    }
    currentPosition_ = line_.find('/', currentPosition_); // 找到路径的起点
    if(isNotFound(currentPosition_)) { // 没有就默认页面
        fileName_ = "index.html";
        httpVersion_ = HTTP_11;
        return PARSE_URI_SUCCESS;
    }
    currentPosition_++; // '/'之后第一个字符
    size_t endPos = line_.find(' ', currentPosition_); // 找到路径的终点
    if(isNotFound(endPos)) return PARSE_URI_ERROR;
    fileName_ = line_.substr(currentPosition_, endPos - currentPosition_);
    size_t temp = endPos;
    if(fileName_.empty()) fileName_ = "index.html";
    else{
        endPos = line_.find("?", currentPosition_, endPos); // ?之后的是参数
        if(!isNotFound(endPos)){ // 如果有
            fileName_ = fileName_.substr(0, endPos - currentPosition_);
        }
    }
    currentPosition_ = temp + 1; // 到了空格后一个
    if(!isNotFound(line_.find("HTTP/1.1", currentPosition_))){
        httpVersion_ = HTTP_11;
    }else if(!isNotFound(line_.find("HTTP/1.0", currentPosition_))){
        httpVersion_ = HTTP_10;
    }else{
        return PARSE_URI_ERROR;
    }

    return PARSE_URI_SUCCESS;
}

HeaderState HttpContext::parseHeader() {
    while(!inputBuffer_.empty()) {
        if (parseHeaderState_ == HEADER_LINE_READ) {
            if (!receiveOneLine()) return PARSE_HEADER_AGAIN; // 没收到一行不处理
            currentPosition_ = line_.find(':', currentPosition_);
            if (isNotFound(currentPosition_))
                return PARSE_HEADER_ERROR;
            std::string key = std::string(line_, 0, currentPosition_);
            if (key.empty()) return PARSE_HEADER_ERROR;
            if (line_[++currentPosition_] != ' ') return PARSE_HEADER_ERROR;
            std::string value = std::string(line_, currentPosition_ + 1, lineEndPos_ - currentPosition_ - 1);
            if (value.empty()) return PARSE_HEADER_ERROR;
            header_[key] = value;
            parseHeaderState_ = HEADER_LINE_DONE;
        }
        if (parseHeaderState_ == HEADER_LINE_DONE) {
            if (inputBuffer_.size() > 1) {
                if (inputBuffer_[0] == '\r' && inputBuffer_[1] == '\n') {
                    inputBuffer_ = inputBuffer_.substr(2);
                    return PARSE_HEADER_SUCCESS;
                } else {
                    parseHeaderState_ = HEADER_LINE_READ;
                }
            } else {
                return PARSE_HEADER_AGAIN;
            }
        }
    }


    return PARSE_HEADER_AGAIN;
}

AnalysisState HttpContext::analysisRequest() {
    std::string header;
    std::string body;
    if(methods_ == METHOD_POST){

    }else if(methods_ == METHOD_GET || methods_ == METHOD_HEAD){
        header += "HTTP/1.1 200 OK\r\n";
        if(header_["Connection"] == "Keep-Alive" || header_["Connection"] == "keep-alive"){
            keepAlive_ = true;
            header += std::string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" +
                      std::to_string(DEFAULT_KEEPALIVE_EXPIRED_TIME) + "\r\n";
        }
        int dot_pos = fileName_.find('.');
        std::string filetype;
        if (dot_pos < 0)
            filetype = MimeType::getMime("default");
        else
            filetype = MimeType::getMime(fileName_.substr(dot_pos));
        // echo test
        if (fileName_ == "hello") {
            outputBuffer_ =
                    "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\nHello World";
            return ANALYSIS_SUCCESS;
        }
        if (fileName_ == "favicon.ico") {
            header += "Content-Type: image/png\r\n";
            header += "Content-Length: " + std::to_string(sizeof favicon) + "\r\n";
            header += "Server: LuoChen Web Server\r\n";

            header += "\r\n";
            outputBuffer_ += header;
            outputBuffer_ += std::string(favicon, favicon + sizeof favicon);

            return ANALYSIS_SUCCESS;
        }

        struct stat sbuf;
        if (stat(fileName_.c_str(), &sbuf) < 0) {
            header.clear();
            handleError(fd_, "404 Not Found!");
            return ANALYSIS_ERROR;
        }
        header += "Content-Type: " + filetype + "\r\n";
        header += "Content-Length: " + std::to_string(sbuf.st_size) + "\r\n";
        header += "Server: LinYa's Web Server\r\n";
        // 头部结束
        header += "\r\n";
        outputBuffer_ += header;

        if (methods_ == METHOD_HEAD) return ANALYSIS_SUCCESS;

        int src_fd = open(fileName_.c_str(), O_RDONLY, 0);
        if (src_fd < 0) {
            outputBuffer_.clear();
            handleError(fd_, "404 Not Found!");
            return ANALYSIS_ERROR;
        }
        void *mmapRet = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
        close(src_fd);
        if (mmapRet == (void *)-1) {
            munmap(mmapRet, sbuf.st_size);
            outputBuffer_.clear();
            handleError(fd_, "404 Not Found!");
            return ANALYSIS_ERROR;
        }
        char *src_addr = static_cast<char *>(mmapRet);
        outputBuffer_ += std::string(src_addr, src_addr + sbuf.st_size);
        ;
        munmap(mmapRet, sbuf.st_size);
        return ANALYSIS_SUCCESS;
    }

    return ANALYSIS_ERROR;
}


bool HttpContext::isNotFound(size_t pos) const {
    return pos == stringEndFlag_;
}

void HttpContext::removePre(size_t end) {
    if(end >= inputBuffer_.size()) inputBuffer_.clear();
    else inputBuffer_ = inputBuffer_.substr(end);
}

bool HttpContext::receiveOneLine() {
    bool res = !isNotFound(lineEndPos_ = inputBuffer_.find('\r', lineEndPos_));
    if(res){
        currentPosition_ = 0;
        line_ = inputBuffer_.substr(0, lineEndPos_);
        removePre(lineEndPos_ + 2);
    }
    return res;
}

void HttpContext::deleteTimer() {
    if(timer_.lock()){
        SP_Timer spTimer(timer_.lock());
        spTimer->deleteTimer();
        timer_.reset();
    }
}


//
// Created by luochensama on 2022/3/3.
//

#pragma once
#include "noncopyable.h"
#include "string"

class FileWriter : noncopyable{
public:
    explicit FileWriter(std::string filename);
    ~FileWriter();
    void append(const char* logline,size_t len);
    void flush();

private:
    size_t write(const char* logline,size_t len);

    static const int buffer_size = 64 * 1024;
    FILE* fp_;
    char buffer_[buffer_size];
};

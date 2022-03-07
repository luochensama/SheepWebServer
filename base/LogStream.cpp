//
// Created by luochensama on 2022/3/4.
//

#include "LogStream.h"
#include "algorithm"

const char digis[] = "9876543210123456789";
const char* zero = digis + 9;

template<typename T>
size_t convert(char buf[], T value){
    T temp = value;
    char* p = buf;
    do{
        *p = zero[static_cast<int>(temp%10)];
        p++;
        temp/=10;
    }while(temp);
    *p = '\0';
    std::reverse(buf,p);

    return p - buf;
}

template<typename T>
void LogStream::formatInteger(T value) {
    if(buffer_.avail() > kMaxNumericSize){
        size_t len = convert(buffer_.current(), value);
        buffer_.add(len);
    }
}

LogStream& LogStream::operator<<(short v) {
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v) {
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(double v) {
    if (buffer_.avail() >= kMaxNumericSize) {
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
        buffer_.add(len);
    }
    return *this;
}

LogStream& LogStream::operator<<(long double v) {
    if (buffer_.avail() >= kMaxNumericSize) {
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12Lg", v);
        buffer_.add(len);
    }
    return *this;
}
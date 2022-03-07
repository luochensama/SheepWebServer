//
// Created by luochensama on 2022/3/1.
//
#pragma once

class noncopyable{
public:
    noncopyable(const noncopyable&)=delete;
    void operator=(const noncopyable&)=delete;

protected:
    noncopyable()=default;
    ~noncopyable()=default;

};
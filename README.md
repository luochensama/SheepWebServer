# SheepWebServer



## 项目介绍

本项目是使用C++编写的一个高并发Web服务器，支持HTTP1.1，支持解析GET,HEAD请求，可处理静态资源，支持长连接，实现了简易的异步日志，能够记录服务器运行状态。整体架构参照了陈硕的[muduo](https://github.com/chenshuo/muduo)和github上的高星项目[WebServer](https://github.com/linyacool/WebServer)。

## 项目环境

- OS： Ubuntu 20.04
- IDE：Clion

## 项目特点

- 使用Epoll边沿触发的IO多路复用技术，非阻塞IO，使用多Reactor模式
- 使用多线程充分利用多核CPU，运用了muduo的one loop per thread思想
- 使用基于小根堆的定时器关闭超时请求
- 主线程只负责accept请求，并以Round Robin的方式分发给其它IO线程，只使用了极少量的锁操作
- 使用eventfd实现了线程的异步唤醒
- 使用双缓冲区技术实现了简单的异步日志系统
- 为减少内存泄漏的可能，使用智能指针（C++11）等RAII机制
- 使用状态机解析了HTTP请求
- 支持优雅关闭连接

## 测试结果

- 本项目基于muduo和WebServer，学习了muduo one loop per thread的思想，网络库的处理和muduo基本一致，不过epoll用的是ET模式，对http连接的处理整体思路参考了WebServer，不过对请求的处理做出一些优化，所以在处理长连接的时候会有明显的优势。
  测试方式与WebServer大体一致，在本地网络下使用WebBench，开启1000个客户端，时间为60s。线程池开启5线程。
  
- CPU：i5-9th，6核
  内存：8g
  
- 短连接

  WebServer：

  ![image-20220314155837936](/home/luochensama/.config/Typora/typora-user-images/image-20220314155837936.png)

  SheepWebServer：

  ![image-20220314160018558](/home/luochensama/.config/Typora/typora-user-images/image-20220314160018558.png)

- 长连接
  WebServer：

  ![image-20220314160121656](/home/luochensama/.config/Typora/typora-user-images/image-20220314160121656.png)

  SheepWebServer：

  ![image-20220314160053487](/home/luochensama/.config/Typora/typora-user-images/image-20220314160053487.png)



## 线程模型

![from WebServer](file:///home/luochensama/TinyWebServer/Pictures/model.png)

## 待改进

- LOG库的实现不够完善，考虑加入分级功能
- 在处理请求的时候使用的是string，可以改为更高效的定制缓冲，类似muduo的处理


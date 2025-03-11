#pragma once
#include <iostream>
#include <sys/epoll.h>
#include <vector>

class Epoll {
public:

    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op);
    std::vector<epoll_event> poll(int timeout = -1);
private:
    int epfd;
    struct epoll_event *events;  //指向 epoll_event 结构体的指针，并不是一个对象，此时没有分配内存。

};

/*
至于为啥，这两个作为私有变量，
因为除了最开始需要用到
 int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
也需要用到
*/
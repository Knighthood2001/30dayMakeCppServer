#pragma once

class InetAddress;
class Socket{
public:
    int fd; //文件描述符 file descriptor
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*); //接受一个指向 InetAddress 类型的指针
    void listen();
    void setnonblocking();

    int accept(InetAddress*);

    int getFd();

private:


}
#pragma once

class InetAddress;  //前向声明，告诉编译器有这么一个类，定义在InetAddress.h中
class Socket
{
private:
    int fd;  //文件描述符 file descriptor
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);
    void listen();
    void setnonblocking();

    int accept(InetAddress*);

    int getFd();
};
/*
void bind(InetAddress*);：绑定函数，接受一个指向InetAddress对象的指针作为参数。InetAddress对象通常包含IP地址和端口号，这个函数用于将套接字与指定的IP地址和端口号绑定。
void listen();：监听函数，将套接字设置为监听状态，等待来自客户端的连接请求。
void setnonblocking();：设置套接字为非阻塞模式。在非阻塞模式下，对套接字的操作（如读、写、接受连接）会立即返回，而不会等待操作完成。
int accept(InetAddress*);：接受函数，用于接受一个来自客户端的连接请求。如果成功，它会创建一个新的套接字用于与客户端通信，并可能通过参数返回客户端的地址信息。
int getFd();：获取函数，返回套接字的文件描述符。
*/

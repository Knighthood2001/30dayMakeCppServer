#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

/**
 * @brief Socket类的构造函数
 *
 * 初始化Socket对象，并创建一个新的套接字。
 *
 * @details 在构造函数中，通过调用socket函数创建一个新的套接字。
 * 构造函数初始化成员变量fd为-1，然后调用socket函数创建一个基于IPv4和TCP协议的套接字。
 * 如果创建套接字失败，即fd的值仍为-1，则抛出异常。
 */
Socket::Socket() : fd(-1){
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}
/**
 * @brief Socket 类的构造函数
 *
 * 用于创建一个新的 Socket 对象，并初始化其文件描述符。
 *
 * @param _fd 初始化的文件描述符
 *
 * @throw std::runtime_error 如果文件描述符无效（例如为 -1），则抛出异常
 */
Socket::Socket(int _fd) : fd(_fd){
    errif(fd == -1, "socket create error");
}

Socket::~Socket(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

/**
 * @brief 绑定套接字到指定的地址
 *
 * 将套接字绑定到指定的地址上。
 *
 * @param addr 指向InetAddress对象的指针，包含要绑定的地址信息
 *
 * @note 如果绑定失败，将抛出异常
 */
void Socket::bind(InetAddress *addr){
    //这里使用的是全局作用域运算符::来明确指定调用的是全局的bind函数
    errif(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "socket bind error");
}

void Socket::listen(){
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}
void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr){
    int clnt_sockfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    return clnt_sockfd;
}

int Socket::getFd(){
    return fd;
}
#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

Socket::Socket() : fd(-1) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}

Socket::Socket(int _fd): fd(_fd){
    errif(fd == -1, "socket create error"); // 将fd赋值为_fd
}
Socket::~Socket(){
    if(fd != -1){
        close(fd);  
        fd = -1;
    }
}
void Socket::bind(InetAddress *addr){
    errif(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "socket bind error");
    
}

void Socket::listen(){
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress* addr){
    int sockfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(sockfd == -1, "socket accept error");
    return sockfd;
};

//是的，Socket::getFd() 函数通常是为了访问类中的私有成员变量 fd 而设计的。
int Socket::getFd(){
    return fd;
}
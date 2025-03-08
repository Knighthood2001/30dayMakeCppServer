#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include "util.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

/**
 * @brief 将文件描述符设置为非阻塞模式
 *
 * 该函数将给定的文件描述符设置为非阻塞模式。
 *
 * @param fd 文件描述符
 */
void setnonblocking(int fd){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  //创建socket文件描述符
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;  //定义服务端地址结构体
    bzero(&serv_addr, sizeof(serv_addr));  //清空结构体
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");

    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");
    
    int epfd = epoll_create1(0);  //创建epoll树
    errif(epfd == -1, "epoll create error");

    struct epoll_event events[MAX_EVENTS], ev;  //定义事件数组和单个事件结构体
    bzero(&events, sizeof(events));  //清空事件数组

    bzero(&ev, sizeof(ev)); //清空单个事件结构体
    ev.data.fd = sockfd; //监听socket的文件描述符赋值给事件结构体
    ev.events = EPOLLIN | EPOLLET; //设置监听socket的事件类型为可读和边缘触发模式
    setnonblocking(sockfd);  //将监听socket设置为非阻塞模式
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);  //将监听socket加入epoll树

    while(true){ //epoll等待事件发生
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1); //等待事件发生
        errif(nfds == -1, "epoll wait error");
        for(int i = 0; i < nfds; ++i){ //遍历事件数组
            //遍历就绪的文件描述符列表events，处理每个事件：
                //如果是监听套接字的可读事件，表示有新的客户端连接。使用accept接受连接，并将新连接的套接字也设置为非阻塞模式，添加到epoll实例中监听其可读事件。
                //如果是其他已连接套接字的可读事件，表示有数据可读。使用read读取数据，并回显给客户端。由于使用了非阻塞I/O，需要处理EAGAIN或EWOULDBLOCK错误，表示当前没有更多数据可读。如果read返回0，表示客户端关闭了连接，服务器关闭对应的套接字。

            if(events[i].data.fd == sockfd){        //新客户端连接
                struct sockaddr_in clnt_addr;  //定义客户端地址结构体
                bzero(&clnt_addr, sizeof(clnt_addr)); 
                socklen_t clnt_addr_len = sizeof(clnt_addr);  //定义客户端地址长度变量

                int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);  //接受客户端连接
                errif(clnt_sockfd == -1, "socket accept error");
                printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

                bzero(&ev, sizeof(ev));
                ev.data.fd = clnt_sockfd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(clnt_sockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev); //将客户端socket加入epoll树
            } else if(events[i].events & EPOLLIN){      //可读事件
                char buf[READ_BUFFER];
                while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
                    bzero(&buf, sizeof(buf));
                    ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                    if(bytes_read > 0){
                        printf("message from client fd %d: %s\n", events[i].data.fd, buf);
                        write(events[i].data.fd, buf, sizeof(buf));
                    } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
                        printf("continue reading");
                        continue;
                    } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
                        printf("finish reading once, errno: %d\n", errno);
                        break;
                    } else if(bytes_read == 0){  //EOF，客户端断开连接
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);   //关闭socket会自动将文件描述符从epoll树上移除
                        break;
                    }
                }
            } else{         //其他事件，之后的版本实现
                printf("something else happened\n");
            }
        }
    }
    close(sockfd);
    return 0;
}

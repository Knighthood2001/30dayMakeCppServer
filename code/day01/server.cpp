#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
    //1. 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //2. 初始化服务器地址结构serv_addr，设置端口号和IP地址
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    //3. 将套接字与服务器地址绑定
    // bind函数将套接字sockfd与服务器地址serv_addr绑定。这样，套接字就被分配到了指定的IP地址和端口上。
    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));  

    //4. 监听端口
    listen(sockfd, SOMAXCONN);  // 监听连接请求，第二个参数是最大监听数
    
    //5. 接受客户端连接请求
    //clnt_addr用于存储客户端的地址信息。clnt_addr_len是clnt_addr的大小。accept函数阻塞等待直到一个连接到来，然后返回一个新的套接字clnt_sockfd，用于与客户端通信。clnt_addr和clnt_addr_len被更新以反映客户端的地址信息。
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);

    //6. 输出客户端信息
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    return 0;
}

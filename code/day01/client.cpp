/*
 * @Author: your name
 * @Date: 2022-01-04 20:03:45
 * @LastEditTime: 2022-01-05 19:08:58
 * @LastEditors: your name
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \30dayMakeCppServer\code\day01\client.cpp
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // 创建socket文件描述符

    struct sockaddr_in serv_addr;  // 服务器地址结构体
    bzero(&serv_addr, sizeof(serv_addr));  // 初始化结构体
    serv_addr.sin_family = AF_INET; // ipv4协议地址族
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器地址
    serv_addr.sin_port = htons(8888);  // 服务器端口号

    //bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)); 客户端不进行bind操作，因为服务器会分配一个端口号

    // 连接服务器，成功返回0，失败-1
    connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));    
    
    return 0;
}

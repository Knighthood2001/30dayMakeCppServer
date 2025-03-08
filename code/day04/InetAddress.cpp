#include "InetAddress.h"
#include <string.h>
InetAddress::InetAddress() : addr_len(sizeof(addr)){
    bzero(&addr, sizeof(addr));
}

/**
 * @brief InetAddress 类的构造函数
 *
 * 初始化 InetAddress 对象，设置 IP 地址和端口号。
 *
 * @param ip IPv4 地址的字符串表示
 * @param port 端口号，使用网络字节序（大端序）
 */
InetAddress::InetAddress(const char* ip, uint16_t port) : addr_len(sizeof(addr)){
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    addr_len = sizeof(addr);
}

InetAddress::~InetAddress(){
}

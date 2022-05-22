#include "../../include/socket/SockFd.h"
#include "../../include/socket/SockAddress.h"
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;

namespace reactor
{
    SockFd::SockFd()
        : _sockfd(createSockFd())
    {
        nonBlock(_sockfd);
        cout << "SockFd()" << endl;
    }
    SockFd::SockFd(int sockfd)
        : _sockfd(sockfd)
    {
        nonBlock(_sockfd);
        cout << "SockFd(int sockfd)" << endl;
    }
    SockFd::~SockFd()
    {
        close(_sockfd);
    }

    void SockFd::shutDownWrite()
    {
        int ret = shutdown(_sockfd, SHUT_WR);
        if (ret == -1)
        {
            perror("shutdown");
        }
    }

    void SockFd::setNonBlock()
    {
        nonBlock(_sockfd);
    }
    // 通过套接字描述符来获取自己的IP地址和连接对端的IP地址
    SockAddress SockFd::getSerAddr(int connfd)
    {
        // getsockname获取与某个套接字关联的本地协议地址
        struct sockaddr_in addr;
        socklen_t len = sizeof(sockaddr_in);
        if (::getsockname(connfd, (struct sockaddr *)&addr, &len) == -1)
        {
            perror("getsockname error");
        }
        return SockAddress(addr);
    }

    SockAddress SockFd::getCliAddr(int connfd)
    {
        // getpeername获取当前连接的客户端的IP地址和端口号
        struct sockaddr_in addr;
        socklen_t len = sizeof(sockaddr_in);
        if (::getpeername(connfd, (struct sockaddr *)&addr, &len) == -1)
        {
            perror("getpeername error");
        }
        return SockAddress(addr);
    }

} // end space
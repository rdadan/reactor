#include "SockFd.h"
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "SockAddress.h"
namespace reactor
{
    SockFd::SockFd()
        : _sockfd(SockFd::createSockFD())
    {
    }
    SockFd::SockFd(int sockfd)
        : _sockfd(sockfd)
    {
    }
    SockFd::~SockFd()
    {
        close(_sockfd);
    }
    int SockFd::createSockFD()
    {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1)
        {
            perror("SockFd");
        }
        return fd;
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
        int status = fcntl(_sockfd, F_GETFD, 0);
        fcntl(_sockfd, F_SETFL, status | O_NONBLOCK);
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
#ifndef __SockFd_H__
#define __SockFd_H__

#include "../util/NonCopy.h"
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
using std::cout;
using std::endl;

// SockFd 主要用于管理sock文件描述符,
// 以及通过sockfd获取其绑定的地址信息（ip/port）
namespace reactor
{
    class SockAddress;
    class SockFd : public NonCopy
    {
    public:
        SockFd();
        SockFd(int sockfd);
        ~SockFd();            // 全关闭
        void shutDownWrite(); // 半关闭
        int getSockFD() const { return _sockfd; }
        void setNonBlock();
        static SockAddress getSerAddr(int sockfd);
        static SockAddress getCliAddr(int sockfd);

    private:
        int _sockfd;
    };

    inline void nonBlock(int fd)
    {
        int status = fcntl(fd, F_GETFD, 0);
        fcntl(fd, F_SETFL, status | O_NONBLOCK);
    }
    inline int createSockFd()
    {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1)
        {
            perror("SockFd");
        }
        return fd;
    }
} // end space
#endif
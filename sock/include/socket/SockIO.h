#ifndef __SOCKIO_H__
#define __SOCKIO_H__
#include <stdio.h>
namespace reactor
{
    class SockIO
    {
    public:
        SockIO();
        SockIO(int sockfd);
        size_t readn(char *buf, size_t len);
        size_t writen(const char *buf, size_t len);
        size_t readline(char *buf, size_t len);

    private:
        size_t recv_peek(char *buf, size_t len);
        int _sockfd;
    };
}
#endif
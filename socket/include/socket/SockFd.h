#ifndef __SockFd_H__
#define __SockFd_H__

#include "NonCopy.h"

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
        int createSockFD();
        int getSockFD() { return _sockfd; }
        void setNonBlock();
        static SockAddress getSerAddr(int sockfd);
        static SockAddress getCliAddr(int sockfd);

    private:
        int _sockfd;
    };

} // end space
#endif
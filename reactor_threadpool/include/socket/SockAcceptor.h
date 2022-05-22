#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__
#include "SockFd.h"
#include "SockAddress.h"
namespace reactor
{

    // 完成三次握手
    class SockAcceptor
    {
    public:
        SockAcceptor(const SockAddress &addr);
        SockAcceptor(int sockfd, const SockAddress &addr);
        int fd() const { return _sock.getSockFD(); }
        ~SockAcceptor();

        void listen();
        void bind();
        int accept();

        void setReuseAddr();
        void setReusePort();
        void ready();

    private:
        SockFd _sock;
        SockAddress _addr;
    };
} // namespace reactor

#endif
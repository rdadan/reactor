#include "SockAcceptor.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
namespace reactor
{
    SockAcceptor::SockAcceptor(const SockAddress &addr)
        : _sock(), _addr(addr)
    {
    }
    SockAcceptor::SockAcceptor(int sockfd, const SockAddress &addr)
        : _sock(sockfd), _addr(addr)
    {
    }
    SockAcceptor::~SockAcceptor()
    {
    }

    void SockAcceptor::bind()
    {
        int fd = _sock.getSockFD();
        const struct sockaddr_in *addr = _addr.getSockAddr();
        int ret = ::bind(fd, (struct sockaddr *)addr, sizeof(sockaddr_in));
        if (ret == -1)
        {
            perror("bind");
        }
    }
    void SockAcceptor::listen()
    {
        int ret = ::listen(_sock.getSockFD(), 10);
        if (ret == -1)
        {
            perror("bind");
            close(_sock.getSockFD());
        }
    }
    int SockAcceptor::accept()
    {
        int sockfd = _sock.getSockFD();
        const struct sockaddr_in *addr = _addr.getSockAddr();
        socklen_t adr_sz = sizeof(*addr);
        int client_fd = ::accept(sockfd, (struct sockaddr *)addr, &adr_sz);
        if (client_fd == -1)
        {
            perror("accept");
        }
        return client_fd;
    }

    void SockAcceptor::setReuseAddr()
    {
        int sockfd = _sock.getSockFD();
        int on = 1;
        int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    }

    void SockAcceptor::setReusePort()
    {
        int sockfd = _sock.getSockFD();
        int on = 1;
        int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    }
    void SockAcceptor::ready()
    {
        SockAcceptor::setReuseAddr();
        SockAcceptor::setReusePort();
        SockAcceptor::bind();
        SockAcceptor::listen();
    }

} //
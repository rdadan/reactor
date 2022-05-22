#include "../../include/socket/SockAcceptor.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;
namespace reactor
{
    SockAcceptor::SockAcceptor(const SockAddress &addr)
        : _sock(), _addr(addr)
    {
        cout << "SockAcceptor(const SockAddress &addr): _sock(), _addr(addr) " << endl;
    }
    SockAcceptor::SockAcceptor(int serverfd, const SockAddress &addr)
        : _sock(serverfd), _addr(addr)
    {
        cout << "SockAcceptor(int serverfd, const SockAddress &addr): _sock(), _addr(addr) " << endl;
    }
    SockAcceptor::~SockAcceptor()
    {
    }

    void SockAcceptor::bind()
    {
        const struct sockaddr_in *addr = _addr.getSockAddr();
        int ret = ::bind(_sock.getSockFD(), (struct sockaddr *)addr, sizeof(sockaddr_in));
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
        const struct sockaddr_in *addr = _addr.getSockAddr();
        socklen_t adr_sz = sizeof(*addr);
        int client_fd = ::accept(_sock.getSockFD(), (struct sockaddr *)addr, &adr_sz);
        if (client_fd == -1)
        {
            perror("accept");
        }
        return client_fd;
    }

    void SockAcceptor::setReuseAddr()
    {
        int on = 1;
        int ret = ::setsockopt(_sock.getSockFD(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
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
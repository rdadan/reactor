#include "SockAddress.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
namespace reactor
{
    SockAddress::SockAddress()
    {
        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        _addr.sin_addr.s_addr = htonl(INADDR_ANY);
        _addr.sin_port = htons(8888);
    }
    SockAddress::SockAddress(const char *ip, unsigned short port)
    {
        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        _addr.sin_addr.s_addr = inet_addr(ip);
        _addr.sin_port = htons(8888);
    }
    SockAddress::SockAddress(const struct sockaddr_in &addr)
        : _addr(addr)
    {
    }
    SockAddress::~SockAddress()
    {
    }

    string SockAddress::getIp() const
    {
        return string(inet_ntoa(_addr.sin_addr));
    }
    unsigned short SockAddress::getPort() const
    {
        return ntohs(_addr.sin_port);
    }
    const struct sockaddr_in *SockAddress::getSockAddr() const
    {
        return &_addr;
    }
} //
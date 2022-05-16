
#include "SockConnection.h"
#include <string.h>

namespace reactor
{

    // SockConnection::SockConnection()
    //     : _sockfd(), _serAddr(), _cliAddr(), _sockIO(), _isShutdownWrite(false)
    // {
    // }
    // SockConnection::SockConnection(int clifd)
    //     : _sockfd(clifd), _sockIO(clifd), _serAddr(), _cliAddr(), _isShutdownWrite(false)
    // {
    // }
    SockConnection::SockConnection(int clifd)
        : _sockfd(clifd),
          _sockIO(clifd),
          _serAddr(SockFd::getSerAddr(clifd)),
          _cliAddr(SockFd::getCliAddr(clifd)),
          _isShutdownWrite(false)
    {
    }

    SockConnection::~SockConnection()
    {
        if (!_isShutdownWrite)
        {
            _isShutdownWrite = true;
            closeConn();
        }
        printf("~TcpConnection()\n");
    }
    void SockConnection::closeConn()
    {
        if (!_isShutdownWrite)
            _sockfd.shutDownWrite();
        _isShutdownWrite = true;
    }

    void SockConnection::sendMsg(const string msg)
    {
        _sockIO.writen(msg.c_str(), msg.size());
    }
    string SockConnection::recvMsg()
    {
        char buf[65536];
        memset(buf, 0, sizeof(buf));
        size_t ret = _sockIO.readline(buf, sizeof(buf));
        if (ret == 0)
        {
            return std::string();
        }
        else
            return std::string(buf);
    }

    string SockConnection::toString()
    {
        char str[100];
        snprintf(str, sizeof(str), "ser:%s:%d -> cli:%s:%d",
                 _serAddr.getIp().c_str(),
                 _serAddr.getPort(),
                 _cliAddr.getIp().c_str(),
                 _cliAddr.getPort());
        return std::string(str);
    }

} // namespace reactor

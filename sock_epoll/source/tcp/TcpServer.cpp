#include "../../include/tcp/TcpServer.h"
#include "../../include/socket/SockAddress.h"
namespace reactor
{
    TcpServer::TcpServer()
        : _sockAccept(SockAddress()),
          _epollPoller(_sockAccept)
    {
    }
    TcpServer::TcpServer(const char *ip, unsigned short port)
        : _sockAccept(SockAddress(ip, port)),
          _epollPoller(_sockAccept)
    {
    }

    TcpServer::~TcpServer()
    {
        _epollPoller.unloop();
    }

    void TcpServer::startTcpServer()
    {
        _sockAccept.ready();
        _epollPoller.setConnectionCallback(_connCallBack);
        _epollPoller.setMessageCallback(_msgCallBack);
        _epollPoller.setCloseCallback(_closeCallBack);
        _epollPoller.loop();
    }
    void TcpServer::stopTcpServer()
    {
        _epollPoller.unloop();
    }

}
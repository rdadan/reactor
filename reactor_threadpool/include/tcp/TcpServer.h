#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include "../../include/socket/SockAcceptor.h"
#include "../../include/epoll/EpollPoller.h"
namespace reactor
{
    // using TcpConnPtr = SockConnPtr;
    class TcpServer
    {
    public:
        using TcpConnCallBack = EpollPoller::EpollConnCallBack;
        TcpServer();
        TcpServer(const char *ip, unsigned short port);
        TcpServer(unsigned short port);
        ~TcpServer();

    public:
        void startTcpServer();
        void stopTcpServer();

    private:
        SockAcceptor _sockAccept;
        EpollPoller _epollPoller;

    public:
        void setTcpConnCallBack(TcpConnCallBack cb) { _connCallBack = cb; }
        void setTcpMsgCallBack(TcpConnCallBack cb) { _msgCallBack = cb; }
        void setTcpCloseCallBack(TcpConnCallBack cb) { _closeCallBack = cb; }

    private:
        TcpConnCallBack _connCallBack;
        TcpConnCallBack _msgCallBack;
        TcpConnCallBack _closeCallBack;
    };
}

#endif
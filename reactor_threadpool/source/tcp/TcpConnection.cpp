#include "../../include/tcp/TcpConnection.h"
#include "../../include/epoll/EpollPoller.h"
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;
namespace reactor
{

    // TcpConnection::TcpConnection()
    //     : _sockfd(), _serAddr(), _cliAddr(), _sockIO(), _isShutdownWrite(false)
    // {
    // }
    // TcpConnection::TcpConnection(int clifd)
    //     : _sockfd(clifd), _sockIO(clifd), _serAddr(), _cliAddr(), _isShutdownWrite(false)
    // {
    // }
    // TcpConnection::TcpConnection(int clifd)
    //     : _sockfd(clifd),
    //       _sockIO(clifd),
    //       _serAddr(SockFd::getSerAddr(clifd)),
    //       _cliAddr(SockFd::getCliAddr(clifd)),
    //       _isShutdownWrite(false)
    // {
    //     _sockfd.setNonBlock();
    // }
    TcpConnection::TcpConnection(int clifd, EpollPoller *pEpollLoop)
        : _sockfd(clifd),
          _sockIO(clifd),
          _serAddr(SockFd::getSerAddr(clifd)),
          _cliAddr(SockFd::getCliAddr(clifd)),
          _isShutdownWrite(false),
          _pEpollLoop(pEpollLoop)

    {
        _sockfd.setNonBlock();
        cout << "TcpConnection(int clifd, EpollPoller *pEpollLoop)" << endl;
    }

    TcpConnection::~TcpConnection()
    {
        if (!_isShutdownWrite)
        {
            _isShutdownWrite = true;
            closeConn();
        }
        printf("~TcpConnection()\n");
    }
    void TcpConnection::closeConn()
    {
        if (!_isShutdownWrite)
            _sockfd.shutDownWrite();
        _isShutdownWrite = true;
    }

    void TcpConnection::sendMsg(const string msg)
    {
        _sockIO.writen(msg.c_str(), msg.size());
    }
    string TcpConnection::recvMsg()
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

    string TcpConnection::toString()
    {
        char str[100];
        snprintf(str, sizeof(str), "ser:%s:%d -> cli:%s:%d",
                 _serAddr.getIp().c_str(),
                 _serAddr.getPort(),
                 _cliAddr.getIp().c_str(),
                 _cliAddr.getPort());
        return std::string(str);
    }

    void TcpConnection::doHandlCallBack(TcpConnCallBack &cbFunc)
    {
        // ?????????????????????A???share_ptr?????????
        // ?????????A??????????????????????????????????????????????????????????????????????????????????????????????????????????????????share_ptr
        if (cbFunc)
            cbFunc(shared_from_this()); //????????????????????????std::share_ptr
    }

    // ??????????????????????????????????????????????????????????????????Epoll?????????
    void TcpConnection::sendMsgToLoop(const string &msg)
    {
        // ???conn????????????epoll
        _pEpollLoop->sendMsgInLoop(std::bind(&TcpConnection::sendMsg, this, msg));
    }

} // namespace reactor

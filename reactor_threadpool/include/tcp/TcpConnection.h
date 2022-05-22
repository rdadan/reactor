#ifndef __SOCKCONNECTION_H__
#define __SOCKCONNECTION_H__
#include "SockFd.h"
#include "SockAddress.h"
#include "SockIO.h"
#include <string>

#include <memory>
#include <functional>
using std::function;
using std::shared_ptr;
namespace reactor
{
    class TcpConnection;
    class EpollPoller;

    using spTcpConnPtr = shared_ptr<TcpConnection>;

    class TcpConnection : NonCopy, public std::enable_shared_from_this<TcpConnection>
    {
    public:
        // TcpConnection();
        // TcpConnection(int clifd);
        TcpConnection(int clifd, EpollPoller *pEpollLoop);
        ~TcpConnection();
        void sendMsg(const string msg);
        string recvMsg();
        void closeConn();
        string toString();

    public:
        // 返回值void,参数为 const TcpConnPtr & 的函数模板
        using TcpConnCallBack = function<void(const spTcpConnPtr &)>;
        void setConnCallBack(TcpConnCallBack cbFunc) { _connFunc = cbFunc; }
        void setMsgCallBack(TcpConnCallBack cbFunc) { _msgFunc = cbFunc; }
        void setCloseCallBackb(TcpConnCallBack cbFunc) { _closeFunc = cbFunc; }

        // handle任务是返回当前的连接对象的shared_ptr
        void doHandlCallBack(TcpConnCallBack &cbFunc);
        void handleConnCallBack() { doHandlCallBack(_connFunc); }
        void handleMsgCallBack() { doHandlCallBack(_msgFunc); }
        void handleCloseCallBack() { doHandlCallBack(_closeFunc); }

    public:
        void sendMsgToLoop(const string &msg);

    private:
        SockFd _sockfd;
        const SockAddress _serAddr;
        const SockAddress _cliAddr;
        SockIO _sockIO;
        bool _isShutdownWrite;

    private:
        EpollPoller *_pEpollLoop;

    private:
        TcpConnCallBack _connFunc;
        TcpConnCallBack _msgFunc;
        TcpConnCallBack _closeFunc;
    };

} // namespace reactor

#endif
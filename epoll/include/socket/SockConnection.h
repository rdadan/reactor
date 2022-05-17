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
    class SockConnection;
    using SockConnPtr = shared_ptr<SockConnection>;

    class SockConnection : NonCopy, public std::enable_shared_from_this<SockConnection>
    {
    public:
        // SockConnection();
        SockConnection(int clifd);
        ~SockConnection();
        void sendMsg(const string msg);
        string recvMsg();
        void closeConn();
        string toString();

    public:
        // 返回值void,参数为 const SockConnPtr & 的函数模板
        using SockConnCallBack = function<void(const SockConnPtr &)>;
        void setConnCallBack(SockConnCallBack cbFunc) { _connFunc = cbFunc; }
        void setMsgCallBack(SockConnCallBack cbFunc) { _msgFunc = cbFunc; }
        void setCloseCallBackb(SockConnCallBack cbFunc) { _closeFunc = cbFunc; }

        // 返回当前的对象的shared_ptr
        void doHandlCallBack(SockConnCallBack &cbFunc);
        void handleConnCallBack() { doHandlCallBack(_connFunc); }
        void handleMsgCallBack() { doHandlCallBack(_msgFunc); }
        void handleCloseCallBack() { doHandlCallBack(_closeFunc); }

    private:
        SockConnCallBack _connFunc;
        SockConnCallBack _msgFunc;
        SockConnCallBack _closeFunc;

    private:
        SockFd _sockfd;
        const SockAddress _serAddr;
        const SockAddress _cliAddr;
        SockIO _sockIO;
        bool _isShutdownWrite;
    };

} // namespace reactor

#endif
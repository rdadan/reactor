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
    using sockConnPtr = shared_ptr<SockConnection>;

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
        // 返回值void,参数为 const sockConnPtr & 的函数模板
        using sockConnFunc = function<void(const sockConnPtr &)>;
        void setConnCallBack(sockConnFunc cbFunc) { _connFunc = cbFunc; }
        void setMsgCallBack(sockConnFunc cbFunc) { _msgFunc = cbFunc; }
        void setCloseCallBackb(sockConnFunc cbFunc) { _closeFunc = cbFunc; }

        // 返回当前的对象的shared_ptr
        void doHandlCallBack(sockConnFunc &cbFunc);
        void handleConnCallBack() { doHandlCallBack(_connFunc); }
        void handleMsgCallBack() { doHandlCallBack(_msgFunc); }
        void handleCloseCallBack() { doHandlCallBack(_closeFunc); }

    private:
        sockConnFunc _connFunc;
        sockConnFunc _msgFunc;
        sockConnFunc _closeFunc;

    private:
        SockFd _sockfd;
        const SockAddress _serAddr;
        const SockAddress _cliAddr;
        SockIO _sockIO;
        bool _isShutdownWrite;
    };

} // namespace reactor

#endif
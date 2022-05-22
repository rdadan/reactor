#ifndef __EPOLLPOLLER_H__
#define __EPOLLPOLLER_H__
#include "../util/NonCopy.h"
#include "../tcp/TcpConnection.h"
#include "../threadpool/Mutex.h"

#include <sys/epoll.h>
#include <vector>
#include <map>

namespace reactor
{
    using namespace THREADPOOL;

    class SockAcceptor;

    class EpollPoller : NonCopy
    {
    public:
        EpollPoller(SockAcceptor &sockAccept);
        ~EpollPoller();
        void loop();
        void unloop();

    public:
        using EpollConnCallBack = TcpConnection::TcpConnCallBack;
        void setConnectionCallback(EpollConnCallBack cbFunc) { _connFunc = cbFunc; }
        void setMessageCallback(EpollConnCallBack cbFunc) { _msgFunc = cbFunc; }
        void setCloseCallback(EpollConnCallBack cbFunc) { _closeFunc = cbFunc; }

    private:
        void waitEpollfd();
        void handleConnection();
        void handleMessage(int peerfd);

    private:
        EpollConnCallBack _connFunc;
        EpollConnCallBack _msgFunc;
        EpollConnCallBack _closeFunc;

    public:
        using EpollSendFunc = std::function<void()>;
        void sendMsgInLoop(const EpollSendFunc &&cb);
        void sendMsgInLoop();
        void writeEventFd();
        void readEventFd();

    private:
        Mutex _mutex;
        std::vector<EpollSendFunc> _vecEpollSendFunc;
        int _eventfd; // 事件通知.用于计算线程和IO线程通信
    private:
        SockAcceptor &_sockAccept;
        int _epfd;
        int _servfd;
        bool _islopping;

    private:
        // 监听的事件集合, 默认大小为1024
        using VecEvent = std::vector<struct epoll_event>;
        VecEvent _vecEvent;
        // key:peerfd, val: shared_ptr<TcpConnection>
        using ConnectionMap = std::map<int, spTcpConnPtr>;
        ConnectionMap _mapConn;
    };

    //预览数据
    inline size_t recvPeek(int sockfd, void *buf, size_t len)
    {
        int nread;
        do
        {
            nread = ::recv(sockfd, buf, len, MSG_PEEK);
        } while (nread == -1 && errno == EINTR);
        return nread;
    }

    //通过预览数据，判断conn是否关闭
    inline bool isConnectionClosed(int sockfd)
    {
        char buf[1024];
        int nread = recvPeek(sockfd, buf, sizeof(buf));
        if (-1 == nread)
        {
            perror("recvPeek--- ");
            return true;
        }
        return (0 == nread);
    }
} // end reactor
#endif
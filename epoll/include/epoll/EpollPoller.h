#ifndef __EPOLLPOLLER_H__
#define __EPOLLPOLLER_H__
#include "../socket/NonCopy.h"
#include "../socket/SockConnection.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
namespace reactor
{
    class SockAcceptor;

    class EpollPoller : NonCopy
    {
    public:
        EpollPoller(SockAcceptor &sockAccept);
        ~EpollPoller();
        void loop();
        void unloop();

    public:
        using EpollConnFunc = SockConnection::sockConnFunc;
        using EpollFunctor = std::function<void()>;

    public:
        void setConnectionCallback(EpollConnFunc cbFunc) { _connFunc = cbFunc; }
        void setMessageCallback(EpollConnFunc cbFunc) { _msgFunc = cbFunc; }
        void setCloseCallback(EpollConnFunc cbFunc) { _closeFunc = cbFunc; }

    private:
        void waitEpollfd();
        void handleConnection();
        void handleMessage(int peerfd);

    private:
        EpollConnFunc _connFunc;
        EpollConnFunc _msgFunc;
        EpollConnFunc _closeFunc;

    private:
    private:
        SockAcceptor &_sockAccept;
        int _epfd;
        int _servfd;
        bool _islopping;

    private:
        // 监听的事件集合, 默认大小为1024
        using VecEvent = std::vector<struct epoll_event>;
        VecEvent _vecEvent;
        // key:peerfd, val: shared_ptr<SockConnection>
        using ConnectionMap = std::map<int, sockConnPtr>;
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
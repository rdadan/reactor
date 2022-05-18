#include "../../include/epoll/EpollPoller.h"
#include "../../include/epoll/Epoll.h"
#include "../../include/socket/SockAcceptor.h"
#include <unistd.h>
#include <assert.h>

namespace reactor
{
    EpollPoller::EpollPoller(SockAcceptor &sockAccept)
        : _sockAccept(sockAccept),
          _epfd(createEpollFd()),
          _servfd(sockAccept.fd()),
          _islopping(false)
    //   _vecEvent(1024)
    {
        _vecEvent.resize(1024);         // 不能用reserve，需要真实开辟空间
        addEpollFdRead(_epfd, _servfd); // 注册服务器的sock
    }
    EpollPoller::~EpollPoller()
    {
        ::close(_epfd);
    }
    void EpollPoller::loop()
    {
        _islopping = true;
        while (_islopping)
        {
            waitEpollfd();
        }
    }

    void EpollPoller::unloop()
    {
        if (_islopping)
            _islopping = false;
    }

    // void EpollPoller::setConnectionCallback(EpollConnCallBack cbFunc)
    // {
    //     _connFunc = cbFunc;
    // }

    // void EpollPoller::setMessageCallback(EpollConnCallBack cbFunc)
    // {
    //     _msgFunc = cbFunc;
    // }

    // void EpollPoller::setCloseCallback(EpollConnCallBack cbFunc)
    // {
    //     _closeFunc = cbFunc;
    // }

    void EpollPoller::waitEpollfd()
    {
        int nready;
        do
        {
            nready = ::epoll_wait(_epfd,
                                  &(*_vecEvent.begin()),
                                  _vecEvent.size(),
                                  3000);          //等待3s返回
        } while (nready == -1 && errno == EINTR); // 循环等待事件触发

        if (nready == -1)
        {
            perror("epoll_wait error");
            exit(EXIT_FAILURE);
        }
        else if (nready == 0)
        {
            printf("epoll_wait timeout\n");
        }
        else
        {
            // 连接池满，做一个扩容的操作
            if (nready == static_cast<int>(_vecEvent.size()))
            {
                _vecEvent.resize(_vecEvent.size() * 2);
            }

            //遍历每一个激活的文件描述符
            for (int idx = 0; idx != nready; ++idx)
            {
                // 服务器处理连接请求
                if (_vecEvent[idx].data.fd == _servfd)
                {
                    if (_vecEvent[idx].events & EPOLLIN)
                    {
                        handleConnection();
                    }
                }
                else
                {
                    // 服务器处理处理客户端消息
                    if (_vecEvent[idx].events & EPOLLIN)
                    {
                        handleMessage(_vecEvent[idx].data.fd);
                    }
                }
            }
        }
    }

    void EpollPoller::handleConnection()
    {
        // 建立连接
        int peerfd = _sockAccept.accept();
        // 注册客户端描述符
        addEpollFdRead(_epfd, peerfd);

        // 创建连接对象
        SockConnPtr pConn(new SockConnection(peerfd));
        // 连接成功后，注册以后将要给客户端发送的信息
        pConn->setConnCallBack(_connFunc);
        pConn->setMsgCallBack(_msgFunc);
        pConn->setCloseCallBackb(_closeFunc);

        std::pair<ConnectionMap::iterator, bool> ret;
        ret = _mapConn.insert(std::make_pair(peerfd, pConn));
        assert(ret.second == true);
        (void)ret;

        pConn->handleConnCallBack();
    }
    void EpollPoller::handleMessage(int peerfd)
    {
        bool isClosed = isConnectionClosed(peerfd);
        // 查找客户端的描述符, 返回 SockConnPtr
        ConnectionMap::iterator it = _mapConn.find(peerfd);
        assert(it != _mapConn.end());
        SockConnPtr pConn = it->second;
        if (isClosed)
        {
            pConn->handleCloseCallBack();
            delEpollFdRead(_epfd, peerfd);
            _mapConn.erase(it);
        }
        else
        {
            pConn->handleMsgCallBack();
        }
    }

} // end reactor
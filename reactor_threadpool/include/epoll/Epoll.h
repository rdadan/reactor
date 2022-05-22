// #ifndef __EPOLLPOLLER_H__
// #define __EPOLLPOLLER_H__
#ifndef __EPOLL_H__
#define __EPOLL_H__
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/eventfd.h>

namespace reactor
{

    inline int createEpollFd()
    {
        int epfd = epoll_create1(0);
        if (epfd == -1)
        {
            perror("epoll_create1");
        }
    }
    inline int createEventFd()
    {
        int event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        if (event_fd == -1)
        {
            perror("eventfd");
        }
        return event_fd;
    }

    inline int addEpollFdRead(int epfd, int fd)
    {
        struct epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLIN | EPOLLET; //边缘触发
        int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
        if (ret == -1)
        {
            perror("epoll_ctl");
            exit(EXIT_FAILURE);
        }
    }
    inline int delEpollFdRead(int epfd, int fd)
    {
        struct epoll_event event;
        event.data.fd = fd;
        int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
        if (ret == -1)
        {
            perror("epoll_ctl");
            exit(EXIT_FAILURE);
        }
    }

} // reactor

#endif
#include "../../include/socket/SockIO.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
using std::cout;
using std::endl;

namespace reactor
{

    SockIO::SockIO()
        : _sockfd()
    {
        cout << "SockIO()" << endl;
    }
    SockIO::SockIO(int sockfd)
        : _sockfd(sockfd)
    {
        cout << "SockIO(int sockfd)" << endl;
    }

    size_t SockIO::readn(char *buf, size_t len)
    {
        size_t needRead = len;
        char *pbuf = buf;
        while (needRead > 0)
        {
            int hasReed = ::read(_sockfd, buf, needRead);
            if (hasReed == -1) // 读取出错
            {
                if (errno == EINTR) //如果是中断信号则忽略
                    continue;
                return EXIT_FAILURE;
            }

            if (hasReed == 0) // EOF 读取结束
                break;

            pbuf += hasReed;
            needRead -= hasReed;
        }
        return len - needRead;
    }
    size_t SockIO::writen(const char *buf, size_t len)
    {
        size_t needWrite = len;
        const char *pbuf = buf;
        while (needWrite > 0)
        {
            int hasReed = ::write(_sockfd, buf, needWrite);
            if (hasReed == -1) // 读取出错
            {
                if (errno == EINTR) //如果是中断信号则忽略
                    continue;
                return EXIT_FAILURE;
            }

            if (hasReed == 0) // EOF 读取结束
                break;

            pbuf += hasReed;
            needWrite -= hasReed;
        }
        return len - needWrite;
    }

    // 按行读取
    size_t SockIO::readline(char *buf, size_t len)
    {
        size_t nleft = len - 1;
        char *pbuf = buf;
        size_t total = 0;
        while (nleft > 0)
        {
            int nread = SockIO::recv_peek(pbuf, nleft);
            if (nread <= 0)
                return nread;

            //检查'\n'
            for (size_t idx = 0; idx != nread; ++idx)
            {
                if (pbuf[idx] == '\n')
                {
                    size_t nsize = idx + 1;
                    if (readn(pbuf, nsize) != nsize)
                        return EXIT_FAILURE;
                    pbuf += nsize;
                    total += nsize;
                    *pbuf = 0;
                    return total;
                }
            }
            if (readn(pbuf, nread) != nread)
                return EXIT_FAILURE;
            pbuf += nread;
            nleft -= nread;
            total += nread;
        }
        *pbuf = 0;
        return len - 1;
    }

    size_t SockIO::recv_peek(char *buf, size_t len)
    {
        int nread;
        do
        {
            // recv()的原型是ssize_t recv(int sockfd, void *buf, size_t len, int flags);
            /*通常flags设置为0，此时recv()函数读取tcp 缓冲区中的数据到buf中，并从tcp 缓冲区中移除已读取的数据。
                如果把flags设置为MSG_PEEK，仅仅是把tcp 缓冲区中的数据读取到buf中，没有把已读取的数据从tcp 缓冲区中移除，
                如果再次调用recv()函数仍然可以读到刚才读到的数据。*/
            nread = ::recv(_sockfd, buf, len, MSG_PEEK);

        } while (nread == -1 && errno == EINTR);

        return nread;
    }
} // space reactor
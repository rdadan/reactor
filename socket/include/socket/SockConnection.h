#ifndef __SOCKCONNECTION_H__
#define __SOCKCONNECTION_H__
#include "SockFd.h"
#include "SockAddress.h"
#include "SockIO.h"
#include <string>
namespace reactor
{
    class SockConnection
    {
    public:
        // SockConnection();
        SockConnection(int clifd);
        ~SockConnection();
        void sendMsg(const string msg);
        string recvMsg();

        void closeConn();
        string toString();

    private:
        SockFd _sockfd;
        const SockAddress _serAddr;
        const SockAddress _cliAddr;
        SockIO _sockIO;
        bool _isShutdownWrite;
    };

} // namespace reactor

#endif
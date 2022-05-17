#ifndef __SOCKADDRESS_H__
#define __SOCKADDRESS_H__
#include <string>
#include <netinet/in.h>
using std::string;
namespace reactor
{
    class SockAddress
    {
    public:
        SockAddress(); // 默认ip和port
        SockAddress(const char *ip, unsigned short port);
        SockAddress(const struct sockaddr_in &addr);
        ~SockAddress();

        string getIp() const;
        unsigned short getPort() const;
        const sockaddr_in *getSockAddr() const;

    private:
        // string _ip;
        // unsigned short _port;
        struct sockaddr_in _addr;
        // struct sockaddr_in _clnt_adr;
    };

} // end space
#endif
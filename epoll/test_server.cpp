// #include "./include/socket/SockFd.h"
// #include "./include/socket/SockAddress.h"
// #include "./include/socket/SockIO.h"
// #include "./include/socket/SockAcceptor.h"
// #include "./include/socket/SockConnection.h"
// #include "./include/epoll/Epoll.h"
// #include "./include/epoll/EpollPoller.h"
#include "./include/tcp/TcpServer.h"

#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using namespace reactor;

void onConnection(const TcpConnectionPtr &pConn)
{
	cout << pConn->toString() << "has connected!" << endl;
	pConn->sendMsg("hello, welcome to Chat Server.\r\n");
}

void onMessage(const TcpConnectionPtr &pConn)
{
	std::string s(pConn->recvMsg());
	cout << "server: " << s << endl;
	pConn->sendMsg(s);
	//做业务逻辑的处理
}

void onClose(const TcpConnectionPtr &pConn)
{
	printf("%s close\n", pConn->toString().c_str());
}

int main(int argc, char const *argv[])
{
	TcpServer tcpserver;
	tcpserver.setTcpConnCallBack(&onConnection);
	tcpserver.setTcpMsgCallBack(&onMessage);
	tcpserver.setTcpCloseCallBack(&onClose);

	tcpserver.startTcpServer();
	return 0;
}

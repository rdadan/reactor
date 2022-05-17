#include "./include/socket/SockFd.h"
#include "./include/socket/SockAddress.h"
#include "./include/socket/SockIO.h"
#include "./include/socket/SockAcceptor.h"
#include "./include/socket/SockConnection.h"
#include "./include/epoll/Epoll.h"
#include "./include/epoll/EpollPoller.h"
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using namespace reactor;

void onConnection(const sockConnPtr &pConn)
{
	cout << pConn->toString() << "has connected!" << endl;
	pConn->sendMsg("hello, welcome to Chat Server.\r\n");
}

void onMessage(const sockConnPtr &pConn)
{
	std::string s(pConn->recvMsg());
	cout << "server: " << s << endl;
	pConn->sendMsg(s);
	//做业务逻辑的处理
}

void onClose(const sockConnPtr &pConn)
{
	printf("%s close\n", pConn->toString().c_str());
}

int main(int argc, char const *argv[])
{
	cout << "eee" << endl;
	SockAddress addr;
	SockAcceptor acceptor(addr);
	acceptor.ready();

	EpollPoller poller(acceptor);
	poller.setConnectionCallback(&onConnection);
	poller.setMessageCallback(&onMessage);
	poller.setCloseCallback(&onClose);

	poller.loop();
	return 0;
}

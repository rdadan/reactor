#include "SockFd.h"
#include "SockAddress.h"
#include "SockIO.h"
#include "SockAcceptor.h"
#include "SockConnection.h"
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using namespace reactor;
int main(int argc, char const *argv[])
{
	SockAddress addr;
	SockAcceptor acceptor(addr);
	acceptor.ready();
	int connfd = acceptor.accept();

	SockConnection conn(connfd);
	conn.sendMsg("welcom to server!");
	string str = conn.toString();
	cout << "str: " << str << endl;
	while (1)
	{
		string msg = conn.recvMsg();
		cout << "server receve: " << msg << endl;
		conn.sendMsg(msg);
	}
	return 0;
}

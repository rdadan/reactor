#include "../../include/socket/SockFd.h"
#include "../../include/socket/SockAddress.h"
#include "../../include/socket/SockIO.h"
#include "../../include/socket/SockAcceptor.h"
#include "../../include/socket/SockConnection.h"
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

// #include "./include/socket/SockFd.h"
// #include "./include/socket/SockAddress.h"
// #include "./include/socket/SockIO.h"
// #include "./include/socket/SockAcceptor.h"
// #include "./include/socket/SockConnection.h"
// #include "./include/epoll/Epoll.h"
// #include "./include/epoll/EpollPoller.h"
#include "./include/tcp/TcpServer.h"
#include "./include/threadpool/ThreadPool.h"

#include <string>
#include <iostream>
#include <memory>
#include <unistd.h>
using std::cout;
using std::endl;
using std::string;
using namespace reactor;
using namespace THREADPOOL;
using std::make_shared;
ThreadPool *g_threadpool = NULL;

class EchoTask : public Task
{
public:
	EchoTask(const string &strTask, const spTcpConnPtr &pConn)
		: _strTask(strTask),
		  _pConn(pConn)
	{
	}
	void process() override // 这里是计算线程，不做IO处理
	{
		cout << "task is processing" << endl;
		//_pConn现在在计算线程里，如果直接去使用send，TcpConnection就在计算线程里面完成了数据发送,不可取
		if (_strTask == "1\n")
		{
			func1();
		}

		// 应该让处于计算线程里的TcpConn通知EpollPoller所在的线程完成IO处理
		_pConn->sendMsgToLoop(_strTask); // 在epoll对象所在线程里完成数据发送
	}

	void func1()
	{
		int i = 999999999999999;
		while (i--)
		{
		}
		cout << "计算线程func1() 进程ID: " << getpid() << " 线程ID: " << pthread_self() << endl;
	}

private:
	string _strTask;
	spTcpConnPtr _pConn;
};

class EchoSession
{
public:
	EchoSession()
		: _threadpool(4, 10),
		  _tcpserver()
	{
	}

	static void onMessage(const spTcpConnPtr &conn);
	static void onConnection(const spTcpConnPtr &conn);
	static void onClose(const spTcpConnPtr &conn);
	void start();

private:
	ThreadPool _threadpool;
	TcpServer _tcpserver;
};

// 在线IO线程
void EchoSession::onMessage(const spTcpConnPtr &pConn)
{
	// 1. 获取数据
	string strTask = pConn->recvMsg();
	// 2. 对数据的处理交给线程池的线程（计算线程）做
	g_threadpool->setTask(new EchoTask(strTask, pConn));
	// 3. 发送数据交给epollpoller对象所在的线程（IO线程）完成，
	// 所以计算线程完成计算后需要通知 IO线程
}

void EchoSession::onConnection(const spTcpConnPtr &pConn)
{
	cout << pConn->toString() << " has connected!" << endl;
	pConn->sendMsg("hello, welcome to Server.\r\n");
}

void EchoSession::onClose(const spTcpConnPtr &pConn)
{
	printf("%s close\n", pConn->toString().c_str());
}
void EchoSession::start()
{
	g_threadpool = &_threadpool;
	_threadpool.start();
	_tcpserver.setTcpConnCallBack(&onConnection);
	_tcpserver.setTcpMsgCallBack(&onMessage);
	_tcpserver.setTcpCloseCallBack(&onClose);
	_tcpserver.startTcpServer();
}
int main(int argc, char const *argv[])
{

	EchoSession echo;
	echo.start();
	return 0;
}
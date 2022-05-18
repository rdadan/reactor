#include "workThread.h"

using namespace THREADPOOL;

WorkThread::WorkThread(ThreadPool &threadPool)
    : _threadPool(threadPool)
{
    cout << " WorkThread() " << endl;
}
WorkThread::~WorkThread()
{
    cout << " ~WorkThread() " << endl;
}
void WorkThread::run()
{
    // WorkThread类外访问ThreadPool的私有成员函数，
    // 需要将WorkThread设为ThreadPool的友元类
    cout << "WorkThread::run()::_threadPool.processTask()" << endl;
    _threadPool.processTask();
}
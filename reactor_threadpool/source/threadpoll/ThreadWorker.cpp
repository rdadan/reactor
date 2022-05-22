#include "../../include/threadpool/ThreadWorker.h"

namespace THREADPOOL
{
    ThreadWorker::ThreadWorker(ThreadPool &threadPool)
        : _threadPool(threadPool)
    {
        // cout << " ThreadWorker() " << endl;
    }
    ThreadWorker::~ThreadWorker()
    {
        // cout << " ~ThreadWorker() " << endl;
    }
    void ThreadWorker::run()
    {
        // ThreadWorker类外访问ThreadPool的私有成员函数，
        // 需要将ThreadWorker设为ThreadPool的友元类
        // cout << "ThreadWorker::run()::_threadPool.processTask()" << endl;
        _threadPool.processTask();
    }
} //
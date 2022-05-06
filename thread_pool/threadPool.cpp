#include "threadPool.h"
#include "workThread.h"
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
using namespace THREADPOOL;

ThreadPool::ThreadPool(size_t threadNum, size_t taskNum)
    : _threadNum(threadNum),
      _taskNum(taskNum),
      _taskQue(taskNum),
      _isExit(false)
{
    cout << "ThreadPool(size_t threadNum, size_t taskNum)" << endl;

    _vecShareThreadPtr.reserve(threadNum);
}

ThreadPool::~ThreadPool()
{
    cout << "~ThreadPool()" << endl;

    if (_isExit)
    {
        stop();
    }
}
// 线程池启动
// 就是启动工作线程
void ThreadPool::start()
{
    cout << "threadpool create WorkThread" << endl;

    while (_threadNum--)
    {
        // shared_ptr<Thread> sp(new WorkThread(*this));
        _vecShareThreadPtr.push_back(shared_ptr<Thread>(new WorkThread(*this)));
        ::sleep(1);
    }
    cout << "WorkThread start... " << endl;
    for (auto &shPthread : _vecShareThreadPtr)
    {
        shPthread->startThread();
        ::sleep(1);
    }
}
// 线程池终止
void ThreadPool::stop()
{
    cout << "threadpool stop..." << endl;

    if (!_isExit)
    {
        // 确认所有的任务全部执行完毕
        while (!_taskQue.isEmpty())
        {
        }
        // 可以退出了
        _isExit = true;
        // 唤醒所有子线程
        _taskQue.notifyALL();

        for (auto &shPthread : _vecShareThreadPtr)
        {
            shPthread->joinThread();
        }
    }
}
//添加任务
void ThreadPool::setTask(Task *task)
{
    _taskQue.pushTask(task);
}
// 获取任务
Task *ThreadPool::getTask()
{
    return _taskQue.popTask();
}

// 线程任务接口, 每一个子线程要做的事儿
void ThreadPool::processTask(void)
{
    cout << "ThreadPool::processTask" << endl;
    while (!_isExit)
    {
        Task *pTa = getTask();
        if (pTa)
        {
            pTa->process();
        }
    }
}
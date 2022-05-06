#include "thread.h"
namespace rdadan
{
    Thread::Thread(ThreadCallBack threadCB)
        : _pthID(0),
          _isRunning(false),
          _threadCB(threadCB) { cout << "Thread()" << endl; }

    void Thread::startThread()
    {
        // this 当前的子线程来作为threadFunc的参数
        cout << "startThread... " << endl;
        pthread_create(&_pthID, NULL, threadFunc, this);
        _isRunning = true;
    }

    void *Thread::threadFunc(void *arg)
    {
        cout << "threadFunc,  this: " << arg << endl;
        Thread *pThread = static_cast<Thread *>(arg);
        if (pThread)
        {
            pThread->_threadCB(); //线程执行绑定的回调函数
        }
    }

    void Thread::joinThread()
    {
        cout << "joinThread... " << endl;
        pthread_join(_pthID, NULL); // 阻塞，等待线程执行完成后才会回收
        cout << "joinThread done... " << endl;
        _isRunning = false;
    }

    Thread::~Thread()
    {
        if (_isRunning)
        {
            cout << "~Thread() pthread_detach... " << endl;
            pthread_detach(_pthID); // 非阻塞， 直接回收当前_pthID线程资源
            _isRunning = false;
        }
    }
}
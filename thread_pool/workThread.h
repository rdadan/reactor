#ifndef __WORKTHREAD_H__
#define __WORKTHREAD_H__
#include "Thread.h"
#include "ThreadPool.h"

namespace THREADPOOL
{
    class WorkThread : public Thread
    {
    public:
        WorkThread(ThreadPool &_threadPool);
        ~WorkThread();

    private:
        virtual void run();

    private:
        ThreadPool &_threadPool;
    };

} //
#endif
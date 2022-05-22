#ifndef __THREADWORKER_H__
#define __THREADWORKER_H__
#include "Thread.h"
#include "ThreadPool.h"

namespace THREADPOOL
{
    class ThreadWorker : public Thread
    {
    public:
        ThreadWorker(ThreadPool &_threadPool);
        ~ThreadWorker();

    private:
        virtual void run();

    private:
        ThreadPool &_threadPool;
    };

} //
#endif
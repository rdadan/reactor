#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <cstddef>
#include <memory>
#include <vector>
#include "taskQueue.h"

using std::shared_ptr;
using std::vector;

// 线程池接受任务， 将任务添加至任务队列
// 然后分配子线程处理任务
namespace THREADPOOL
{
    class Task
    {
    public:
        virtual void process() = 0;
    };

    class Thread;
    class WorkThread;

    class ThreadPool
    {
    public:
        ThreadPool(size_t _threadNum, size_t _taskNum);
        ~ThreadPool();
        // 线程池启动
        void start();
        // 线程池终止
        void stop();
        //添加任务
        void setTask(Task *);

    private:
        // 获取任务
        Task *getTask();
        // 线程任务接口
        friend WorkThread;
        void processTask(void);

    private:
        size_t _threadNum;
        size_t _taskNum;
        bool _isExit;

        // thread
        vector<shared_ptr<Thread>> _vecShareThreadPtr;
        // task
        TaskQueue _taskQue; //这里用到类的构造函数了, 不能用前向声明
    };
} //
#endif
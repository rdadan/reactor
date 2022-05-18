#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__
#include "mutex.h"
#include "condition.h"
#include <cstddef>
#include <queue>
using std::queue;

// 任务队列是临界资源，线程共享
// Mutex,Condition类和 TaskQUEUE建立组合关系
namespace THREADPOOL
{
    class Task;
    using pTask = Task *;
    class TaskQueue
    {

    public:
        TaskQueue(size_t queSize);
        ~TaskQueue();
        size_t getQueSize();
        void pushTask(pTask task);
        pTask popTask();

        bool isEmpty();
        bool isFull();
        // 唤醒所有子线程
        void notifyALL();

    private:
        size_t _queSize;
        Mutex _mutex;
        Condition _condNotEmpty;
        Condition _condNotFull;
        queue<pTask> _que;

        bool _wakeupFlag; //是否唤醒全部子线程
    };
}
#endif
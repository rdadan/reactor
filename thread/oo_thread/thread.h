#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <iostream>
using std::cout;
using std::endl;
// 面向对象方式封装：使用继续和多态
namespace rdadan
{
    class Thread
    {
    public:
        Thread();
        virtual ~Thread();
        void startThread();
        void joinThread();

    private:
        //线程执行体，作为借口提供给子线程
        virtual void run() = 0;
        // 线程需要具体功能, 函数指针作为 pthread_create()的参数
        static void *threadFunc(void *);

    private:
        pthread_t _pthID;
        bool _isRunning;
    };
}
#endif
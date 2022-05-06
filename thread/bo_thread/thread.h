#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <iostream>
using std::cout;
using std::endl;
// 基于对象： 使用回调函数, 提供线程要完成的任务ThreadCallBack 给Thread类
// 包装类对象成员函数
#include <functional>
// typedef std::function<void(void)> ThreadCallBack;
using ThreadCallBack = std::function<void(void)>;
namespace rdadan
{
    class Thread
    {
    public:
        Thread(ThreadCallBack);
        ~Thread();
        void startThread();
        void joinThread();

    private:
        // 线程需要具体功能, 函数指针作为 pthread_create()的参数
        static void *threadFunc(void *);

    private:
        pthread_t _pthID;
        bool _isRunning;
        ThreadCallBack _threadCB;
    };
}
#endif
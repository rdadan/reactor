#include "thread.h"
#include <memory>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

using std::unique_ptr;
void getNowTime()
{
    time_t timer; // time_t就是long int 类型
    struct tm *tblock;
    timer = time(NULL);
    tblock = localtime(&timer);
    printf("Local time is: %s\n", asctime(tblock));
}

class MyThread : public rdadan::Thread
{
public:
    MyThread() { cout << "MyThread()" << endl; }
    ~MyThread() { cout << "~MyThread()" << endl; }

private:
    virtual void run()
    {
        int cnt = 5;
        while (cnt--)
        {
            cout << "MyThread 线程id " << pthread_self() << "    ";
            getNowTime();
            sleep(2);
        }
    }
};

void test01()
{
    cout << "test 线程id " << pthread_self() << endl;
    // Thread *pThread = new myThread;
    unique_ptr<rdadan::Thread> pThread(new MyThread);
    pThread->startThread(); //开启子线程
    pThread->joinThread();  //阻塞函数 等待线程执行结束后才被回收
    cout << "test over... " << endl;
}

void test02()
{
    cout << "test02 线程id " << pthread_self() << endl;
    unique_ptr<rdadan::Thread> pThread(new MyThread);
    pThread->startThread(); //开启子线程
    // 主线程睡3s, 子线程继续执行。
    // 3s后回收主线程，主线程析构中调用pthread_detach(_pthid)) 回收子线程资源
    sleep(3);
    cout << "test over... " << endl;
}

int main()
{
    cout << "main 线程id " << pthread_self() << endl;
    test02();
    cout << "main 线程id " << pthread_self() << endl;
    return 0;
}
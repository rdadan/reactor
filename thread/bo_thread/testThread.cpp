#include "thread.h"
#include <memory>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

using std::unique_ptr;
// ThreadCallBack-> void getNowTime()
void getNowTime(void)
{
    int cnt = 5;
    while (cnt--)
    {
        cout << "MyThread 线程id " << pthread_self() << "    ";
        time_t timer; // time_t就是long int 类型
        struct tm *tblock;
        timer = time(NULL);
        tblock = localtime(&timer);
        printf("Local time is: %s\n", asctime(tblock));
        sleep(2);
    }
}

void test01()
{
    cout << "test01 线程id " << pthread_self() << endl;
    rdadan::Thread thread(getNowTime);
    thread.startThread(); //开启子线程
    thread.joinThread();  //阻塞函数 等待线程执行结束后才被回收
    cout << "test over... " << endl;
}

void test02()
{
    cout << "test02 线程id " << pthread_self() << endl;
    rdadan::Thread thread(getNowTime);
    thread.startThread(); //开启子线程
    // 主线程睡3s, 子线程继续执行。
    // 3s后回收主线程，主线程析构中调用pthread_detach(_pthid)) 回收子线程资源
    sleep(3);
    cout << "test over... " << endl;
}

class Task
{
public:
    void getTime()
    {
        getNowTime();
    }
};

void test03()
{
    cout << "test03 线程id " << pthread_self() << endl;
    Task task;
    auto f = std::bind(&Task::getTime, &task); // f: function<void(void)>
    rdadan::Thread thread(f);
    thread.startThread(); //开启子线程
    thread.joinThread();  //阻塞函数 等待线程执行结束后才被回收
    cout << "test over... " << endl;
}
int main()
{
    cout << "main 线程id " << pthread_self() << endl;
    test03();
    cout << "main 线程id " << pthread_self() << endl;
    return 0;
}
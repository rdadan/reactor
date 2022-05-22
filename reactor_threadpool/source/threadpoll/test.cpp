// #include "threadPool.h"
// #include <unistd.h>
// #include <time.h>
// #include <stdlib.h>
// #include <iostream>
// #include <memory>

// using std::cout;
// using std::endl;
// using std::unique_ptr;
// using namespace THREADPOOL;

// class Task1 : public Task
// {
// public:
//     virtual void process()
//     {
//         ::sleep(1);
//         ::srand(::time(NULL));
//         int number = ::rand() % 100;
//         cout << "thID: " << pthread_self() << " ------------------ Task1 produce: " << number << endl;
//         ::sleep(1);
//     }
// };

// class Task2 : public Task
// {
// public:
//     virtual void process()
//     {
//         ::sleep(1);
//         cout << "thID: " << pthread_self() << " ------------------ Task2: " << endl;
//         ::sleep(1);
//     }
// };

// class Task3 : public Task
// {
// public:
//     virtual void process()
//     {
//         ::sleep(1);
//         cout << "thID: " << pthread_self() << " ------------------ Task3: " << endl;
//         ::sleep(1);
//     }
// };

// int main(void)
// {
//     //对于多核CPU来说，最理想的情况是一核对应一个线程
//     ThreadPool threadpool(4, 20);
//     ::sleep(1);
//     threadpool.start();
//     unique_ptr<Task> up1_0(new Task1);
//     unique_ptr<Task> up2_0(new Task2);
//     unique_ptr<Task> up3_0(new Task3);
//     cout << "setTask()" << endl
//          << endl;
//     ::sleep(5);

//     threadpool.setTask(up1_0.get());
//     ::sleep(2);
//     threadpool.setTask(up2_0.get());
//     ::sleep(2);
//     threadpool.setTask(up3_0.get());
//     ::sleep(2);
//     threadpool.stop();
//     return 0;
// }

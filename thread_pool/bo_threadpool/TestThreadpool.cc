 ///
 /// @file    TestThreadpool.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-03-19 11:57:03
 ///
 
#include "Threadpool.h"

#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::unique_ptr;

class Producer 
{
public:
	void getNumber()
	{
		::srand(::time(NULL));
		int number = ::rand() % 100;
		cout <<  pthread_self() <<" > produce a number : " << number << endl;
		::sleep(1);
	}
};
 
int main(void)
{
	//对于多核CPU来说，最理想的情况是一核对应一个线程
	wd::Threadpool threadpool(4, 10);
	threadpool.start();

	int cnt = 20;
	while(cnt--)
	{
		threadpool.addTask(std::bind(&Producer::getNumber, Producer()));
		cout << " cnt = " << cnt << endl;
	}

	threadpool.stop();
	return 0;
}

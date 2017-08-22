#include <iostream>
#include "thread-pool.h"

class MyThread : public ECThread
{
private:
	int _n;

public:
	MyThread(int n): _n(n)
	{
		std::cout << "[" << __func__ << "]" << std::endl;
	}
	~MyThread()
	{
		std::cout << "[" << __func__ << "]" << std::endl;
	}
	void run(void *param)
	{
		std::cout << "[1] thread id: " <<  _thread_id << ", n = " << _n << ", start task" <<  std::endl;
		_n++;
		std::this_thread::sleep_for (std::chrono::seconds(2));
		std::cout << "[1] thread id: " <<  _thread_id << ", n = " << _n << ", end task" <<  std::endl;
	}
};

int main()
{
	unsigned num_cpus = std::thread::hardware_concurrency();
	std::cout << "CPU(s): " << num_cpus << "\n";

	/*
	ECThreadManager thread_manager;
	thread_manager.create_thread(new MyThread(5));
	thread_manager.create_thread(new MyThread(100));
	thread_manager.create_thread(new MyThread(20));
	thread_manager.push_task(NULL);
	thread_manager.push_task(NULL);
	thread_manager.push_task(NULL);
	thread_manager.push_task(NULL);
	thread_manager.push_task(NULL);
	thread_manager.poll();
	*/

	MyThread thd(96);
	std::this_thread::sleep_for (std::chrono::seconds(1));
	if(thd.schedule(NULL) == true)
	{
		std::cout << "thread schedule success" << std::endl;
	}else{
		std::cout << "thread schedule failed" << std::endl;
	}
	std::this_thread::sleep_for (std::chrono::seconds(5));
	if(thd.schedule(NULL) == true)
	{
		std::cout << "thread schedule success" << std::endl;
	}else{
		std::cout << "thread schedule failed" << std::endl;
	}
	thd.join();

	return 0;
}

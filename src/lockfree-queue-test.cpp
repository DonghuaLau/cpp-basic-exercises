#include <iostream>
#include <time.h>
#include <chrono>
#include <vector>

#include "smart-pointer.h"
#include "lockfree-queue.h"
#include "locker-queue.h"
#include "thread-pool.h"

/*
int test_1()
{
	ec::LockFreeQueue<int> q();

	{
		ec::SmartPointer<int> p(new int(2));
		q.push(p);
	}
	{
		ec::SmartPointer<int> p(new int(4));
		q.push(p);
	}
	{
		ec::SmartPointer<int> p(new int(8));
		q.push(p);
	}

	q.print();

	std::cout << "---------\n";
	while(true){
		ec::SmartPointer<int> t(q.pop());
		if(*t == nullptr) break;
		std::cout << *t << ", ";
	}
	std::cout << std::endl;

	
	
	return 0;
}
*/


void test_2()
{
	ec::LockFreeQueue q;

	int *n;
	
	n = new int(2);
	q.push(n);
	n = new int(4);
	q.push(n);
	n = new int(8);
	q.push(n);
	//n = new int(16);
	//q.push(n);

	q.print();

	std::cout << "---------\n";
	int *t = nullptr;
	t = (int*)q.pop();
	std::cout << *t << ", ";
	t = (int*)q.pop();
	std::cout << *t << ", ";
	t = (int*)q.pop();
	std::cout << *t << ", ";
	//t = (int*)q.pop();
	//std::cout << *t << ", ";

	std::cout << *t << "\n";

	
}

void test_3()
{
	ec::LockerQueue q;

	int *n;
	
	n = new int(2);
	q.push(n);
	n = new int(4);
	q.push(n);
	n = new int(8);
	q.push(n);
	//n = new int(16);
	//q.push(n);

	q.print();

	std::cout << "---------\n";
	int *t = nullptr;
	t = (int*)q.pop();
	std::cout << *t << ", ";
	t = (int*)q.pop();
	std::cout << *t << ", ";
	t = (int*)q.pop();
	std::cout << *t << ", ";
	//t = (int*)q.pop();
	//std::cout << *t << ", ";

	std::cout << "\n";

	
}

#define MAX_LOOP 1000000
#define MAX_THREADS 2
//#define MAX_LOOP 100
//#define MAX_THREADS 10

//ec::LockFreeQueue g_lock_free_queue;
ec::LockerQueue g_lock_free_queue;

class PushThread : public ECThread
{
private:
	int _n;

public:
	PushThread(int n): _n(n)
	{
		//std::cout << "[" << __func__ << "]" << std::endl;
		//std::cout << "[" << __func__ << "] thread id: " << _thread_id << std::endl;
	}
	~PushThread()
	{
		//std::cout << "[" << __func__ << "]" << std::endl;
	}
	void run(void *param)
	{
		//std::cout << "[1] thread id: " <<  get_thread_id() << ", START\n";
		int *p = NULL;
		for(int i = 0; i < MAX_LOOP; i++)
		{
			++_n;
			p = new int(_n);
			g_lock_free_queue.push(p);
		} 
		//std::cout << "[1] thread id: " <<  get_thread_id() << ", END\n";
	}
};

class PopThread : public ECThread
{
private:
	int _n;

public:
	PopThread(int n): _n(n)
	{
		//std::cout << "[" << __func__ << "]" << std::endl;
	}
	~PopThread()
	{
		//std::cout << "[" << __func__ << "]" << std::endl;
	}
	void run(void *param)
	{
		//std::cout << "[1] thread id: " <<  get_thread_id() << ", START\n";
		int *p = nullptr;
		for(int i = 0; i < MAX_LOOP; i++)
		{
			p = (int*)g_lock_free_queue.pop();
			delete p;
		} 
		//std::cout << "[1] thread id: " <<  get_thread_id() << ", END\n";
	}
};

void test_4()
{

	clock_t start_clock;
	clock_t end_clock;

	time_t start_time;
	time_t end_time;

	std::vector<PushThread *> vpt;
	PushThread *p = nullptr;

	for(int i = 0; i < MAX_THREADS; i++)
	{
		p = new PushThread(i * MAX_LOOP);
		p->set_one_time();
		vpt.push_back(p);
		while(!p->is_ready()){
			std::this_thread::sleep_for (std::chrono::milliseconds(10));
		}
	}

	time(&start_time);
	start_clock = clock();

	for(int i = 0; i < MAX_THREADS; i++)
	{
		vpt[i]->schedule(NULL);
	}

	for(int i = 0; i < MAX_THREADS; i++)
	{
		vpt[i]->join();
	}

	//std::this_thread::sleep_for (std::chrono::seconds(1));

	time(&end_time);
	end_clock = clock();

	
	std::cout << "size: " << g_lock_free_queue.size() << "\n";
	std::cout << "start_time: " << start_time << ", end_time: " << end_time << "\n";
	std::cout << "start_clock: " << start_clock << ", end_clock: " << end_clock << "\n";
}

int main()
{
	//test_3();
	test_4();
	return 0;
}

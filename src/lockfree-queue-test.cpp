#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>

#include "thread-pool.h"
#include "lockfree-queue.h"
#include "smart-pointer.h"

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
	ec::LockFreeQueue<int> q;

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
	q.pop(&t);
	std::cout << *t << ", ";
	q.pop(&t);
	std::cout << *t << ", ";
	q.pop(&t);
	std::cout << *t << ", ";
	//t = (int*)q.pop();
	//std::cout << *t << ", ";

	std::cout << *t << "\n";

	
}

void test_3()
{
	ec::LockerQueue<int> q;

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
	q.pop(&t);
	std::cout << *t << ", ";
	q.pop(&t);
	std::cout << *t << ", ";
	q.pop(&t);
	std::cout << *t << ", ";
	//t = (int*)q.pop();
	//std::cout << *t << ", ";

	std::cout << "\n";

	
}


//ec::LockFreeQueue<int> g_lock_free_queue;
//ec::LockerQueue<int> g_locker_queue;

ec::Queue<int> *g_lock_free_queue = nullptr;
ec::Queue<int> *g_locker_queue = nullptr;

class PushThread : public ECThread
{
private:
	int _n;
	int _max_loop;
	ec::Queue<int> *_queue;

public:
	PushThread(ec::Queue<int> *queue, int n = 0, int max_loop = 100): _queue(queue), _n(n), _max_loop(max_loop)
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
		for(int i = 0; i < _max_loop; i++)
		{
			++_n;
			p = new int(_n);
			_queue->push(p);
		} 
		//std::cout << "[1] thread id: " <<  get_thread_id() << ", END\n";
	}
};

class PopThread : public ECThread
{
private:
	int _n;
	int _max_loop;
	ec::Queue<int> *_queue;

public:
	PopThread(ec::Queue<int> *queue, int n = 0, int max_loop = 100): _queue(queue), _n(n), _max_loop(max_loop)
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
		bool ret = false;
		for(int i = 0; i < _max_loop; i++)
		{
			ret = _queue->pop(&p);
			if(ret == false) break;
			delete p;
		} 
		//std::cout << "[1] thread id: " <<  get_thread_id() << ", END\n";
	}
};

void test_lockfree_queue_push(int threads_num, int max_loop)
{
	std::vector<PushThread *> vpt;
	PushThread *p = nullptr;

	for(int i = 0; i < threads_num; i++)
	{
		p = new PushThread(g_lock_free_queue, i * max_loop, max_loop);
		p->set_one_time();
		vpt.push_back(p);
		while(!p->is_ready()){
			std::this_thread::sleep_for (std::chrono::milliseconds(10));
		}
	}

	clock_t start_clock;
	clock_t end_clock;

	start_clock = clock();

	for(int i = 0; i < threads_num; i++)
	{
		vpt[i]->schedule(NULL);
	}

	for(int i = 0; i < threads_num; i++)
	{
		vpt[i]->join();
		delete vpt[i];
	}

	end_clock = clock();

	std::cout << "[" << __func__ << "] "
			  << "size: " << g_lock_free_queue->size()
			  << ", second(s): " << (float)(end_clock - start_clock) / CLOCKS_PER_SEC 
			  << ", start: " << start_clock << ", end: " << end_clock  
			  << std::endl;
}

void test_lockfree_queue_pop(int threads_num, int max_loop)
{
	std::vector<PopThread *> vpp;
	PopThread *p = nullptr;

	for(int i = 0; i < threads_num; i++)
	{
		p = new PopThread(g_lock_free_queue, i * max_loop, max_loop);
		p->set_one_time();
		vpp.push_back(p);
		while(!p->is_ready()){
			std::this_thread::sleep_for (std::chrono::milliseconds(10));
		}
	}

	clock_t start_clock;
	clock_t end_clock;

	start_clock = clock();

	for(int i = 0; i < threads_num; i++)
	{
		vpp[i]->schedule(NULL);
	}

	for(int i = 0; i < threads_num; i++)
	{
		vpp[i]->join();
		delete vpp[i];
	}

	end_clock = clock();

	std::cout << "[" << __func__ << "] "
			  << "size: " << g_lock_free_queue->size()
			  << ", second(s): " << (float)(end_clock - start_clock) / CLOCKS_PER_SEC
			  << ", start: " << start_clock << ", end: " << end_clock  
			  << std::endl;
}

void test_locker_queue_push(int threads_num, int max_loop)
{
	std::vector<PushThread *> vpt;
	PushThread *p = nullptr;

	for(int i = 0; i < threads_num; i++)
	{
		p = new PushThread(g_locker_queue, i * max_loop, max_loop);
		p->set_one_time();
		vpt.push_back(p);
		while(!p->is_ready()){
			std::this_thread::sleep_for (std::chrono::milliseconds(10));
		}
	}

	clock_t start_clock;
	clock_t end_clock;

	start_clock = clock();

	for(int i = 0; i < threads_num; i++)
	{
		vpt[i]->schedule(NULL);
	}

	for(int i = 0; i < threads_num; i++)
	{
		vpt[i]->join();
		delete vpt[i];
	}

	end_clock = clock();

	std::cout << "[" << __func__ << "] "
			  << "size: " << g_locker_queue->size()
			  << ", second(s): " << (float)(end_clock - start_clock) / CLOCKS_PER_SEC
			  << ", start: " << start_clock << ", end: " << end_clock  
			  << std::endl;
}

void test_locker_queue_pop(int threads_num, int max_loop)
{
	std::vector<PopThread *> vpt;
	PopThread *p = nullptr;

	for(int i = 0; i < threads_num; i++)
	{
		p = new PopThread(g_locker_queue, i * max_loop, max_loop);
		p->set_one_time();
		vpt.push_back(p);
		while(!p->is_ready()){
			std::this_thread::sleep_for (std::chrono::milliseconds(10));
		}
	}

	clock_t start_clock;
	clock_t end_clock;

	start_clock = clock();

	for(int i = 0; i < threads_num; i++)
	{
		vpt[i]->schedule(NULL);
	}

	for(int i = 0; i < threads_num; i++)
	{
		vpt[i]->join();
		delete vpt[i];
	}

	end_clock = clock();

	std::cout << "[" << __func__ << "] "
			  << "size: " << g_locker_queue->size()
			  << ", second(s): " << (float)(end_clock - start_clock) / CLOCKS_PER_SEC 
			  << ", start: " << start_clock << ", end: " << end_clock  
			  << std::endl;
}

void test_queues_compare(int threads_num, int max_loop)
{
	g_lock_free_queue = new ec::LockFreeQueue<int>();
	g_locker_queue = new ec::LockerQueue<int>();
	test_lockfree_queue_push(threads_num, max_loop);
	//test_lockfree_queue_pop(threads_num, max_loop);

	test_locker_queue_push(threads_num, max_loop);
	//test_locker_queue_pop(threads_num, max_loop);
}


int main()
{
	//test_3();
	//test_4();

	
	#define MAX_LOOP 500000
	#define MAX_THREADS 2
	test_queues_compare(MAX_THREADS, MAX_LOOP);
	return 0;
}

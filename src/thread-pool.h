#ifndef _EC_THREAD_POOL_H_
#define _EC_THREAD_POOL_H_

// C++11

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>

#include "smart-pointer.h"

//template <typename T>
class ECThread;
class ECThreadManager;

enum ECThreadStatus
{
	 _EC_THREAD_INIT	= 0
	,_EC_THREAD_IDLE	= 1
	,_EC_THREAD_WAITING	= 2
	,_EC_THREAD_RUNNING	= 3
	,_EC_THREAD_STOPED	= 4
};

//template <typename T>
class ECThread
{
protected:
	std::thread _thread;
	std::mutex _mutex;
	std::condition_variable _cond;

	std::thread::id _thread_id;
 
	std::atomic<int> _status;
	bool _one_time_running;
	
	void *_param;

public:
	ECThread() : 
		 _thread(&this->thread_enter, this)
		,_status(_EC_THREAD_INIT)
		,_one_time_running(false)
		,_param(NULL)
	{
		_thread_id = _thread.get_id();
		//std::cout << "[" << __func__ << "] thread id: " << _thread_id << std::endl;
	}
	virtual ~ECThread() 
	{ 
		//std::cout << "[" << __func__ << "]" << std::endl;
	}

	std::thread::id get_thread_id()
	{
		return _thread_id;
	}

	void set_one_time(bool one_time = true)
	{
		_one_time_running = one_time;
	}

	/*
		don't call this too fast after new object
	*/
	virtual bool schedule(void *param)
	{
		std::unique_lock<std::mutex> try_lock(_mutex);

		//std::cout << "[" << __func__ << "] thread id: " << _thread_id << ", status: " << _status << std::endl;
		
		if(_status.load() != _EC_THREAD_IDLE)
		{
			//std::cout << "[" << __func__ << "] thread id: " << _thread_id << ", not idle" << std::endl;
			return false;
		}

		set_status(_EC_THREAD_WAITING);
		_param = param;
		_cond.notify_one(); // There's a long time between notify and wait 
		
		//std::cout << "[" << __func__ << "] thread id: " << _thread_id << ", notified" << std::endl;
		return true;
	}

	bool is_ready()
	{
		if(_status.load() != _EC_THREAD_INIT)
			return true;
		else 
			return false;
	}

	/*
	void stop()
	{
		std::unique_lock<std::mutex> try_lock(_mutex);

		set_status(_EC_THREAD_STOPED);
		_cond.notify_one(); // There's a long time between notify and wait 
	}
	*/

	void join()
	{
		_thread.join();
	}


	virtual void run(void *param)
	{
		//_thread_id = std::this_thread::get_id();
		std::cout << "[0] thread id: " <<  get_thread_id() << std::endl;
	}


protected:
	static void thread_enter(void *param)
	{
		ECThread *pthread = (ECThread*)param;
		pthread->do_task();
	}

	void do_task()
	{
		//std::cout << "[" << __func__ << "] thread id: " << _thread_id << std::endl;
		while(true)
		{
			{
				set_status(_EC_THREAD_IDLE);
				std::unique_lock<std::mutex> lock(_mutex);
				_cond.wait(lock); // Will the wait failed?
				if(get_status() == _EC_THREAD_STOPED) break;
				set_status(_EC_THREAD_RUNNING);
			}
			run(_param);
			if(_one_time_running) break;
		}
		set_status(_EC_THREAD_STOPED);
	}

private:
	void set_status(int status)
	{
		_status.store(status);
	}

	int get_status()
	{
		return _status.load();
	}

};

class ECThreadManager
{
private:
	int _threads_size;

	std::vector<ECThread*> _threads;

public:
	ECThreadManager() : _threads_size(0)
	{
		std::cout << "[" << __func__ << "]" << std::endl;
	}
	virtual ~ECThreadManager()
	{
		std::cout << "[" << __func__ << "]" << std::endl;
	}

	int size()
	{
		return _threads_size;
	}

	bool create_thread(ECThread *pThread)
	{
		_threads.push_back(pThread);
		_threads_size++;
		return true;
	}

	/*
	bool start()
	{
		std::cout << "[" << __func__ << "] threads size: " << _threads_size << std::endl;
		ECThread *pThread = NULL;
		for(int i = 0; i < _threads_size; i++)
		{
			//pThread = new ECThread()
			_threads.push_back(new ECThread());
		}
		return true;
	}
	*/

	void poll()
	{
		std::cout << "[" << __func__ << "] threads size: " << _threads_size << std::endl;
		for(int i = 0; i < _threads_size; i++)
		{
			_threads[i]->join();
		}
	}

	int push_task(void *param)
	{
		std::cout << "[" << __func__ << "] push task" << std::endl;
	
		int flag = 0;
		while(true)
		{
			for(int i = 0; i < _threads_size; i++)
			{
				if(_threads[i]->schedule(param) == true)
				{
					flag = 1;
					break;
				}
			}
			if(flag == 1) break;
			else std::this_thread::sleep_for (std::chrono::milliseconds(10));
		}
		std::cout << "[" << __func__ << "] push task complete" << std::endl;
	}
};

#endif


#ifndef _EC_LOCKFREE_QUEUE_H_
#define _EC_LOCKFREE_QUEUE_H_

// C++11
// Ref: 1.http://www.cplusplus.com/reference/atomic/atomic_compare_exchange_weak/
//		2.http://coolshell.cn/articles/8239.html

#include <iostream>
#include <atomic>
#include <mutex>
//#include "smart-pointer.h"

namespace ec{

/*
template <typename T>
class Queue;

template <typename T>
class LockFreeQueue;
//class FixedLockFreeQueue;

template <typename T>
class LockerQueue;
*/

template <typename T>
class Node
{
public:
	Node<T> *_next;
	T *_value;

	Node(): _next(nullptr) {}
	Node(T *value): _next(nullptr), _value(value) {}
	~Node()
	{ 
		//delete _value; 
	}
};

template <typename T>
class Queue
{
public:
	Queue(){}
	~Queue(){}

	virtual bool push(T *value) = 0;
	virtual bool pop(T **value) = 0;
	virtual int size() = 0;
	virtual bool empty() = 0;
	virtual void clear() = 0;
};

template<typename T>
class LockFreeQueue : public Queue<T>
{
private:
	Node<T> *_head;
	Node<T> *_tail;
	std::atomic<int> _size;
	std::atomic_flag _lock;

public:
	LockFreeQueue(): _head(nullptr), _tail(nullptr), _size(0), _lock(ATOMIC_FLAG_INIT)
	{
	}

	~LockFreeQueue()
	{
		clear();
	}

	virtual bool push(T *value)
	{
		Node<T>* p = new Node<T>(value);

		while(!_lock.test_and_set())
		{
			//std::cout << "waiting: " << *value << "\n";
			//std::this_thread::yield(); // necessary ?
		}

		if(_head == nullptr)
		{
			_tail = p;
			_head = p;
		}else{
			_tail->_next = p;
			_tail = p;
		}
		_size ++;

		_lock.clear();

		return true;
	}

	virtual bool pop(T **value)
	{
		while(!_lock.test_and_set())
		{
			//std::this_thread::yield(); // necessary ?
		}

		bool result = false;
		if(_head != nullptr) 
		{
			Node<T> *head = _head;
			_head = _head->_next;
			*value = head->_value; // need release by the caller
			//delete head; //!!!!!!
			result = true;
			_size --;
		}

		return result;
	}

	virtual void clear()
	{
		Node<T> *p = _head;
		Node<T> *q = nullptr;
		while(p)
		{
			q = p->_next;
			delete p->_value;
			delete p;
			p = q;
		}

		_head = nullptr;
		_tail = nullptr;
		_size.store(0);
	}

	virtual int size()
	{
		return _size.load();
	}

	virtual bool empty()
	{
		if(_size.load() == 0) return true;
		else return false;
	}

	// for test
	void print()
	{
		std::cout << __func__ << " == start ==\n";
		std::cout << "size: " << size() << "\n";
		Node<T>* p= _head;
		while(p)
		{
			std::cout << "value: " << *((T*)p->_value) << ", address: " << static_cast<void*>(p) << ", next: " << static_cast<void*>(p->_next) << "\n";
			p = p->_next;
		}

		std::cout << __func__ << " == end ==\n";
	}
};

template <typename T>
class LockerQueue : public Queue<T>
{
private:
	Node<T> *_head;
	Node<T> *_tail;
	int _size;

	std::mutex _mutex;

public:
	LockerQueue(): _head(nullptr), _tail(nullptr), _size(0)
	{
	}

	~LockerQueue()
	{
		clear();
	}

	bool push(T *value)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		Node<T>* p = new Node<T>(value);

		// add first node
		if(_head == nullptr)
		{
			//std::cout << "queue is null.\n";
			_head = p;
			_tail = p;
		}else{
			_tail->_next = p;
			_tail = p;
		}
		++ _size;

		return true;
	}

	bool pop(T **value)
	{
		std::unique_lock<std::mutex> lock(_mutex);

		if(_head == nullptr) return nullptr;

		bool result = false;
		if(_head != nullptr) 
		{
			Node<T> *head = _head;
			_head = _head->_next;
			*value = head->_value; // need release by the caller
			delete head;
			result = true;
			_size --;
		}

		return result;
	}

	void clear()
	{
		std::unique_lock<std::mutex> lock(_mutex);

		Node<T> *p = _head;
		Node<T> *q = nullptr;
		while(p)
		{
			q = p->_next;
			delete p->_value;
			delete p;
			p = q;
		}

		_head = nullptr;
		_tail = nullptr;
		_size = 0;
	}

	int size()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		return _size;
	}

	bool empty()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if(_size == 0) return true;
		else return false;
	}

	// for test
	void print()
	{
		std::cout << __func__ << " == start ==\n";
		std::cout << "size: " << size() << "\n";
		Node<T>* p= _head;
		while(p)
		{
			std::cout << "value: " << *((T*)p->_value) << ", address: " << static_cast<void*>(p) << ", next: " << static_cast<void*>(p->_next) << "\n";
			p = p->_next;
		}

		std::cout << __func__ << " == end ==\n";
	}
};

} // end namespace ec

#endif


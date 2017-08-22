#ifndef _EC_LOCKER_QUEUE_H_
#define _EC_LOCKER_QUEUE_H_

#include <iostream>
#include <mutex>

namespace ec{

class LockerQueue;

class Node2
{
public:
	Node2 *_next;
	void *_value;

	Node2(): _next(nullptr) {}
	Node2(void *value): _next(nullptr), _value(value) {}
	~Node2(){}
};

class LockerQueue
{
private:
	Node2* _head;
	Node2* _tail;
	int _size;

	std::mutex _mutex;
	//std::unique_lock<std::mutex> try_lock(_mutex);

public:
	LockerQueue(): _head(nullptr), _tail(nullptr), _size(0)
	{
	}

	~LockerQueue()
	{
		clear();
	}

	bool push(void *value)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		Node2* p = new Node2(value);

		// add first node
		if(_head == nullptr)
		{
			std::cout << "queue is null.\n";
			_head = p;
			_tail = p;
		}else{
			_tail->_next = p;
			_tail = p;
		}
		++ _size;

		return true;
	}

	void *pop()
	{
		std::unique_lock<std::mutex> lock(_mutex);

		if(_head == nullptr) return nullptr;

		Node2* phead = _head;
		_head = _head->_next;
		-- _size;
		void *value = phead->_value;
		delete phead;

		return value; // need release by the caller
	}

	void clear()
	{
		Node2 *p = _head;
		Node2 *q = nullptr;
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
		return _size;
	}

	bool empty()
	{
		if(_size == 0) return true;
		else return false;
	}

	// for test
	void print()
	{
		std::cout << __func__ << " == start ==\n";
		std::cout << "size: " << size() << "\n";
		Node2* p= _head;
		while(p)
		{
			std::cout << "value: " << *((int*)p->_value) << ", address: " << static_cast<void*>(p) << ", next: " << static_cast<void*>(p->_next) << "\n";
			p = p->_next;
		}

		std::cout << __func__ << " == end ==\n";
	}
};

} // end namespace ec

#endif

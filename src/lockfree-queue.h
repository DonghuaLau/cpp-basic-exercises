#ifndef _EC_LOCKFREE_QUEUE_H_
#define _EC_LOCKFREE_QUEUE_H_

// C++11
// Ref: 1.http://www.cplusplus.com/reference/atomic/atomic_compare_exchange_weak/
//		2.http://coolshell.cn/articles/8239.html

#include <iostream>
#include <atomic>
#include "smart-pointer.h"

namespace ec{

class LockFreeQueue;

class Node
{
public:
	Node *_next;
	void *_value;

	Node(): _next(nullptr) {}
	Node(void *value): _next(nullptr), _value(value) {}
	~Node(){}
};

class LockFreeQueue
{
private:
	std::atomic<Node*> _head;
	std::atomic<Node*> _tail;
	int _size;

public:
	LockFreeQueue(): _head(nullptr), _tail(nullptr), _size(0)
	{
	}

	~LockFreeQueue()
	{
		clear();
	}

	bool push(void *value)
	{
		Node* p = new Node(value);
		//std::cout << "new element: " << static_cast<void*>(p) << "\n";

		Node* ptail = _tail.load(std::memory_order_relaxed);

		//Node* phead = _head.load(std::memory_order_relaxed);


		bool result = false;

		// add first node
		if(ptail == nullptr)
		{
			//std::cout << "queue is null.\n";
			do{
				if(ptail != nullptr) break; // set by other thread and push normal below
				Node* phead = _head.load(std::memory_order_relaxed);
				if(_tail.compare_exchange_weak(ptail, p) == true)
				{
					_head.compare_exchange_weak(phead, p);
					++ _size;
					return true;
				}
			}while( true );
		}

		//std::cout << "head: " << static_cast<void*>(phead) << ", next: " << static_cast<void*>(phead->_next) << "\n";
		//std::cout << "tail: " << static_cast<void*>(ptail) << ", next: " << static_cast<void*>(ptail->_next) << "\n";

		// add not first node
		do{
			while(ptail->_next != nullptr)
			{
				ptail = ptail->_next;
			}

			result = _tail.compare_exchange_weak(ptail, p);
			if(result == true)
			{
				++ _size;
				ptail->_next = p;
				return true;
			}
		}while( true );

		return false;
	}

	void *pop()
	{
		if(_head == nullptr) return nullptr;

		Node* phead = _head.load(std::memory_order_relaxed);
		bool result = false;
		do{
			result = _head.compare_exchange_weak(phead, phead->_next);
			if(result == true)
			{
				-- _size;
				break;
			}
		}while( true );

		void *value = phead->_value;
		delete phead;
		return value; // need release by the caller
	}

	void clear()
	{
		Node *p = _head;
		Node *q = nullptr;
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
		Node* p= _head.load(std::memory_order_relaxed);
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


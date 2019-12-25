#ifndef _EC_QUEUES_H_
#define _EC_QUEUES_H_

namespace ec{

class Queue;
class ListQueue;
class FixedQueue;
class LockerQueue;

class Node
{
public:
	Node *_next;
	void *_value;

	Node(): _next(nullptr) {}
	Node(void *value): _next(nullptr), _value(value) {}
	~Node(){}
};

class Queue
{
public:
	Queue(){}
	~Queue(){}

	virtual bool push() = 0;
	virtual bool pop() = 0;
};

} // end namespace ec

#endif


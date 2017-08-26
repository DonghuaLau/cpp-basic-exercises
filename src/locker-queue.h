#ifndef _EC_LOCKER_QUEUE_H_
#define _EC_LOCKER_QUEUE_H_

#include <iostream>
#include <mutex>

namespace ec{

class LockerQueue;

template <typename T>
class Node2
{
public:
	Node2 *_next;
	void *_value;

	Node2(): _next(nullptr) {}
	Node2(void *value): _next(nullptr), _value(value) {}
	~Node2(){}
};


} // end namespace ec

#endif

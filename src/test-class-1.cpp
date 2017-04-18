#include <iostream>
#include "common.h"
using namespace std;

/*
静态对象是否调用构造函数？

PS: 不调用
*/

class A
{
public:
    A() { cout << "A's Constructor Called " << endl;  }
};

class A1
{
public:
    A1() { cout << "A1's Constructor Called " << endl;  }
};

class B
{
    static A a;
    A1 a1;
public:
    B() { cout << "B's Constructor Called " << endl; }
};

void test_class_01()
{
	print_func(__FUNCTION__);
    B b;
}


/*
下面代码会报错吗？为什么？

PS: 不会，函数地址是唯一的。一般会与虚函数一起考查，虚函数会报错，因为是它是动态绑定，运行时才知道调用哪个函数。 
*/

class A2 {
public:
  int m;
  void print() {  cout << "A2\n";  } 
};

void test_class_02()
{
	print_func(__FUNCTION__);
	A2 *pa = 0;
	pa->print();
}

#ifdef SRC_MAIN

int main()
{
	test_class_01();
	test_class_02();
    return 0;
}

#endif

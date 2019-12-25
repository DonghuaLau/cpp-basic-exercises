#include <iostream>
#include <vector>

class A{
public:
	int n;

	void print()
	{
		std::cout << "n = " << n << std::endl;
	}
};

void test_nullptr()
{
	A *a = nullptr;
	a->print();
}

void test_auto()
{
	auto n = 1;
	auto s = "Hello world!";
	char m = n;
	std::cout << "n = " << n << std::endl;
	std::cout << "m = " << m << std::endl;
	std::cout << "s = " << s << std::endl;

	std::vector<int> v;
	decltype(v.begin()) it;

	
}

constexpr int add(int a, int b)
{
	return a + b;
}

void test_constexpr()
{
	constexpr auto n = 5 + 7;
	auto m = 8;
	auto x = add(2, 8);
	std::cout << "n = " << n << std::endl;
	std::cout << "x = " << x << std::endl;
}

int foo_n = 1;
int& foo()
{
	return foo_n;
}

int foobar()
{
	return 0;
}

void test_values()
{
	// lvalues:
	int i = 42;
	i = 43; // ok, i is an lvalue 
	int* p = &i; // ok, i is an lvalue 
	int& foo();
	foo() = 42; // ok, foo() is an lvalue
	int* p1 = &foo(); // ok, foo() is an lvalue
	
	// rvalues: 
	int foobar(); 
	int j = 0;
	j = foobar(); // ok, foobar() is an rvalue
	int k = j + 2; // ok, j+2 is an rvalue
	//int* p2 = &foobar(); // error, cannot take the address of an rvalue 
	j = 42; // ok, 42 is an rvalue
}

namespace ref_1 {

void foo(int & i) { std::cout << "int & " << i << std::endl; }
void foo(const int & i) { std::cout << "const int & " << i << std::endl; }
void foo(int && i) { std::cout << "int && " << i << std::endl; }
void foo(const int && i) { std::cout << "const int && " << i << std::endl; }

void test_ref()
{
	int i = 2;
	const int &j = i;
	foo(i);
	foo(j);
	foo(2);
	foo([]()->const int && {return 5;}());
}

}

void test_init_list_1()
{
	int a[] = {1, 2, 3, 4, 5};
	std::vector<std::string> v = {"hello", "world", "!"};

	for(auto &i : a){
		std::cout << i << " ";
	}
	std::cout << std::endl;

	for(auto &i : v){
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

class B
{
private:
	std::string name;
	int age;

public:
	B(const std::string &&_name, const int &&_age) : name(_name), age(_age){}

	void show(){
		std::cout << "name: " << name << ", age: " << age << std::endl;
	}
};

struct C
{
	int x;
	int y;
};

struct D
{
	int x;
	int y;
	int z;

	D(std::initializer_list<int> list)  
    {  
        auto it= list.begin();  
        x = *it++;  
        y = *it++;  
        z = *it++;  
    }  
};

/*
B CreateB()
{
	return {"Tom", 25};
}

B CreateB(const std::string &&name, const int &&age)
{
	return {name, age};
}
*/

void test_init_list_2()
{
	B b1{"Jack", 20};
	b1.show();

	//B b2 = CreateB("Tom", 25);
	//B b2 = CreateB();
	//b2.show();

	C c1{1, 2};
	C c2 = {1, 2};

	D d = {1, 2, 3};
}



int main()
{
	//test_auto();
	//test_constexpr();
	//test_values();
	//ref_1::test_ref();
	test_init_list_1();
	test_init_list_2();
	return 0;
}

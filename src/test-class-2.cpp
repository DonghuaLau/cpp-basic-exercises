#include <iostream>    
using namespace std;   

class Base 
{   
public: 
    Base() {} 

    virtual void func() 
    { 
        cout<<"func in Base"<<endl; 
    } 

    virtual ~Base() 
    { 
        cout<<"destruct in Base"<<endl; 
    } 
}; 

class Sub:public Base   
{   
public:   
    Sub() {} 
    virtual void func() 
    { 
        cout<<"func in Sub"<<endl; 
    } 

    virtual ~Sub() 
    { 
        cout<<"destruct in Sub"<<endl; 
    } 
};   

void test(Base &temp) 
{ 
    temp.func(); 
} 

void release(Base *temp) 
{ 
    delete temp; 
} 

int main() 
{   
    Sub *a=new Sub();
	test(*a);
    release(a); 

	return 0;
}

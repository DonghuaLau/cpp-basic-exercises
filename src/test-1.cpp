#include <iostream>
#include <stdio.h>

#include <iomanip>
#include <bitset>

#include "common.h"

using namespace std;

void test_1_p()
{
	//print_scales(1);
	//print_scales(-1);
	//print_scales(-2);
	//print_scales(-3);
	//print_scales(1 << 10);
	//print_scales(1 << 20);
	//print_scales(1 << 1);
	//print_scales(1 << 2);
	//print_scales(1 << 30);
	//print_scales(1 << 31);
	//print_scales(1 << 31 - 3);
	//print_scales(-(1 << 31 - 3));
	//print_scales(1 << 32);
	//print_scales(2 >> 3);
	int n = 2 ^ 31 - 3;
	n = 2;
	//print_scales(n);
	n = 31 - 3;
	print_scales(n);
	n = 2 ^ 31 - 3;
	print_scales(n);
	print_scales(-n);
}

/*

如下函数，在32 bit系统foo(2^31-3)的值是：（）
A：0 	B: 1 	C: 2 	D: 4

PS: （见注释吧）
*/

int foo(int x)
{
    return x&-x;
}

/*
 *	结果1或2跟奇偶有关？
 */
void test_1_00()
{
	print_func(__FUNCTION__);
	int n = (1 << 30) - 3; // 负数为正数取反加1，所以不管x是多少，只要不溢出，且运算的结果为1 (??????)
	//n = 2 ^ 31 - 3; // 如果是这样考，那就尴尬了
	print_scales(n);
	print_scales(-n);
	int m = foo(n);
	cout << "n = " << n << endl;
	cout << "m = " << m << endl;

	print_func(__FUNCTION__);
	n = 2 ^ 31 - 3; // 如果是这样考，那就尴尬了
	print_scales(n);
	print_scales(-n);
	m = foo(n);
	cout << "n = " << n << endl;
	cout << "m = " << m << endl;

	print_func(__FUNCTION__);
	n = 5;
	print_scales(n);
	print_scales(-n);
	m = foo(n);
	cout << "n = " << n << endl;
	cout << "m = " << m << endl;
}

/*
运算符优先级 
unsigned char i=0x80; 
printf("0x%x\n", ~i>>3+1); 
输出什么？

PS: 结果0xfffffff7	位运算会强制转化为int？
*/

void test_1_01()
{
	print_func(__FUNCTION__);

	unsigned char i=0x80; 
	printf("0x%x\n", ~i>>3+1); 
}

/*
下面代码的输出是什么？（非常考基础水平的一道题）

PS: POINTERSTEW. POINT+ER+ST+EW
*/


void test_1_02()
{
	print_func(__FUNCTION__);

	char *c[] = {"ENTER","NEW","POINT","FIRST"};  
	char **cp[] = { c + 3 , c + 2 , c + 1 , c};  
	char ***cpp = cp;  
	printf("%s",**++cpp);  
	printf("%s",*--*++cpp+3);  
	printf("%s",*cpp[-2]+3);  
	printf("%s\n",cpp[-1][-1]+1);  
	
}

#ifdef SRC_MAIN

int main()
{
	//test_1_p();

	test_1_00();
	//test_1_01();
	//test_1_02();


}

#endif

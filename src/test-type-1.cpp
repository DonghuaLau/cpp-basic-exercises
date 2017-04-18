#include <iostream>
#include <string>
#include <stdio.h>

#include "common.h"

using namespace std;

/*
union问题

PS: 266
*/
union
{
    int i;
    char x[2];
}a;

void test_type_1()
{
	print_func(__FUNCTION__);
    a.x[0] = 10;
    a.x[1] = 1;
    printf("%d\n",a.i);
}

/*
结构体，输出？

PS: 8 2

*/
struct data2
{
	int a;
	unsigned short b;
};

void test_type_2()
{
	print_func(__FUNCTION__);
	data2 mData;
	mData.b = 0x0102;
	char *pData = (char *)&mData;
	printf("%d %d\n", sizeof(pData), (int)(*(pData + 4)));
	//printf("%d %d\n", (int)(*(pData + 5)), (int)(*(pData + 6)));
	//printf("%d %d\n", (int)(*(pData + 2)), (int)(*(pData + 3)));
}

/*
改变string变量的值？

PS: 原样输出，没改变
*/

void chg_str(string str) 
{
	str = "ichgit";
}

void test_type_3()
{
	print_func(__FUNCTION__);
	string s = "sarrr";
	chg_str(s);
	printf("%s\n", s.c_str());
	cout << s << endl;
}


/*
静态变量的输出

PS: 
*/

int sum(int a) 
{
	int c = 0;
	static int b = 3; // 只执行一次
	c++;
	b += 2;
	return (a + b + c);
}
void test_type_4()
{
	print_func(__FUNCTION__);
	int i;
	int a = 2;
	for(i = 0; i < 5; ++i) {
	    printf("%d\n", sum(a));
	}
}











/* =================================== */

#ifdef SRC_MAIN

int main()
{
	test_type_1();
	test_type_2();
	test_type_3();
	test_type_4();
    return 0;
}

#endif


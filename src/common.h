#include <iostream>
#include <stdio.h>

#include <iomanip>
#include <bitset>

using namespace std;

void print_scales(int n)
{
	int len = sizeof(n);
	cout << dec << "Len: " << len << endl; 
	cout << dec << "Dec: " << n << endl; 
	cout << hex << "Hex: " << n << endl;
	cout << oct << "Oct: " << n << endl;
	if(len <= 1){
		cout << "Bin: " << bitset<8>(n) << endl;
	}else if(len == 2){
		cout << "Bin: " << bitset<16>(n) << endl;
	}else if(len == 4){
		cout << "Bin: " << bitset<32>(n) << endl;
	}else{
		cout << "Bin: " << bitset<64>(n) << endl;
	}
	cout << dec << endl;
}

void print_func(const char *func)
{
	cout << endl << "## in function '" << func << "' ##" << endl;
}

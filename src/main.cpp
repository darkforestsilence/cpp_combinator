#include<iostream>

#include"parser.h"

using namespace std;

int main(void){

	auto [c,rest] = matchChar('a')("abc");

	cout << c.value_or('_') << " " << rest.value_or("None") << endl;
	return 0;
}


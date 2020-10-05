#include<iostream>
#include<string>

#include"parser.h"

using namespace std;

int main(void){
	auto digit = matchPred<char, string>([](char c){ return c >= '0' && c <= '9'; });
	auto integer = some(digit);
		

	cout << "Input test string: ";
	string input;
	getline(cin, input);

	auto [result, rest] = integer(input);

	if(result){
		int i = fold<char,int> (
			[](char c, int i) -> int { return (i * 10) + (c - '0'); },
			*result,
			0);
		cout << "Parsed Successfully: " << i << endl;
	} else {
		cout << "Parse Error" << endl;

	}

	return 0;
}

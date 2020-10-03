#include<iostream>
#include<string>

#include"parser.h"

using namespace std;

int main(void){
	auto digit = matchPred([](char c){ return c >= '0' && c <= '9'; });
	auto number = some(digit);

	cout << "Input test string: ";
	string input;
	getline(cin, input);

	auto [result, rest] = number(input);

	if(result) {
		auto nums = map<char, int>([](char c) -> int { return c - '0'; }, *result);

		cout << nums;
		cout << endl << rest.value_or("String End") << endl;
	}

	return 0;
}

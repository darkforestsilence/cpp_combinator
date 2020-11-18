#include<iostream>
#include<string>
#include<list>

#include"parser.h"

using namespace std;

Parser integer = pfold<char, int, char>(
			[](char c, int i) -> int { return (i * 10) + (c - '0'); },
			some(matchPred<char>([](char c){ return isdigit(c);})),
			0);

int main(void){
	cout << "Input test string: ";
	string input;
	getline(cin, input);

	auto [i, rest] = integer(list(input.begin(), input.end()));

	if(i){
		cout << "Parsed Successfully: " << *i << endl;
	} else {
		cout << "Parse Error" << endl;

	}

	return 0;
}

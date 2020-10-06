#include<iostream>
#include<string>

#include"parser.h"

using namespace std;

int main(void){
	// Match a character from 0-9
	auto digit = matchPred<char,char>([](char c){ return c >= '0' && c <= '9'; });
	// match one or more digits
	auto integer = some(digit);
		

	// get some input to check
	cout << "Input test string: ";
	string input;
	getline(cin, input);

	// run the parser
	auto [result, rest] = integer(list<char>(input.begin(), input.end()));

	// use the fold function to reduce the list to an int
	if(result){
		int i = fold<char,int> (
			[](char c, int i) -> int { return (i * 10) + (c - '0'); },
			*result,
			0);
		cout << "Parsed Successfully: " << i << endl;
	} else {
		// we didn't have a result
		cout << "Parse Error" << endl;

	}

	return 0;
}

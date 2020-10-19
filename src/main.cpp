#include<iostream>
#include<string>
#include<list>

#include"parser.h"

using namespace std;

ParserReturn<unsigned int, char> integer(ParserInput<char> input){
	auto[result, rest] = some(matchPred<char, char>([](char c){
		return c >= '0' && c <= '9';
	}))(input);

	return make_pair(opfold<char, int>(
		[](char c, int i) -> int { return (i * 10) + (c- '0'); },
		result,
		0), rest);
}

int main(void){
	

	// get some input to check
	cout << "Input test string: ";
	string input;
	getline(cin, input);

	// run the parser
	auto [i, rest] = integer(
		list(input.begin(), input.end()));

	if(i){
		cout << "Parsed Successfully: " << *i << endl;
	} else {
		// we didn't have a result
		cout << "Parse Error" << endl;

	}

	return 0;
}

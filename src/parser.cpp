#include<optional>
#include<utility>
#include<string>
#include<functional>
#include<parser.h>

using namespace std;

ParserReturn<char, string> getChar(optional<string> str){
	return str 
		? make_pair(
			str->length() > 0 
				? (optional<char>) str->at(0) 
				: nullopt,
			str->length() > 1 
				? (optional<string>) str->substr(1) 
				: nullopt)
		: make_pair(nullopt, nullopt);
}


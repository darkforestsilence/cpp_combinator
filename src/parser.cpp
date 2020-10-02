#include<variant>
#include<optional>
#include<utility>
#include<string>
#include<functional>
#include<vector>

using namespace std;

template<typename T>
using ParserReturn = pair<optional<T>,optional<string>>;

template<typename T>
using Parser = function<ParserReturn<T>(optional<string>)>;

ParserReturn<char> getChar(optional<string> str){
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


Parser<char> matchPred(function<bool(char)> pred){
	return [pred] (optional<string> str) -> ParserReturn<char> {
		if(!str) return make_pair(nullopt, nullopt);

		auto [ch, rest] = getChar(str);
		return !ch || !pred(*ch)
			? make_pair(nullopt, str)
			: make_pair(ch, rest);
	};
}

Parser<char> matchChar(char c){
	return matchPred([c] (char ch) -> bool { return c == ch; });
}

#include<variant>
#include<optional>
#include<utility>
#include<string>
#include<functional>
#include<sstream>

using namespace std;

template<typename T>
using ParserReturn = pair<optional<T>,optional<string>>;

template<typename T>
using Parser = function<ParserReturn<T>(optional<string>)>;

ParserReturn<char> getChar(optional<string> str){
	if(str){
		if(str->length() > 0){
			auto ch = str->at(0);
			auto rest = str->substr(1);

			return make_pair(ch, 
					rest.length() == 0 
					? nullopt 
					: (optional<string>) rest);
		}
		return make_pair(nullopt, nullopt);
	}
	return make_pair(nullopt, str);
}


Parser<char> matchPred(function<bool(char)> pred){
	return [pred] (optional<string> str) -> ParserReturn<char> {
		if(str){
			auto [ch, rest] = getChar(str);
			if(!ch || !pred(*ch))
				return make_pair(nullopt, str);
			return make_pair(ch, rest);
		}

		return make_pair(nullopt, nullopt);
	};
}

Parser<char> matchChar(char c){
	return matchPred([c] (char ch) -> bool { return c == ch; });
}

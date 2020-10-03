#include<optional>
#include<utility>
#include<string>
#include<functional>
#include<list>

template<typename T>
using ParserReturn = std::pair<std::optional<T>,std::optional<std::string>>;

template<typename T>
using Parser = std::function<ParserReturn<T>(std::optional<std::string>)>;

ParserReturn<char> getChar(std::optional<std::string> str);
Parser<char> matchPred(std::function<bool(char)> pred);
Parser<char> matchChar(char c);

template<typename T,typename U>
Parser<std::pair<T,U>> andThen(Parser<T> first, Parser<U> second){
	return [first, second](std::optional<std::string> str) -> ParserReturn<std::pair<T,U>> {
		auto [p1, rest1] = first(str);
		if(p1){
			auto [p2, rest] = second(rest1);
			if(p2){
				return std::make_pair(std::make_pair(*p1, *p2), rest);
			}
		}
		return std::make_pair(std::nullopt, std::nullopt);
	};
}

template<typename T>
Parser<T> either(Parser<T> a, Parser<T> b){
	return [a, b](std::optional<std::string> str) ->
		ParserReturn<T> {
			auto [p1, rest1] = a(str);
			if(p1){
				return std::make_pair(
						p1, rest1);
			}
			return b(str);
		};
}

template<typename T>
Parser<std::list<T>*> many(Parser<T> parser);

template<typename T>
Parser<std::list<T>*> some(Parser<T> parser){
	return [parser](std::optional<std::string> str) -> 
		ParserReturn<std::list<T>*> {
			auto [first, rest1] = parser(str);
			if(!first)
				return std::make_pair(std::nullopt, 
						std::nullopt);
			auto [list, rest] = many(parser)(rest1);
			(*list)->push_front(*first);
			return std::make_pair(list, rest);
		};
}

template<typename T>
Parser<std::list<T>*> many(Parser<T> parser){
	return [parser](std::optional<std::string> str) ->
		ParserReturn<std::list<T>*> {
			auto [first, rest1] = parser(str);
			if(!first)
				return std::make_pair(new std::list<T>(), 
						str);
			auto [list, rest] = many(parser)(rest1);
			(*list)->push_front(*first);
			return std::make_pair(list, rest);
		};
}

template<typename T, typename U>
std::list<U>* map(std::function<U(T)> func, std::list<T>* list) {
	std::list<U>* l2 = new std::list<U>();
	for(typename std::list<T>::iterator it = list->begin();
			it != list->end();
			++it){
		l2->push_back(func(*it));
	}
	return l2;
}

#include<optional>
#include<utility>
#include<string>
#include<functional>
#include<list>

template<typename T, typename U>
using ParserReturn = std::pair<std::optional<T>,std::optional<U>>;

template<typename T, typename U>
using Parser = std::function<ParserReturn<T,U>(std::optional<U>)>;

ParserReturn<char, std::string> getChar(std::optional<std::string> str);

template<typename T, typename U>
Parser<T,U> matchPred(std::function<bool(T)> pred){
	return [pred] (std::optional<U> str) -> ParserReturn<T,U> {
		if(!str) return make_pair(std::nullopt, std::nullopt);

		auto [ch, rest] = getChar(str);
		return ch && pred(*ch)
			? make_pair(ch, rest)
			: make_pair(std::nullopt, str);
	};
}

Parser<char, std::string> matchChar(char c);

template<typename T,typename U, typename V>
Parser<std::pair<T,U>, V> andThen(Parser<T,V> first, Parser<U,V> second){
	return [first, second](std::optional<V> str) -> ParserReturn<std::pair<T,U>, V> {
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

template<typename T, typename U>
Parser<T,U> either(Parser<T,U> a, Parser<T,U> b){
	return [a, b](std::optional<U> str) ->
		ParserReturn<T,U> {
			auto [p1, rest1] = a(str);
			if(p1){
				return std::make_pair(
						p1, rest1);
			}
			return b(str);
		};
}

template<typename T, typename U>
Parser<std::list<T>*, U> many(Parser<T, U> parser);

template<typename T, typename U>
Parser<std::list<T>*, U> some(Parser<T, U> parser){
	return [parser](std::optional<U> str) -> 
		ParserReturn<std::list<T>*, U> {
			auto [first, rest1] = parser(str);
			if(!first)
				return std::make_pair(std::nullopt, 
						std::nullopt);
			auto [list, rest] = many(parser)(rest1);
			(*list)->push_front(*first);
			return std::make_pair(list, rest);
		};
}

template<typename T, typename U>
Parser<std::list<T>*, U> many(Parser<T,U> parser){
	return [parser](std::optional<U> str) ->
		ParserReturn<std::list<T>*, U> {
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

template<typename T, typename U>
U fold(std::function<U(T, U)> func, std::list<T>* list, U u){
	for(typename std::list<T>::iterator it = list->begin();
			it != list->end();
			++it){
		u =  func(*it, u);
	}
	return u;
}

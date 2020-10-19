#include<optional>
#include<utility>
#include<functional>
#include<list>


// define some type names to make the code simpler
template<typename T>
using Maybe = std::optional<T>;
template<typename U>
using ParserInput = Maybe<std::list<U>>;
template<typename T, typename U>
using ParserReturn = std::pair<Maybe<T>,Maybe<std::list<U>>>;
template<typename T, typename U>
using Parser = std::function<ParserReturn<T,U>(ParserInput<U>)>;

// maybe pull a value from the front of the input string
template<typename T, typename U>
ParserReturn<T, U> getValue(ParserInput<U> string){
	return string
		? std::make_pair(
			string->size() > 0 
				? (Maybe<char>) *(string->begin())
				: std::nullopt,
			string->size() > 1 
				? (ParserInput<char>) std::list<char>(++(string->begin()), string->end())
				: std::nullopt)
		: make_pair(std::nullopt, std::nullopt);
}


// return a function that will match a value against a predicate
template<typename T, typename U>
Parser<T,U> matchPred(std::function<bool(T)> pred){
	return [pred] (ParserInput<U> list_value) -> ParserReturn<T,U> {
		if(!list_value) return make_pair(std::nullopt, std::nullopt);

		auto [ch, rest] = getValue<T,U>(std::list<char>(list_value->begin(), list_value->end()));
		return ch && pred(*ch)
			? std::make_pair(ch, rest)
			: std::make_pair(std::nullopt, list_value);
	};
}

// get a function that will match against a specific value
template<typename T, typename U>
Parser<T,U> matchVal(T c){
	return matchPred<T, U>([c] (T ch) -> bool { return c == ch; });
}

// match a sequence of parsers
template<typename T,typename U, typename V>
Parser<std::pair<T,U>, V> andThen(Parser<T,V> first, Parser<U,V> second){
	return [first, second](ParserInput<V> str) -> ParserReturn<std::pair<T,U>, V> {
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

// select between one of two options
template<typename T, typename U>
Parser<T,U> either(Parser<T,U> a, Parser<T,U> b){
	return [a, b](ParserInput<U> str) ->
		ParserReturn<T,U> {
			auto [p1, rest1] = a(str);
			if(p1){
				return std::make_pair(
						p1, rest1);
			}
			return b(str);
		};
}

// forward declare many for use in some
template<typename T, typename U>
Parser<std::list<T>*, U> many(Parser<T, U> parser);

// match one or more of a parser 
template<typename T, typename U>
Parser<std::list<T>*, U> some(Parser<T, U> parser){
	return [parser](ParserInput<U> str) -> 
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

// match 0 or more of a parser
template<typename T, typename U>
Parser<std::list<T>*, U> many(Parser<T,U> parser){
	return [parser](ParserInput<U> str) ->
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

// map a function over a list
// this can mutate the list to be another type
// or just change the values
// eg. map(add(5), list)
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

// reduce a list down to a single value using an accumulator
// this might be better to write using recursion
// but a loop is ok for the current inputs
template<typename T, typename U>
U fold(std::function<U(T, U)> func, std::list<T>* list, U u){
	for(typename std::list<T>::iterator it = list->begin();
			it != list->end();
			++it){
		u =  func(*it, u);
	}
	return u;
}

template<typename T, typename U>
Maybe<U> opfold(std::function<U(T,U)> func, Maybe<std::list<T>*> list, Maybe<U> u){
	if(!u)
		return std::nullopt;
	if(!list)
		return std::nullopt;
	return fold(func, *list, *u);
}

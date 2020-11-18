#include<optional>
#include<utility>
#include<functional>
#include<list>


// define some type names to make the code simpler
template<typename Type>
using Maybe = std::optional<Type>;
template<typename InputElement>
using ParserInput = Maybe<std::list<InputElement>>;
template<typename Result, typename InputElement>
using ParserReturn = std::pair<Maybe<Result>,Maybe<std::list<InputElement>>>;
template<typename Result, typename InputElement>
using Parser = std::function<ParserReturn<Result,InputElement>(ParserInput<InputElement>)>;

template<typename InputElement>
Maybe<InputElement> maybeFirst(ParserInput<InputElement> list){
	return list->size() > 0 ? (Maybe<InputElement>) *(list->begin()) : std::nullopt;
}

template<typename InputElement>
ParserInput<InputElement> maybeRest(ParserInput<InputElement> list){
	return list->size() > 1 ?
		(ParserInput<InputElement>) std::list<InputElement>(++(list->begin()), list->end()) : std::nullopt;
}

// maybe pull a value from the front of the input string
template<typename InputElement>
ParserReturn<InputElement, InputElement> getValue(ParserInput<InputElement> list){
	return list 
		? std::make_pair(maybeFirst(list), maybeRest(list))
		: std::make_pair(std::nullopt, std::nullopt);
}


// return a function that will match a value against a predicate
template<typename InputElement>
Parser<InputElement,InputElement> matchPred(std::function<bool(InputElement)> pred){
	return [pred] (ParserInput<InputElement> list) -> ParserReturn<InputElement,InputElement> {
		auto [element, rest] = getValue<InputElement>(list);
		return element && pred(*element)
			? std::make_pair(element, rest)
			: std::make_pair(std::nullopt, list);
	};
}

// get a function that will match against a specific value
template<typename InputElement>
Parser<InputElement, InputElement> matchVal(InputElement needed){
	return matchPred<InputElement>([needed] (InputElement value) -> bool { return value == needed; });
}

// match a sequence of parsers
template<typename Type1,typename Type2, typename InputElement>
Parser<std::pair<Type1,Type2>, InputElement> andThen(Parser<Type1,InputElement> first, Parser<Type2,InputElement> second){
	return [first, second](ParserInput<InputElement> list) -> ParserReturn<std::pair<Type1,Type2>, InputElement> {
		auto [result1, rest1] = first(list);
		if(result1){
			auto [result2, rest] = second(rest1);
			if(result2){
				return std::make_pair(std::make_pair(*result1, *result2), rest);
			}
		}
		return std::make_pair(std::nullopt, std::nullopt);
	};
}

// select between one of two options
template<typename Result, typename InputElement>
Parser<Result,InputElement> either(Parser<Result,InputElement> first, Parser<Result,InputElement> second){
	return [first, second](ParserInput<InputElement> list) -> ParserReturn<Result,InputElement> {
		auto result = first(list);
		return result.first ? result : second(list);
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
template<typename Type, typename Accumulator>
Maybe<Accumulator> fold(std::function<Accumulator(Type, Accumulator)> func, Maybe<std::list<Type>*> list, Accumulator acc){
	if(!list)
		return std::nullopt;

	for(typename std::list<Type>::iterator it = (*list)->begin(); it != (*list)->end();){
		acc = func(*(it++), acc);
	}
	return acc;
}

template<typename Type, typename Accumulator, typename InputElement>
Parser<Accumulator, InputElement> pfold(std::function<Accumulator(Type, Accumulator)> func, Parser<std::list<Type>*, InputElement> parser, Accumulator identity){
	return [func, parser, identity](ParserInput<InputElement> list) -> ParserReturn<Accumulator,InputElement>{
		auto [result, rest] = parser(list);
		return std::make_pair(fold(func, result, identity), rest);
	};
}

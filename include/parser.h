#include<optional>
#include<string>
#include<functional>

template<typename T>
using ParserReturn = std::pair<std::optional<T>,std::optional<std::string>>;

template<typename T>
using Parser = std::function<ParserReturn<T>(std::optional<std::string>)>;

ParserReturn<char> getChar(std::optional<std::string> str);
Parser<char> matchPred(std::function<bool(char)> pred);
Parser<char> matchChar(char c);

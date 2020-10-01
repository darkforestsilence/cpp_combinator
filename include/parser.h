#include<optional>
#include<string>
#include<functional>

using namespace std;

template<typename T>
using ParserReturn = pair<optional<T>,optional<string>>;

template<typename T>
using Parser = function<ParserReturn<T>(optional<string>)>;

ParserReturn<char> getChar(optional<string> str);
Parser<char> matchPred(function<bool(char)> pred);
Parser<char> matchChar(char c);

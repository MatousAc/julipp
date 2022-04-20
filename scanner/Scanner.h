#pragma once
#include <unordered_map>
#include "../include.h"
#include "../tools/LoxError.h"
#include "Token.h"

class Scanner {
	string source;
	int start, current, line;

	void scanToken();
	int isDone();
	char next();
	char peek();
	char peekNext();
	bool nextChar(char c);
	void addString();
	void addNumber();
	void addIdentifier();
	void addToken(TokenType type, LitVal lit = NULL);
public:
	Scanner(string source);
	vector<Token> tokens;
	vector<Token> scanTokens();
	static std::unordered_map<string, TokenType> keywords;
};


#pragma once
#include <unordered_map>
#include "../include.h"
#include "../tools/LoxError.h"
#include "Token.h"

class Scanner {
	string source;
	bool afterNum, afterParen;
	int start, current, line;

	void scanToken();
	int isDone();
	char next();
	char peek();
	char peekNext();
	bool nextChar(char c);
	void addString(); // scans string literal
	// scans a number literal
	void addNumber(bool inFractional = false);
	void addIdentifier(); // adds id
	// just scans the next token in a mode
	// that allows for implied multiplication
	void scanAfterNum();
	void scanAfterParen();
	void addImpliedMultiply();
	void addToken(TokenType type, LitVal lit = NULL);
public:
	Scanner(string source);
	vector<Token> tokens;
	vector<Token> scanTokens();
	static std::unordered_map<string, TokenType> keywords;
};


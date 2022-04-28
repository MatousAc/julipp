#pragma once
#include "../include.h"
#include "LitVal.h"

enum TokenType {
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, STATEND, QUEST, COLON,
	PLUS, MINUS, MODULUS, STAR, FSLASH, BSLASH, CARET,

	// One or two character tokens.
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	// Literals.
	IDENTIFIER, IMPLIED_MULTIPLY, STRING, NUMBER,

	// Keywords.
	AND, BREAK, CASE, CLASS, CONTINUE, DEFAULT, ELSE, EXIT, FALSE, FUN, FOR, 
	IF, NIL, OR, PRINT, RETURN, SUPER, SWITCH, THIS, TRUE, VAR, WHILE,

	EoF
};

struct Token {
	TokenType type;
	string lexeme;
	LitVal lit;
	int line;

	// constructors
	Token(TokenType type, string lexeme, LitVal lit, int line);

	// methods
	string display();
	string toString();
};

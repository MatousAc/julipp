#pragma once
#include "../include.h"
#include "LitVal.h"

enum TokenType {
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN,
	COMMA, DOT, STATEND, QUEST, COLON, IMPLIED_MULTIPLY,
	PLUS, MINUS, MODULUS, STAR, FSLASH, BSLASH, CARET,

	// One or two character tokens.
	AND, OR,
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	// Literals.
	IDENTIFIER, STRING, NUMBER,

	// Keywords.
	BEGIN, BREAK, CASE, CLASS, CONTINUE, DO, DEFAULT,
	ELSE, ELSEIF, END, EXIT, FALSE, IF, NIL, PRINT,
	RETURN, SUPER, SWITCH, THIS, TRUE, WHILE,

	// Scope Types
	BUBBLE, GLOBAL, LOCAL,

	EoF,

	// unsupported keywords
	BAREMODULE, CATCH, EXPORT, FINALLY, FOR, FUN, IMPORT,
	LET, MACRO, MODULE, QUOTE, STRUCT, TRY, USING
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

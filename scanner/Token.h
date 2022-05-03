#pragma once
#include "../include.h"
#include "LitVal.h"

enum TokenType {
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, COLON,
	COMMA, DOT, STATEND, QUEST, IMPLIED_MULTIPLY,
	PLUS, MINUS, MODULUS, STAR, FSLASH, BSLASH, CARET,

	// One or two character tokens.
	AND, OR,
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,
	// Updating operators
	PLUS_EQUAL, MINUS_EQUAL, MODULUS_EQUAL, CARET_EQUAL,
	STAR_EQUAL, BSLASH_EQUAL, FSLASH_EQUAL,

	// Literals.
	IDENTIFIER, STRING, NUMBER,

	// Supported Keywords.
	BUBBLE, GLOBAL, LOCAL, // scoping
	BEGIN, BREAK, CONTINUE, DO,
	ELSE, ELSEIF, END, FALSE, IF, 
	NOTHING, TRUE, WHILE,

	EoF,

	// Unsupported Keywords
	BAREMODULE, CASE, CATCH, CLASS, DEFAULT,
	EXPORT, FINALLY, FOR, FUN, IMPORT,
	LET, MACRO, MODULE, QUOTE, RETURN, 
	STRUCT, SUPER, SWITCH, TRY, USING
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

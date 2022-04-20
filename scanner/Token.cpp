#include "Token.h"
Token::Token(TokenType type, string lexeme, LitVal lit, int line)
	: type{ type }, lexeme{ lexeme },
	lit{ lit }, line{ line } {}

string Token::display() {
	string literal = "";
	if ((type == STRING) || (type == NUMBER))
		literal = lit.toString();
	return std::to_string(line) + ": " + std::to_string((int)type)
		+ "\t| " + lexeme + " \t" + literal;
}

string Token::toString() {
	return lexeme;
}

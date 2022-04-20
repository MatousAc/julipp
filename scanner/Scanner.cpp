// contains a scanner that tokenizes the source code
// Syntax note: substr(start_pos, len)
#include <stdlib.h>
#include "Scanner.h"

std::unordered_map<string, TokenType> Scanner::keywords;

Scanner::Scanner(string source)
	: source{ source },
	tokens{ vector<Token>() },
	start{ 0 }, current{ 0 }, line{ 1 } {
	// initilize keywords
	keywords["and"] = AND;
	keywords["break"] = BREAK;
	keywords["case"] = CASE;
	keywords["continue"] = CONTINUE;
	keywords["class"] = CLASS;
	keywords["default"] = DEFAULT;
	keywords["else"] = ELSE;
	keywords["exit"] = EXIT;
	keywords["false"] = FALSE;
	keywords["fun"] = FUN;
	keywords["for"] = FOR;
	keywords["if"] = IF;
	keywords["nil"] = NIL;
	keywords["or"] = OR;
	keywords["print"] = PRINT;
	keywords["return"] = RETURN;
	keywords["super"] = SUPER;
	keywords["switch"] = SWITCH;
	keywords["this"] = THIS;
	keywords["true"] = TRUE;
	keywords["var"] = VAR;
	keywords["while"] = WHILE;
}

vector<Token> Scanner::scanTokens() {
	while (!isDone()) {
		start = current;
		scanToken();
	}
	tokens.push_back(Token(EoF, "", NULL, line));
	return tokens;
}

void Scanner::scanToken() {
	char c = next();
	switch (c) {
	case '(': addToken(LEFT_PAREN); break;
	case ')': addToken(RIGHT_PAREN); break;
	case '{': addToken(LEFT_BRACE); break;
	case '}': addToken(RIGHT_BRACE); break;
	case ',': addToken(COMMA); break;
	case '.': addToken(DOT); break;
	case '-': addToken(MINUS); break;
	case '+': addToken(PLUS); break;
	case ';': addToken(SEMICOLON); break;
	case '*': addToken(STAR); break;
	case '?': addToken(QUEST); break;
	case ':': addToken(COLON); break;
	case '!':
		addToken(nextChar('=') ? BANG_EQUAL : BANG);
		break;
	case '=':
		addToken(nextChar('=') ? EQUAL_EQUAL : EQUAL);
		break;
	case '<':
		addToken(nextChar('=') ? LESS_EQUAL : LESS);
		break;
	case '>':
		addToken(nextChar('=') ? GREATER_EQUAL : GREATER);
		break;
	case '/':
		if (nextChar('/'))
			while (peek() != '\n' && !isDone()) next();
		else addToken(SLASH);
		break;
	case ' ':
	case '\r':
	case '\t':
		break;
	case '\n':
		line++;
		break;
	case '"':
		addString();
		break;
	default:
		if (isdigit(c)) {
			addNumber();
		} else if (isalpha(c)) {
			addIdentifier();
			// consume Nulls that some text editors add
		} else if (c != 0) { // otherwise err
			err->report(line, "Unexpected character.", to_string(c));
		}
		break;
	}
}

int Scanner::isDone() {
	return current >= (int)source.length();
}

char Scanner::next() {
	return source[current++];
}

char Scanner::peek() {
	if (isDone()) return '\0';
	return source[current];
}

char Scanner::peekNext() {
	if (current + 1 >= (int)source.length()) return '\0';
	return source[current + 1];
}

bool Scanner::nextChar(char c) {
	if (isDone()) return false;
	if (source[current] != c) return false;
	current++;
	return true;
}

void Scanner::addString() {
	while (peek() != '"' && !isDone()) {
		if (peek() == '\n') line++;
		next();
	}
	if (isDone()) {
		err->report(line, "unterminated string");
		return;
	}
	next();
	addToken(STRING, source.substr(start + 1, (current - start) - 2));
}

void Scanner::addNumber() {
	while (isdigit(peek())) next();
	if (peek() == '.' && isdigit(peekNext())) {
		next();
		while (isdigit(peek())) next();
	}
	double num = atof(source.substr(start, (current - start)).c_str());
	addToken(NUMBER, num);
}

void Scanner::addIdentifier() {
	while (isalnum(peek())) next();
	string id = source.substr(start, (current - start));
	if (keywords.find(id) == keywords.end()) {
		// not keyword >> must be identifier
		addToken(IDENTIFIER);
		return;
	} // keyword
	addToken(keywords[id]);
}

void Scanner::addToken(TokenType type, LitVal lit) {
	string lexeme = source.substr(start, (current - start));
	tokens.push_back(Token(type, lexeme, lit, line));
}

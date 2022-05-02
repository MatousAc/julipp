// contains a scanner that tokenizes the source code
// Syntax note: substr(start_pos, len)
#include <stdlib.h>
#include "Scanner.h"

std::unordered_map<string, TokenType> Scanner::keywords;

Scanner::Scanner(string source)
	: source{ source },
	tokens{ vector<Token>() },
	afterNum{ false }, afterParen{ false },
	start{ 0 }, current{ 0 }, line{ 1 } {
	// initilize keywords
	keywords["begin"] = BEGIN;
	keywords["break"] = BREAK;
	keywords["case"] = CASE;
	keywords["continue"] = CONTINUE;
	keywords["do"] = DO;
	keywords["default"] = DEFAULT;
	keywords["else"] = ELSE;
	keywords["elseif"] = ELSEIF;
	keywords["end"] = END;
	keywords["exit"] = EXIT;
	keywords["false"] = FALSE;
	keywords["global"] = GLOBAL;
	keywords["if"] = IF;
	keywords["local"] = LOCAL;
	keywords["nil"] = NIL;
	keywords["print"] = PRINT;
	keywords["return"] = RETURN;
	keywords["super"] = SUPER;
	keywords["switch"] = SWITCH;
	keywords["this"] = THIS;
	keywords["true"] = TRUE;
	keywords["while"] = WHILE;
	// unsupported
	keywords["baremodule"] = BAREMODULE;
	keywords["class"] = CLASS;
	keywords["catch"] = CATCH;
	keywords["export"] = EXPORT;
	keywords["finally"] = FINALLY;
	keywords["for"] = FOR;
	keywords["fun"] = FUN;
	keywords["import"] = IMPORT;
	keywords["let"] = LET;
	keywords["macro"] = MACRO;
	keywords["module"] = MODULE;
	keywords["quote"] = QUOTE;
	keywords["struct"] = STRUCT;
	keywords["try"] = TRY;
	keywords["using"] = USING;
}

vector<Token> Scanner::scanTokens() {
	while (!isDone()) {
		start = current;
		scanToken();
	} // add a statend so code on last line will work
	tokens.push_back(Token(STATEND, "", NULL, line));
	tokens.push_back(Token(EoF, "", NULL, line));
	return tokens;
}

void Scanner::scanToken() {
	char c = next();
	switch (c) { // single character tokens
	case ',': addToken(COMMA); break;
	case ';': addToken(STATEND); break;
	case '?': addToken(QUEST); break;
	case ':': addToken(COLON); break;
	case '-': addToken(MINUS); break;
	case '+': addToken(PLUS); break;
	case '\\':addToken(BSLASH); break;
	case '*': addToken(STAR); break;
	case '^': addToken(CARET); break;
	case '%': addToken(MODULUS); break;
	// two-character tokens
	case '!': addToken(nextChar('=') ? BANG_EQUAL : BANG); break;
	case '=': addToken(nextChar('=') ? EQUAL_EQUAL : EQUAL); break;
	case '<': addToken(nextChar('=') ? LESS_EQUAL : LESS); break;
	case '>': addToken(nextChar('=') ? GREATER_EQUAL : GREATER); break;
	case '/': addToken(FSLASH); break;
	case '#': while (peek() != '\n' && !isDone()) next(); break;
	case '|':
		if (nextChar('|')) addToken(OR);
		else err->report(line, "Unsupported operator.", to_string(c));
		break;
	case '&':
		if (nextChar('&')) addToken(AND);
		else err->report(line, "Unsupported operator.", to_string(c));
		break;
	case ' ': case '\r': case '\t': break; // whitespace
	case '\n':addToken(STATEND); line++; break; // newlines
	case '\'': break; // addCharacterLiteral?
	case '"': addString(); break;
	case '(':
		if (afterParen) addImpliedMultiply();
		addToken(LEFT_PAREN);
		break;
	case ')': addToken(RIGHT_PAREN);
		scanAfterParen();
		break;
	case '.': 
		if (!isdigit(peek())) addToken(DOT);
		else addNumber(true);
		break;
	default:
		if (isdigit(c)) {
			addNumber();
		} else if (isalpha(c) || c == '_') {
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

char Scanner::advanceCurrentBy(int numChars) {
	current += numChars;
	return source[current];
}

char Scanner::peek() {
	if (isDone()) return '\0';
	return source[current];
}

char Scanner::peekNext() {
	if (current + 1 >= (int)source.length()) return '\0';
	return source[current + 1];
}

char Scanner::peekBy(int numChars) {
	if (current + numChars >= (int)source.length()) return '\0';
	return source[current + numChars];
}

bool Scanner::nextChar(char c) {
	if (isDone()) return false;
	if (source[current] != c) return false;
	current++;
	return true;
}

// based on this implementation by davka on SO
// https://stackoverflow.com/a/5612287/14062356
string unescape(const string& s)
{
	string res;
	string::const_iterator it = s.begin();
	while (it != s.end())
	{
		char c = *it++;
		if (c == '\\' && it != s.end())
		{
			switch (*it++) {
			case '\\': c = '\\'; break;
			case 'n': c = '\n'; break;
			case 't': c = '\t'; break;
			case 'v': c = '\v'; break;
			case 'b': c = '\b'; break;
			case 'r': c = '\r'; break;
			case '"': c = '\"'; break;
			case '\'': c = '\''; break;
				// all other escapes
			default:
				// invalid escape sequence - skip it.
				continue;
			}
		}
		res += c;
	}

	return res;
}

void Scanner::addString() {
	// czech for """string"""
	string str;

	bool isTripleQuote = false;
	int newLnWidth = (peekBy(2) == '\n'); // 0 or 1
	if (peek() == '"' && peekNext() == '"') {
		isTripleQuote = true;
	}

	while (true) {
		if (isDone()) { // error if EoF
			err->report(line, "unterminated string");
			return;
		} // end if """ (triple quote string)
		else if (isTripleQuote && peek() == '"' &&
			peekNext() == '"' && peekBy(2) == '"') {
			advanceCurrentBy(3);
			str = source.substr(start + 3 + newLnWidth,
				((current - start) - 6 - newLnWidth));
			break;
		} // or single quote below:
		else if (!isTripleQuote && peek() == '"' 
			&& peekBy(-1) != '\\') {
			next();
			str = source.substr(start + 1, (current - start) - 2);
			break;
		}
		
		if (peek() == '\n') line++;
		next();
	}

	str = unescape(str);
	addToken(STRING, str);
}

void Scanner::addNumber(bool inFractional) {
	while (isdigit(peek())) next();
	if (peek() == '.' && isdigit(peekNext()) && !inFractional) {
		next();
		while (isdigit(peek())) next();
	}
	double num = atof(source.substr(start, (current - start)).c_str());
	if (afterParen) addImpliedMultiply();
	addToken(NUMBER, num);
	if (peek() == '(') addImpliedMultiply();
	scanAfterNum(); // triggers another mode
}

void Scanner::addIdentifier() {
	while (isalnum(peek()) || peek() == '_') next();
	string id = source.substr(start, (current - start));
	if (keywords.find(id) == keywords.end()) {
		// not keyword >> must be identifier
		if (afterNum || afterParen) addImpliedMultiply();
		addToken(IDENTIFIER);
		return;
	} // keyword
	addToken(keywords[id]);
}

void Scanner::addToken(TokenType type, LitVal lit) {
	string lexeme = source.substr(start, (current - start));
	tokens.push_back(Token(type, lexeme, lit, line));
}

void Scanner::scanAfterNum() {
	if (!isDone()) {
		start = current;
		afterNum = true;
		scanToken();
		afterNum = false;
	}
}

void Scanner::scanAfterParen() {
	if (!isDone()) {
		start = current;
		afterParen = true;
		scanToken();
		afterParen = false;
	}
}

void Scanner::addImpliedMultiply() {
	tokens.push_back(Token(
		IMPLIED_MULTIPLY, // type
		tokens[tokens.size() - 1].lexeme, // prev
		NULL, line // curr line
	));
}
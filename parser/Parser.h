#pragma once
#include "../include.h"
#include "../scanner/Token.h"
#include "Expr.hpp"
#include "Stmt.hpp"
using std::runtime_error;

// protos
struct ParseExcept;

class Parser {
	vector<Token> tokens;
	int current;
	int loopDepth;

public: // constructor
	Parser(vector<Token> tokens);
	vector<Stmt*> parse();
private:
	Stmt* declaration();
	Stmt* varDeclaration();
	Stmt* statement();
	Stmt* breakStatement();
	Stmt* continueStatement();
	Stmt* caseStatement(Expr* switchOn);
	Stmt* defaultCaseStatement();
	Stmt* exitStatement();
	Stmt* forStatement();
	Stmt* switchStatement();
	Stmt* whileStatement();
	Stmt* ifStatement();
	Stmt* printStatement();
	Stmt* expressionStatement();
	Stmt* function(string kind);
	Stmt* returnStatement();
	vector<Stmt*> block();

	Expr* expression();
	Expr* ternary();
	Expr* Or(); // or && and are reserved
	Expr* And();
	Expr* assignment();
	Expr* equality();
	Expr* comparison();
	Expr* term();
	Expr* factor();
	Expr* unary();
	Expr* call();
	Expr* primary();

	// helpers
	Expr* finishCall(Expr* callee);
	bool match(vector<TokenType> types);
	Token consume(TokenType type, string message);
	bool check(TokenType type);
	Token advance();
	bool isAtEnd();
	Token peek();
	Token previous();

	// errors
	ParseExcept pex(Token token, string message);

	void synchronize();
	bool inLoop();
};

struct ParseExcept : public runtime_error {
	//here we're using base = runtime_error;
	explicit ParseExcept(const string& message);
	explicit ParseExcept();
};


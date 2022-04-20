// parses tokens into an Abstract Syntax Tree
#include "Parser.h"
#include "../tools/LoxError.h"
#include "../tools/AstPrinter.h"
#include "../tools/helpers.h" // for instance_of
#include <typeinfo>

Parser::Parser(vector<Token> tokens)
	: tokens{ tokens },
	current{ 0 },
	loopDepth{ 0 } {};

// parsing statements
vector<Stmt*> Parser::parse() {
	vector<Stmt*> statements{};
	while (!isAtEnd()) {
		statements.push_back(declaration());
	}
	return statements;
}

Stmt* Parser::declaration() {
	try {
		if (match({ FUN })) return function("function");
		if (match({ VAR })) return varDeclaration();
		return statement();
	}
	catch (ParseExcept error) {
		// keep parsing if possible
		synchronize();
		return NULL;
	}
}

Stmt* Parser::varDeclaration() {
	Token name = consume(IDENTIFIER, "Expect variable name.");

	Expr* initilizer = NULL;
	if (match({ EQUAL })) initilizer = expression();

	consume(SEMICOLON, "Expect ';' after variable declaration.");
	return new Var(name, initilizer);
}

Stmt* Parser::statement() {
	if (match({ BREAK })) return breakStatement();
	if (match({ CASE })) throw pex(previous(),
		"'case' must be inside switch statement");
	if (match({ CONTINUE })) return continueStatement();
	if (match({ DEFAULT })) throw pex(previous(),
		"'default' must be inside switch statement");
	if (match({ EXIT })) return exitStatement();
	if (match({ PRINT })) return printStatement();
	if (match({ FOR })) return forStatement();
	if (match({ SWITCH })) return switchStatement();
	if (match({ WHILE })) return whileStatement();
	if (match({ IF })) return ifStatement();
	if (match({ RETURN })) return returnStatement();
	if (match({ LEFT_BRACE })) return new Block(block());
	return expressionStatement();
}

Stmt* Parser::breakStatement() {
	if (!inLoop())
		throw pex(previous(),
			"'break' must be inside while or for loop");
	consume(SEMICOLON, "Expect ';' after 'break'.");
	return new Break();
}

Stmt* Parser::caseStatement(Expr* switchOn) {
	// collect case (desugaring to if-else)
	Token eq(EQUAL_EQUAL, "==", NULL, NULL);
	Expr* compareTo = expression();
	Expr* condition = new Binary(switchOn, eq, compareTo);
	consume(COLON, "Expect ':' after case expression.");
	Stmt* body = statement(); // grab one statement

	// get any more cases
	Stmt* nextCase = nullptr;
	if (match({ CASE }))
		nextCase = caseStatement(switchOn);
	else if (match({ DEFAULT }))
		nextCase = defaultCaseStatement();

	// reverse recursive constuction of if-else statements:
	return new If(condition, body, nextCase);
}

Stmt* Parser::continueStatement() {
	if (!inLoop())
		throw pex(previous(),
			"'continue' must be inside while or for loop");
	consume(SEMICOLON, "Expect ';' after 'continue'.");
	return new Continue();
}

Stmt* Parser::defaultCaseStatement() {
	consume(COLON, "Expect ':' after default.");
	return statement();
}

Stmt* Parser::exitStatement() {
	consume(SEMICOLON, "Expect ';' after 'exit'.");
	return new Exit();
}

Stmt* Parser::forStatement() {
	// init
	consume(LEFT_PAREN, "Expect '(' after 'for'.");
	Stmt* initializer;
	if (match({ SEMICOLON })) {
		initializer = nullptr;
	} else if (match({ VAR })) {
		initializer = varDeclaration();
	} else {
		initializer = expressionStatement();
	}
	// condition
	Expr* condition = nullptr;
	if (!check(SEMICOLON)) {
		condition = expression();
	}
	consume(SEMICOLON, "Expect ';' after loop condition.");
	// increment
	Expr* increment = nullptr;
	if (!check(RIGHT_PAREN)) {
		increment = expression();
	}
	consume(RIGHT_PAREN, "Expect ')' after increment.");

	loopDepth++;
	// body
	Stmt* body = statement();
	loopDepth--;
	return new For(initializer, condition, increment, body);
}

Stmt* Parser::switchStatement() {
	Stmt* body = nullptr;
	Expr* switchOn = expression();
	consume(LEFT_BRACE, "Expect '{' after switch expression.");
	if (match({ CASE })) {
		body = caseStatement(switchOn);
	}
	consume(RIGHT_BRACE, "Expect '}' after switch statement.");
	return body;
}

Stmt* Parser::whileStatement() {
	consume(LEFT_PAREN, "Expect '(' after 'while'.");
	Expr* condition = expression();
	consume(RIGHT_PAREN, "Expect ')' after condition.");
	loopDepth++;
	Stmt* body = statement();
	loopDepth--;
	return new While(condition, body);
}

Stmt* Parser::ifStatement() {
	consume(LEFT_PAREN, "Expect '(' after 'if'.");
	Expr* condition = expression();
	consume(RIGHT_PAREN, "Expect ')' after if condition.");

	Stmt* thenBranch = statement();
	Stmt* elseBranch = nullptr;
	if (match({ ELSE })) {
		elseBranch = statement();
	}

	return new If(condition, thenBranch, elseBranch);
}

Stmt* Parser::printStatement() {
	Expr* value = expression();
	consume(SEMICOLON, "Expect ';' after value.");
	return new Print(value);
}

Stmt* Parser::expressionStatement() {
	Expr* expr = expression();
	consume(SEMICOLON, "Expect ';' after expression.");
	return new Expression(expr);
}

Stmt* Parser::function(string kind) {
	Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
	consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
	vector<Token> params{};
	if (!check(RIGHT_PAREN)) {
		do {
			if (params.size() >= 255) {
				err->error(peek(), "Can't have more than 255 parameters.");
			}

			params.push_back(
				consume(IDENTIFIER, "Expect parameter name."));
		} while (match({ COMMA }));
	}
	consume(RIGHT_PAREN, "Expect ')' after parameters.");
	consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
	vector<Stmt*> body = block();
	return new Function(name, params, body);
}

vector<Stmt*> Parser::block() {
	vector<Stmt*> statements{};

	while (!check(RIGHT_BRACE) && !isAtEnd()) {
		statements.push_back(declaration());
	}

	consume(RIGHT_BRACE, "Expect '}' after block.");
	return statements;
}

Stmt* Parser::returnStatement() {
	Token keyword = previous();
	Expr* value = NULL;
	if (!check(SEMICOLON)) {
		value = expression();
	}

	consume(SEMICOLON, "Expect ';' after return value.");

	return new Return(keyword, value);
}
// parsing expressions
Expr* Parser::expression() { return assignment(); }

Expr* Parser::assignment() {
	Expr* expression = ternary();

	if (match({ EQUAL })) {
		Token equals = previous();
		Expr* value = assignment();

		if (instanceof<Variable>(expression)) {
			Token name = ((Variable*)expression)->name;
			return new Assign(name, value);
		}

		pex(equals, "Invalid assignment target.");
	}
	return expression;
}

Expr* Parser::ternary() {
	Expr* condition = Or();
	while (match({ QUEST })) {
		Expr* ifTrue;
		Expr* ifFalse;
		ifTrue = expression();
		if (!match({ COLON })) {
			advance(); // get to the EoF
			throw pex(previous(), "Expect '?' to have matching ':'.");
		}
		ifFalse = expression();
		return new Ternary(condition, ifTrue, ifFalse);
	}
	return condition;
}

Expr* Parser::Or() {
	Expr* expr = And();

	while (match({ OR })) {
		Token op = previous();
		Expr* right = And();
		expr = new Logical(expr, op, right);
	}
	return expr;
}

Expr* Parser::And() {
	Expr* expr = equality();

	while (match({ AND })) {
		Token op = previous();
		Expr* right = equality();
		expr = new Logical(expr, op, right);
	}
	return expr;
}

Expr* Parser::equality() {
	Expr* expression = comparison();

	while (match(vector<TokenType>{ BANG_EQUAL, EQUAL_EQUAL })) {
		Token op = previous();
		Expr* right = comparison();
		expression = new Binary(expression, op, right);
	}
	return expression;
}

Expr* Parser::comparison() {
	Expr* expr = term();

	while (match(vector<TokenType>{ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL })) {
		Token op = previous();
		Expr* right = term();
		expr = new Binary(expr, op, right);
	}
	return expr;
}

Expr* Parser::term() {
	Expr* expression = factor();

	while (match(vector<TokenType>{ MINUS, PLUS })) {
		Token op = previous();
		Expr* right = factor();
		expression = new Binary(expression, op, right);
	}
	return expression;
}

Expr* Parser::factor() {
	Expr* expression = unary();

	while (match(vector<TokenType>{ SLASH, STAR })) {
		Token op = previous();
		Expr* right = unary();
		expression = new Binary(expression, op, right);
	}
	return expression;
}

Expr* Parser::unary() {
	if (match(vector<TokenType>{BANG, MINUS})) {
		Token op = previous();
		Expr* right = unary();
		return new Unary(op, right);
	}
	return call();
}

Expr* Parser::call() {
	Expr* expression = primary();

	while (true) {
		if (match({ LEFT_PAREN })) {
			expression = finishCall(expression);
		} else {
			break;
		}
	}
	return expression;
}

Expr* Parser::primary() {
	// we take false, true, or nil and create a Literal that holds a LoxType
	if (match({FALSE})) return new Literal(LoxType{ false });
	if (match({TRUE})) return new Literal(LoxType{ true });
	if (match({NIL})) return new Literal(LoxType{});

	// package a LitVal as a LoxType and store it in Literal
	if (match(vector<TokenType>{NUMBER, STRING})) {
		return new Literal(LoxType{ previous().lit.retrieve() });
	}

	if (match({ IDENTIFIER })) {
		return new Variable{ previous() };
	}

	if (match(vector<TokenType>{LEFT_PAREN})) {
		Expr* expressionVar = expression();
		consume(RIGHT_PAREN, "Expect ')' after expression.");
		return new Grouping(expressionVar);
	}
	throw pex(peek(), "Expected expression match ");
}

// helpers
Expr* Parser::finishCall(Expr* callee) {
	vector<Expr*> arguments{};
	if (!check(RIGHT_PAREN)) {
		do {
			if (arguments.size() >= 255) {
				err->error(peek(), 
				"Can't have more than 255 arguments.");
			}
			arguments.push_back(expression());
		} while (match({ COMMA }));
	}

	Token paren = consume(RIGHT_PAREN,
		"Expect ')' after arguments.");

	return new Call(callee, paren, arguments);
}

bool Parser::match(vector<TokenType> types) {
	auto type = types.begin();
	auto end = types.end();
	while (type != end) {
		if (check(*type)) {
			advance();
			return true;
		}
		type++;
	}
	return false;
}

bool Parser::check(TokenType type) {
	if (isAtEnd())
		return false;
	return peek().type == type;
}

Token Parser::consume(TokenType type, string message) {
	if (check(type)) return advance();
	throw pex(peek(), message);
}

Token Parser::advance() {
	if (!isAtEnd())
		current++;
	return previous();
}

bool Parser::isAtEnd() { return peek().type == EoF; }

Token Parser::peek() { return tokens[current]; }

Token Parser::previous() { return tokens[current - 1]; }

// errors
ParseExcept Parser::pex(Token token, string message) {
	err->error(token, message);
	return ParseExcept();
}

void Parser::synchronize() {
	advance();

	while (!isAtEnd()) {
		if (previous().type == SEMICOLON) return;

		switch (peek().type) {
		case CASE:
		case CLASS:
		case DEFAULT:
		case FUN:
		case VAR:
		case FOR:
		case IF:
		case PRINT:
		case RETURN:
		case SWITCH:
		case WHILE:
			return;
		}

		advance();
	}
}

ParseExcept::ParseExcept() : runtime_error{ "" } {}
ParseExcept::ParseExcept(const string& message) : runtime_error{ message.c_str() } {}

bool Parser::inLoop() { return loopDepth > 0; }

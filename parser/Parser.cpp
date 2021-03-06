// parses tokens into an Abstract Syntax Tree
#include "Parser.h"
#include "../tools/JError.h"
#include "../tools/AstPrinter.h"
#include "../tools/helpers.h" // for instance_of
#include <typeinfo>

Parser::Parser(vector<Token> tokens)
	: tokens{ tokens },
	current{ 0 },
	nest{} {};

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
		if (check(LOCAL) || check(GLOBAL))
			return varDeclaration();
		return statement();
	}
	catch (ParseExcept error) {
		// keep parsing if possible
		synchronize();
		return NULL;
	}
}

Stmt* Parser::varDeclaration() {
	TokenType scope = advance().type;
	Token name = consume(IDENTIFIER, "Expect variable name.");

	Expr* initilizer = NULL;
	if (match({ EQUAL })) initilizer = expression();

	consume(STATEND, "Expect ';' or end of line after variable declaration.");
	return new Declare(scope, name, initilizer);
}

Stmt* Parser::statement() {
	if (match({ STATEND })) return nullptr; // NoOp valid stmt
	if (match({ BEGIN })) return block();
	if (match({ BREAK })) return breakStatement();
	/*if (match({ CASE })) throw pex(previous(),
		"'case' must be inside switch statement");*/
	if (match({ CONTINUE })) return continueStatement();
	if (match({ DEFAULT })) throw pex(previous(),
		"'default' must be inside switch statement");
	//if (match({ SWITCH })) return switchStatement();
	if (match({ WHILE })) return whileStatement();
	if (match({ IF })) return ifStatement();
	if (match({ RETURN })) return returnStatement();
	return expressionStatement();
}

Stmt* Parser::breakStatement() {
	if (!inLoop())
		throw pex(previous(),
			"'break' must be inside while or for loop");
	consume(STATEND, "Expect ';' or end of line after 'break'.");
	return new Break();
}

//Stmt* Parser::caseStatement(Expr* switchOn) {
//	// collect case (desugaring to if-else)
//	Token eq(EQUAL_EQUAL, "==", NULL, NULL);
//	Expr* compareTo = expression();
//	Expr* condition = new Binary(switchOn, eq, compareTo);
//	consume(COLON, "Expect ':' after case expression.");
//	Stmt* body = statement(); // grab one statement
//
//	// get any more cases
//	Stmt* nextCase = nullptr;
//	if (match({ CASE }))
//		nextCase = caseStatement(switchOn);
//	else if (match({ DEFAULT }))
//		nextCase = defaultCaseStatement();
//
//	// reverse recursive constuction of if-else statements:
//	return new If(condition, body, nextCase);
//}

Stmt* Parser::continueStatement() {
	if (!inLoop())
		throw pex(previous(),
			"'continue' must be inside while or for loop");
	consume(STATEND, "Expect ';' or end of line after 'continue'.");
	return new Continue();
}

//Stmt* Parser::defaultCaseStatement() {
//	consume(COLON, "Expect ':' after default.");
//	return statement();
//}

//Stmt* Parser::switchStatement() {
//	Stmt* body = nullptr;
//	Expr* switchOn = expression();
//	consume(BEGIN, "Expect '{' after switch expression.");
//	if (match({ CASE })) {
//		body = caseStatement(switchOn);
//	}
//	consume(END, "Expect '}' after switch statement.");
//	return body;
//}

Stmt* Parser::whileStatement() {
	Expr* condition = expression();
	nest.push_back(WHILE);
	Stmt* body = block();
	nest.pop_back();
	return new While(condition, body);
}

Stmt* Parser::ifStatement() {
	nest.push_back(IF);
	vector<Expr*> conditions{};
	vector<Stmt*> thenBranches{};
	do  {
		conditions.push_back(expression());
		thenBranches.push_back(ifBlock());
	} while (match({ ELSEIF }));	
	Stmt* elseBranch = nullptr;
	if (match({ ELSE })) {
		elseBranch = block();
	} else { // only consume if we don't get regular block
		consume(END, "Incomplete 'if' Expect 'end'");
	}
	nest.pop_back();
	return new If(conditions, thenBranches, elseBranch);
}

Stmt* Parser::expressionStatement() {
	Expr* expr = expression();
	consume(STATEND, "Expect ';' or end of line after expression.");
	return new Expression(expr);
}

Stmt* Parser::function(string kind) {
	nest.push_back(FUN);
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
	consume(BEGIN, "Expect '{' before " + kind + " body.");
	vector<Stmt*> body = block()->statements;
	nest.pop_back();
	return new Function(name, params, body);
}

Block* Parser::block() {
	vector<Stmt*> statements{};

	while (!check(END) && !isAtEnd()) {
		statements.push_back(declaration());
	}

	consume(END, "Expect 'end' after block.");
	return new Block(statements);
}

Block* Parser::ifBlock() {
	vector<Stmt*> statements{};

	while (!check(ELSEIF) && !check(ELSE) &&
		!check(END) && !isAtEnd()) {
		statements.push_back(declaration());
	}
	// we don't consume the end of scope here
	// cause we may need it depending on what 
	// it is
	return new Block(statements);
}

Stmt* Parser::returnStatement() {
	Token keyword = previous();
	Expr* value = NULL;
	if (!check(STATEND)) {
		value = expression();
	}

	consume(STATEND, "Expect ';' or end of line after return value.");

	return new Return(keyword, value);
}
// parsing expressions
Expr* Parser::expression() { return assignment(); }

Expr* Parser::assignment() {
	Expr* expression = ternary();

	if (match({ EQUAL, PLUS_EQUAL, MINUS_EQUAL, 
		MODULUS_EQUAL, CARET_EQUAL, STAR_EQUAL,
		BSLASH_EQUAL, FSLASH_EQUAL })) {
		Token op = previous();
		Expr* value = assignment();
		// if we've already got it
		if (instanceof<Variable>(expression)) {
			Token name = ((Variable*)expression)->name;
			return new Assign(op,
				getAssignmentScope(), name, value);
		}
		pex(op, "Invalid assignment target.");
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
	Expr* expression = implyMult();

	while (match(vector<TokenType>{ FSLASH, BSLASH, STAR, MODULUS })) {
		Token op = previous();
		Expr* right = implyMult();
		expression = new Binary(expression, op, right);
	}
	return expression;
}

Expr* Parser::implyMult() {
	Expr* expression = power();

	while (match(vector<TokenType>{ IMPLIED_MULTIPLY })) {
		Token op = previous();
		// at this point, we just treat this as basic multiplication 
		// now that we're @ the right precedence level : D
		op.type = STAR;
		Expr* right = power(); // this is a grouping/number/identifier
		expression = new Binary(expression, op, right);
	}
	return expression;
}

Expr* Parser::power() {
	Expr* expression = unary();

	while (match(vector<TokenType>{ CARET })) {
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
	if (match({ FALSE })) return new Literal(JType{ false });
	if (match({ TRUE })) return new Literal(JType{ true });
	if (match({ NOTHING })) return new Literal(JType{ nothing{} });

	// package a LitVal as a LoxType and store it in Literal
	if (match(vector<TokenType>{NUMBER, STRING})) {
		return new Literal(JType{ previous().lit.retrieve() });
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

int Parser::loopDepth() {
	int res = 0;
	for (TokenType type : nest)
		if (type == WHILE) res++;
	return res;
}

bool Parser::inLoop() { return loopDepth() > 0; }

TokenType Parser::getAssignmentScope() {
	if (nest.size() == 0) return GLOBAL;
	TokenType end = *(nest.end() - 1);
	switch (end) {
	case BEGIN: return BEGIN;
	case IF: return BEGIN;
	case WHILE: return BUBBLE;
	case FUN: return BUBBLE;
	default: return BUBBLE;
	};
}

// errors
ParseExcept Parser::pex(Token token, string message) {
	err->error(token, message);
	return ParseExcept();
}

void Parser::synchronize() {
	advance();

	while (!isAtEnd()) {
		if (previous().type == STATEND) return;

		switch (peek().type) {
		case CASE:
		case CLASS:
		case DEFAULT:
		case FUN:
		case LOCAL:
		case FOR:
		case IF:
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

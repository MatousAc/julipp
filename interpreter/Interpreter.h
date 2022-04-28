#pragma once
#include "../include.h"
#include "../parser/Expr.hpp"
#include "../julipp.h"
#include "LoxType.h"
#include "Environment.h"

class Interpreter : ExprVisitor, StmtVisitor {
public:
	Interpreter();
	void interpret(vector<Stmt*> statements);
	LoxType getResult();
	// these two are for LoxFunction to use
	void executeBlock(vector<Stmt*> statements, Environment* environment);
	Environment* globals;

private:
	LoxType result;
	Environment* environment;
	Token curToken; // for error reporting

	void execute(Stmt* stmt);
	void evaluate(Expr* expression);
	// statements
	
	virtual void visitBlock(const Block* statement) override;
	virtual void visitBreak(const Break* statement) override;
	virtual void visitContinue(const Continue* statement) override;
	virtual void visitExit(const Exit* statement) override;
	virtual void visitExpression(const Expression* statement) override;
	virtual void visitFor(const For* statement) override;
	virtual void visitFunction(const Function* statement) override;
	virtual void visitIf(const If* statement) override;
	virtual void visitPrint(const Print* statement) override;
	virtual void visitReturn(const Return* statement) override;
	virtual void visitVar(const Var* statement) override;
	virtual void visitWhile(const While* statement) override;
	// expressions
	virtual void visitAssign(const Assign* expression) override;
	virtual void visitBinary(const Binary* expression) override;
	virtual void visitCall(const Call* expression) override;
	virtual void visitGrouping(const Grouping* expression) override;
	virtual void visitLogical(const Logical* expression) override;
	virtual void visitLiteral(const Literal* expression) override;
	virtual void visitUnary(const Unary* expression) override;
	virtual void visitTernary(const Ternary* expression) override;
	virtual void visitVariable(const Variable* expression) override;	
	// helpers
	bool isUnderscores(string s);
};

struct BreakExcept : public runtime_error {
	explicit BreakExcept(const string& message);
	explicit BreakExcept();
};

struct ContinueExcept : public runtime_error {
	explicit ContinueExcept(const string& message);
	explicit ContinueExcept();
};

//struct ReturnExcept : public runtime_error {
//	explicit ReturnExcept(const string& message);
//	explicit ReturnExcept();
//};

struct ReturnExcept {
	LoxType value;
	ReturnExcept(LoxType value);
};
	 
// global, stateful interpreter
inline Interpreter* interpreter = new Interpreter;


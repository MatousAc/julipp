#pragma once
#include "../include.h"
#include "../parser/Expr.hpp"

class AstPrinter : ExprVisitor {
	string result;
public:
	AstPrinter() : result{} {};
	void read(Expr* expression);

	void visitAssign(const Assign* expression) override;
	void visitBinary(const Binary* expression) override;
	void visitCall(const Call* expression) override;
	void visitGrouping(const Grouping* expression) override;
	void visitLiteral(const Literal* expression) override;
	void visitLogical(const Logical* expression) override;
	void visitTernary(const Ternary* expression) override;
	void visitUnary(const Unary* expression) override;
	void visitVariable(const Variable* expression) override;

	string parenthesize(string name, vector<Expr*> expressions);

	string getResult();
};


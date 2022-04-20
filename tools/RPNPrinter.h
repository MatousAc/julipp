#pragma once
#include "../include.h"
#include "../parser/Expr.hpp"

class RPNPrinter : ExprVisitor {
	string result;
public:
	RPNPrinter() : result{} {};
	void read(Expr* expression);

	void visitTernary(const Ternary* expression) override;
	void visitBinary(const Binary* expression) override;
	void visitGrouping(const Grouping* expression) override;
	void visitLiteral(const Literal* expression) override;
	void visitUnary(const Unary* expression) override;

	string collect(string name, vector<Expr*> expressions);

	string getResult();
};


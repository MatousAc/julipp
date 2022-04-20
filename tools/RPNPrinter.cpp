#include "RPNPrinter.h"
void RPNPrinter::read(Expr* expression) {
	expression->accept(this);
}

void RPNPrinter::visitTernary(const Ternary* expression) {
	expression->condition->accept(this);
	collect(
		"? " + getResult(),
		vector{ expression->ifTrue, expression->ifFalse }
	);
}
void RPNPrinter::visitBinary(const Binary* expression) {
	collect(
		expression->op.lexeme,
		vector{ expression->left, expression->right }
	);
}
void RPNPrinter::visitGrouping(const Grouping* expression) {
	expression->expression->accept(this);
}

void RPNPrinter::visitLiteral(const Literal* expression) {
	if (expression->value.isnil()) result += "nil";
	else result += expression->value.toString();
}

void RPNPrinter::visitUnary(const Unary* expression) {
	collect(expression->op.lexeme, vector{ expression->right });
}

string RPNPrinter::collect(string name, vector<Expr*> expressions) {
	auto cur = expressions.begin();
	auto end = expressions.end();
	while (cur != end) {
		if (result.length() and
			result[result.length() - 1] != ' ')
			result += " ";
		(*cur)->accept(this);
		cur++;
	}
	result += " " + name;
	return result;
}

string RPNPrinter::getResult() { return result; }
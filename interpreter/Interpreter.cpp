// interprets the Abstract Syntax Tree
#include "Interpreter.h"
#include "../tools/LoxError.h"
#include "LoxCallable.h"
#include "LoxFunction.h"
#include "../tools/helpers.h"
#include "ClockFunction.hpp"

// protos
struct BreakExcept;
struct CountinueExcept;

// public
Interpreter::Interpreter() :
	result{},
	globals{ new Environment{} },
	environment{ this->globals },
	curToken{ EoF, "start", NULL, -1 } {
	globals->define("clock", new ClockFunction{});
};

void Interpreter::interpret(vector<Stmt*> statements) {
	try {
		auto statement = statements.begin();
		auto end = statements.end();
		while (statement != end) {
			execute(*statement);
			result = getResult();
			statement++;
		}
	}
	catch (RunError error) {
		err->handleRunError(RunError(curToken, error.message));
	}
}

LoxType Interpreter::getResult() {
	return result;
}

// execution helpers
void Interpreter::executeBlock(vector<Stmt*> statements,
	Environment* environment) {
	Environment* previous = this->environment;
	try {
		this->environment = environment;
		for (auto& statement : statements) {
			execute(statement);
		}
	}
	catch (RunError) {}
	this->environment = previous;
}

// private
void Interpreter::execute(Stmt* stmt) {
	stmt->accept(this);
}

void Interpreter::evaluate(Expr* expression) {
	expression->accept(this);
}

// visiting statements
void Interpreter::visitBlock(const Block* statement) {
	executeBlock(statement->statements, new Environment(environment));
}
void Interpreter::visitExpression(const Expression* statement) {
	evaluate(statement->expression);
}
void Interpreter::visitBreak(const Break* statement) {
	throw BreakExcept();
}
void Interpreter::visitContinue(const Continue* statement) {
	throw ContinueExcept();
}
void Interpreter::visitExit(const Exit* statement) {
	exit(0); // we just quit the interpreter here
}
void Interpreter::visitFor(const For* statement) {
	if (statement->initializer != nullptr) // init
		execute(statement->initializer);
	// loop
	evaluate(statement->condition);
	while (getResult().isTruthy()) {
		try { execute(statement->body); }
		catch (BreakExcept) { break; }
		catch (ContinueExcept) {} // try to increment and eval cond.
		if (statement->increment != nullptr)
			evaluate(statement->increment);
		evaluate(statement->condition);
	}
}
void Interpreter::visitFunction(const Function* statement) {
	LoxFunction* function{ new LoxFunction{ 
		new Function(
		statement->name, // we have to make a new function node here
		statement->params, // cause of the const constraints
		statement->body
		) } };
	environment->define(statement->name.lexeme, LoxType{ function });
}
void Interpreter::visitIf(const If* statement) {
	evaluate(statement->condition); // eval cond
	if (getResult().isTruthy()) {
		execute(statement->thenBranch);
	} else if (statement->elseBranch != nullptr) {
		execute(statement->elseBranch);
	}
}
void Interpreter::visitReturn(const Return* statement) {
	LoxType value{};
	if (statement->value != NULL) {
		evaluate(statement->value);
		value = getResult();
	}
	throw ReturnExcept(value);
}
void Interpreter::visitPrint(const Print* statement) {
	evaluate(statement->expression);
	LoxType value = getResult();
	cout << value.toString() << endl;
}
void Interpreter::visitVar(const Var* statement) {
	LoxType value{};
	if (statement->initializer != NULL) {
		evaluate(statement->initializer);
		value = getResult();
	}
	environment->define(statement->name.lexeme , value);
}
void Interpreter::visitWhile(const While* statement) {
	evaluate(statement->condition); // eval condition
	while (getResult().isTruthy()) { // check condition
		try { execute(statement->body); }
		catch (BreakExcept brk) { break; }
		catch (ContinueExcept cnt) {} // eval condition
		evaluate(statement->condition);
	}
}

// visiting expressions
void Interpreter::visitAssign(const Assign* expression) {
	evaluate(expression->value);
	LoxType value = getResult();
	environment->assign(expression->name, value);
	result = value;
}
void Interpreter::visitBinary(const Binary* expression) {
	curToken = expression->op;
	evaluate(expression->left);
	LoxType left = getResult();
	evaluate(expression->right);
	LoxType right = getResult();

	switch (expression->op.type) {
	case EQUAL_EQUAL:
		result = left == right;
		break;
	case BANG_EQUAL:
		result = left != right;
		break;
	case GREATER:
		result = left > right;
		break;
	case GREATER_EQUAL:
		result = left >= right;
		break;
	case LESS:
		result = left < right;
		break;
	case LESS_EQUAL:
		result = left <= right;
		break;
	case PLUS:
		result = left + right;
		break;
	case MINUS:
		result = left - right;
		break;
	case STAR:
		result = left * right;
		break;
	case SLASH:
		result = left / right;
		break;
	default:
		break;
	}
}
void Interpreter::visitCall(const Call* expression) {
	evaluate(expression->callee);
	LoxType callee = getResult();

	vector<LoxType> arguments{};
	for (auto argument : expression->arguments) {
		evaluate(argument);
		if (getResult() < LoxType{ 0.0 }) {
			evaluate(argument);
		}
		arguments.push_back(getResult());
	}

	if (!holds_alternative<LoxCallable*>(callee.value)) {
		throw RunError(expression->paren,
			"Can only call functions and classes.");
	}

	LoxCallable* function = get<LoxCallable*>(callee.value);
	if (arguments.size() != function->arity()) {
	throw RunError(expression->paren, "Expected " +
		to_string(function->arity()) + " arguments but got " +
		to_string(arguments.size()) + ".");
	}
	result = function->call(this, arguments);
}
void Interpreter::visitGrouping(const Grouping* expression) {
	evaluate(expression->expression);
}
void Interpreter::visitLiteral(const Literal* expression) {
	result = expression->value;
}
void Interpreter::visitLogical(const Logical* expression) {
	evaluate(expression->left);
	LoxType left = getResult();
	// FIXME can we simplify this?
	if (expression->op.type == OR) {
		if (left.isTruthy()) return;
	} else { // must be AND
		if (!left.isTruthy()) return;
	}
	evaluate(expression->right);
}
void Interpreter::visitUnary(const Unary* expression) {
	curToken = expression->op;
	evaluate(expression->right);
	LoxType right = getResult();

	switch (expression->op.type) {
	case MINUS:
		result = -right;
		break;
	case BANG:
		result = !right;
		break;
	default:
		break;
	}
}
void Interpreter::visitTernary(const Ternary* expression) {
	expression->condition->accept(this);
	if (getResult().isTruthy())
		expression->ifTrue->accept(this);
	else
		expression->ifFalse->accept(this);
}
void Interpreter::visitVariable(const Variable* expression) {
	result = environment->grab(expression->name);
}

// exceptions
BreakExcept::BreakExcept() : runtime_error{ "" } {}
BreakExcept::BreakExcept(const string& message) : runtime_error{ message.c_str() } {}

ContinueExcept::ContinueExcept() : runtime_error{ "" } {}
ContinueExcept::ContinueExcept(const string& message) : runtime_error{ message.c_str() } {}

// FIXME We can throw anything in C++
//ReturnExcept::ReturnExcept() : runtime_error{ "" } {}
//ReturnExcept::ReturnExcept(const string& message) : runtime_error{ message.c_str() } {}

ReturnExcept::ReturnExcept(LoxType value) 
	: value{ value } {}

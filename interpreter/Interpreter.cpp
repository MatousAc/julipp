// interprets the Abstract Syntax Tree
#include "Interpreter.h"
#include "../tools/JError.h"
#include "JCallable.h"
#include "../functions/JFunction.h"
#include "../tools/helpers.h"
#include "../functions/String.hpp"
#include "../functions/Math.hpp"
#include "../functions/Print.hpp"
#include "../functions/ReadLine.hpp"
#include "../functions/Nothing.hpp"

// protos
struct BreakExcept;
struct CountinueExcept;

// public
Interpreter::Interpreter() :
	result{},
	globals{ new Environment{} },
	//environment{ this->globals },
	environment{},
	curToken{ EoF, "start", NULL, -1 } {
	// default functions
	globals->define(GLOBAL, "length", new Length{});
	globals->define(GLOBAL, "sqrt", new Sqrt{});
	globals->define(GLOBAL, "abs", new Abs{});
	globals->define(GLOBAL, "floor", new Floor{});
	globals->define(GLOBAL, "ceil", new Ceil{});
	globals->define(GLOBAL, "round", new Round{});
	globals->define(GLOBAL, "print", new Print{});
	globals->define(GLOBAL, "println", new PrintLn{});
	globals->define(GLOBAL, "readline", new ReadLine{});
	globals->define(GLOBAL, "parsenum", new ParseNum{});
	globals->define(GLOBAL, "Nothing", new Nothing{});
	globals->define(GLOBAL, "exit", new Exit{});
	environment = globals;
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

JType Interpreter::getResult() {
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
	}	// making sure we un-nest when using
		// break and continue exeptions 
	catch (BreakExcept brk) {
		this->environment = previous;
		throw brk;
	} catch (Continue cnt) {
		this->environment = previous;
		throw cnt;
	} catch (RunError re) {
		this->environment = previous;
		throw re;
	}
	this->environment = previous;
}

// private
void Interpreter::execute(Stmt* stmt) {
	if (stmt) stmt->accept(this); // if not nullptr/NoOp
}

void Interpreter::evaluate(Expr* expression) {
	expression->accept(this);
}

// visiting statements
void Interpreter::visitBlock(const Block* statement) {
	executeBlock(statement->statements, new Environment(environment));

	//Environment* old = environment;
	//try {
	//	executeBlock(statement->statements, new Environment(environment));
	//	environment = old;
	//} catch (BreakExcept brk) {
	//	environment = old;
	//	throw brk;
	//} catch (ContinueExcept cnt) {
	//	environment = old;
	//	throw cnt;
	//}
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
void Interpreter::visitFunction(const Function* statement) {
	JFunction* function{ new JFunction{ 
		new Function(
		statement->name, // we have to make a new function node here
		statement->params, // cause of the const constraints
		statement->body
		) } };
	environment->define(LOCAL, statement->name.lexeme, JType{ function });
}
void Interpreter::visitIf(const If* statement) {
	int i = 0;
	for (Expr* condition : statement->conditions) {
		evaluate(condition);
		Stmt* thenBranch = statement->thenBranches.at(i++);
		if (getResult().isTruthy()) {
			execute(thenBranch);
			return;
		}
	} // else
	if (statement->elseBranch != nullptr) {
		execute(statement->elseBranch);
	}
}
void Interpreter::visitReturn(const Return* statement) {
	JType value{};
	if (statement->value != NULL) {
		evaluate(statement->value);
		value = getResult();
	}
	throw ReturnExcept(value);
}
void Interpreter::visitDeclare(const Declare* statement) {
	JType value{};
	if (statement->initializer != NULL) {
		evaluate(statement->initializer);
		value = getResult();
	}
	environment->define(statement->scope, statement->name.lexeme , value);
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
	JType value = getResult();
	if (expression->op.type != EQUAL) { // updating operator
		value = getUpdatingOpVal(expression->name, expression->op, value);
	}
	environment->assign(expression->scope, expression->name, value);
	result = value;
}
void Interpreter::visitBinary(const Binary* expression) {
	curToken = expression->op;
	evaluate(expression->left);
	JType left = getResult();
	evaluate(expression->right);
	JType right = getResult();

	switch (expression->op.type) {
	case EQUAL_EQUAL:	result = left == right; break;
	case BANG_EQUAL:	result = left != right; break;
	case GREATER:		result = left > right; break;
	case GREATER_EQUAL:	result = left >= right; break;
	case LESS:			result = left < right; break;
	case LESS_EQUAL:	result = left <= right; break;
	case PLUS:			result = left + right; break;
	case MINUS:			result = left - right; break;
	case MODULUS:		result = left % right; break;
	case STAR:			result = left * right; break;
	case FSLASH:		result = left / right; break;
	case BSLASH:		result = right / left; break;
	case CARET:			result = left ^ right; break;
	default:			break;
	}
}
void Interpreter::visitCall(const Call* expression) {
	evaluate(expression->callee);
	JType callee = getResult();

	vector<JType> arguments{};
	for (auto argument : expression->arguments) {
		evaluate(argument);
		arguments.push_back(getResult());
	}

	if (!holds_alternative<JCallable*>(callee.value)) {
		throw RunError(expression->paren,
			"Can only call functions and classes.");
	}

	JCallable* function = get<JCallable*>(callee.value);
	if (arguments.size() != function->arity() && 
		!takesVariableArgs(function)) {
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
	JType left = getResult();
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
	JType right = getResult();

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
	if (isUnderscores(expression->name.lexeme))
		err->handleRunError(RunError{ 
		"all-underscore identifier used as rvalue" });
	result = environment->grab(expression->name);
	if (result.isundefined()) throw RunError(expression->name,
		"Variable has not been defined.");
}

// helpers
bool Interpreter::isUnderscores(string s) {
	for (char const& c : s)
		if (c != '_') return false;
	return true;
}
bool Interpreter::takesVariableArgs(JCallable* function) {
	string name = function->toString();
	int pos = name.find("(");
	string args = name.substr(pos + 1, name.length() - 2);
	return strcmp(args.c_str(), "...");
}
JType Interpreter::getUpdatingOpVal(Token name, Token op, JType right) {
	JType left{};
	if (environment->has(name.lexeme))
		left = environment->grab(name);
	else throw RunError(name, "Variable has not been declared.");
	if (left.isundefined()) throw RunError(name,
		"Variable has not been defined.");
	switch (op.type) {
	case PLUS_EQUAL:	return left + right;
	case MINUS_EQUAL:	return left - right;
	case MODULUS_EQUAL:	return left % right;
	case STAR_EQUAL:	return left * right;
	case FSLASH_EQUAL:	return left / right;
	case BSLASH_EQUAL:	return right / left;
	case CARET_EQUAL:	return left ^ right;
	default:			return left;
	}
}
// exceptions
BreakExcept::BreakExcept() : runtime_error{ "" } {}
BreakExcept::BreakExcept(const string& message) : runtime_error{ message.c_str() } {}

ContinueExcept::ContinueExcept() : runtime_error{ "" } {}
ContinueExcept::ContinueExcept(const string& message) : runtime_error{ message.c_str() } {}

// FIXME We can throw anything in C++
//ReturnExcept::ReturnExcept() : runtime_error{ "" } {}
//ReturnExcept::ReturnExcept(const string& message) : runtime_error{ message.c_str() } {}

ReturnExcept::ReturnExcept(JType value) 
	: value{ value } {}

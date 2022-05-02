#pragma once
#include "../include.h"
#include "../parser/Stmt.hpp"
#include "../interpreter/Interpreter.h"
#include "../interpreter/JCallable.h"

struct JFunction : JCallable {
	Function* declaration;
public:
	JFunction(Function* declaration);
	int arity() override;
	JType call(Interpreter* interpreter, vector<JType> arguments) override;
	string toString() override;
};

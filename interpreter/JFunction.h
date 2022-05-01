#pragma once
#include "../include.h"
#include "../parser/Stmt.hpp"
#include "Interpreter.h"
#include "JCallable.h"

struct JFunction : JCallable {
	Function* declaration;
public:
	JFunction(Function* declaration);
	int arity() override;
	JType call(Interpreter* interpreter, vector<JType> arguments) override;
	string toString() override;
};

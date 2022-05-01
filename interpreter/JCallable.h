#pragma once
#include "../include.h"
#include "../parser/Stmt.hpp"
#include "JType.h"
#include "Interpreter.h"

//class Interpreter;

struct JCallable {
	virtual int arity() = 0;
	virtual JType call(Interpreter* interpreter, vector<JType> arguments) = 0;
	virtual string toString() = 0;
};
// no need to use getResult() here because we know a function must return
// a JType (even if that is nil)

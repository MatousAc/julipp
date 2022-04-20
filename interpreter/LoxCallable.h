#pragma once
#include "../include.h"
#include "../parser/Stmt.hpp"
#include "LoxType.h"
#include "Interpreter.h"

//class Interpreter;

struct LoxCallable {
	virtual int arity() = 0;
	virtual LoxType call(Interpreter* interpreter, vector<LoxType> arguments) = 0;
	virtual string toString() = 0;
};
// no need to use getResult() here because we know a function must return
// a LoxType (even if that is nil)

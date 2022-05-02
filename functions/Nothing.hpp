#pragma once
#include "../interpreter/JCallable.h"

struct Nothing : JCallable {
	int arity() override { return 0; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		return JType{ nothing{} }; // the nothing type
	}
	string toString() { return "Nothing()"; }
	Nothing() {};
};
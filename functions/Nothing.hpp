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

struct Exit : JCallable {
	int arity() override { return 0; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		exit(0);
		return JType{};
	}
	string toString() { return "exit()"; }
	Exit() {};
};
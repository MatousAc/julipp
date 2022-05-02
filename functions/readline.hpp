#pragma once
#include "../interpreter/JCallable.h"

struct ReadLine : JCallable {
	int arity() override { return 0; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		string str;
		std::getline(cin, str);
		JType input{str};
		return input;
	}
	string toString() { return "readline()"; }
	ReadLine() {};
};
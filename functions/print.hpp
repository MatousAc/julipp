#pragma once
#include <chrono>
#include "../interpreter/JCallable.h"
using namespace std::chrono;

struct Print : JCallable {
	int arity() override { return 1; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		for (JType arg : arguments)
			cout << arg.toString();
		return JType{};
	}
	string toString() { return "print(...)"; }
	Print() {};
};

struct PrintLn : JCallable {
	int arity() override { return 1; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		for (JType arg : arguments)
			cout << arg.toString();
		cout << endl; // print newline
		return JType{};
	}
	string toString() { return "println(...)"; }
	PrintLn() {};
};
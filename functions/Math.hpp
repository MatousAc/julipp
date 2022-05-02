#pragma once
#include <cmath>
#include "../interpreter/JCallable.h"

struct Sqrt : JCallable {
	int arity() override { return 1; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		JType val = arguments[0];
		if (val.type() != "number") throw RunError(
			interpreter->curToken,
			"sqrt only takes one number as it's argument");
		return sqrt(get<double>(val.value));
	}
	string toString() { return "sqrt(number)"; }
	Sqrt() {};
};

struct Abs : JCallable {
	int arity() override { return 1; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		JType val = arguments[0];
		if (val.type() != "number") throw RunError(
			interpreter->curToken,
			"abs only takes one number as it's argument");
		return abs(get<double>(val.value));
	}
	string toString() { return "abs(number)"; }
	Abs() {};
};

struct Floor : JCallable {
	int arity() override { return 1; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		JType val = arguments[0];
		if (val.type() != "number") throw RunError(
			interpreter->curToken,
			"floor only takes one number as it's argument");
		return floor(get<double>(val.value));
	}
	string toString() { return "floor(number)"; }
	Floor() {};
};

struct Ceil : JCallable {
	int arity() override { return 1; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		JType val = arguments[0];
		if (val.type() != "number") throw RunError(
			interpreter->curToken,
			"ceil only takes one number as it's argument");
		return ceil(get<double>(val.value));
	}
	string toString() { return "ceil(number)"; }
	Ceil() {};
};

struct Round : JCallable {
	int arity() override { return 1; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		JType val = arguments[0];
		if (val.type() != "number") throw RunError(
			interpreter->curToken,
			"round only takes one number as it's argument");
		return round(get<double>(val.value));
	}
	string toString() { return "round(number)"; }
	Round() {};
};
#pragma once
#include <string>
#include "../interpreter/JCallable.h"

struct StringLen : JCallable {
	int arity() override { return 1; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		JType val = arguments[0];
		if (val.type() != "string") throw RunError(
			interpreter->curToken, // line number reporting
			"strlen only takes one string as it's argument"); //msg
		string str = get<string>(val.value);
		return JType{ (double)str.length() };
	}
	string toString() { return "strlen(string)"; }
	StringLen() {};
};

struct ParseNum : JCallable {
	int arity() override { return 1; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		JType val = arguments[0];
		if (val.type() != "string") throw RunError(
			interpreter->curToken,
			"parseNum only takes one string as it's argument");
		return JType{ std::stod(get<string>(val.value)) };
	}
	string toString() { return "parseNum(string)"; }
	ParseNum() {};
};
#pragma once
#include <unordered_map>
#include "../include.h"
#include "../scanner/Token.h"
#include "LoxType.h"
#include "../tools/LoxError.h"
using std::unordered_map;

struct Environment {
	unordered_map<string, LoxType> values;

	Environment* enclosing;
	Environment(Environment* enclosing = nullptr);
	void define(string name, LoxType value);
	void assign(Token name, LoxType value);
	LoxType grab(Token name);
	void dump();
	bool isGlobal();
};


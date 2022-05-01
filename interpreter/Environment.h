#pragma once
#include <unordered_map>
#include "../include.h"
#include "../scanner/Token.h"
#include "JType.h"
#include "../tools/JError.h"
using std::unordered_map;

struct Environment {
	unordered_map<string, JType> values;

	Environment* enclosing;
	Environment(Environment* enclosing = nullptr);
	void define(string name, JType value);
	void assign(Token name, JType value);
	JType grab(Token name);
	void dump();
	bool isGlobal();
};


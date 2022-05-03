#include "Environment.h"

Environment::Environment(Environment* enclosing)
	: values{},
	enclosing{ enclosing } {}

bool Environment::has(string name) {
	if (inCurEnv(name)) return true;
	else if (enclosing != nullptr) return enclosing->has(name);
	else return false;
}
bool Environment::inCurEnv(string name) {
	return values.find(name) != values.end();
}

void Environment::define(TokenType scope, string name, JType value) {
	if (scope == LOCAL) values[name] = value; // force local declaration
	else if (scope == GLOBAL) {
		if (enclosing != nullptr) enclosing->define(scope, name, value);
		// define if not already defined or being redefined
		else if (value.isdefined() || inCurEnv(name))
			values[name] = value;
		/*if (enclosing == nullptr) { // @ global, pass up pointer to var
			if (!value.isundefined() || values.find(name) == values.end()) {
				values[name] = value; // define if not already defined or being redefined
			}
		} else { // define in outer scope. get pointer to that var
			values[name] = *(enclosing->define(scope, name, value));
		}*/
	} // if default, make local if not already present
	/*else if (scope == BUBBLE && !has(name)) values[name] = value;
	else if (scope == BUBBLE) values[name] = value;*/
}

void Environment::assign(TokenType scope, Token name, JType value) {
	if (inCurEnv(name.lexeme))
		values[name.lexeme] = value; // if it's local just update
	else if (has(name.lexeme)) // update variable in enclosing scope
		enclosing->assign(scope, name, value);
	else switch (scope) {
		case GLOBAL: define(GLOBAL, name.lexeme, value); break;
		case LOCAL: define(LOCAL, name.lexeme, value); break;
		case BUBBLE: define(LOCAL, name.lexeme, value); break;
		case BEGIN: define(GLOBAL, name.lexeme, value); break;
	}
}

JType Environment::grab(Token name) {
	// search in current scope
	if (values.find(name.lexeme) != values.end())
		return values[name.lexeme];
	// search in enclosing scope
	if (enclosing != nullptr)
		return enclosing->grab(name);

	throw new RunError(name,
		"Undeclared variable '" + name.lexeme + "'.");
}

bool Environment::isGlobal() { 
	return values.find("exit") != values.end();
}

void Environment::dump() {
	cout << "_________ " 
		<< (isGlobal() ? "Global " : " Local ")
		<< "Environment"
		<<" _________" << endl;
	//stolen from https://www.techiedelight.com/
	for (auto const& pair : this->values) {
		cout << "{" << pair.first
			<< ": " << pair.second.toString() << "}\n";
	}
}
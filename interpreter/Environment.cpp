#include "Environment.h"

Environment::Environment(Environment* enclosing)
	: values{},
	enclosing{ enclosing } {}

JType* Environment::define(TokenType scope, string name, JType value) {
	if (scope == LOCAL) values[name] = value;
	else if (scope == GLOBAL) {
		if (enclosing == nullptr) { // @ global, pass up pointer to var
			if (!value.isundefined() || values.find(name) == values.end()) {
				values[name] = value; // define if not already defined or being redefined
			}
		} else { // define in outer scope. get pointer to that var
			values[name] = *(enclosing->define(scope, name, value));
		}
	}
	return &(values[name]);
}

void Environment::assign(TokenType scope, Token name, JType value) {
	if (values.find(name.lexeme) != values.end()) {
		// below we assign the value inside the JType
		// so that any references to this in outer scopes
		// will also be updated
		values[name.lexeme].value = value.value;
		return;
	}

	if (enclosing != nullptr) {
		enclosing->assign(scope, name, value);
		return;
	}

	define(scope, name.lexeme, value);
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
	return values.find("strlen") != values.end();
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
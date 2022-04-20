#include "Environment.h"

Environment::Environment(Environment* enclosing)
	: values{},
	enclosing{ enclosing } {}

void Environment::define(string name, LoxType value) {
	values[name] = value;
}

void Environment::assign(Token name, LoxType value) {
	if (values.find(name.lexeme) != values.end()) {
		values[name.lexeme] = value;
		return;
	}

	if (enclosing != nullptr) {
		enclosing->assign(name, value);
		return;
	}

	throw new RunError(name,
		"Undefined variable '" + name.lexeme + "'.");
}

LoxType Environment::grab(Token name) {
	// search in current scope
	if (values.find(name.lexeme) != values.end())
		return values[name.lexeme];
	// search in enclosing scope
	if (enclosing != nullptr)
		return enclosing->grab(name);

	throw new RunError(name,
		"Undefined variable '" + name.lexeme + "'.");
}

bool Environment::isGlobal() { 
	return values.find("clock") != values.end();
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
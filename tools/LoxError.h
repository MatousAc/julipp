#pragma once
#include "../include.h"
#include "../scanner/Token.h"
using std::runtime_error;

// what we throw when we have runtime errors
struct RunError : public runtime_error {
	Token token;
	string message;
	explicit RunError();
	explicit RunError(const string& message);
	explicit RunError(const Token token, const string& message);
};

// handles error reporting
class LoxError {
public:
	bool hadError;
	bool hadRunError;

	LoxError();

	// error reporting
	void report(int line, string msg, string where = "");
	void error(Token token, string message);

	// errors
	RunError runErrorMUT();
	RunError runErrorMBT();
	void handleRunError(RunError error);
};

inline LoxError* err = new LoxError;
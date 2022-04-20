#include "LoxError.h"

RunError::RunError() :
	token{ EoF, "placeholder", NULL, -1 },
	message{ "" },
	runtime_error{ "" } {}
RunError::RunError(const string& message) :
	token{ EoF, "placeholder", NULL, -1 },
	message{ message },
	runtime_error{ message } {}
RunError::RunError(const Token token, const string& message)
	: token{ token },
	message{ message },
	runtime_error{ message.c_str() } {}


LoxError::LoxError() :
	hadRunError{ false },
	hadError{ false } {};

void LoxError::report(int line, string msg, string where) {
	cout << "[line " << line
		<< "] Error at " << where << " : "
		<< msg << endl;
	hadError = true;
}

void LoxError::error(Token token, string message) {
	if (token.type == EoF) {
		report(token.line, message, "end");
	} else {
		report(token.line, message, " '" + token.lexeme + "'");
	}
}

RunError LoxError::runErrorMUT() {
	throw RunError{ "mismatched type at unary operator" };
}

RunError LoxError::runErrorMBT() {
	throw RunError{ "mismatched types around binary operator" };
}

void LoxError::handleRunError(RunError error) {
	cout << error.message + "\n[line "
		+ to_string(error.token.line) + "] ";
	hadRunError = true;
}

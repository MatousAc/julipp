#include "JFunction.h"
#include "JType.h"

struct ReturnExcept;

JFunction::JFunction(Function* declaration) 
	: declaration{ declaration } {}

int JFunction::arity() {
    return (int)declaration->params.size();
}

JType JFunction::call(Interpreter* interpreter, 
    vector<JType> arguments) {
    Environment* environment = new Environment(interpreter->globals);
    interpreter->globals->dump();
    for (int i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params[i].lexeme,
            arguments[i]);
    }
    environment->dump();

    try {
        interpreter->executeBlock(declaration->body, environment);
    }
    catch (ReturnExcept returnValue) {
        return returnValue.value;
    }
    return JType{};
}

string JFunction::toString() {
    return "<fn " + declaration->name.lexeme + ">";
}

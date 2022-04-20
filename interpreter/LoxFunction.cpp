#include "LoxFunction.h"
#include "LoxType.h"

struct ReturnExcept;

LoxFunction::LoxFunction(Function* declaration) 
	: declaration{ declaration } {}

int LoxFunction::arity() {
    return (int)declaration->params.size();
}

LoxType LoxFunction::call(Interpreter* interpreter, 
    vector<LoxType> arguments) {
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
    return LoxType{};
}

string LoxFunction::toString() {
    return "<fn " + declaration->name.lexeme + ">";
}

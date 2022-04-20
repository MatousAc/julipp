#pragma once
#include "../include.h"
#include "../scanner/Scanner.h"
#include "../parser/Parser.h"
#include "../tools/tools.h"
#include "../scanner/Token.h"
#include "../interpreter/Interpreter.h"

void testStrComp();
string interpretLineRPN(string& source);

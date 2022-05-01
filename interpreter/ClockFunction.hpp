#pragma once
#include <chrono>
#include "JCallable.h"
using namespace std::chrono;

struct ClockFunction : JCallable {
	int arity() override { return 0; }
	JType call(Interpreter* interpreter, vector<JType> arguments) {
		return (double)duration_cast<milliseconds>(
			time_point_cast<milliseconds>(system_clock::now()
				).time_since_epoch()).count() / 1000;
	}
	string toString() { return "<native fn>"; }
	ClockFunction() {};
};
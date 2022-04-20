#pragma once
#include "../include.h"
#include "../interpreter/LoxType.h"
#define DOUBLE_PRECISION 1e-7

struct LitVal {
	variant<string, double> value;
	LitVal(string str);
	LitVal(double dbl);
	bool empty() const;
	string toString() const;
	string numToString() const;
	LoxType retrieve() const;
};


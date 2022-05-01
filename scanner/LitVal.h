#pragma once
#include "../include.h"
#include "../interpreter/JType.h"
#define DOUBLE_PRECISION 1e-7

struct LitVal {
	variant<string, double> value;
	LitVal(string str);
	LitVal(double dbl);
	bool empty() const;
	string toString() const;
	string numToString() const;
	JType retrieve() const;
};

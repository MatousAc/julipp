#pragma once
#include "../include.h"
#define DOUBLE_PRECISION 1e-7

// we forward declare the callable here rather than 
// including it because that causes thousands of errors
struct LoxCallable;
//struct LoxFunction;

struct LoxType {
	variant<
		monostate,
		string, double, bool,
		LoxCallable*
		//LoxFunction*
	> value;
	LoxType();
	LoxType(string str);
	LoxType(double dbl);
	LoxType(bool bl);
	LoxType(LoxCallable* callable);
	//LoxType(LoxFunction* function);
	bool isnil() const;
	bool isTruthy() const;
	string type() const;
	string toString() const;
	string numToLoxStr() const;

	// operator overloads
	bool operator==(const LoxType& r);
	bool operator!=(const LoxType& r);
	bool operator>(const LoxType& r);
	bool operator>=(const LoxType& r);
	bool operator<(const LoxType& r);
	bool operator<=(const LoxType& r);

	LoxType operator+(const LoxType& r);
	LoxType operator-(const LoxType& r);
	LoxType operator*(const LoxType& r);
	LoxType operator/(const LoxType& r);

	LoxType operator-();
	bool operator!();
};

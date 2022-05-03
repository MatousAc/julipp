#pragma once
#include "../include.h"
#define DOUBLE_PRECISION 1e-7

// we forward declare the callable here rather than 
// including it because that causes thousands of errors
struct JCallable;
struct nothing {};

struct JType {
	variant <
		monostate,
		string, double, bool,
		JCallable*,
		nothing
	> value;
	JType();
	JType(string str);
	JType(nothing n);
	JType(double dbl);
	JType(bool bl);
	JType(JCallable* callable);
	bool isundefined() const;
	bool isdefined() const;
	bool isnothing() const;
	bool isTruthy() const;
	bool isInt() const;
	string type() const;
	string toString() const;
	string numToLoxStr() const;

	// operator overloads
	bool operator==(const JType& r);
	bool operator!=(const JType& r);
	bool operator>(const JType& r);
	bool operator>=(const JType& r);
	bool operator<(const JType& r);
	bool operator<=(const JType& r);

	JType operator+(const JType& r);
	JType operator-(const JType& r);
	JType operator%(const JType& r);
	JType operator*(const JType& r);
	JType operator/(const JType& r);
	JType operator^(const JType& r);

	JType operator-();
	bool operator!();
};

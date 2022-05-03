#include <math.h>
#include "JType.h"
#include "../tools/JError.h"
#include "../tools/helpers.h"

// various constructors
JType::JType() {}
JType::JType(nothing n)
	: value{ n } {}
JType::JType(string str)
	: value{ str } {}
JType::JType(double dbl)
	: value{ dbl } {}
JType::JType(bool bl)
	: value{ bl } {}
JType::JType(JCallable* callable)
	: value{ callable } {}


// czechs if empty (contains NULL)
bool JType::isundefined() const {
	return holds_alternative<monostate>(value);
}
bool JType::isdefined() const {
	return !isundefined();
}
bool JType::isnothing() const {
	return holds_alternative<nothing>(value);
}

bool JType::isTruthy() const {
	if (isundefined())
		return false;
	else if (holds_alternative<bool>(value))
		return get<bool>(value);
	else
		return true;
}

bool JType::isInt() const {
	double dbl = get<double>(value);
	int integer = (int)dbl;
	double diff = (dbl - integer);
	if (diff < 0) diff *= -1;
	return  diff < DOUBLE_PRECISION;
}
// czechs type
string JType::type() const {
	string res = "unknown type";
	if (isundefined())
		res = "nil";
	else if (holds_alternative<string>(value))
		res = "string";
	else if (holds_alternative<double>(value))
		res = "number";
	else if (holds_alternative<bool>(value))
		res = "bool";
	else if (holds_alternative<JCallable*>(value))
		res = "LoxCallable*";
	else if (holds_alternative<nothing>(value))
		res = "nothing";
	else if (holds_alternative<monostate>(value))
		res = "undefined";
	return res;
}

string JType::toString() const {
	string res;
	if (isundefined())
		res = "undefined";
	else if (isnothing())
		res = "nothing";
	else if (holds_alternative<string>(value))
		res = get<string>(value);
	else if (holds_alternative<double>(value))
		res = this->numToLoxStr();
	else if (holds_alternative<bool>(value))
		res = (get<bool>(value)) ? "true" : "false";
	else if (holds_alternative<JCallable*>(value))
		res = "function() . . .";
	else
		res = "unknown type";

	return res;
}
// helper
string JType::numToLoxStr() const {
	double dbl = get<double>(value);
	string res;
	if (this->isInt())
		return to_string((int)dbl);
	return to_string(dbl);
}


///// ~~ operator overloads ~~ /////
// logical
// == : equals (different types cannpt be equal)
bool JType::operator==(const JType& r) {
	if (isnothing() && r.isnothing())
		return true;
	else if (isundefined() || r.isundefined())
		return false;
	// double == double
	else if (holds_alternative<double>(value) &&
		holds_alternative<double>(r.value))
		return get<double>(value) == get<double>(r.value);
	// string == string
	else if (holds_alternative<string>(value) &&
		holds_alternative<string>(r.value))
		return get<string>(value).compare(get<string>(r.value)) == 0;
	else // type mismatch
		return false;
}
// !=
bool JType::operator!=(const JType& r) {
	return !((*this) == r);
}
// > : numerical && textual greater than
bool JType::operator>(const JType& r) {
	if (isnothing() || r.isnothing())
		return false;
	// double > double
	else if (holds_alternative<double>(value) &&
		holds_alternative<double>(r.value))
		return get<double>(value) > get<double>(r.value);
	// string > string
	else if (holds_alternative<string>(value) &&
		holds_alternative<string>(r.value))
		return get<string>(value).compare(get<string>(r.value)) > 0;
	else // mismatched types
		err->runErrorMBT();
	return false;
}
// >=
bool JType::operator>=(const JType& r) {
	return *this > r || *this == r;
}
// < : num && text less than
bool JType::operator<(const JType& r) {
	if (isnothing() || r.isnothing())
		return false;
	// double > double
	else if (holds_alternative<double>(value) &&
		holds_alternative<double>(r.value))
		return get<double>(value) < get<double>(r.value);
	// string > string
	else if (holds_alternative<string>(value) &&
		holds_alternative<string>(r.value))
		return get<string>(value).compare(get<string>(r.value)) < 0;
	else // mismatched types
		err->runErrorMBT();
	return false;
}
// <=
bool JType::operator<=(const JType& r) {
	return *this < r || *this == r;
}

// arithmetic
// + : addition, concatenation
JType JType::operator+(const JType& r) {
	if (isnothing()) // if this is empty
		return r;
	else if (r.isnothing())
		return this;
	// double + double
	else if (holds_alternative<double>(value) &&
		holds_alternative<double>(r.value))
		return get<double>(value) + get<double>(r.value);
	// string + string => RunError
	else if (holds_alternative<string>(value) &&
		holds_alternative<string>(r.value))
		throw RunError{ "No method to add strings. Use '*' for concatenation."};
	// string + toString(double)
	else if (holds_alternative<string>(value) &&
		holds_alternative<double>(r.value))
		return get<string>(value) + r.numToLoxStr();
	// toString(double) + string
	else if (holds_alternative<double>(value) &&
		holds_alternative<string>(r.value))
		return this->numToLoxStr() + get<string>(r.value);
	else // mismatched types
		err->runErrorMBT();
	return JType{};
}
// - : subtraction
JType JType::operator-(const JType& r) {
	if (isnothing()) // if this is empty
		return (-JType{ r });
	else if (r.isnothing())
		return this;
	// double - double
	else if (holds_alternative<double>(value) &&
		holds_alternative<double>(r.value))
		return get<double>(value) - get<double>(r.value);
	else // mismatched types
		err->runErrorMBT();
	return JType{};
}
// % : modulus
JType JType::operator%(const JType& r) {
	if(!holds_alternative<double>(value) ||
	   !holds_alternative<double>(r.value))
		// no support for other types
		err->runErrorMBT();
	double left = get<double>(value);
	double right = get<double>(r.value);
	if (this->isInt() && r.isInt())
		return (double)((int)left % (int)right);
	else {
		while (left >= right) left -= right;
		return left;
	}
}
// * : multiplication and string duplication
JType JType::operator*(const JType& r) {
	if (isnothing() || r.isnothing()) // x * 0 = 0
		return JType{ nothing {} };
	// double * double
	else if (holds_alternative<double>(value) &&
		holds_alternative<double>(r.value))
		return get<double>(value) * get<double>(r.value);
	// string * double
	else if (holds_alternative<string>(value) &&
		holds_alternative<double>(r.value))
		return repeat(get<string>(value), (size_t)get<double>(r.value));
	// double * string
	else if (holds_alternative<double>(value) &&
		holds_alternative<string>(r.value))
		return repeat(get<string>(r.value), (size_t)get<double>(value));
	// string * string => concatenation
	else if (holds_alternative<string>(value) &&
		holds_alternative<string>(r.value))
		return get<string>(value) + get<string>(r.value);
	else // mismatched types
		err->runErrorMBT();
	return JType{};
}
// / : division and line breaks
JType JType::operator/(const JType& r) {
	if (isnothing()) // if this is empty
		return JType{ nothing{} };
	else if (r.isnothing())
		throw RunError("division by nothing");
	// double / double
	else if (holds_alternative<double>(value) &&
		holds_alternative<double>(r.value))
		return get<double>(value) / get<double>(r.value);
	// string / string
	else if (holds_alternative<string>(value) &&
		holds_alternative<string>(r.value))
		return get<string>(value) + "\n" + get<string>(r.value);
	else // mismatched types
		err->runErrorMBT();
	return JType{};
}
// ^ : exponentiation
JType JType::operator^(const JType& r) {
	if (!holds_alternative<double>(value) ||
		!holds_alternative<double>(r.value))
		// no support for other types
		err->runErrorMBT();
	return pow(get<double>(value), get<double>(r.value));
}

// unary
// - : negates numbers
JType JType::operator-() {
	if (holds_alternative<double>(value)) {
		return -(get<double>(value));
	} else // mismatched type
		err->runErrorMUT();
	return JType{};
}
// ! : works on numbers and bools
bool JType::operator!() {
	if (isnothing())
		return true;
	else if (holds_alternative<string>(value))
		return (get<string>(value).empty());
	else if (holds_alternative<double>(value))
		return !(get<double>(value));
	else if (holds_alternative<bool>(value))
		return !(get<bool>(value));
	else
		return false;
}
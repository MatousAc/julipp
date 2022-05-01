#include <math.h>
#include "LoxType.h"
#include "../tools/LoxError.h"
#include "../tools/helpers.h"

// various constructors
LoxType::LoxType() {}
LoxType::LoxType(string str)
	: value{ str } {}
LoxType::LoxType(double dbl)
	: value{ dbl } {}
LoxType::LoxType(bool bl)
	: value{ bl } {}
LoxType::LoxType(LoxCallable* callable)
	: value{ callable } {}


// czechs if empty (contains NULL)
bool LoxType::isnil() const {
	return holds_alternative<monostate>(value);
}

bool LoxType::isTruthy() const {
	if (isnil())
		return false;
	else if (holds_alternative<bool>(value))
		return get<bool>(value);
	else
		return true;
}

bool LoxType::isInt() const {
	double dbl = get<double>(value);
	int integer = (int)dbl;
	double diff = (dbl - integer);
	if (diff < 0) diff *= -1;
	return  diff < DOUBLE_PRECISION;
}
// czechs type
string LoxType::type() const {
	string res = "unknown type";
	if (isnil())
		res = "nil";
	else if (holds_alternative<string>(value))
		res = "string";
	else if (holds_alternative<double>(value)) {
		res = "double";
	}
	else if (holds_alternative<bool>(value)) {
		res = "bool";
	}
	else if (holds_alternative<LoxCallable*>(value)) {
		res = "LoxCallable*";
	}
	else if (holds_alternative<monostate>(value)) {
		res = "nil";
	}
	return res;
}

string LoxType::toString() const {
	string res;
	if (isnil())
		res = "nil";
	else if (holds_alternative<string>(value))
		res = get<string>(value);
	else if (holds_alternative<double>(value))
		res = this->numToLoxStr();
	else if (holds_alternative<bool>(value))
		res = (get<bool>(value)) ? "true" : "false";
	else if (holds_alternative<LoxCallable*>(value))
		res = "function() . . .";
	else
		res = "unknown type";

	return res;
}
// helper
string LoxType::numToLoxStr() const {
	double dbl = get<double>(value);
	string res;
	if (this->isInt())
		return to_string((int)dbl);
	return to_string(dbl);
}


///// ~~ operator overloads ~~ /////
// logical
// == : equals (different types cannpt be equal)
bool LoxType::operator==(const LoxType& r) {
	if (isnil() && r.isnil())
		return true;
	else if (isnil() || r.isnil())
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
bool LoxType::operator!=(const LoxType& r) {
	return !((*this) == r);
}
// > : numerical && textual greater than
bool LoxType::operator>(const LoxType& r) {
	if (isnil() || r.isnil())
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
bool LoxType::operator>=(const LoxType& r) {
	return *this > r || *this == r;
}
// < : num && text less than
bool LoxType::operator<(const LoxType& r) {
	if (isnil() || r.isnil())
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
bool LoxType::operator<=(const LoxType& r) {
	return *this < r || *this == r;
}

// arithmetic
// + : addition, concatenation
LoxType LoxType::operator+(const LoxType& r) {
	if (isnil()) // if this is empty
		return r;
	else if (r.isnil())
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
	return LoxType{};
}
// - : subtraction
LoxType LoxType::operator-(const LoxType& r) {
	if (isnil()) // if this is empty
		return (-LoxType{ r });
	else if (r.isnil())
		return this;
	// double - double
	else if (holds_alternative<double>(value) &&
		holds_alternative<double>(r.value))
		return get<double>(value) - get<double>(r.value);
	else // mismatched types
		err->runErrorMBT();
	return LoxType{};
}
// % : modulus
LoxType LoxType::operator%(const LoxType& r) {
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
LoxType LoxType::operator*(const LoxType& r) {
	if (isnil() || r.isnil()) // x * 0 = 0
		return LoxType{};
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
	return LoxType{};
}
// / : division and line breaks
LoxType LoxType::operator/(const LoxType& r) {
	if (isnil()) // if this is empty
		return LoxType{};
	else if (r.isnil())
		throw RunError("division by nil");
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
	return LoxType{};
}
// ^ : exponentiation
LoxType LoxType::operator^(const LoxType& r) {
	if (!holds_alternative<double>(value) ||
		!holds_alternative<double>(r.value))
		// no support for other types
		err->runErrorMBT();
	return pow(get<double>(value), get<double>(r.value));
}

// unary
// - : negates numbers
LoxType LoxType::operator-() {
	if (holds_alternative<double>(value)) {
		return -(get<double>(value));
	} else // mismatched type
		err->runErrorMUT();
	return LoxType{};
}
// ! : works on numbers and bools
bool LoxType::operator!() {
	if (isnil())
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
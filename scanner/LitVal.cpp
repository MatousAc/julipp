#include "LitVal.h"

LitVal::LitVal(string str)
	: value{ str } {}
LitVal::LitVal(double dbl)
	: value{ dbl } {}

bool LitVal::empty() const {
	if (holds_alternative<string>(value) ||
		(holds_alternative<double>(value)))
		return false;
	else
		return true;
}

string LitVal::toString() const {
	if (holds_alternative<string>(value))
		return get<string>(value);
	else if (holds_alternative<double>(value)) {
		string res = this->numToString();
		return res.substr(0, res.length() - 5);
	} else
		return "unrecognized type";
}

string LitVal::numToString() const {
	double dbl = get<double>(value);
	int integer = (int)dbl;
	string res;
	if ((dbl - integer) > DOUBLE_PRECISION)
		res = to_string(dbl);
	res = to_string(integer);
	return res;
}

LoxType LitVal::retrieve() const {
	if (holds_alternative<string>(value))
		return LoxType{ get<string>(value) };
	else if (holds_alternative<double>(value))
		return LoxType{ get<double>(value) };
	else
		return LoxType{};
}
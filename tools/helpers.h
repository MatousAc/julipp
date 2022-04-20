#pragma once
#include "../include.h"

void trim(string& str);
vector<string> split(string str, string token);
string toLower(string str);
string repeat(string str, const size_t n);
template<typename Base, typename T>

// helpers
// instance of for C++ from panzi on SO
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}
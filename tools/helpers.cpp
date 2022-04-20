#include "helpers.h"


// adapted from DelftStack:
void trim(string& str) {
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
}

// adapted from Shiqi Ai on SO:
vector<string> split(string str, string token) {
	vector<string> result;
	while (str.size()) {
		auto index = str.find(token);
		string substr;
		if ((substr = str.substr(0, index)) == "") {
			str = str.substr(index + token.size());
		} else if (index != string::npos) {
			result.push_back(substr);
			str = str.substr(index + token.size());
		} else {
			result.push_back(str);
			str = "";
		}
	}
	return result;
}

string toLower(string str) {
	for (int i = 0; i < str.length(); i++) {
		char c = str[i];
		if (c <= 'Z' && c >= 'A')
			str[i] -= ('Z' - 'z');
	}
	return str;
}

// from Daniel on SO
// repeats a string a number of times
string repeat(string str, const size_t n) {
	if (n == 0) {
		str.clear();
		str.shrink_to_fit();
		return str;
	} else if (n == 1 || str.empty()) {
		return str;
	}
	const auto period = str.size();
	if (period == 1) {
		str.append(n - 1, str.front());
		return str;
	}
	str.reserve(period * n);
	size_t m{ 2 };
	for (; m < n; m *= 2) str += str;
	str.append(str.c_str(), (n - (m / 2)) * period);
	return str;
}


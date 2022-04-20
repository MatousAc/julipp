#include "generator.h"
#include "helpers.h" // for trim|split helpers
#include "tools.h" // for temp vector print

void generator(vector<string> args) {
	auto argc = args.size();
	string outputDir;
	if (argc < 4) {
		printf("Usage: ... -t generator <output directory>\n");
		return;
	} else {
		outputDir = args[3];
	}
	cout << "generating in: " << outputDir << std::endl;
	writeSyntax(outputDir, "Expr",
		{ "../scanner/Token.h" }, {
		"Assign : Token name, Expr* value",
			"Binary : Expr* left, Token op, Expr* right",
			"Call : Expr* callee, Token paren, vector<Expr*> arguments",
			"Grouping : Expr* expression",
			"Literal : LoxType value",
			"Logical : Expr* left, Token op, Expr* right",
			"Ternary : Expr* condition, Expr* ifTrue, Expr* ifFalse",
			"Unary : Token op, Expr* right",
			"Variable : Token name"
		});
	writeSyntax(outputDir, "Stmt",
		{ "Expr.hpp" }, {
		"Block : vector<Stmt*> statements",
			"Break",
			"Continue",
			"Exit",
			"Expression	: Expr* expression",
			"For : Stmt* initializer, Expr* condition, Expr* increment, Stmt* body",
			"Function : Token name, vector<Token> params, vector<Stmt*> body",
			"If : Expr* condition, Stmt* thenBranch, Stmt* elseBranch",
			"Print : Expr* expression",
			"Return : Token keyword, Expr* value",
			"Var : Token name, Expr* initializer",
			"While : Expr* condition, Stmt* body"
		});
}

void writeSyntax(string outputDir, string base,
	vector<string> include, vector<string> types) {
	string fp = outputDir + "/" + base + ".hpp";
	const char* filepath = fp.c_str();
	FILE* file; // safely open a file
	errno_t err;
	if (((err = fopen_s(&file, filepath, "w")) != 0) ||
		(file == nullptr)) {
		cout << filepath << " could not be opened: " << file;
		return;
	}

	// struct names
	vector<string> names(types);
	auto name = names.begin();
	auto back = names.end();
	while (name != back) {
		size_t pos = name->find(':');
		if (pos == std::string::npos) {
			name++;
			continue;
		}
		*name = name->substr(0, pos);
		trim(*name);
		name++;
	}

	// header
	string hpp;
	hpp = "#pragma once\n";
	auto curIncl = include.begin();
	auto lastIncl = include.end();
	while (curIncl != lastIncl) {
		*curIncl = curIncl->substr(0, curIncl->find(':'));
		trim(*curIncl);
		hpp += "#include \"" + *curIncl + "\"\n\n";
		curIncl++;
	}

	// prototype structs
	hpp += buildProtoStructs(names);
	// visitor struct
	hpp += buildVisitorStruct(base, names);
	// expression struct
	hpp += buildBase(base);
	// derived structs
	auto type = types.begin();
	auto end = types.end();
	name = names.begin();
	while (type != end) {
		string contents = "";
		auto pos = type->find(':');
		if (pos != std::string::npos)
			// if not dummy struct
			contents = type->substr(++pos);
		hpp += buildStruct(base, *name, split(contents, ","));
		type++;
		name++;
	}

	fprintf(file, hpp.c_str());
	fclose(file);
}

string buildProtoStructs(vector<string> names) {
	string code = "";
	auto name = names.begin();
	auto back = names.end();
	while (name != back) {
		code += "struct " + *name + ";\n";
		name++;
	}
	code += "\n";
	return code;
}

string buildVisitorStruct(string base, vector<string> names) {
	string code = "struct " + base + "Visitor {\n";
	if (names.empty()) return code += "}\n";
	auto name = names.begin();
	auto end = names.end();
	while (name != end) {
		code += TAB + "virtual void visit" + *name + "(const " +
			*name + "* " + toLower(base) + ") = 0;\n";
		name++;
	}
	code += "};\n\n";
	return code;
}

string buildBase(string base) {
	string code = "struct " + base + " {\n";
	code += TAB + "virtual ~" + base + "() = default;\n";
	code += TAB + "virtual void accept(" + base +
		"Visitor* visitor) = 0;\n";
	code += "};\n\n";
	return code;
}

string buildStruct(string base, string name, vector<string> fields) {
	// string splitting
	vector<vector<string>> members{};
	auto field = fields.begin();
	auto back = fields.end();
	while (field != back)
		members.push_back({ split(*(field++), " ") });
	trim(name);

	// putting the struct together
	string code = "struct " + name + " : " + base + " {\n";
	if (fields.empty()) { // handle dummy statements
		code += TAB + name + "() {}\n\n";
		code += TAB + "void accept(" + base + "Visitor* visitor) override {\n";
		code += TABx2 + "visitor->visit" + name + "(this);\n";
		code += TAB + "}\n};\n\n";
		return code;
	}

	if (members.empty()) return "";
	auto member = members.begin();
	auto end = members.end();
	// declerations
	while (member != end) {
		code += TAB + (*member)[0] + " " +
			(*member)[1] + ";\n";
		member++;
	}

	// constructor
	code += "\n" + TAB + name + "(";
	member = members.begin();
	while (member != end) {
		if (member != members.begin()) code += ", ";
		code += (*member)[0] + " " + (*member)[1];
		member++;
	}
	code += ")\n" + TABx2 + ":";
	member = members.begin();
	while (member != end) {
		if (member != members.begin()) code += ", ";
		code += (*member)[1] + "{ " + (*member)[1] + " }";
		member++;
	}
	code += " {}\n\n";
	// accept
	code += TAB + "void accept(" + base +
		"Visitor* visitor) override {\n";
	code += TABx2 + "visitor->visit" + name + "(this);\n";
	code += TAB + "}\n";

	//end
	code += "};\n\n";
	return code;
}

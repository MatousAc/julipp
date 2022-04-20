#include "julipp.h"
#include "tools/tools.h"
#include "tools/LoxError.h"
#include "scanner/Token.h"
#include "scanner/Scanner.h"
#include "tools/AstPrinter.h"
#include "interpreter/Interpreter.h"

int main(int argc, char* argv[]) {
	// running tools
	vector<string> args{};
	for (int i{ 0 }; i < argc; i++) {
		args.push_back(argv[i]);
	};

	if ((argc > 1) and args[1] == "-t") {
		runTool(args);
		return 0;
	};

	if (argc <= 1)
		runPrompt();
	else if (argc > 2)
		printf("invalid # of args\n");
	else
		runFile(argv[1]);
	return 0;
}

void runPrompt() {
	string line;
	while (true) {
		cout << "> ";
		getline(cin, line, '\n');
		if (line == "") break;
		// czech if just expression
		exprToPrint(line);

		int code = run(line);
		switch (code) {
		case 65:
			// keep going
			err->hadError = false;
			break;
		case 70:
			exit(70);
			break;
		default:
			break;
		}
	}
}

void runFile(char* filepath) {
	struct stat sb {};
	string program;
	FILE* file; // safely open a file
	errno_t error;
	if (((error = fopen_s(&file, filepath, "r")) != 0) ||
		(file == nullptr)) {
		cout << filepath << " could not be opened: " << file;
		return;
	}
	stat(filepath, &sb);
	program.resize(sb.st_size);
	fread(const_cast<char*>(program.data()), sb.st_size, 1, file);
	fclose(file);

	// struct stat sb {};
	// string program;
	// FILE *file;
	// file = fopen(filepath,"r");
	// fread(const_cast<char*>(program.data()), sb.st_size, 1, file);
	// fclose(file);

	// run and czech for errors
	int code = run(program);
	switch (code) {
	case 65:
		exit(65);
		break;
	case 70:
		exit(70);
		break;
	default:
		break;
	}
	if (err->hadError) return;
}

int run(string& source) {
	Scanner scanner(source); // scan into tokens
	vector<Token> tokens = scanner.scanTokens();
	//printTokens(tokens);
	Parser parser{ tokens }; // parse into AST
	vector<Stmt*> statements = parser.parse();

	// return codes for handling based on REPL/file exe
	if (err->hadError) return 65;
	if (err->hadRunError) return 70;

	// interpret code
	interpreter->interpret(statements);
	return 0;
}

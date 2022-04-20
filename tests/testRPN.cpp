#include "testRPN.h"

void testRPN(vector<string> args) {
	bool verbose = false;
	auto argc = args.size();
	if (argc > 3 && args[3] == "-verbose")
		verbose = true;
	vector<string> input = {
		"1 + 2",
		"(4 * \"hi\") == 12",
		"3 / 5 + 2 * (-2 - -5)",
		"\"two\" * (4 / 2)",
		"3 / 2 + 1 + -12",
		"true == false * 3",
		"2 - 3 + 4 / 3 * 8 + (3 + 4)",
		"4",
		"-34",
		"29 - 12 == 2 != 7 < 6",
		"4 + (1 - 2) != 24 > 3 - 4",
		"(\"sup\" + \" \") * 12",
		"9 / 5 == 3 * 7 / (9 - -2) != 7"
	};
	vector<string> expected = {
		"1 2 +",
		"4 hi * 12 ==",
		"3 5 / 2 2 - 5 - - * +",
		"two 4 2 / *",
		"3 2 / 1 + 12 - +",
		"true false 3 * ==",
		"2 3 - 4 3 / 8 * + 3 4 + +",
		"4",
		"34 -",
		"29 12 - 2 == 7 6 < !=",
		"4 1 2 - + 24 3 4 - > !=",
		"sup   + 12 *",
		"9 5 / 3 7 * 9 2 - - / == 7 !="
	};
	string actual;
	for (int i = 0; i < input.size(); i++) {
		actual = runLineRPN(input[i]);
		if (verbose) {
			cout << input[i] << "\nshould be: "
				<< expected[i] << "\nand is: "
				<< actual << endl;
			cout << ((actual != expected[i]) ? "FAIL\n" : "PASS\n");
		} else if (actual != expected[i])
			cout << actual << "\nshould be: "
			<< expected[i] << "\nFAIL";
	}
}

string runLineRPN(string& source) {
	Scanner scanner(source);
	vector<Token> tokens = scanner.scanTokens();
	Parser parser{ tokens };
	//Expr* expression = parser.parse();

	// reading
	/*RPNPrinter printer;
	printer.read(expression);
	return printer.getResult();*/
	return "";
}

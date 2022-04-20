#include "testStrComp.h"


void testStrComp() {
	vector<string> input = {
		"\"cat\" < \"dog\"",
		"\"canary\" < \"cat\"",
		"\"food\" > \"lettuce\"",
		"\"cake\" <= \"cake\"",
		"\"sweet\" != \"sour\"",
		"\"crate\" < \"great\"",
		"\"grate\" >= \"great\"",
		"\"gift\" > \"lift\"",
		"\"lift\" > \"gft\"",
		"\"tired\" > \"tired\"",
		"\"tired\" == \"tired\"",
		"\"cat\" != \"cat\"",
		"\"zoo\" > \"loneliness\"",
		"\"exhaustion\" >= \"exhaust\"",
		"\"sword\" > \"sword drill\"",
		"\"sleep\" > \"secret\"",
		"\"\" < \"kitten\"",
		"\"kite\" < \"fight\"",
		"\"!1\" < \"82\"",
		"\"lazy\" >= \"laziness\"",
		"\"junk\" == \"sleep\"",
		"\"dreams\" <= \"sweet\""
	};
	vector<string> expected = {
		"true",
		"true",
		"false",
		"true",
		"true",
		"true",
		"false",
		"false",
		"true",
		"false",
		"true",
		"false",
		"true",
		"true",
		"false",
		"true",
		"true",
		"false",
		"true",
		"true",
		"false",
		"true"
	};
	string actual;
	for (int i = 0; i < input.size(); i++) {
		actual = interpretLineRPN(input[i]);
		if (actual != expected[i])
			cout << "_" << input[i] << "_"
			<< " ~should be~ "
			<< "_" << expected[i] << "_" << endl;
	}

}

string interpretLineRPN(string& source) {
	Scanner scanner(source);
	vector<Token> tokens = scanner.scanTokens();
	Parser parser{ tokens };
	vector<Stmt*> statements = parser.parse();

	// interpreting
	Interpreter* interpreter = new Interpreter;
	interpreter->interpret(statements);
	return interpreter->getResult().toString();
}
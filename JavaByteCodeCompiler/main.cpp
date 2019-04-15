

#include "Parser.h"
#include "Interpreter.h"

int main()
{
	printf("\n*****TEST1*****\n");
	Parser parser("inputs/Test1.class");
	parser.parse();
	parser.print();

	Interpreter interpreter(parser.constPool, parser.methods);
	interpreter.interpret();

	/*printf("\n*****TEST2*****\n");
	Parser parser2("inputs/Test2.class");
	parser2.parse();
	parser2.print();

	printf("\n*****TEST3*****\n");
	Parser parser3("inputs/Test3.class");
	parser3.parse();
	parser3.print();

	printf("\n*****TEST4*****\n");
	Parser parser4("inputs/Test4.class");
	parser4.parse();
	parser4.print();

	printf("\n*****TEST5*****\n");
	Parser parser5("inputs/Test5.class");
	parser5.parse();
	parser5.print();*/
}
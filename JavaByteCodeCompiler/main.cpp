

#include "Parser.h"
#include "Interpreter.h"

int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		//if (std::string(*(argv + i)) == "custom_inputs/Test2") {
			printf("\n*****%s*****\n", *(argv + i));
			Parser parser(std::string(*(argv + i)) + ".class");
			parser.parse();
			printf("Interpret\n");
			Interpreter interpreter(parser.constPool, parser.methods);
			interpreter.interpret();
		//}
	}
}
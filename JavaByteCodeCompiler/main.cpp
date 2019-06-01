

#include "Parser.h"
#include "Interpreter.h"
#include "SSAgenerator.h"

int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		if (std::string(*(argv + i)) == "inputs/Test1") {
			printf("\n*****%s*****\n", *(argv + i));
			Parser parser(std::string(*(argv + i)) + ".class");
			parser.parse();
			//printf("Interpret\n");
			//Interpreter interpreter(parser.constPool, parser.methods);
			//interpreter.interpret();
			printf("SSA\n");
			SSAgenerator ssa = SSAgenerator(parser.constPool, parser.methods);
			ssa.generate();
		}
	}
}
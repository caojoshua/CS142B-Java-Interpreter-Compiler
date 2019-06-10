

#include "Parser.h"
#include "Interpreter.h"
#include "SSAgenerator.h"
#include "SSAoutput.h"
#include "RegAllocator.h"
#include "RegMap.h"
#include "x86executor.h"

int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		//if (std::string(*(argv + i)) == "inputs/Test3") {
			try
			{
				//redirect output to an output file
				FILE* fp;
				std::string out = std::string(*(argv + i));
				//replace '/' with '_'
				size_t pos = out.find("/");
				while (pos != std::string::npos)
				{
					out.replace(pos, 1, "_");
					pos = out.find("/");
				}
				out = "outputs/" + out + ".txt";
				if((fp=freopen(out.c_str(), "w" ,stdout))==NULL) {
					printf("Cannot open file.\n");
					exit(1);
				}

				//interpret and compile
				//printf("\n*****%s*****\n", *(argv + i));
				Parser parser(std::string(*(argv + i)) + ".class");
				parser.parse();

				printf("**********Interpret**********\n");
				Interpreter interpreter(parser.constPool, parser.methods);
				interpreter.interpret();

				printf("\n**********SSA**********\n");
				SSAgenerator ssa = SSAgenerator(parser.constPool, parser.methods);
				SSAoutput SSAout = ssa.generate();

				printf("\n**********Register Allocation**********\n");
				RegAllocator alloc = RegAllocator(SSAout);
				RegMap map;
				alloc.regAlloc(map);

				printf("\n**********x86 execution**********\n");
				x86executor x86(SSAout, map);
				x86.execute();
			}
			catch (...)
			{
				printf("An exception occurred.\n");
			}
		//}
	}
}
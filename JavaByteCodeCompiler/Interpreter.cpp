#include "Interpreter.h"

//PRIVATE



//PUBLIC

Interpreter::~Interpreter()
{
}

void Interpreter::interpret()
{
	for (Method m : methods)
	{
		printf("%s\n", m.name.c_str());
		for (uint8_t i : m.code.byteCodes)
		{
			printf("\t%X\n", i);
		}
		for (uint8_t byteCode : m.code.byteCodes)
		for (int i = 0; i < m.code.byteCodes.size(); ++i)
		{
			uint8_t byteCode = m.code.byteCodes[i];
			switch (byteCode)
			{
				case iload:
				{
					printf("iload: \n");
					break;
				}
				case istore:
				{
					printf("istore\n");
					break;
				}
				case iadd:
				{
					printf("iadd\n");
					break;
				}
				default:
				{
				
				}
			}
		}
	}
}


#ifndef X86_H
#define X86_H

#include "RegMap.h"
#include "SSAoutput.h"
#include "JITmethod.h"
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

void printTest();
void printInt(int i);

class x86executor
{
private:
	HANDLE hHeap;
	uint8_t* JITcode;
	SSAoutput SSAout;
	//a list of pairs where pair.first is name of method, and pair.second is a pointer to location of function
	std::list<JITmethod> JITmethods;

	void genX86(SSAmethod& m);

	//emit functions
	void emit(uint8_t* heapPtr, int& count, uint8_t byte);
	void emit16(uint8_t* heapPtr, int& count, uint16_t byte);
	void emit32(uint8_t* heapPtr, int& count, uint32_t byte);
	void emit64(uint8_t* heapPtr, int& count, uint64_t byte);
public:
	x86executor(SSAoutput out);
	~x86executor();
	void execute();
};

#endif

#ifndef MACHINE_H
#define MACHINE_H

#include "X86constants.h"
#include "RegMap.h"
#include "SSAoutput.h"
#include "JITmethod.h"
#include "SSAInstruction.h"

//int REX_WW = 0x48;
//int IDKWHATTHEFUCKISGOINGON;

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

	//creates JIT methods and adds them to JITmethods
	void createJITmethods();
	//creates a JITmethod associated with m, inserts instructions into it, and adds it to JITmethods
	void genX86(JITmethod& jit, SSAmethod& m);

	//helpers called from switch statement, all inserts instructions into jit
	void mov(JITmethod& jit, RegMap& map, SSA::OperandUse dest, SSA::OperandUse src);
	void ret(JITmethod& jit);

	//further helpers to insert specific instructions that are seen commonly e.g. mov
	//arguments are registers
	void mov_rrm(JITmethod& jit, uint8_t dest, uint8_t src);
	void pop(JITmethod& jit, uint8_t reg);
	void push(JITmethod& jit, uint8_t reg);

	//other helpers
	//return jit method with name
	JITmethod& getJITmethods(std::string name);
	//return ModR/M byte, will shift over bits to right bit locations
	//mod contains two bytes, regOpcode three, rm three
	uint8_t modrm(uint8_t mod, uint8_t regOpcode, uint8_t rm);
	//sets prologue in jit
	void prologue(JITmethod& jit);

public:
	x86executor(SSAoutput out);
	~x86executor();
	void execute();
};

#endif
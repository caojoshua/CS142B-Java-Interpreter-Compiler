
#ifndef MACHINE_H
#define MACHINE_H

#include "X86constants.h"
#include "RegMap.h"
#include "SSAoutput.h"
#include "JITmethod.h"
#include "SSAInstruction.h"


void printTest();
void Println(int i);

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
	//special handling to phi, remove phi functions and insert movs 
	//this part moved to BEFORE register allocation
	void handlePhi();
	//creates a JITmethod associated with m, inserts instructions into it, and adds it to JITmethods
	void genX86(JITmethod& jit, SSAmethod& m);

	//helpers called from switch statement, all inserts instructions into jit
	void iBinary(JITmethod& jit, RegMap& map, SSA::Instruction* ins);
	void iUnary(JITmethod& jit, RegMap& map, SSA::Instruction* ins);
	void call(JITmethod& jit, RegMap& map, SSA::Instruction* ins);
	void cmp(JITmethod& jit, RegMap& map, SSA::Instruction* ins);
	void jcc(JITmethod& jit, SSA::Instruction* ins);
	void jmp(JITmethod& jit, SSA::Instruction* ins);
	void mov(JITmethod& jit, RegMap& map, SSA::Instruction* ins);
	void ret(JITmethod& jit);

	//further helpers to insert specific instructions that are seen commonly e.g. mov
	//arguments are registers
	//binary op r to rm
	void iBinary_rmr(JITmethod& jit, SSAopcode ssa_opcode,uint8_t dest, uint8_t src1, uint8_t src2);
	//binary op imm to rm
	void iBinary_rimm(JITmethod& jit, SSAopcode ssa_opcode, uint8_t dest, uint8_t src1, uint32_t constValue);
	//compare r against const, we can ONLY cmp against int of size 1 byte
	void cmp_rimm(JITmethod& jit, uint8_t r, int8_t constValue);
	//mov r to rm
	void mov_rmr(JITmethod& jit, uint8_t dest, uint8_t src);
	//mov imm to r
	void mov_rimm(JITmethod& jit, uint8_t dest, uint32_t constValue);
	void pop(JITmethod& jit, uint8_t reg);
	void push(JITmethod& jit, uint8_t reg);
	void push(JITmethod& jit, RegMap& map, SSA::OperandUse op);

	//other helpers
	//return jit method with name
	JITmethod& getJITmethod(std::string name);
	//return pointer to method with given name
	fptr getFuncPtr(std::string name);
	//return ModR/M byte, will shift over bits to right bit locations
	//mod contains two bytes, regOpcode three, rm three
	uint8_t modrm(uint8_t mod, uint8_t regOpcode, uint8_t rm);
	//sets prologue in jit
	void prologue(JITmethod& jit, RegMap& map);

public:
	x86executor(SSAoutput out);
	~x86executor();
	void execute();
};

#endif
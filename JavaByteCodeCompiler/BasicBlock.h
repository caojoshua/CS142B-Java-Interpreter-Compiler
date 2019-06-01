
#ifndef BASICBLOCK_H
#define BASICBLOCK_H

#include "SSAInstruction.h"
#include <vector>
#include <list>
#include <string>

class BasicBlock
{
private:
	int index;
	int startLine;
	int endLine;
	int inStackDepth;
	std::list<int> pred;
	std::list<int> succ;
	std::vector<SSA::Instruction*> instructions;

public:
	BasicBlock(int index, int startLine, int endLine) : index(index), startLine(startLine), endLine(endLine) {}
	~BasicBlock();
	int getIndex();
	int getStartLine();
	int getEndLine();
	int getInStackDepth();
	std::list<int> getPred();
	std::list<int> getSucc();
	std::vector<SSA::Instruction*> getInstructions();
	void addInstruction(SSA::Instruction* i);
	void addPhi(SSA::Operand);
	void addPred(int i);
	void addSucc(int i);
	void setInStackDepth(int liveInStackDepth);
	bool operator==(BasicBlock& other);
};

#endif


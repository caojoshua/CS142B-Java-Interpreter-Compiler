
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
	int SSAstartLine;
	int SSAendLine;
	int inStackDepth;
	std::list<int> pred;
	std::list<int> succ;
	std::list<SSA::OperandUse> liveIn;
	std::vector<SSA::Instruction*> instructions;

public:
	BasicBlock(int index, int startLine, int endLine) : index(index), startLine(startLine), endLine(endLine) {}
	~BasicBlock();
	bool operator==(BasicBlock& other);
	int getIndex();
	int getStartLine();
	int getEndLine();
	int getSSAstart();
	int getSSAend();
	int getInStackDepth();
	std::list<int> getPred();
	std::list<int> getSucc();
	std::vector<SSA::Instruction*> getInstructions();
	void addInstruction(SSA::Instruction* i);
	void addPhi(SSA::Operand op);
	void addPhi(SSA::PhiInstruction* phi);
	//adds phi src with basicblock index bb to phi with destination dest
	void updatePhiSrc(SSA::Operand dest, int bb, SSA::OperandUse src);
	//stores list of phi sources with destination dest and basic block index bb into srcs
	void renamePhiSrc(SSA::Operand dest, int bb, SSA::OperandUse src);
	void addPred(int i);
	void addSucc(int i);
	void setInStackDepth(int liveInStackDepth);
	void setSSAstart(int i);
	void setSSAend(int i);
	

	//live interval functions
	std::list<SSA::OperandUse> getLiveIn();
	void addLiveIn(SSA::OperandUse op);

};

#endif


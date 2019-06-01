
#ifndef SSAGENERATOR_H
#define SSAGENERATOR_H

#include "Constants.h"
#include "ConstantPoolEntry.h"
#include "Method.h"
#include "BasicBlock.h"
#include <vector>
#include <list>
#include <iterator>

class SSAgenerator
{
private:
	std::vector<ConstPoolEntry*> constPool;
	std::vector<Method> methods;

	//creates basic blocks for method m
	void createBasicBlocks(Method& m);
	//gets all the leaders INCLUDING the final line + 1, its necessary to find the ending line of the last block
	std::list<int> getLeaders(Method& m);
	//generates basic block code given m and leaders
	std::vector<BasicBlock> genBasicBlockCode(Method& m, std::list<int> leaders);
	//insert phi functions into BBs, which is passed by reference
	void insertPhi(std::vector<BasicBlock>& BBs);

	void iconst(BasicBlock& b, int stackNum, int val);
	void iload(BasicBlock& b, int stackNum, int localNum);
	void istore(BasicBlock& b, int stackNum, int localNum);
	void unary(BasicBlock& b, SSA::UnaryInstruction::Opcode opcode, int dest, int src);
	void binary(BasicBlock& b, SSA::BinaryInstruction::Opcode opcode, int dest, int src);
	//compare src1 against src2
	void cond(std::vector<BasicBlock>& BBs, BasicBlock& b, SSA::CondBranchInstruction::Opcode opcode, int tmp, int src1, int src2, int BBindex1, int BBindex2);
	//when src 2 is not given for cond, compare src1 against 0
	void cond(std::vector<BasicBlock>& BBs, BasicBlock& b, SSA::CondBranchInstruction::Opcode opcode, int tmp, int src1, int BBindex1, int BBindex2);
	//returns stack offset determined by number of parameters and return type of method
	int call(BasicBlock& b, int constPoolIndex);

	//helpers
	Method getMethod(std::string name);
	int getBBindex(std::vector<BasicBlock> BBs, int line);


	//phi function insertion helpers

	//returns whether X strictly dominates Y, assumes BBs are already linked
	bool dominates(std::vector<BasicBlock>& BBs, BasicBlock& X, BasicBlock& Y);
	//recursive helper
	bool dominates(std::vector<BasicBlock>& BBs, std::list<int>& visited, BasicBlock& entry, BasicBlock& X, BasicBlock& Y);

	//adds indices that represent the dominance frontier of d to df
	void dominanceFrontier(std::list<int>& df, std::vector<BasicBlock>& BBs, BasicBlock& d);
	//dominance frontier helpers
	//returns true if n is in DF(d)
	bool inDominanceFrontier(std::vector<BasicBlock>& BBs, BasicBlock& d, BasicBlock& n);
	//returns true if d dominates n or any of its predecessors
	bool dominatePred(std::vector<BasicBlock>& BBs, std::list<int> visited, BasicBlock& d, BasicBlock& n);
	//appends variables in BBs that have assigments to it to vars
	void getVars(std::list<SSA::Operand>& vars, std::vector<BasicBlock>& BBs);
	//returns true if BB contains an assignment to o
	bool containsAssignment(BasicBlock& BB, SSA::Operand o);
public:
	SSAgenerator(std::vector<ConstPoolEntry*> constPool, std::vector<Method> methods) : constPool(constPool), methods(methods) {}
	~SSAgenerator();
	//call this to generate SSA
	void generate();
};


#endif


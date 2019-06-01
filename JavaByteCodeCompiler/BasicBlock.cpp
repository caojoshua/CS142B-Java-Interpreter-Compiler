#include "BasicBlock.h"


//probably should create operator= and deep copy instructions
BasicBlock::~BasicBlock()
{
	/*for(int i = 0; i < instructions.size(); ++i)
	{
		delete instructions[i];
	}*/
}

int BasicBlock::getIndex()
{
	return index;
}

int BasicBlock::getStartLine()
{
	return startLine;
}

int BasicBlock::getEndLine()
{
	return endLine;
}

int BasicBlock::getInStackDepth()
{
	return inStackDepth;
}

std::list<int> BasicBlock::getPred()
{
	return pred;
}

std::list<int> BasicBlock::getSucc()
{
	return succ;
}

std::vector<SSA::Instruction*> BasicBlock::getInstructions()
{
	return instructions;
}

void BasicBlock::addInstruction(SSA::Instruction * i)
{
	instructions.push_back(i);
}

void BasicBlock::addPhi(SSA::Operand op)
{
	instructions.insert(instructions.cbegin(), new SSA::PhiInstruction(op));
}

void BasicBlock::addPred(int i)
{
	if(std::find(std::cbegin(pred), std::cend(pred), i) == std::cend(pred))
		pred.push_back(i);
}

void BasicBlock::addSucc(int i)
{
	if(std::find(std::cbegin(succ), std::cend(succ), i) == std::cend(succ))
		succ.push_back(i);
}

void BasicBlock::setInStackDepth(int liveInStackDepth)
{
	this->inStackDepth = liveInStackDepth;
}

bool BasicBlock::operator==(BasicBlock & other)
{
	return index == other.index;
}

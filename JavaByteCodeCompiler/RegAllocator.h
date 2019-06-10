
#ifndef REGALLOC_H
#define REGALLOC_H

#include "SSAoutput.h"
#include "SSAInstruction.h"
#include "Interval.h"
#include "RegMap.h"
#include <list>

class RegAllocator
{
private:
	SSAoutput ssa;
	void computeLiveIntervals(IntervalList& intervals, std::vector<BasicBlock>& bbs);
	//Chaitin's algorithm to color nodes/assign minimal registers
	//stores SSA operand to register map inside map
	void chaitin(RegMap& map, InterferenceGraph IGorig);

	//returns the index of the loop ending basic block, -1 if not a loop header
	int loopHeader(std::vector<BasicBlock>& bbs, int b);
	//recursive helper for isLoopHeader
	int isLoopHeaderR(std::vector<BasicBlock>& bbs, std::list<int>& visited, int src, int b, int prev);
public:
	RegAllocator(SSAoutput ssa) : ssa(ssa) {}
	~RegAllocator();
	void regAlloc(RegMap map);
};

#endif
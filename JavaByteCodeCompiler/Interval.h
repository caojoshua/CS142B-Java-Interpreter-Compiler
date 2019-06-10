
#ifndef INTERVAL_H
#define INTERVAL_H

#include "SSAInstruction.h"
#include "InterferenceGraph.h"
#include <list>

//represents the ranges of liveness for op
class Interval
{
private:
	SSA::OperandUse op;
	//(from, to) pairs where ints are line ids
	//i think in SSA have a list isn't necessary because when vars have uses, they shouldn't have more than one range
	std::list<std::pair<int, int>> ranges;
public:
	Interval(SSA::OperandUse op) : op(op) { }
	~Interval();
	void operator+=(Interval& other);
	void addRange(int from, int to);
	void setFrom(int from);
	SSA::OperandUse getOp();
	std::list<std::pair<int, int>> getRanges();
	std::string getStr() const;
	//return true if other conflicts with this
	bool interferes(const Interval& other) const;
};


class IntervalList
{
private:
	std::vector<Interval> intervals;
public:
	IntervalList() {}
	void operator+=(IntervalList& other);
	void addRange(SSA::OperandUse op, int from, int to);
	//include "to" in case interval with operand doesn't exist yet
	void setFrom(SSA::OperandUse op, int from, int to);
	std::string getStr() const;
	//constructs inteference graph in reference to IG, assumes IG is empty
	void constructInterferenceGraph(InterferenceGraph& IG);
};


#endif

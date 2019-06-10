#include "Interval.h"



Interval::~Interval()
{
}

void Interval::operator+=(Interval & other)
{
	if (!(op == other.op))
	{
		return;
	}
	for(std::pair<int,int> pair : other.ranges)
	{
		ranges.push_back(std::pair<int, int>(pair.first, pair.second));
	}
}

void Interval::addRange(int from, int to)
{
	//create new range if ranges is empty
	if(ranges.empty())
	{
		ranges.push_front(std::pair<int, int>(from, to));
		return;
	}
	//front returns a reference
	std::pair<int, int> range = ranges.front();
	if(from < range.first)
	{
		range.first = from;
	}
	if(to > range.second)
	{
		range.second = to;
	}	
}

void Interval::setFrom(int from)
{
	//change from of first pair to given from, the first pair is always most recently added pair
	//ranges should always not be empty, but we check anyway
	if (!ranges.empty()) 
	{
		//front() returns a reference
		ranges.front().first = from;
	}
}

SSA::OperandUse Interval::getOp()
{
	return SSA::OperandUse(op);
}

std::list<std::pair<int, int>> Interval::getRanges()
{
	return ranges;
}

std::string Interval::getStr() const
{
	std::string s = op.getStr();
	for (std::pair<int, int> range : ranges)
	{
		s += ", (" + std::to_string(range.first) + "," + std::to_string(range.second) + ")";
	}
	return s;
}

bool Interval::interferes(const Interval & other) const
{
	for (std::pair<int, int> thisRanges : ranges)
	{
		for (std::pair<int, int> otherRanges : other.ranges)
		{
			////check if ranges overlap
			//bool result = (thisRanges.first <= otherRanges.first && thisRanges.second >= otherRanges.first)
			//	|| (thisRanges.first <= otherRanges.second && thisRanges.second >= otherRanges.second);
			bool result = !(thisRanges.first > otherRanges.second || thisRanges.second < otherRanges.first);
			if (result)
			{
				return true;
			}
		}
	}
	return false;
}

void IntervalList::operator+=(IntervalList & other)
{
	for(Interval otherInt : other.intervals){
		bool exists = false;
		for (Interval& thisInt : intervals)
		{
			if (thisInt.getOp() == otherInt.getOp())
			{
				thisInt += otherInt;
				exists = true;
				break;
			}
		}
		//if otherInt not in interval list yet, we create it
		if (!exists)
		{
			intervals.push_back(otherInt);
		}
	}
}

void IntervalList::addRange(SSA::OperandUse op, int from, int to)
{
	//exit if from>to, which would be an invalid range
	if(from > to)
	{
		return;
	}
	if(!op.isVar())
	{
		return;
	}
	for (Interval& interval : intervals)
	{
		//if interval with op already exists, we prepend the range to it
		if (op == interval.getOp())
		{
			interval.addRange(from, to);
			return;
		}
	}
	//if interval with op doesn't exist, we create one
	Interval interval(op);
	interval.addRange(from, to);
	intervals.push_back(interval);
}

void IntervalList::setFrom(SSA::OperandUse op, int from, int to)
{
	if(!op.isVar())
	{
		return;
	}
	for (Interval& interval : intervals)
	{
		//if interval with op already exists, we prepend the range to it
		if (op == interval.getOp())
		{
			interval.setFrom(from);
			return;
		}
	}
	//if operand with op doesnt exist, we create a new one
	Interval interval(op);
	//WARNING
	//not sure if new interval should to "from" or "to"
	//I think that if we use this operand in another BB, the interval should already be set
	//when adding successor's live-in to live set, so this shouldn't happen, but this could break
	interval.addRange(from, from);
	intervals.push_back(interval);
}

std::string IntervalList::getStr() const
{
	std::string s = "";
	for (Interval interval : intervals)
	{
		s += interval.getStr() + "\n";
	}
	return s;
}

void IntervalList::constructInterferenceGraph(InterferenceGraph & IG)
{
	//first add nodes to graph
	for (Interval interval : intervals)
	{
		IG.addNode(interval.getOp());
	}
	//create edges based on interferences
	int numNodes = intervals.size();
	for(int i = 0; i < numNodes; ++i)
	{
		for(int j = 0; j < i; ++j)
		{
			if(intervals[i].interferes(intervals[j]))
			{
				IG.addEdge(intervals[i].getOp(), intervals[j].getOp());
			}
		}
	}

}

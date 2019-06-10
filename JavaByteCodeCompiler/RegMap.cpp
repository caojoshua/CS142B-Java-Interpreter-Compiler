#include "RegMap.h"



RegMap::RegMap()
{
}


RegMap::~RegMap()
{
}

void RegMap::insert(SSA::OperandUse op, int i)
{
	for (std::pair<SSA::OperandUse, int>& pair : map)
	{
		if (op == pair.first)
		{
			pair.second = i;
			return;
		}
	}
	//if op doesn't exist yet, create a new entry
	map.push_back(std::pair<SSA::OperandUse, int>(SSA::OperandUse(op), i));
}

int RegMap::getReg(SSA::OperandUse op)
{
	for (std::pair<SSA::OperandUse, int>& pair : map)
	{
		if (op == pair.first)
		{
			return pair.second;
		}
	}
	return -1;
}

bool RegMap::contains(SSA::OperandUse op)
{
	for (std::pair<SSA::OperandUse, int>& pair : map)
	{
		if (op == pair.first)
		{
			return true;
		}
	}
	return false;
}

std::string RegMap::getStr()
{
	std::string s = "";
	for (std::pair<SSA::OperandUse, int>& pair : map)
	{
		s += pair.first.getStr() + ": " + std::to_string(pair.second) + "\n";
	}
	return s;
}

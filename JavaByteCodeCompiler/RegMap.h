
#ifndef REGMAP_H
#define REGMAP_H

#include "SSAInstruction.h"
#include <list>
#include <string>

class RegMap
{
private:
	std::list < std::pair<SSA::OperandUse, int>> map;
public:
	RegMap();
	~RegMap();
	void insert(SSA::OperandUse op, int i);
	int getReg(SSA::OperandUse op);
	bool contains(SSA::OperandUse op);
	std::string getStr();
};

#endif

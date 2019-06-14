
#ifndef SSAOUT_H
#define SSAOUT_H

#include "BasicBlock.h"
#include "SSAinstruction.h"
#include "RegMap.h"
#include <list>

class SSAmethod
{
private:
	std::string name;
	std::vector<BasicBlock> bbs;
	RegMap map;
public:
	SSAmethod(std::string name, std::vector<BasicBlock> bbs) : name(name), bbs(bbs) {}
	std::string getName();
	std::vector<BasicBlock>& getBasicBlocks();
	RegMap getMap();
	void addRegMap(RegMap map);
};

class SSAoutput
{
private:
	//list of methods, each has a name and vector of basic blocks
	std::list<SSAmethod> out;
public:
	SSAoutput();
	~SSAoutput();
	void addMethod(std::string name, std::vector<BasicBlock> bbs);
	std::list<SSAmethod>& getOutput();
};

#endif
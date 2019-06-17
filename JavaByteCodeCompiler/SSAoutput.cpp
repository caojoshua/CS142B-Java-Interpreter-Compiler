#include "SSAoutput.h"



SSAoutput::SSAoutput()
{
}


SSAoutput::~SSAoutput()
{
}

void SSAoutput::addMethod(std::string name, bool isVoid, std::vector<BasicBlock> bbs)
{
	out.push_back(SSAmethod(name, isVoid, bbs));
}

std::list<SSAmethod>& SSAoutput::getOutput()
{
	return out;
}

std::string SSAmethod::getName()
{
	return name;
}

bool SSAmethod::getIsVoid()
{
	return isVoid;
}

std::vector<BasicBlock>& SSAmethod::getBasicBlocks()
{
	return bbs;
}

RegMap SSAmethod::getMap()
{
	return map;
}

void SSAmethod::addRegMap(RegMap map)
{
	this->map = map;
}

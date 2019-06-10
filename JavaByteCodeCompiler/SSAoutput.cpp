#include "SSAoutput.h"



SSAoutput::SSAoutput()
{
}


SSAoutput::~SSAoutput()
{
}

void SSAoutput::addMethod(std::string name, std::vector<BasicBlock> bbs)
{
	out.push_back(SSAmethod(name, bbs));
}

std::list<SSAmethod> SSAoutput::getOutput()
{
	return out;
}

std::string SSAmethod::getName()
{
	return name;
}

std::vector<BasicBlock> SSAmethod::getBasicBlocks()
{
	return bbs;
}

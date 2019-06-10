
#ifndef X86_H
#define X86_H

#include "RegMap.h"
#include "SSAoutput.h"

class x86executor
{
private:
	SSAoutput SSAout;
	RegMap regMap;
public:
	x86executor(SSAoutput out, RegMap map) : SSAout(out), regMap(map) {}
	~x86executor();
	void execute();
};

#endif
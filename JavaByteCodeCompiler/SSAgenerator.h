
#ifndef SSAGENERATOR_H
#define SSAGENERATOR_H

#include "Constants.h"
#include "ConstantPoolEntry.h"
#include <vector>

class SSAgenerator
{
	std::vector<ConstPoolEntry*> constPool;
	//std::vector<Method> methods;
public:
	SSAgenerator();
	~SSAgenerator();
	void generate();
};


#endif



#ifndef INTERNALREP_H
#define INTERNALREP_H

#include <vector>
#include <iostream>


class Code
{
private:
	unsigned short maxStack;
	unsigned short maxLocals;
	
public:
	Code(unsigned short ms, unsigned short ml, std::vector<uint8_t> bc) :
		maxStack(ms), maxLocals(ml), byteCodes(bc) {}
	std::vector<uint8_t> byteCodes;
};

class Method
{
public:
	unsigned short accessFlags;
	std::string name;
	std::string descriptor;
	Code code;
	Method(unsigned short accessFlags, std::string name, std::string descriptor, Code code) :
		accessFlags(accessFlags), name(name), descriptor(descriptor), code(code)
	{
	}
	~Method();
};
#endif
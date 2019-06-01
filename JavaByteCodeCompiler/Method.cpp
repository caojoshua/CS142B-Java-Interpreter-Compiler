#include "Method.h"

//TODO: fix memory leak
//when we create methods, we create a vector of BasicBlock* 
//calling operator= on vector will naively copy pointers
//when returning to the caller, garbage collection
//will delete the vector and apparently each pointer in the vector
//
//possible solution: implement deep copy of vector and Attrs
//this will involve overriding for each Attr subclass
Method::~Method()
{
	/*for (Attr* attr : attrs)
	{
		delete attr;
	}*/
}

std::string Method::getName() const
{
	return name;
}

std::string Method::getDescriptor()
{
	return descriptor;
}

unsigned short Method::getMaxLocals()
{
	return maxLocals;
}

int Method::getByteCodeSize()
{
	return byteCodes.size();
}

uint8_t Method::getByte(int index)
{
	if (index >= byteCodes.size())
	{
		//printf("attmped to access byte code with index %d when the number of byte codes is %d\n", index, byteCodes.size());
		exit(1);
	}
	return byteCodes[index];
}

int16_t Method::getTwoByte(int index)
{
	uint8_t a = getByte(index);
	uint8_t b = getByte(index + 1);
	//printf("%d, %d\n", a, b);
	return (uint16_t) a << 8 | b;
}

int32_t Method::getFourByte(int index)
{
	uint16_t a = getTwoByte(index);
	uint16_t b = getTwoByte(index + 2);
	return (uint32_t) a << 16 | b;
}

//std::vector<BasicBlock> Method::getBasicBlocks()
//{
//	return basicBlocks;
//}
//
//void Method::setBasicBlocks(std::vector<BasicBlock> BBs)
//{
//	basicBlocks = BBs;
//}

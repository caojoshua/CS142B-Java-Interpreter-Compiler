
#ifndef JITMETHOD_H
#define JITMETHOD_H

#include <string>
#include <windows.h>

typedef void (*fptr)();

class JITmethod
{
private:
	HANDLE hHeap;
	uint8_t* heapPtr;
	std::string name;
	//keeps track of what byte we are on while adding instructions
	int count;
	//param0 maps to local0_0, param1 maps to local1_0, etc.
	int numParams;
public:
	JITmethod(std::string name);
	~JITmethod();

	//getters
	std::string getName();
	uint8_t*getHeapPtr();
	int getNumParams();

	//setters
	void setNumParams(int n);

	//emit functions
	void emit(uint8_t byte);
	void emit16(uint16_t byte);
	void emit32(uint32_t byte);
	void emit64(uint64_t byte);

	//executes the JITmethod
	void execute();
};


#endif

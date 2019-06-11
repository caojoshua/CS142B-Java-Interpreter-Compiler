
#ifndef JITMETHOD_H
#define JITMETHOD_H

#include <string>
#include <windows.h>

//uint8_t nani = 0;

//std::string bust;

class JITmethod
{
private:
	HANDLE hHeap;
	uint8_t* heapPtr;
	std::string name;
	//keeps track of what byte we are on while adding instructions
	int count;
public:
	JITmethod(std::string name);
	~JITmethod();

	//getters
	std::string getName();

	//emit functions
	void emit(uint8_t byte);
	void emit16(uint16_t byte);
	void emit32(uint32_t byte);
	void emit64(uint64_t byte);

	//executes the JITmethod
	void execute();
};


#endif

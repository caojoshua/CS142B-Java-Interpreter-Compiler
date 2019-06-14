
#ifndef JITMETHOD_H
#define JITMETHOD_H

#include <string>
#include <windows.h>
#include <list>

typedef void (*fptr)();

class JITmethod
{
private:

	//keep track of Basic Block index and address, where to insert address(jump instructions)
	//this is important because when emitting bytes, we don't know the address of some basic blocks
	//so we insert addresses the BBaddr at jumpAddr
	class BBrelocation
	{
	public:
		uint32_t jmpAddr;
		uint32_t BBaddr;
		int BBindex;
		BBrelocation() : jmpAddr(0), BBaddr(0), BBindex(0) {}
	};

	HANDLE hHeap;
	uint8_t* heapPtr;
	std::string name;
	//keeps track of what byte we are on while adding instructions
	int count;
	//param0 maps to local0_0, param1 maps to local1_0, etc.
	int numParams;

	//data structs to keep track of Basic Block index and address, where to insert address(jump instructions)
	//this is important because when emitting bytes, we don't know the address of some basic blocks
	//so we insert addresses the BBaddr at jumpAddr, all addrs are line numbers, and we jmp by displacements
	//maps bbindices to bb addrs(should have been hashmap but oh well)
	std::list<std::pair<int, uint32_t>> BBaddrs;
	//maps jump target addrs to bb index
	std::list<std::pair<uint32_t, int>> jmpAddrs;

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

	//relocation table methods
	//create a new basic block address with line index equal to current count
	void newBBaddr(int BBindex);
	//set new jump address to count and targets BBindex
	void newJmpAddr(int BBindex);
	//sets jumps to target the correct address based on BBaddrs and jmpAddrs
	void relocateJmps();

	//executes the JITmethod
	void execute();

	//prints out bytes
	void print();
};


#endif

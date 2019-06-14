#include "JITmethod.h"


JITmethod::JITmethod(std::string name) : name(name), count(0), numParams(0)
{
	//create heap and pointer to the heap
	hHeap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
	if (hHeap == NULL)
	{
		printf("heap is null");
	}
	heapPtr = (uint8_t*) HeapAlloc(hHeap, 0, 1000);
	if(heapPtr == NULL)
	{
		printf("failed to allocate on heap\n");
	}
}

JITmethod::~JITmethod()
{
}

std::string JITmethod::getName()
{
	return name;
}

uint8_t* JITmethod::getHeapPtr()
{
	return heapPtr;
}

int JITmethod::getNumParams()
{
	return numParams;
}

void JITmethod::setNumParams(int n)
{
	numParams = n;
}

void JITmethod::emit(uint8_t byte)
{
	*(heapPtr + count++) = byte;
	printf("\\x%02X", byte);
}

void JITmethod::emit16(uint16_t byte)
{
	for (int i = 0; i < 2; ++i)
	{
		emit((uint8_t)(byte & 0xFF));
		byte = byte >> 8;
	}
}

void JITmethod::emit32(uint32_t byte)
{
	for (int i = 0; i < 4; ++i)
	{
		emit((uint8_t)(byte & 0xFF));
		byte = byte >> 8;
	}
}

void JITmethod::emit64(uint64_t byte)
{
	for (int i = 0; i < 8; ++i)
	{
		emit((uint8_t)(byte & 0xFF));
		byte = byte >> 8;
	}
}

void JITmethod::newBBaddr(int BBindex)
{
	BBaddrs.push_back(std::pair<int, uint32_t> (BBindex, count));
}

void JITmethod::newJmpAddr(int BBindex)
{
	jmpAddrs.push_back(std::pair<uint32_t,int> (count, BBindex));
}

void JITmethod::relocateJmps()
{
	//relocate each jmp addr
	for (std::pair<uint32_t, int>& jmpAddr : jmpAddrs)
	{
		//find the associated basic block
		for (std::pair<int, uint32_t>& BBaddr : BBaddrs)
		{
			if (jmpAddr.second == BBaddr.first)
			{
				//insert address; address is a displacement from AFTER jmpAddr
				int32_t BBdisp = BBaddr.second - (jmpAddr.first + 4);
				for (int i = 0; i < 4; ++i)
				{
					*(heapPtr + jmpAddr.first + i) = (uint8_t)(BBdisp & 0xFF);
					BBdisp = BBdisp >> 8;
				}
			}
		}
	}
}

void JITmethod::execute()
{
	//maybe attach a return to make sure we always return, double returns shouldn't be a big deal
	//emit(0xc3);
	printf("\n****x86 output****\n");
	fptr funcPtr = (fptr) heapPtr;
	funcPtr();
	printf("returned from x86 execution!");
}

void JITmethod::print()
{
	//output after inserting jmp addrs
	printf("X86 code after inserting jmp addrs\n");
	for(int i = 0; i < count; ++i)
	{
		printf("\\x%X ", *(heapPtr + i));
	}
	printf("\n");
}

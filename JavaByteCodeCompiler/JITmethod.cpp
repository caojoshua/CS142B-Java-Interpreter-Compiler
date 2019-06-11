#include "JITmethod.h"





JITmethod::JITmethod(std::string name) : name(name), count(0)
{
	//create heap and pointer to the heap
	HANDLE hHeap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
	if (hHeap == NULL)
	{
		printf("heap is null\n");
	}
	uint8_t* heapPtr = (uint8_t*) HeapAlloc(hHeap, 0, 1000);
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

void JITmethod::emit(uint8_t byte)
{
	*(heapPtr + count++) = byte;
	printf("0x%X ", byte);
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

void JITmethod::execute()
{
	void (*funcPtr)() = (void(*)()) heapPtr;
	funcPtr();
	printf("returned from x86 execution!");
}

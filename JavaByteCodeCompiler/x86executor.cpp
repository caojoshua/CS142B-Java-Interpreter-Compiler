
#include "x86executor.h"
#include "x86constants.h"


void x86executor::genX86(SSAmethod& m)
{
	//create a pair of string and code and save it to JITcode
	//std::pair<std::string, uint8_t*> pair("",nullptr);
	//pair.first = m.getName();

	////create heap and pointer to the heap
	//HANDLE hHeap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
	//if (hHeap == NULL)
	//{
	//	printf("heap is null\n");
	//}
	//uint8_t* heapPtr = (uint8_t*) HeapAlloc(hHeap, 0, 1000);
	//if(heapPtr == NULL)
	//{
	//	printf("failed to allocate on heap\n");
	//}
	//pair.second = heapPtr;
	////push pair to JITcode
	//JITcode.push_back(pair);

	JITmethod jit(m.getName());

	//define useful variables
	std::vector<BasicBlock> bbs = m.getBasicBlocks();
	RegMap map = m.getMap();
	//count represents what byte we are on
	int count = 0;

	//sequentially go through basic blocks and generate code
	for (int i = 0; i < bbs.size(); ++i)
	{
		BasicBlock bb = bbs[i];
		//go through instructions of bb
		for (SSA::Instruction* instruction : bb.getInstructions())
		{

		}
	}
}

void x86executor::emit(uint8_t* heapPtr, int& count, uint8_t byte)
{
	*(heapPtr + count++) = byte;
	printf("0x%X ", byte);
}

void x86executor::emit16(uint8_t* heapPtr, int& count, uint16_t byte)
{
	for (int i = 0; i < 2; ++i)
	{
		emit(heapPtr, count, (uint8_t)(byte & 0xFF));
		byte = byte >> 8;
	}
}

void x86executor::emit32(uint8_t* heapPtr, int& count, uint32_t byte)
{
	for (int i = 0; i < 4; ++i)
	{
		emit(heapPtr, count, (uint8_t)(byte & 0xFF));
		byte = byte >> 8;
	}
}

void x86executor::emit64(uint8_t* heapPtr, int& count, uint64_t byte)
{
	for (int i = 0; i < 8; ++i)
	{
		emit(heapPtr, count, (uint8_t)(byte & 0xFF));
		byte = byte >> 8;
	}
}

x86executor::x86executor(SSAoutput out)
{
	//this->SSAout = out;
	//count = 0;
	//hHeap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
	//if (hHeap == NULL)
	//{
	//	printf("heap is null\n");
	//}
	//JITcode = (uint8_t*) HeapAlloc(hHeap, 0, 1000);
	//if(JITcode == NULL)
	//{
	//	printf("failed to allocate on heap\n");
	//}

	//printInt(0x69);

	////mov: REX.W + B8+ rd io
	////rd: lower 3 bits in opcode encode register
	//emit(0xB8 + EAX);
	//emit32((uint32_t) &printTest);
	////call
	//emit(0xFF);
	////mod 11 + reg 2 + EAX
	//emit(0b11000000 + 0b010000 + EAX);

	////mov: REX.W + B8+ rd io
	////rd: lower 3 bits in opcode encode register
	//emit(0xB8 + EAX);
	//emit32((uint32_t) &printInt);
	////mov printInt arg into ecx
	///*emit(0xB8 + ECX);
	//emit32((uint32_t) 0xABCD);*/
	////push
	//emit(0x68);
	//emit32((uint32_t) 0xABCD);
	///*emit(0x6A);
	//emit(0x69);*/
	////call
	//emit(0xFF);
	////mod 11 + reg 2 + EAX
	//emit(0b11000000 + 0b010000 + EAX);

	////add 4 to esp
	//emit(0x83);
	//emit(0xc4);
	//emit(0x04);

	//////subtract 4 from stack pointer
	////emit(0x81);
	////// /5
	////emit(0b11000000 + 0b101000 + ESP);
	////emit((uint32_t)4);

	////return
	//emit(0xc3);
}

x86executor::~x86executor()
{
}

void x86executor::execute()
{
	//gen x86 code
	for (SSAmethod& m : SSAout.getOutput())
	{
		genX86(m);
	}
	for (JITmethod jit : JITmethods)
	{
		if (jit.getName() == "main")
		{
			jit.execute();
			return;
		}
	}
}

void printTest()
{
	printf("called printTest!\n");
}

void printInt(int i)
{
	i &= 0xFFFFFFFF;
	printf("%d\n", i);
	printf("0x%x\n", i);
}

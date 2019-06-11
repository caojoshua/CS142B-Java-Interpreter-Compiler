
#include "x86executor.h"


void x86executor::createJITmethods()
{
	for (SSAmethod& m : SSAout.getOutput())
	{
		JITmethods.push_back(JITmethod(m.getName()));
	}
}

void x86executor::genX86(JITmethod& jit, SSAmethod& m)
{
	//define useful variables
	std::vector<BasicBlock> bbs = m.getBasicBlocks();
	RegMap map = m.getMap();
	//count represents what byte we are on
	int count = 0;

	//output jit name
	printf("***%s***\n", jit.getName().c_str());

	//first insert prologue
	prologue(jit);

	//sequentially go through basic blocks and generate code
	for (int i = 0; i < bbs.size(); ++i)
	{
		BasicBlock bb = bbs[i];
		//go through instructions of bb
		for (SSA::Instruction* ins : bb.getInstructions())
		{
			SSAopcode opcode = ins->getSSAopcode();
			switch (opcode)
			{
			case MOV:
			mov(jit, map, SSA::OperandUse(ins->getDest()), SSA::OperandUse(ins->getSrc1()));
			break;
			case ADD:
			break;
			case SUB:
			break;
			case MUL:
			break;
			case DIV:
			break;
			case SHL:
			break;
			case SHR:
			break;
			case INC:
			break;
			case DEC:
			break;
			case CALL:
			break;
			case CMP:
			break;
			case CONDBRANCH:
			break;
			case UNCONDBRANCH:
			break;
			case RET:
				ret(jit);
				break;
			case PHI:
				//we ignore these, special handling of phi functions at end of each basic block
			default:
				//shouldn't happen
				break;
			}
		}
		//TODO: special handling of phi functions
	}
	//printf("%X\n",EAX);
}

void x86executor::mov(JITmethod& jit, RegMap& map, SSA::OperandUse dest, SSA::OperandUse src)
{
	//MOV r imm
	if (src.isConst())
	{
		//+rd: encode register into lower three bits of opcode
		jit.emit(MOV_R_IMM + intToReg[map.getReg(dest)]);
		//id: 4-byte immediate
		jit.emit32(src.getVal());
	}
	//MOV r r
	else if (src.isVar())
	{
		//jit.emit(MOV_R_RM);
		//// /r, modrm contains both register and r/m
		//jit.emit(modrm(MOD_REG, intToReg[map.getReg(dest)], intToReg[map.getReg(src)]));
		//jit.emit(MOV_RM_R);
		//// /r, modrm contains both register and r/m
		//jit.emit(modrm(MOD_REG, intToReg[map.getReg(src)], intToReg[map.getReg(dest)]));
		mov_rrm(jit, intToReg[map.getReg(src)], intToReg[map.getReg(dest)]);
	}
}

void x86executor::ret(JITmethod & jit)
{
	////mov esp, ebp
	mov_rrm(jit, ESP, EBP);
	pop(jit, EBP);
	jit.emit(RET86);
	printf("\n");
}

void x86executor::mov_rrm(JITmethod& jit, uint8_t dest, uint8_t src)
{
	jit.emit(MOV_R_RM);
	jit.emit(modrm(MOD_REG, dest, src));
	printf("\n");
}

void x86executor::pop(JITmethod & jit, uint8_t reg)
{
	jit.emit(POP_RM);
	jit.emit(modrm(MOD_REG, 0, reg));
	printf("\n");
}

void x86executor::push(JITmethod & jit, uint8_t reg)
{
	jit.emit(PUSH_RM);
	// /6 store 6 into reg 
	jit.emit(modrm(MOD_REG, 6, reg));
	printf("\n");
}

JITmethod & x86executor::getJITmethods(std::string name)
{
	for (JITmethod& jit : JITmethods)
	{
		if (jit.getName() == name)
		{
			return jit;
		}
	}
}

uint8_t x86executor::modrm(uint8_t mod, uint8_t regOpcode, uint8_t rm)
{
	return (mod << 6) + (regOpcode << 3) + rm;
}

void x86executor::prologue(JITmethod & jit)
{
	push(jit, EBP);
	mov_rrm(jit, EBP, ESP);
}

x86executor::x86executor(SSAoutput out) : SSAout(out)
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
	//first create methods, we need to do this first or else we won't know addresses of methods when we want to call them
	createJITmethods();
 	//generate machine code
	for (JITmethod& jit : JITmethods)
	{
		//find corresponding SSAmethod
		for (SSAmethod& m : SSAout.getOutput())
		{
			if (m.getName() == jit.getName())
			{
				genX86(jit, m);
			}
		}
	}
	//execute main function
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

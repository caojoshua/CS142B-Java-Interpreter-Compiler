
//KNOWN BUGS: method w/params but are empty reaches arrayoutofbounds, params don't get assigned regs when they should?


#include "x86executor.h"


void x86executor::createJITmethods()
{
	for (SSAmethod& m : SSAout.getOutput())
	{
		JITmethods.push_back(JITmethod(m.getName()));
	}
	//not so elegant, but set number of params for each method first for epilogue gen
	for (JITmethod& jit : JITmethods)
	{
		for (SSAmethod& m : SSAout.getOutput())
		{
			if (m.getName() == jit.getName())
			{
				//go through each instruction to find call instructions
				for(BasicBlock b : m.getBasicBlocks())
				{
					for(SSA::Instruction* ins : b.getInstructions())
					{
						if(ins->getSSAopcode() == CALL)
						{
							//set number of params for called JIT method to number of args in the call instruction
							getJITmethod(ins->getMethodName()).setNumParams(ins->getSrcs().size());
						}
					}
				}
			}
		}
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
	prologue(jit, map);

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
				mov(jit, map, ins);
				break;
			//do same for all binary
			case ADD:
			case SUB:
			case MUL:
			case DIV:
			case SHL:
			case SHR:
				iBinary(jit, map, ins);
				break;
			case INC:
			case DEC:
				iUnary(jit, map, ins);
				break;
			case CALL:
				call(jit, map, ins);
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
	}
	//SPECIAL PHI HANDLING TODO: figure out how to insert mov functions for phi
	for(BasicBlock bb : bbs)
	{
		for (SSA::Instruction* ins : bb.getInstructions())
		{
			if(ins->isPhi())
			{
				//for each phi src, find its associate basic block and insert mov from src to dest
				for(std::pair<int, SSA::Operand*> phiSrc : ins->getPhiSrcs())
				{
					//insert code at end of basic blocks here
				}
			}
		}
	}

	//printf("%X\n",EAX);
}

void x86executor::iBinary(JITmethod & jit, RegMap & map, SSA::Instruction * ins)
{
	uint8_t opcode = 0;
	SSA::OperandUse dest(ins->getDest());
	SSA::OperandUse src1(ins->getSrc1());
	SSA::OperandUse src2(ins->getSrc2());
	//assume src1 is always var
	if (src2.isVar())
	{
		iBinary_rmr(jit, ins->getSSAopcode(), intToReg[map.getReg(dest)], intToReg[map.getReg(src1)], intToReg[map.getReg(src2)]);
	}
	else if (src2.isConst())
	{
		iBinary_rimm(jit, ins->getSSAopcode(), intToReg[map.getReg(dest)], intToReg[map.getReg(src1)], src2.getVal());
	}
}

//only unarys we care about are INC and DEC, we just use ADD and SUB
//DONT call iBinary functions because they insert movs before binary op
void x86executor::iUnary(JITmethod & jit, RegMap & map, SSA::Instruction * ins)
{
	uint8_t opcode;
	uint8_t slashDigit; // /d: number that will will but in "R" of modrxSSA::OperandUse dest(ins->getDest());
	SSA::OperandUse dest(ins->getDest());
	//assume src is constant
	SSA::OperandUse src(ins->getSrc1());
	switch (ins->getSSAopcode())
	{
	case INC:
		opcode = ADD_RM_IMM;
		slashDigit = 0;
		break;
	case DEC:
		opcode = SUB_RM_IMM;
		slashDigit = 5;
		break;
	default:
		return;
	}
	//emit code
	jit.emit(opcode);
	jit.emit(modrm(MOD_REG, slashDigit, intToReg[map.getReg(dest)]));
	jit.emit32(src.getVal());
}

void x86executor::call(JITmethod & jit, RegMap & map, SSA::Instruction* ins)
{
	std::vector<SSA::Operand*> args = ins->getSrcs();
	std::string methodName = ins->getMethodName();
	getJITmethod(methodName).setNumParams(args.size());
	//push eax, ecx, edx
	push(jit, EAX);
	push(jit, ECX);
	push(jit, EDX);
	//push arguments
	//only do this for println for now since our own functions are busted
	if (methodName == "Println")
	{
		for (SSA::Operand* op : args)
		{
			push(jit, map, op);
		}
	}
	//try saving to caller saved registers since pushing isn't working
	for(int i = 0; i < args.size(); ++i)
	{
		if (args[i]->isConst())
		{
			mov_rimm(jit, callerSavedRegs[i], args[i]->getVal());
		}
		else if (args[i]->isVar())
		{
			mov_rmr(jit, callerSavedRegs[i], intToReg[map.getReg(args[i])]);
		}
	}
	//call function
	//move address to eax, which was already pushed so we don't care if we lose its value
	mov_rimm(jit, EAX, (uint32_t) getFuncPtr(methodName));
	jit.emit(CALL_RM);
	// /2
	jit.emit(modrm(MOD_REG, 2, EAX));
	//special handling for Println cus they don't pop arguments, they assign based on offset
	if (methodName == "Println")
	{
		//TODO make this subtract from ESP instead
		for(SSA::Operand* op : args)
		{
			//we gonna pop into EDX later so we don't care about its value, just popping values from stack
			pop(jit, EDX);
		}
	}
	//pop edx, ecx, eaz
	pop(jit, EDX);
	pop(jit, ECX);
	pop(jit, EAX);
}

void x86executor::mov(JITmethod& jit, RegMap& map, SSA::Instruction* ins)
{
	SSA::OperandUse dest(ins->getDest());
	SSA::OperandUse src(ins->getSrc1());
	//MOV r imm
	if (src.isConst())
	{
		mov_rimm(jit, intToReg[map.getReg(dest)], src.getVal());
	}
	//MOV r r
	else if (src.isVar())
	{
		mov_rmr(jit, intToReg[map.getReg(dest)], intToReg[map.getReg(src)]);
	}
}

void x86executor::ret(JITmethod & jit)
{
	//pop edi, esi, ebx
	pop(jit, EDI);
	pop(jit, ESI);
	pop(jit, EBX);
	//mov esp, ebp
	mov_rmr(jit, ESP, EBP);
	//pop ebp
	pop(jit, EBP);
	//ret
	jit.emit(RET86);
	printf("\n");
}

void x86executor::iBinary_rmr(JITmethod & jit, SSAopcode ssa_opcode,uint8_t dest, uint8_t src1, uint8_t src2)
{
	uint8_t opcode;
	switch (ssa_opcode)
	{
	case ADD:
		opcode = ADD_RM_R;
		break;
	case SUB:
		opcode = SUB_RM_R;
		break;
	case MUL:
	case DIV:
	case SHL:
	case SHR:
	default:
		return;
	}

	//emit code
	//if a src has the same mapping as dest, we mov src into dest
	//if neither src is same as dest, it doesnt' matter which we move first
	if (src1 == dest)
	{
		mov_rmr(jit, dest, src1);
		//then add src2 to dest
		jit.emit(opcode);
		jit.emit(modrm(MOD_REG, src2, dest));
	}
	else
	{
		mov_rmr(jit, dest, src2);
		//then add src1 to dest
		jit.emit(opcode);
		jit.emit(modrm(MOD_REG, src1, dest));
	}
	
}

void x86executor::iBinary_rimm(JITmethod& jit, SSAopcode ssa_opcode, uint8_t dest, uint8_t src1, uint32_t constValue)
{
	uint8_t opcode;
	uint8_t slashDigit; // /d: number that will will but in "R" of modrx
	switch (ssa_opcode)
	{
	case ADD:
		opcode = ADD_RM_IMM;
		slashDigit = 0;
		break;
	case SUB:
		opcode = SUB_RM_IMM;
		slashDigit = 5;
		break;
	case MUL:
	case DIV:
	case SHL:
	case SHR:
	default:
		return;
	}

	//emit code
	//first move src 1 to dest
	mov_rmr(jit, dest, src1);
	//then add constValue to dest
	jit.emit(opcode);
	jit.emit(modrm(MOD_REG, slashDigit, dest));
	jit.emit32(constValue);
}

void x86executor::mov_rmr(JITmethod& jit, uint8_t dest, uint8_t src)
{
	jit.emit(MOV_R_RM);
	jit.emit(modrm(MOD_REG, dest, src));
	printf("\n");
}

void x86executor::mov_rimm(JITmethod & jit, uint8_t dest, uint32_t constValue)
{
	//+rd: encode register into lower three bits of opcode
	jit.emit(MOV_R_IMM + dest);
	//id: 4-byte immediate
	jit.emit32(constValue);
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

void x86executor::push(JITmethod & jit, RegMap& map, SSA::OperandUse op)
{
	if (op.isConst())
	{
		jit.emit(PUSH_IMM);
		// /d 4 bytes of immediate
		jit.emit32(op.getVal());
		printf("\n");
	}
	else if (op.isVar())
	{
		push(jit, intToReg[map.getReg(op)]);
	}
}

JITmethod & x86executor::getJITmethod(std::string name)
{
	for (JITmethod& jit : JITmethods)
	{
		if (jit.getName() == name)
		{
			return jit;
		}
	}
}

fptr x86executor::getFuncPtr(std::string name)
{
	//special handling for Println
	if (name == "Println")
	{
		return (fptr) &Println;
	}
	//look for JITmethod with name and return a pointer to its code
	for (JITmethod m : JITmethods)
	{
		if (m.getName() == name)
		{
			return (fptr)m.getHeapPtr();
		}
	}
}

uint8_t x86executor::modrm(uint8_t mod, uint8_t regOpcode, uint8_t rm)
{
	return (mod << 6) + (regOpcode << 3) + rm;
}

void x86executor::prologue(JITmethod & jit, RegMap& map)
{
	//push ebp
	push(jit, EBP);
	//mov ebp, esp
	mov_rmr(jit, EBP, ESP);
	//pop params, THIS WILL BREAK IF MORE THAN 3 PARAMS because we are only allocating params to caller saved registers
	int numParams = jit.getNumParams();
	/*for (int i = 0; i < numParams; ++i)
	{
		pop(jit, callerSavedRegs[i]);
	}*/
	//push ebx, esi, edi
	push(jit, EBX);
	push(jit, ESI);
	push(jit, EDI);
	//move params, which are stored in callee saved registers, to their mapped registers(not the right way to do things but it works hopefully)
	for (int i = 0; i < numParams; ++i)
	{
		mov_rmr(jit, intToReg[map.getReg(SSA::OperandUse(SSA::Operand::local, numParams - 1 - i, 0))], callerSavedRegs[i]);
		//mov_rmr(jit, intToReg[map.getReg(SSA::OperandUse(SSA::Operand::local, numParams - 1 - i, 0))], callerSavedRegs[i]);
	}
}

x86executor::x86executor(SSAoutput out) : SSAout(out)
{
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
	
	//WORKING CALL TO PRINTLN
	//printf("\n\nTEST");
	//JITmethod test("test");
	////epilogue(test);
	//mov_rimm(test, EAX, (uint32_t)0x6969);
	//push(test, EAX);
	//mov_rimm(test, EAX, (uint32_t) &Println);
	//test.emit(CALL_RM);
	//test.emit(modrm(MOD_REG, 2, EAX));
	//ret(test);
	//test.execute();

	//Println(69);
	////TRYING CALL TO JITmethod
	//printf("\n\nTEST");
	//JITmethod test("test");
	////epilogue(test);
	//mov_rimm(test, EAX, (uint32_t)0x6969);
	//push(test, EAX);
	//mov_rimm(test, EAX, (uint32_t) getJITmethod("PrintInt").getHeapPtr());
	//test.emit(CALL_RM);
	//test.emit(modrm(MOD_REG, 2, EAX));
	//ret(test);
	//test.execute();
	
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

void Println(int i)
{
	//i &= 0xFFFFFFFF;
	printf("%d\n", i);
	printf("0x%x\n", i);
}

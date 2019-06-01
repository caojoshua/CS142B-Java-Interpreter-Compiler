#include "SSAgenerator.h"




void SSAgenerator::createBasicBlocks(Method& m)
{
	std::list<int> leaders = getLeaders(m);

	printf("***METHOD: %s***\n", m.getName().c_str());
	/*for (std::list<int>::const_iterator i = leaders.cbegin(); i != leaders.cend(); ++i)
	{
		printf("%d\n", *i);
	}
	printf("\n");*/

	std::vector<BasicBlock> BBs = genBasicBlockCode(m, leaders);
	insertPhi(BBs);
	//printf("%d\n", BBs.size());
	for(int i = 0; i < BBs.size(); ++i)
	{
		BasicBlock BB = BBs[i];
		printf("Basic Block (#%d)\n", i);
		for (SSA::Instruction* i : BB.getInstructions())
		{
			printf("%s\n", i->getStr().c_str());
		}
	}
}

std::list<int> SSAgenerator::getLeaders(Method& m)
{
	//first get basic block leaders
	std::list<int> leaders = { 0 };
	int byteCodeSize = m.getByteCodeSize(); 
	int i = 0;
	while (i < byteCodeSize)
	{
		uint8_t byteCode = m.getByte(i++);
		//branch statement
		if (std::find(std::cbegin(branchCodes), std::cend(branchCodes), byteCode) != std::cend(branchCodes))
		{
			leaders.push_front(i + m.getTwoByte(i) - 1);
			i += 2;
			leaders.push_front(i);
		}
		//goto
		else if (byteCode == goto_b)
		{
			leaders.push_front(i + m.getTwoByte(i) - 1);
			i += 2;
		}
		//codes that take up one byte, no need to do anything here because we already incremented i
		else if (std::find(std::cbegin(oneByteCodes), std::cend(oneByteCodes), byteCode) != std::cend(oneByteCodes))
		{
		}
		//codes that take up two bytes
		else if (std::find(std::cbegin(twoByteCodes), std::cend(twoByteCodes), byteCode) != std::cend(twoByteCodes))
		{
			i += 1;
		}
		//codes that take up three bytes
		else if (std::find(std::cbegin(threeByteCodes), std::cend(threeByteCodes), byteCode) != std::cend(threeByteCodes))
		{
			i += 2;
		}
		else {
			printf("unknown byte code 0x%x\n", byteCode);
		}
	}
	//add the final line into leaders if not done so yet
	/*if (std::find(std::cbegin(leaders), std::cend(leaders), byteCodeSize - 1) == std::cend(leaders))
	{
		leaders.push_front(byteCodeSize - 1);
	}*/
	//add final line + 1 into leaders
	leaders.push_front(byteCodeSize);
	leaders.sort();
	return leaders;
}

std::vector<BasicBlock> SSAgenerator::genBasicBlockCode(Method& m, std::list<int> leaders)
{
	//first create the basic blocks
	std::vector<BasicBlock> basicBlocks;
	int index = 0;
	for (std::list<int>::const_iterator leader = leaders.cbegin(); std::next(leader) != leaders.cend(); ++leader)
	{
		basicBlocks.push_back(BasicBlock(index, *leader, *next(leader) - 1));
		index++;
	}
	int numBasicBlocks = basicBlocks.size();
	//link adjacent basic blocks if the last bytecode isn't a branch statement
	for (int i = 0; i < numBasicBlocks - 1; ++i)
	{
		uint8_t byteCode = m.getByte(basicBlocks[i].getEndLine());
		if (std::find(branchCodes.cbegin(), branchCodes.cend(), byteCode) == branchCodes.cend() || byteCode != goto_b)
		{
			basicBlocks[i].addSucc(i + 1);
			basicBlocks[i + 1].addPred(i);
		}
	}
	int stackCount = 0;
	for (std::vector<BasicBlock>::iterator iter = basicBlocks.begin(); iter != basicBlocks.end(); ++iter)
	{
		//i contains which byte we are on while going thru a method
		BasicBlock& basicBlock = *iter;
		basicBlock.setInStackDepth(stackCount);
		int i = basicBlock.getStartLine();
		int tempCount = 0;
		while (i <= basicBlock.getEndLine())
		{
			uint8_t byteCode = m.getByte(i++);
			switch (byteCode)
			{
				//loading and storing
				case iconst_m1_b:
					iconst(basicBlock, stackCount++, -1);
					break;
				case iconst_0_b:
					iconst(basicBlock, stackCount++, 0);
					break;
				case iconst_1_b:
					iconst(basicBlock, stackCount++, 1);
					break;
				case iconst_2_b:
					iconst(basicBlock, stackCount++, 2);
					break;
				case iconst_3_b:
					iconst(basicBlock, stackCount++, 3);
					break;
				case iconst_4_b:
					iconst(basicBlock, stackCount++, 4);
					break;
				case iconst_5_b:
					iconst(basicBlock, stackCount++, 5);
					break;
				case iload_b:
					iload(basicBlock, stackCount++, m.getByte(i++));
					break;
				case iload0_b:
					iload(basicBlock, stackCount++, 0);
					break;
				case iload1_b:
					iload(basicBlock, stackCount++, 1);
					break;
				case iload2_b:
					iload(basicBlock, stackCount++, 2);
					break;
				case iload3_b:
					iload(basicBlock, stackCount++, 3);
					break;
				case istore_b:
					istore(basicBlock, --stackCount, m.getByte(i++));
					break;
				case istore_0_b:
					istore(basicBlock, --stackCount, 0);
					break;
				case istore_1_b:
					istore(basicBlock, --stackCount, 1);
					break;
				case istore_2_b:
					istore(basicBlock, --stackCount, 2);
					break;
				case istore_3_b:
					istore(basicBlock, --stackCount, 3);
					break;
				//arithmetic
				case iadd_b:
					binary(basicBlock, SSA::BinaryInstruction::add, stackCount - 2, stackCount - 1);
					--stackCount;
					break;
				case iinc_b:
					unary(basicBlock, SSA::UnaryInstruction::inc, m.getByte(i), m.getByte(i+1));
					i += 2;
					break;
				case isub_b:
					binary(basicBlock, SSA::BinaryInstruction::sub, stackCount - 2, stackCount - 1);
					--stackCount;
					break;
				case imul_b:
					binary(basicBlock, SSA::BinaryInstruction::mul, stackCount - 2, stackCount - 1);
					--stackCount;
					break;
				case idiv_b:
					binary(basicBlock, SSA::BinaryInstruction::div, stackCount - 2, stackCount - 1);
					--stackCount;
					break;
				case ishl_b:
					binary(basicBlock, SSA::BinaryInstruction::shl, stackCount - 2, stackCount - 1);
					--stackCount;
					break;
				case ishr_b:
					binary(basicBlock, SSA::BinaryInstruction::shr, stackCount - 2, stackCount - 1);
					--stackCount;
					break;
				//control transfer
				case if_icmpne_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::ne, tempCount, stackCount - 2, stackCount - 1, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					stackCount -= 2;
					i += 2;
					break;
				case if_icmpeq_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::eq, tempCount, stackCount - 2, stackCount - 1, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					stackCount -= 2;
					i += 2;
					break;
				case if_icmpgt_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::gt, tempCount, stackCount - 2, stackCount - 1, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					stackCount -= 2;
					i += 2;
					break;
				case if_icmpge_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::gte, tempCount, stackCount - 2, stackCount - 1, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					stackCount -= 2;
					i += 2;
					break;
				case if_icmplt_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::le, tempCount, stackCount - 2, stackCount - 1, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					stackCount -= 2;
					i += 2;
					break;
				case if_icmple_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::lte, tempCount, stackCount - 2, stackCount - 1, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					stackCount -= 2;
					i += 2;
					break;
				case ifeq_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::eq, tempCount, --stackCount, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					i += 2;
					break;
				case ifne_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::ne, tempCount, --stackCount, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					i += 2;
					break;
				case ifgt_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::gt, tempCount, --stackCount, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					i += 2;
					break;
				case ifge_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::gte, tempCount, --stackCount, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					i += 2;
					break;
				case iflt_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::le, tempCount, --stackCount, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					i += 2;
					break;
				case ifle_b:
					cond(basicBlocks, basicBlock, SSA::CondBranchInstruction::lte, tempCount, --stackCount, getBBindex(basicBlocks, i+2), getBBindex(basicBlocks, i + m.getTwoByte(i) - 1));
					i += 2;
					break;
				//other
				case goto_b:
				{
					int target = getBBindex(basicBlocks, i + m.getTwoByte(i) - 1);
					basicBlock.addInstruction(new SSA::UncondBranchInstruction(SSA::Branch(target)));
					basicBlock.addSucc(target);
					basicBlocks[target].addPred(basicBlock.getIndex());
					i += 2;
					break;
				}
				case bipush_b:
					iconst(basicBlock, stackCount++, m.getByte(i++));
					break;
				case invokestatic_b:
					stackCount += call(basicBlock, m.getTwoByte(i));
					i += 2;
					break;
				case invokevirtual_b:
					basicBlock.addInstruction(new SSA::PrintInstruction(SSA::Operand(SSA::Operand::stack, --stackCount)));
					i += 2;
					break;
				case return_b:
					basicBlock.addInstruction(new SSA::ReturnInstruction());
					break;
				case ireturn_b:
					basicBlock.addInstruction(new SSA::ReturnInstruction(SSA::Operand(SSA::Operand::stack, --stackCount)));
					break;
				//skip over these
				case getstatic_b:
					i += 2;
					break;
				default:
					printf("unknown byte code 0x%x\n", byteCode);
			}
		}
	}
	return basicBlocks;
}

void SSAgenerator::insertPhi(std::vector<BasicBlock>& BBs)
{
	// algorithm here if its not taken down: https://www.ics.uci.edu/~yeouln/course/ssa.pdf
	// lists are ints corresponding to Basic Block indices
	std::list<SSA::Operand> vars;
	getVars(vars, BBs);
	for(SSA::Operand var : vars)
	{
		std::list<int> workList;
		std::list<int> hasPhi;
		for(int i = 0; i < BBs.size(); ++i)
		{
			if(containsAssignment(BBs[i], var))
			{
				workList.push_back(i);
			}
		}
		std::list<int> everOnWorkList = workList;
		while(!workList.empty())
		{
			int n = workList.front();
			workList.pop_front();
			std::list<int> df;
			dominanceFrontier(df, BBs, BBs[n]);
			for(int d : df)
			{
				//insert phi into BBs[d] if it doesn't already have phi AND
				//(var is not stack OR basic block live-in stack depth is greater than variable value)
				//simple phi optimization: if var is a stack var, do not insert Phi if the live-in stack depth of the
				//dominance frontier is smaller than or equal to the stack variable index
				if(std::find(hasPhi.cbegin(), hasPhi.cend(), d) == hasPhi.cend() && (var.getType() != SSA::Operand::stack || BBs[d].getInStackDepth() > var.getVal()))
				{
					BBs[d].addPhi(var);
					hasPhi.push_back(d);
					if(std::find(everOnWorkList.cbegin(), everOnWorkList.cend(), d) == everOnWorkList.cend())
					{
						//we haven't checked if d is on workList, so we do that before pushing it
						if(std::find(workList.cbegin(), workList.cend(), d) == workList.cend())
						{
							workList.push_back(d);
						}
						everOnWorkList.push_back(d);
					}
				}
			}
		}
	}
}


void SSAgenerator::iconst(BasicBlock & b, int stackNum, int val)
{
	b.addInstruction(new SSA::MovInstruction(SSA::Operand(SSA::Operand::stack, stackNum), SSA::Operand(SSA::Operand::constant, val)));
}
void SSAgenerator::iload(BasicBlock & b, int stackNum, int localNum)
{
	b.addInstruction(new SSA::MovInstruction(SSA::Operand(SSA::Operand::stack, stackNum), SSA::Operand(SSA::Operand::local, localNum)));
}
void SSAgenerator::istore(BasicBlock & b, int stackNum, int localNum)
{
	b.addInstruction(new SSA::MovInstruction(SSA::Operand(SSA::Operand::local, localNum), SSA::Operand(SSA::Operand::stack, stackNum)));
}
void SSAgenerator::unary(BasicBlock & b, SSA::UnaryInstruction::Opcode opcode, int dest, int other)
{
	b.addInstruction(new SSA::UnaryInstruction(opcode, SSA::Operand(SSA::Operand::local, dest), SSA::Operand(SSA::Operand::constant, other)));
}
void SSAgenerator::binary(BasicBlock & b, SSA::BinaryInstruction::Opcode opcode, int dest, int other)
{
	b.addInstruction(new SSA::BinaryInstruction(opcode, SSA::Operand(SSA::Operand::stack, dest),
							SSA::Operand(SSA::Operand::stack, dest), SSA::Operand(SSA::Operand::stack, other)));
}
void SSAgenerator::cond(std::vector<BasicBlock>& BBs, BasicBlock & b, SSA::CondBranchInstruction::Opcode opcode, int tmp, int src1, int src2, int BBindex1, int BBindex2)
{
	SSA::Operand cond = SSA::Operand(SSA::Operand::temp, tmp);
	b.addInstruction(new SSA::CmpInstruction(cond, SSA::Operand(SSA::Operand::stack, src1), SSA::Operand(SSA::Operand::stack, src2)));
	b.addInstruction(new SSA::CondBranchInstruction(opcode, cond, SSA::Branch(BBindex1), SSA::Branch(BBindex2)));
	b.addSucc(BBindex1);
	b.addSucc(BBindex2);
	BBs[BBindex1].addPred(b.getIndex());
	BBs[BBindex2].addPred(b.getIndex());
}

void SSAgenerator::cond(std::vector<BasicBlock>& BBs, BasicBlock & b, SSA::CondBranchInstruction::Opcode opcode, int tmp, int src1, int BBindex1, int BBindex2)
{
	SSA::Operand cond = SSA::Operand(SSA::Operand::temp, tmp);
	b.addInstruction(new SSA::CmpInstruction(cond, SSA::Operand(SSA::Operand::stack, src1), SSA::Operand(SSA::Operand::constant, 0)));
	b.addInstruction(new SSA::CondBranchInstruction(opcode, cond, SSA::Branch(BBindex1), SSA::Branch(BBindex2)));
	b.addSucc(BBindex1);
	b.addSucc(BBindex2);
	BBs[BBindex1].addPred(b.getIndex());
	BBs[BBindex2].addPred(b.getIndex());
}

int SSAgenerator::call(BasicBlock & b, int constPoolIndex)
{
	//get name of method and add object to the basic block
	ConstPoolEntry* methodRef = constPool[constPoolIndex];
	ConstPoolEntry* nameType = constPool[methodRef->getNameTypeIndex()];
	ConstPoolEntry* strRef = constPool[nameType->getNameIndex()];
	Method m = getMethod(strRef->getStr());
	b.addInstruction(new SSA::CallInstruction(m));
	//descriptor format (param1param2...paramN)return
	//e.g. (II)V takes to ints and returns void
	std::string descriptor = m.getDescriptor();
	//number of params is number of chars between '(' and ')'
	//main method has string as arg, we will ignore main's args
	//in a fully functioning JVM we would need to deal with types other than int(I)
	int stackOffset = m.getName() == "main" ? 0 : -(descriptor.find(')') - descriptor.find('(') - 1);
	//return type is last character of descriptor
	char returnType = descriptor[descriptor.length() - 1];
	if (returnType == 'I')
	{
		stackOffset += 1;
	}
	return stackOffset;
}

Method SSAgenerator::getMethod(std::string name)
{
	for (Method m : methods)
	{
		if (m.getName() == name)
		{
			return m;
		}
	}
	printf("method with name \"%s\" does not exist\n", name.c_str());
	exit(1);
}

int SSAgenerator::getBBindex(std::vector<BasicBlock> BBs, int line)
{
	for (int i = 0; i < BBs.size(); ++i)
	{
		if (BBs[i].getStartLine() == line) {
			return i;
		}
	}
	printf("no basic block exists with starting line number %d\n", line);
	return -1;
}

bool SSAgenerator::dominates(std::vector<BasicBlock>& BBs, BasicBlock & X, BasicBlock & Y)
{
	return dominates(BBs, std::list<int>(), BBs[0], X, Y);
}

bool SSAgenerator::dominates(std::vector<BasicBlock>& BBs, std::list<int>& visited, BasicBlock & entry, BasicBlock & X, BasicBlock & Y)
{
	int entryIndex = entry.getIndex();
	// base case 1: if reached X without reaching Y OR we already visited entry,
	// X MIGHT dominate Y, do not continue along this path
	// this needs to be base case 1 because if entry == X == Y, we want X to dominate Y
	if (entry == X || std::find(visited.cbegin(), visited.cend(), entryIndex) != visited.cend())
	{
		return true;
	}
	// if all paths from entry to Y must path through X, then X dominates Y
	// base case 2: if reached Y without reaching X, X does NOT dominate Y
	else if (entry == Y)
	{
		return false;
	}
	std::list<int> entrySucc = entry.getSucc();
	visited.push_back(entryIndex);
	//recursive case: go through paths for each successor, if one of them returns false
	//then there is a path to Y that does NOT pass through X, and X does NOT dominate Y
	for(std::list<int>::const_iterator i = entrySucc.begin(); i != entrySucc.end(); ++i)
	{
		if (!dominates(BBs, visited, BBs[*i], X, Y))
		{
			return false;
		}
	}
	return true;
}

void SSAgenerator::dominanceFrontier(std::list<int>& df, std::vector<BasicBlock>& BBs, BasicBlock & d)
{
	for (BasicBlock& BB : BBs)
	{
		if (inDominanceFrontier(BBs, d, BB))
		{
			df.push_back(BB.getIndex());
		}
	}
}

bool SSAgenerator::inDominanceFrontier(std::vector<BasicBlock>& BBs, BasicBlock & d, BasicBlock & n)
{
	//return false if d dominates n or d==n because if d strictly dominates n, n is not in DF(d)
	if(dominates(BBs, d, n) || d == n)
	{
		return false;
	}
	//return true if d dominates a predecessor of n
	for(int i : n.getPred())
	{
		if (dominatePred(BBs, std::list<int>(), d, BBs[i]))
		{
			return true;
		}
	}
	return false;
}

bool SSAgenerator::dominatePred(std::vector<BasicBlock>& BBs, std::list<int> visited, BasicBlock & d, BasicBlock & n)
{
	int nIndex = n.getIndex();
	//base case 1: if d dominates n, return true
	if(dominates(BBs, d, n))
	{
		return true;
	}
	//base case 2: if already visited n, return false
	else if (std::find(visited.cbegin(), visited.cend(), nIndex) != visited.cend())
	{
		return false;
	}
	//go through predecessors and check if d dominates any of them
	visited.push_back(n.getIndex());
	for(int i : n.getPred())
	{
		if (dominatePred(BBs, visited, d, BBs[i]))
		{
			return true;
		}
	}
	return false;
}

void SSAgenerator::getVars(std::list<SSA::Operand>& vars, std::vector<BasicBlock>& BBs)
{
	for(BasicBlock BB : BBs)
	{
		for (SSA::Instruction* instruction : BB.getInstructions())
		{
			//only add to var list if its not already in it
			SSA::Operand dest = instruction->getDest();
			if(dest.getType() != SSA::Operand::null_op && std::find(vars.cbegin(), vars.cend(), dest) == vars.cend())
			{
				vars.push_back(SSA::Operand(dest));
			}
		}
	}
}

bool SSAgenerator::containsAssignment(BasicBlock & BB, SSA::Operand o)
{
	for (SSA::Instruction* instruction : BB.getInstructions())
	{
		if (instruction->getDest() == o)
		{
			return true;
		}
	}
	return false;
}

SSAgenerator::~SSAgenerator()
{
}

void SSAgenerator::generate()
{
	for (Method m : methods)
	{
		if (m.getName() != "<init>")
		{
			createBasicBlocks(m);
		}
	}
}

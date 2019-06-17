#include "SSAInstruction.h"



SSA::Instruction::Instruction()
{
}


SSA::Instruction::~Instruction()
{
}

SSA::Operand* SSA::Instruction::getDest() const
{
	return new Operand(Operand::null_op, 0);
}

SSA::Operand* SSA::Instruction::getSrc1() const
{
	return new Operand(Operand::null_op, 0);
}

SSA::Operand* SSA::Instruction::getSrc2() const
{
	return new Operand(Operand::null_op, 0);
}

std::vector<SSA::Operand*> SSA::Instruction::getSrcs() const
{
	return std::vector<Operand*>();
}

void SSA::Instruction::setDest(OperandUse op)
{
}

void SSA::Instruction::setSrc1(OperandUse op)
{
}

void SSA::Instruction::setSrc2(OperandUse op)
{
}

void SSA::Instruction::setSrcs(std::vector<OperandUse> srcs)
{
}

void SSA::Instruction::updatePhiSrc(int bb, OperandUse op)
{
}

void SSA::Instruction::renamePhiSrc(int bb, OperandUse op)
{
}

bool SSA::Instruction::isPhi() const
{
	return false;
}

std::list<std::pair<int, SSA::Operand*>> SSA::Instruction::getPhiSrcs()
{
	return std::list<std::pair<int, Operand*>>();
}

std::string SSA::Instruction::getMethodName()
{
	return std::string();
}

SSA::Branch SSA::Instruction::getBranch()
{
	return Branch(-1);
}

bool SSA::Instruction::getIsVoid() const
{
	return true;
}

SSA::Operand::Operand(const Operand & other)
{
	opType = other.opType;
	val = other.val;
}

bool SSA::Operand::operator==(const Operand & other) const
{
	return this->opType == other.opType && this->val == other.val;
}

std::string SSA::Operand::getStr() const
{
	switch (opType)
	{
	case stack:
		return "stack" + std::to_string(val);
	case local:
		return "local" + std::to_string(val);
	case constant:
		return "const" + std::to_string(val);
	case temp:
		return "tmp" + std::to_string(val);
	case null_op:
		return "null" + std::to_string(val);
	}
}
SSA::Operand::OpType SSA::Operand::getType() const
{
	return opType;
}

int SSA::Operand::getVal() const
{
	return val;
}

bool SSA::Operand::isConst() const
{
	return opType == constant;
}

bool SSA::Operand::isVar() const
{
	return opType == stack || opType == local/* || opType == temp*/;
}

bool SSA::Operand::isUseVar() const
{
	return opType == stack || opType == local;
}

int SSA::Operand::getUse() const
{
	return -1;
}

bool SSA::Operand::sameUse(Operand* other) const
{
	return *this == *other && getUse() == other->getUse();
}

std::string SSA::Branch::getStr() const
{
	return "#" + std::to_string(targetBasicBlock);
}

int SSA::Branch::getTargetBB() const
{
	return targetBasicBlock;
}

std::string SSA::MovInstruction::getStr() const
{
	return "MOV " + dest->getStr() + ", " + src->getStr();
}

SSAopcode SSA::MovInstruction::getSSAopcode() const
{
	return MOV;
}

SSA::Operand* SSA::MovInstruction::getDest() const
{
	return new OperandUse(dest);
}

SSA::Operand* SSA::MovInstruction::getSrc1() const
{
	return new OperandUse(src);
}

std::vector<SSA::Operand*> SSA::MovInstruction::getSrcs() const
{
	std::vector<Operand*> l = { src };
	return l;
}

void SSA::MovInstruction::setDest(OperandUse op)
{
	delete dest;
	dest = new OperandUse(op);
}

void SSA::MovInstruction::setSrc1(OperandUse op)
{
	delete src;
	src = new OperandUse(op);
}

void SSA::MovInstruction::setSrcs(std::vector<OperandUse> srcs)
{
	if (srcs.size() != 1)
	{
		throw 0;
	}
	delete src;
	src = new OperandUse(srcs[0]);
}

std::string SSA::UnaryInstruction::getStr() const
{
	std::string right = " " + dest->getStr() + ", " + src->getStr();
	switch(opcode)
	{
		case inc:
			return "INC" + right;
		case dec:
			return "DEC" + right;
	}
}

SSAopcode SSA::UnaryInstruction::getSSAopcode() const
{
	switch(opcode)
	{
		case inc:
			return INC;
		case dec:
			return DEC;
	}
}

SSA::Operand* SSA::UnaryInstruction::getDest() const
{
	return new OperandUse(dest);
}

SSA::Operand* SSA::UnaryInstruction::getSrc1() const
{
	return new OperandUse(src);
}

std::vector<SSA::Operand*> SSA::UnaryInstruction::getSrcs() const
{
	std::vector<Operand*> l = { src };
	return l;
}

void SSA::UnaryInstruction::setDest(OperandUse op)
{
	delete dest;
	dest = new OperandUse(op);
}

void SSA::UnaryInstruction::setSrc1(OperandUse op)
{
	delete src;
	src = new OperandUse(op);
}

void SSA::UnaryInstruction::setSrcs(std::vector<OperandUse> srcs)
{
	if (srcs.size() != 1)
	{
		throw 0;
	}
	delete src;
	src = new OperandUse(srcs[0]);
}

std::string SSA::BinaryInstruction::getStr() const
{
	std::string right = " " + dest->getStr() + ", " + src1->getStr() + ", " + src2->getStr();
	switch(opcode)
	{
	case add:
		return "ADD" + right;
	case sub:
		return "SUB" + right;
	case mul:
		return "MUL" + right;
	case div:
		return "DIV" + right;
	case shl:
		return "SHL" + right;
	case shr:
		return "SHR" + right;
	}
}

SSAopcode SSA::BinaryInstruction::getSSAopcode() const
{
	switch(opcode)
	{
	case add:
		return ADD;
	case sub:
		return SUB;
	case mul:
		return MUL;
	case div:
		return DIV;
	case shl:
		return SHL;
	case shr:
		return SHR;
	}
}

SSA::Operand* SSA::BinaryInstruction::getDest() const
{
	return new OperandUse(dest);
}

SSA::Operand* SSA::BinaryInstruction::getSrc1() const
{
	return new OperandUse(src1);
}

SSA::Operand* SSA::BinaryInstruction::getSrc2() const
{
	return new OperandUse(src2);
}

std::vector<SSA::Operand*> SSA::BinaryInstruction::getSrcs() const
{
	std::vector<Operand*> l = { src1, src2 };
	return l;
}

void SSA::BinaryInstruction::setDest(OperandUse op)
{
	delete dest;
	dest = new OperandUse(op);
}

void SSA::BinaryInstruction::setSrc1(OperandUse op)
{
	delete src1;
	src1 = new OperandUse(op);
}

void SSA::BinaryInstruction::setSrc2(OperandUse op)
{
	delete src2;
	src2 = new OperandUse(op);
}

void SSA::BinaryInstruction::setSrcs(std::vector<OperandUse> srcs)
{
	if (srcs.size() != 2)
	{
		throw 0;
	}
	delete src1;
	delete src2;
	src1 = new OperandUse(srcs[0]);
	src2 = new OperandUse(srcs[1]);
}


std::string SSA::CmpInstruction::getStr() const
{
	return "CMP " + cond->getStr() + ", " + src1->getStr() + ", " + src2->getStr();
}

SSAopcode SSA::CmpInstruction::getSSAopcode() const
{
	return CMP;
}

SSA::Operand* SSA::CmpInstruction::getDest() const
{
	return new OperandUse(cond);
}

SSA::Operand* SSA::CmpInstruction::getSrc1() const
{
	return new OperandUse(src1);
}

SSA::Operand* SSA::CmpInstruction::getSrc2() const
{
	return new OperandUse(src2);
}

std::vector<SSA::Operand*> SSA::CmpInstruction::getSrcs() const
{
	std::vector<Operand*> l = { src1, src2 };
	return l;
}

void SSA::CmpInstruction::setDest(OperandUse op)
{
	delete cond;
	cond = new OperandUse(op);
}

void SSA::CmpInstruction::setSrc1(OperandUse op)
{
	delete src1;
	src1 = new OperandUse(op);
}

void SSA::CmpInstruction::setSrc2(OperandUse op)
{
	delete src2;
	src2 = new OperandUse(op);
}

void SSA::CmpInstruction::setSrcs(std::vector<OperandUse> srcs)
{
	if (srcs.size() != 2)
	{
		throw 0;
	}
	delete src1;
	delete src2;
	src1 = new OperandUse(srcs[0]);
	src2 = new OperandUse(srcs[1]);
}

SSA::CondBranchInstruction::CondBranchInstruction(Opcode opcode, Operand * cond, Branch branch1, Branch branch2) : opcode(opcode), cond(cond), branch1(branch1), branch2(branch2)
{
		int x = 0;
		int y = 0;
}

std::string SSA::CondBranchInstruction::getStr() const
{
	std::string s = "BR";
	switch (opcode)
	{
		case eq:
			s += "_EQ ";
			break;
		case ne:
			s += "_NE ";
			break;
		case lt:
			s += "_LT ";
			break;
		case lte:
			s += "_LTE ";
			break;
		case gt:
			s += "_GT ";
			break;
		case gte:
			s += "_GTE ";
			break;
	}
	s += cond->getStr() + ", " + branch1.getStr() + ", " + branch2.getStr();
	return s;
}

SSAopcode SSA::CondBranchInstruction::getSSAopcode() const
{
	switch (opcode)
	{
		case eq:
			return JE;
		case ne:
			return JNE;
		case lt:
			return JL;
		case lte:
			return JLE;
		case gt:
			return JG;
		case gte:
			return JGE;
	}
}

SSA::Operand* SSA::CondBranchInstruction::getSrc1() const
{
	return new OperandUse(cond);
}

std::vector<SSA::Operand*> SSA::CondBranchInstruction::getSrcs() const
{
	std::vector<Operand*> l = { cond };
	return l;
}

void SSA::CondBranchInstruction::setSrc1(OperandUse op)
{
	delete cond;
	cond = new OperandUse(op);
}

void SSA::CondBranchInstruction::setSrcs(std::vector<OperandUse> srcs)
{
	if (srcs.size() != 1)
	{
		throw 0;
	}
	delete cond;
	cond = new OperandUse(srcs[0]);
}

SSA::Branch SSA::CondBranchInstruction::getBranch()
{
	return branch2;
}

std::string SSA::UncondBranchInstruction::getStr() const
{
	return "BRUNCOND " + branch.getStr();
}

SSAopcode SSA::UncondBranchInstruction::getSSAopcode() const
{
	return JMP;
}

SSA::Branch SSA::UncondBranchInstruction::getBranch()
{
	return branch;
}

std::string SSA::CallInstruction::getStr() const
{
	std::string s = "CALL " + methodName;
	if (args.size() > 0)
	{
		s += "(";
		for (std::list<Operand*>::const_iterator iter = args.cbegin(); iter != args.cend(); ++iter)
		{
			if (iter != args.cbegin())
				s += ", ";
			s += (*iter)->getStr();
		}
		s += ")";
	}
	return s;
}

std::string SSA::CallInstruction::getMethodName()
{
	return methodName;
}

SSAopcode SSA::CallInstruction::getSSAopcode() const
{
	return CALL;
}

SSA::Operand * SSA::CallInstruction::getDest() const
{
	return new SSA::OperandUse(dest);
}

std::vector<SSA::Operand*> SSA::CallInstruction::getSrcs() const
{
	std::vector<Operand*> v;
	for (Operand* arg : args)
	{
		v.push_back(new OperandUse(arg));
	}
	return v;
}

void SSA::CallInstruction::setSrcs(std::vector<OperandUse> srcs)
{
	if (srcs.size() != args.size())
	{
		throw 0;
	}
	int i = 0;
	for (Operand*& arg : args)
	{
		delete arg;
		arg = new OperandUse(srcs[i]);
		++i;
	}
}

bool SSA::CallInstruction::getIsVoid() const
{
	return isVoid;
}

std::string SSA::ReturnInstruction::getStr() const
{
	if (isVoid)
		return "RETURN";
	return "RETURN " + src->getStr();
}

SSAopcode SSA::ReturnInstruction::getSSAopcode() const
{
	return RET;
}

SSA::Operand* SSA::ReturnInstruction::getSrc1() const
{
	return new OperandUse(src);
}

std::vector<SSA::Operand*> SSA::ReturnInstruction::getSrcs() const
{
	std::vector<Operand*> l = { src };
	return l;
}

void SSA::ReturnInstruction::setSrc1(OperandUse op)
{
	delete src;
	src = new OperandUse(op);
}

void SSA::ReturnInstruction::setSrcs(std::vector<OperandUse> srcs)
{
	if (srcs.size() != 1)
	{
		throw 0;
	}
	delete src;
	src = new OperandUse(srcs[0]);
}

bool SSA::ReturnInstruction::getIsVoid() const
{
	return isVoid;
}

std::string SSA::PhiInstruction::getStr() const
{
	std::string s = "PHI " + op->getStr();
	for (std::pair<int, Operand*> src : srcs)
	{
		s += ", (BB#" + std::to_string(src.first) + ", " + src.second->getStr() + ")";
	}
	return s;
}

SSAopcode SSA::PhiInstruction::getSSAopcode() const
{
	return PHI;
}

SSA::Operand* SSA::PhiInstruction::getDest() const
{
	return new OperandUse(op);
}

void SSA::PhiInstruction::setDest(OperandUse op)
{
	delete this->op;
	this->op = new OperandUse(op);
}

void SSA::PhiInstruction::updatePhiSrc(int bb, OperandUse op)
{
	for (std::pair<int, Operand*>& src : srcs)
	{
		if (bb == src.first)
		{
			delete src.second;
			src.second = new OperandUse(op);
			return;
		}
	}
	srcs.push_back(std::pair<int, Operand*>(bb, new OperandUse(op)));
}

void SSA::PhiInstruction::renamePhiSrc(int bb, OperandUse op)
{
	for (std::pair<int, Operand*>& src : srcs)
	{
		if (bb == src.first)
		{
			delete src.second;
			src.second = new OperandUse(op);
		}
	}
}

bool SSA::PhiInstruction::isPhi() const
{
	return true;
}

std::list<std::pair<int, SSA::Operand*>> SSA::PhiInstruction::getPhiSrcs()
{
	return srcs;
}

int SSA::OperandUse::getUse() const
{
	return use;
}

//bool SSA::OperandUse::sameUse(Operand * other) const
//{
//	return *this == *other && use == other->getUse();
//}

SSA::OperandUse::OperandUse(const OperandUse & other) : Operand(other)
{
	this->use = other.use;
}

SSA::OperandUse::OperandUse(const Operand & other) : Operand(other)
{
	use = -1;
}

bool SSA::OperandUse::operator==(const OperandUse& other) const
{
	return Operand::operator==(other) && use == other.use;
}

bool SSA::OperandUse::operator==(const Operand * other) const
{
	return Operand::operator==(*other) && use == other->getUse();
}


bool SSA::OperandUse::operator<(const OperandUse & other) const
{
	if (getType() < other.getType())
	{
		return true;
	}
	else if (getVal() < other.getVal())
	{
		return true;
	}
	return use < other.use;
}

std::string SSA::OperandUse::getStr() const
{
	if (use < 0)
		return Operand::getStr();
	return Operand::getStr() + "_" + std::to_string(use);
}

//std::string SSA::Method::getName()
//{
//	return name;
//}
//
//std::vector<BasicBlock> SSA::Method::getBasicBlocks()
//{
//	return bbs;
//}

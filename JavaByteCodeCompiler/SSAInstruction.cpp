#include "SSAInstruction.h"



SSA::Instruction::Instruction()
{
}


SSA::Instruction::~Instruction()
{
}

SSA::Operand SSA::Instruction::getDest() const
{
	return Operand(Operand::null_op, 0);
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
	switch(opType)
	{
	case stack:
		return "stack" + std::to_string(val);
	case local:
		return "local" + std::to_string(val);
	case constant:
		return "const" + std::to_string(val);
	case temp:
		return "temp" + std::to_string(val);
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

std::string SSA::Branch::getStr() const
{
	return "#" + std::to_string(targetBasicBlock);
}

std::string SSA::MovInstruction::getStr() const
{
	return "MOV " + dest.getStr() + ", " + op.getStr();
}

SSA::Operand SSA::MovInstruction::getDest() const
{
	return Operand(dest);
}

std::string SSA::UnaryInstruction::getStr() const
{
	std::string right = " " + dest.getStr() + ", " + op.getStr();
	switch(opcode)
	{
		case inc:
			return "INC" + right;
		case dec:
			return "DEC" + right;
	}
}

SSA::Operand SSA::UnaryInstruction::getDest() const
{
	return Operand(dest);
}

std::string SSA::BinaryInstruction::getStr() const
{
	std::string right = " " + dest.getStr() + ", " + op1.getStr() + ", " + op2.getStr();
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

SSA::Operand SSA::BinaryInstruction::getDest() const
{
	return Operand(dest);
}

std::string SSA::CmpInstruction::getStr() const
{
	return "CMP " + cond.getStr() + ", " + op1.getStr() + ", " + op2.getStr();
}

std::string SSA::CondBranchInstruction::getStr() const
{
	return "BRCOND " + cond.getStr() + ", " + branch1.getStr() + ", " + branch2.getStr();
}

std::string SSA::UncondBranchInstruction::getStr() const
{
	return "BRUNCOND " + branch1.getStr();
}

std::string SSA::CallInstruction::getStr() const
{
	return "CALL " + m.getName();
}

std::string SSA::ReturnInstruction::getStr() const
{
	if (isVoid)
		return "RETURN";
	return "RETURN " + op.getStr();
}

std::string SSA::PrintInstruction::getStr() const
{
	return "PRINT " + op.getStr();
}

std::string SSA::PhiInstruction::getStr() const
{
	return "Phi " + op.getStr();
}

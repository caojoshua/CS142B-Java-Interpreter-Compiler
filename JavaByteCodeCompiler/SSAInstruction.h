
#ifndef SSAINSTRUCTION_H
#define SSAINSTRUCTION_H

#include "Method.h"
#include <string>
#include <vector>

namespace SSA
{
	class Operand
	{
	public:
		enum OpType { stack, local, constant, temp, null_op };
	private:
		OpType opType;
		int val;
	public:
		Operand(OpType opType, int val) : opType(opType), val(val) {}
		Operand(const Operand& other);
		bool operator==(const Operand& other) const;
		std::string getStr() const;
		OpType getType() const;
		int getVal() const;
	};

	class Branch
	{
	private:
		//could make this be a Basic Block instead of index of basic block
		int targetBasicBlock;
	public:
		Branch(int targetBasicBlock) : targetBasicBlock(targetBasicBlock) {}
		std::string getStr() const;
	};

	class Instruction
	{
	public:
		Instruction();
		~Instruction();
		virtual std::string getStr() const = 0;
		virtual Operand getDest() const;
	};

	class MovInstruction : public Instruction
	{
	private:
		Operand dest;
		Operand op;
	public:
		MovInstruction(Operand op1, Operand op2) : dest(op1), op(op2) {}
		std::string getStr() const;
		Operand getDest() const;
	};

	class UnaryInstruction : public Instruction
	{
	public:
		enum Opcode { inc, dec };
	private:
		Opcode opcode;
		Operand dest;
		Operand op;
	public:
		UnaryInstruction(Opcode opcode, Operand dest, Operand op2) : opcode(opcode), dest(dest), op(op2) {}
		std::string getStr() const;
		Operand getDest() const;
	};

	class BinaryInstruction : public Instruction
	{
	public:
		enum Opcode { add, sub, mul, div, shl, shr };
	private:
		Opcode opcode;
		Operand dest;
		Operand op1;
		Operand op2;
	public:
		BinaryInstruction(Opcode opcode, Operand dest, Operand op2, Operand op3) : opcode(opcode), dest(dest), op1(op2), op2(op3) {}
		std::string getStr() const;
		Operand getDest() const;
	};

	class CmpInstruction : public Instruction
	{
	private:
		Operand cond;
		Operand op1;
		Operand op2;
	public:
		CmpInstruction(Operand cond, Operand op1, Operand op2) : cond(cond), op1(op1), op2(op2) {}
		std::string getStr() const;
	};

	class CondBranchInstruction : public Instruction
	{
	public:
		enum Opcode { eq, ne, le, lte, gt, gte};
	private:
		Opcode opcode;
		Operand cond;
		Branch branch1;
		Branch branch2;
	public:
		CondBranchInstruction(Opcode opcode, Operand cond, Branch branch1, Branch branch2) : opcode(opcode), cond(cond), branch1(branch1), branch2(branch2) {}
		std::string getStr() const;
	};

	class UncondBranchInstruction : public Instruction
	{
	private:
		Branch branch1;
	public:
		UncondBranchInstruction(Branch branch1) : branch1(branch1) {}
		std::string getStr() const;
	};

	class CallInstruction : public Instruction
	{
	private:
		Method m;
	public:
		CallInstruction(Method& m) : m(m) {}
		std::string getStr() const;
	};

	class ReturnInstruction : public Instruction
	{
	private:
		bool isVoid;
		Operand op;
	public:
		ReturnInstruction() : isVoid(true), op(Operand(Operand::constant, 0)) {}
		ReturnInstruction(Operand op) : isVoid(false), op(op) {}
		std::string getStr() const;
	};

	//not really part of SSA, but works for this class
	class PrintInstruction : public Instruction
	{
	private:
		Operand op;
	public:
		PrintInstruction(Operand op) : op(op) {}
		std::string getStr() const;
	};

	//TODO this class
	class PhiInstruction : public Instruction
	{
	private:
		Operand op;
	public:
		PhiInstruction(Operand op) : op(op) {}
		std::string getStr() const;
	};
}


#endif

#include "Interpreter.h"

//PRIVATE

int Interpreter::interpret(Method m, std::vector<int> args)
{
	//set variables that will be used throughout interpretting
	int byteCodeSize = m.getByteCodeSize();
	Frame frame = Frame(m.getMaxLocals());
	int i = 0;
	//set frame local variables to args
	for (int argIndex = 0; argIndex < args.size(); ++argIndex)
	{
		frame.setLocalVar(argIndex, args[argIndex]);
	}
	while(i < m.getByteCodeSize())
	{
		uint8_t byteCode = m.getByte(i++);
		switch (byteCode)
		{
			//loading and storing
			case iconst_m1_b:
				iconst(frame, -1);
				break;
			case iconst_0_b:
				iconst(frame, 0);
				break;
			case iconst_1_b:
				iconst(frame, 1);
				break;
			case iconst_2_b:
				iconst(frame, 2);
				break;
			case iconst_3_b:
				iconst(frame, 3);
				break;
			case iconst_4_b:
				iconst(frame, 4);
				break;
			case iconst_5_b:
				iconst(frame, 5);
				break;
			case iload_b:
				iload(frame, m.getByte(i++));
				break;
			case iload0_b:
				iload(frame, 0);
				break;
			case iload1_b:
				iload(frame, 1);
				break;
			case iload2_b:
				iload(frame, 2);
				break;
			case iload3_b:
				iload(frame, 3);
				break;
			case istore_b:
				istore(frame, m.getByte(i++));
				break;
			case istore_0_b:
				istore(frame, 0);
				break;
			case istore_1_b:
				istore(frame, 1);
				break;
			case istore_2_b:
				istore(frame, 2);
				break;
			case istore_3_b:
				istore(frame, 3);
				break;
			//arithmetic
			case iadd_b:
				iadd(frame);
				break;
			case iinc_b:
				iinc(frame, m.getByte(i), m.getByte(i+1));
				i += 2;
				break;
			case isub_b:
				isub(frame);
				break;
			case imul_b:
				imul(frame);
				break;
			case idiv_b:
				idiv(frame);
				break;
			case ishl_b:
				ishl(frame);
				break;
			case ishr_b:
				ishr(frame);
				break;
			//control transfer
			case if_icmpne_b:
				i += if_icmpne(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			case if_icmpeq_b:
				i += if_icmpeq(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			case if_icmpgt_b:
				i += if_icmpgt(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			case if_icmpge_b:
				i += if_icmpge(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			case if_icmplt_b:
				i += if_icmplt(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			case if_icmple_b:
				i += if_icmple(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			case ifeq_b:
				i += ifeq(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			case ifne_b:
				i += ifne(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			case ifgt_b:
				i += ifgt(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			case ifge_b:
				i += ifge(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			case iflt_b:
				i += iflt(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			case ifle_b:
				i += ifle(frame) ? m.getTwoByte(i) - 1 : 2;
				break;
			//other
			case goto_b:
				i += m.getTwoByte(i) - 1;
				break;
			case bipush_b:
				bipush(frame, m.getByte(i++));
				break;
			//need to make this actually call the method
			case invokestatic_b:
				invokestatic(frame, m.getTwoByte(i));
				i += 2;
				break;
			case invokevirtual_b:
				printf("%i\n", frame.pop());
				i += 2;
				break;
			case return_b:
				return 0;
			case ireturn_b:
				return frame.pop();
			//skip over these
			case getstatic_b:
				i += 2;
				break;
			default:
				printf("unknown byte code 0x%x\n", byteCode);
		}
	}
	return 0;
}

Method Interpreter::getMethod(std::string name)
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


void Interpreter::iconst(Frame& f, int val)
{
	f.push(val);
}

void Interpreter::iload(Frame & f, uint8_t index)
{
	f.push(f.getLocalVar(index));
}

void Interpreter::istore(Frame & f, uint8_t index)
{
	f.setLocalVar(index, f.pop());
}

void Interpreter::iadd(Frame & f)
{
	f.push(f.pop() + f.pop());
}

void Interpreter::iinc(Frame & f, uint8_t index, int8_t value)
{
	f.setLocalVar(index, f.getLocalVar(index) + value);
}

void Interpreter::isub(Frame & f)
{
	std::pair<int, int> pair = f.popTwo();
	f.push(pair.first - pair.second);
}

void Interpreter::imul(Frame & f)
{
	f.push(f.pop() * f.pop());
}

void Interpreter::idiv(Frame & f)
{
	std::pair<int, int> pair = f.popTwo();
	f.push(pair.first / pair.second);
}

void Interpreter::ishr(Frame & f)
{
	std::pair<int, int> pair = f.popTwo();
	f.push(pair.first >> pair.second);
}

void Interpreter::ishl(Frame & f)
{
	std::pair<int, int> pair = f.popTwo();
	f.push(pair.first << pair.second);
}

bool Interpreter::if_icmpne(Frame & f)
{
	return f.pop() != f.pop();
}

bool Interpreter::if_icmpeq(Frame & f)
{
	return f.pop() == f.pop();
}

bool Interpreter::if_icmpgt(Frame & f)
{
	std::pair<int, int> pair = f.popTwo();
	return pair.first > pair.second;
}

bool Interpreter::if_icmpge(Frame & f)
{
	std::pair<int, int> pair = f.popTwo();
	return pair.first >= pair.second;
}

bool Interpreter::if_icmplt(Frame & f)
{
	std::pair<int, int> pair = f.popTwo();
	return pair.first < pair.second;
}

bool Interpreter::if_icmple(Frame & f)
{
	std::pair<int, int> pair = f.popTwo();
	return pair.first <= pair.second;
}

bool Interpreter::ifeq(Frame & f)
{
	return f.pop() == 0;
}

bool Interpreter::ifne(Frame& f)
{
	return f.pop() != 0;
}

bool Interpreter::ifgt(Frame & f)
{
	return f.pop() > 0;
}

bool Interpreter::ifge(Frame & f)
{
	return f.pop() >= 0;
}

bool Interpreter::iflt(Frame & f)
{
	return f.pop() < 0;
}

bool Interpreter::ifle(Frame & f)
{
	return f.pop() <= 0;
}

void Interpreter::bipush(Frame & f, uint8_t val)
{
	f.push(val);
}

void Interpreter::invokestatic(Frame & f, uint16_t constPoolIndex)
{
	//get the name of the method and the associated method ibject
	ConstPoolEntry* methodRef = constPool[constPoolIndex];
	ConstPoolEntry* nameType = constPool[methodRef->getNameTypeIndex()];
	ConstPoolEntry* strRef = constPool[nameType->getNameIndex()];
	Method m = getMethod(strRef->getStr());
	//descriptor format (param1param2...paramN)return
	//e.g. (II)V takes to ints and returns void
	std::string descriptor = m.getDescriptor();
	//number of params is number of chars between '(' and ')'
	//main method has string as arg, we will ignore main's args
	//in a fully functioning JVM we would need to deal with types other than int(I)
	int numParams = m.getName() == "main" ? 0 :descriptor.find(')') - descriptor.find('(') - 1;
	//return type is last character of descriptor
	char returnType = descriptor[descriptor.length() - 1];
	int result = interpret(m, f.popN(numParams));
	if (returnType == 'I')
	{
		f.push(result);
	}
}


//PUBLIC

Interpreter::~Interpreter()
{
}

void Interpreter::interpret()
{
	interpret(getMethod("main"), std::vector<int>());
}


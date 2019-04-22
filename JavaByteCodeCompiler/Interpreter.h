
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Constants.h"
#include "ConstantPoolEntry.h"
#include "Method.h"
#include "Frame.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>


class Interpreter
{
private:
	std::vector<ConstPoolEntry*> constPool;
	std::vector<Method> methods;

	//interpret the given method
	int interpret(Method m, std::vector<int> args);

	//get the method with the given name
	Method getMethod(std::string name);

	//bytecode instructions
	//loading and storing
	void iconst(Frame& f, int val);
	void iload(Frame& f, uint8_t index);
	void istore(Frame&f, uint8_t index);
	//arithmetic
	void iadd(Frame& f);
	void iinc(Frame& f, uint8_t index, int8_t value);
	void isub(Frame& f);
	void imul(Frame& f);
	void idiv(Frame& f);
	void ishr(Frame& f);
	void ishl(Frame& f);
	//conditionals return the evaluated bool
	bool if_icmpne(Frame& f);
	bool if_icmpeq(Frame& f);
	bool if_icmpgt(Frame& f);
	bool if_icmpge(Frame& f);
	bool if_icmplt(Frame& f);
	bool if_icmple(Frame& f);
	bool ifeq(Frame& f);
	bool ifne(Frame& f);
	bool ifgt(Frame& f);
	bool ifge(Frame& f);
	bool iflt(Frame& f);
	bool ifle(Frame& f);
	//others
	void bipush(Frame& f, uint8_t val);
	void invokestatic(Frame&f, uint16_t constPoolIndex);

public:
	Interpreter(std::vector<ConstPoolEntry*> constPool, std::vector<Method> methods) :
		constPool(constPool), methods(methods) {/*TODO: deep copy these vectors*/};
	~Interpreter();

	//call this to begin interpretation, it will find "main" and interpret it
	void interpret();
};

#endif

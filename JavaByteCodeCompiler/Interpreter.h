
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ConstantPoolEntry.h"
#include "Tables.h"
#include <iostream>
#include <string>
#include <vector>


const uint8_t iconst_m1 = 0x02;
const uint8_t iconst_0 = 0x03;
const uint8_t iconst_1 = 0x04;
const uint8_t iconst_2 = 0x05;
const uint8_t iconst_3 = 0x06;	
const uint8_t iconst_4 = 0x07;
const uint8_t iconst_5 = 0x08;
const uint8_t iload = 0x15;
const uint8_t istore = 0x36;
const uint8_t iadd = 0x60;
const uint8_t if_icmpne = 0xa0;
const uint8_t if_cmpeq = 0x9f;
const uint8_t if_icompgt = 0xa3;
const uint8_t if_icomplt = 0xa1;
const uint8_t ifeq = 0x99;
const uint8_t ifne = 0x9a;
const uint8_t ifgt = 0x9d;
const uint8_t iflt = 0x9b;
const uint8_t _goto = 0xa7;
const uint8_t invokestatic = 0xb8;
const uint8_t _return = 0xb1;



class Interpreter
{
private:
	std::vector<ConstPoolEntry*> constPool;
	std::vector<Method> methods;

public:
	Interpreter(std::vector<ConstPoolEntry*> constPool, std::vector<Method> methods) :
		constPool(constPool), methods(methods) {/*TODO: deep copy these vectors*/};
	~Interpreter();

	void interpret();
};

#endif

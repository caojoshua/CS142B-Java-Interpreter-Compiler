
#ifndef PARSER_H
#define PARSER_H

#include "ConstantPoolEntry.h"
#include "Method.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <typeinfo>

class Parser
{
private:
	//constants
	static const int magicNumSize = 4;

	//data members
	std::ifstream f;
	unsigned char magicNum[magicNumSize];
	unsigned short minorVersion;
	unsigned short majorVersion;
	unsigned short constPoolCount;
	unsigned short accessFlags;
	unsigned short thisClass;
	unsigned short superClass;
	unsigned short interfaceCount;
	unsigned short fieldCount;
	unsigned short methodCount;
	unsigned short attributeCount;


	//functions

	//table parsing
	void parseConstPool();
	void parseFields();
	void parseMethods();

	//attr parsing
	void skipAttrList(unsigned int size);

	//read data types from input stream
	uint8_t readUInt8();
	unsigned char readUChar();
	unsigned short readUShort();
	unsigned int readUInt();
	int readInt();
	float readFloat();
	//might be busted
	long readLong();
	double readDouble();
	void readUChar(unsigned char* dest, int size);
	void skipBytes(int size);

	//parse constant table entries
	void parseStrEntry();
	void parseIntEntry();
	void parseFloatEntry();
	void parseLongEntry();
	void parseDoubleEntry();
	void parseClassRefEntry();
	void parseStrRefEntry();
	void parseClassNameTypeRefEntry();
	void parseNameTypeDescriptorEntry();

	
public:
	//table data members
	std::vector<ConstPoolEntry*> constPool;
	std::vector<Method> methods;

	Parser(std::string s);
	~Parser();
	void parse();
	void print();
};

#endif


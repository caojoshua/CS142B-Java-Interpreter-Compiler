
#ifndef CONSTANT_POOL_ENTRY_H
#define CONSTANT_POOL_ENTRY_H

#include "Constants.h"
#include <string>

union NumericalValue
{
	int i;
	float f;
	long l;
	double d;
};

class ConstPoolEntry
{
public:
	ConstPoolEntry();
	~ConstPoolEntry();
	virtual std::string getStr() { return std::string(); }
	virtual NumericalValue getNumericalValue() { return NumericalValue(); }
	virtual int getNameIndex() { return 0; }
	virtual int getTypeIndex() { return 0; }
	virtual int getClassRefIndex() { return 0; }
	virtual int getNameTypeIndex() { return 0; }
};

//put these in invalid indicies (e.g. index 0, second byte of double/long)
class NullEntry : public ConstPoolEntry {};

//only for constant strings
class StrEntry : public ConstPoolEntry
{
private:
	std::string s;
public:
	StrEntry(std::string s) : s(s) {}
	std::string getStr() { return s; }
};

//int, float, long, and double entries
class NumericalEntry : public ConstPoolEntry
{
private:
	NumericalValue value;
public:
	NumericalEntry(int i) { value.i = i; }
	NumericalEntry(float f) { value.f = f; }
	NumericalEntry(long l) { value.l = l; }
	NumericalEntry(double d) { value.d = d; }
	NumericalValue getValue() { return value; };
};

//string and class reference entries
//only has pointer to a string representing its name
class NameRefEntry : public ConstPoolEntry
{
private:
	int nameIndex;
public:
	NameRefEntry(int nameIndex) : nameIndex(nameIndex) {}
	int getNameIndex() { return nameIndex; }
};

//for field, method, and interface method references
//has index of a class ref and name type descriptor
class ClassNameTypeRefEntry : public ConstPoolEntry
{
private:
	int classRefIndex;
	int nameTypeIndex;
public:
	ClassNameTypeRefEntry(int classRefIndex, int nameTypeIndex) : classRefIndex(classRefIndex), nameTypeIndex(nameTypeIndex) {}
	int getClassRefIndex() { return classRefIndex; }
	int getNameTypeIndex() { return nameTypeIndex; }
};

//has index of a name and a type descriptor
class NameTypeDescriptorEntry : public ConstPoolEntry
{
private:
	int nameIndex;
	int typeDescriptorIndex;
public:
	NameTypeDescriptorEntry(int nameIndex, int typeDescriptorIndex) : nameIndex(nameIndex), typeDescriptorIndex(typeDescriptorIndex) {}
	int getNameIndex() { return nameIndex; }
	int getTypeDescriptorIndex() { return typeDescriptorIndex; }
};





#endif

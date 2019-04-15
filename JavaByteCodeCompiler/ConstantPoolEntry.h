
#ifndef CONSTANT_POOL_ENTRY_H
#define CONSTANT_POOL_ENTRY_H

#include <string>

//tab bytes for const pool entries we care about
const int strTag = 1;
const int intTag = 3;
const int floatTag = 4;
const int longTag = 5;
const int doubleTag = 6;
const int classRefTag = 7;
const int strRefTag = 8;
const int fieldRefTag = 9;
const int methodRefTag = 10;
const int interfaceMethodRefTag = 11;
const int nameTypeDescriptorTag = 12;

//tag bytes for const pool entries we will skip over
const int methodHandleTag = 15;
const int methodTypeTag = 16;
const int dynamicTag = 17;
const int invokeDynamicTag = 18;
const int moduleTag = 19;
const int packageTag = 20;

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
//has pointer to a class ref and name type descriptor
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




//chopping block, may use the classes above and remove these
class IntEntry : public ConstPoolEntry
{
private:
	int i;
public:
	IntEntry(int i) : i(i)
	{
	}
};

class FloatEntry : public ConstPoolEntry
{
private:
	float f;
public:
	FloatEntry(float f) : f(f)
	{
	}
};

class LongEntry : public ConstPoolEntry
{
private:
	long l;
public:
	LongEntry(long l) : l(l)
	{
	}
};


class DoubleEntry : public ConstPoolEntry
{
private:
	double d;
public:
	DoubleEntry(double d) : d(d)
	{
	}
};

class ClassRefEntry : public ConstPoolEntry
{
private:
	int index;
public:
	ClassRefEntry(int index) : index(index)
	{
	}
	int getNameIndex() { return index; }
};

class StrRefEntry : public ConstPoolEntry
{
private:
	int index;
public:
	StrRefEntry(int index) : index(index)
	{
	}
};

class FieldRefEntry : public ConstPoolEntry
{
private:
	int classRefIndex;
	int nameTypeIndex;
public:
	FieldRefEntry(int classRefIndex, int nameTypeIndex) : classRefIndex(classRefIndex), nameTypeIndex(nameTypeIndex)
	{
	}
};

class MethodRefEntry : public ConstPoolEntry
{
private:
	int classRefIndex;
	int nameTypeIndex;
public:
	MethodRefEntry(int classRefIndex, int nameTypeIndex) : classRefIndex(classRefIndex), nameTypeIndex(nameTypeIndex)
	{
	}
};

class InterfaceMethodRefEntry : public ConstPoolEntry
{
private:
	int classRefIndex;
	int nameTypeIndex;
public:
	InterfaceMethodRefEntry(int classRefIndex, int nameTypeIndex) : classRefIndex(classRefIndex), nameTypeIndex(nameTypeIndex)
	{
	}
};


#endif

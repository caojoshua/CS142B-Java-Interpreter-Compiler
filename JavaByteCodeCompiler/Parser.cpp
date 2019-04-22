
#include "Parser.h"

Parser::Parser(std::string s)
{
	printf("%s\n", s.c_str());
	f = std::ifstream(s, std::ios::in | std::ios::binary);
	constPool.push_back(new NullEntry());
}

Parser::~Parser()
{
	f.close();
	for (ConstPoolEntry* c : constPool)
	{
		delete c;
	}
}

void Parser::parse()
{
	if (f.is_open())
	{

		//magic number, bytes 0-3
		readUChar(magicNum, magicNumSize);

		//minor version, bytes 4-5
		minorVersion = readUShort();

		//major version, bytes 6-7
		majorVersion = readUShort();

		//constant pool size, bytes 8-9
		constPoolCount = readUShort();

		//constant pool bytes 10-(10+cpsize-1)
		parseConstPool();

		//access flags bytes (10+cpsize)-(11+cpsize)
		accessFlags = readUShort();
		
		//access flags bytes (12+cpsize)-(13+cpsize)
		thisClass = readUShort();
		
		//access flags bytes (14+cpsize)-(15+cpsize)
		superClass = readUShort();

		//access flags bytes (16+cpsize)-(17+cpsize)
		interfaceCount = readUShort();

		//interface table bytes bytes (18+cpsize)-(18+cpsize+isize-1)
		skipBytes(2 * interfaceCount);

		//field count bytes (18+cpsize+isize)-(19+cpsize+isize)
		fieldCount = readUShort();

		//TODO: test this on test case where field count > 0
		//field table bytes (20+cpsize+isize)-(20+cpsize+isize+fsize-1)
		parseFields();

		//method count bytes (20+cpsize+isize+fsize)-(21+cpsize+isize+fsize)
		methodCount = readUShort();

		//method table bytes (22+cpsize+isize+fsize)-(22+cpsize+isize+fsize+msize-1)
		parseMethods();

		//we don't need to parse attribute table, so we just stop here
	}
	else
	{
		std::cout << "file error\n";
		exit(1);
	}
}

void Parser::parseConstPool()
{
	unsigned char tagByte;
	for (short i = 0; i < constPoolCount - 1; ++i)
	{
		tagByte = readUChar();
		switch (tagByte)
		{
		case strTag:
			parseStrEntry();
			break;
		case intTag:
			parseIntEntry();
			break;
		case floatTag:
			parseFloatEntry();
			break;
		case longTag:
			parseLongEntry();
			i += 1;
			break;
		case doubleTag:
			parseDoubleEntry();
			i += 1;
			break;
		case classRefTag:
			parseClassRefEntry();
			break;
		case strRefTag:
			parseStrRefEntry();
			break;
		case fieldRefTag:
		case methodRefTag:
		case interfaceMethodRefTag:
			parseClassNameTypeRefEntry();
			break;
		case nameTypeDescriptorTag:
			parseNameTypeDescriptorEntry();
			break;
		//entries we will skip over
		case methodHandleTag:
		case dynamicTag:
		case invokeDynamicTag:
			skipBytes(4);
			break;
		case methodTypeTag:
		case moduleTag:
		case packageTag:
			skipBytes(2);
			break;
		default:
		printf("unrecognized tag byte '%d' while parsing const pool entry #%d\n", tagByte, i+1);
			exit(1);
			break;
		}
	}
}

void Parser::parseFields()
{
	for (int i = 0; i < fieldCount; ++i)
	{
		//access flags, name_index, descriptor_index
		skipBytes(6);
		unsigned short attrCount = readUShort();
		skipAttrList(attrCount);
	}
}

void Parser::parseMethods()
{
	for (int i = 0; i < methodCount; ++i)
	{
		unsigned short accessFlags = readUShort();
		unsigned short nameIndex = readUShort();
		unsigned short descriptorIndex = readUShort();
		unsigned short attrCount = readUShort();

		for (int i = 0; i < attrCount; ++i)
		{
			unsigned short attrNameIndex = readUShort();
			unsigned int attrLen = readUInt();
			std::string s = constPool[attrNameIndex]->getStr();
			if (s == "Code")
			{
				unsigned short maxStack = readUShort();
				unsigned short maxLocals = readUShort();
				unsigned int codeLen = readUInt();
				std::vector<uint8_t> byteCodes;
				//TODO: actually store code rather than run over it
				for (unsigned int i = 0; i < codeLen; ++i)
				{
					byteCodes.push_back(readUInt8());
				}
				//TODO: test where expTableLen > 0
				unsigned short expTableLen = readUShort();
				skipBytes(expTableLen * 8);
				unsigned short attrCount = readUShort();
				skipAttrList(attrCount);
				methods.push_back(Method(accessFlags, constPool[nameIndex]->getStr(), constPool[descriptorIndex]->getStr(), maxStack, maxLocals, byteCodes));
				break;
			}
			else
			{
				skipBytes(attrLen);
			}
		}
	}
}

void Parser::skipAttrList(unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i)
	{
		unsigned short attrNameIndex = readUShort();
		unsigned int attrLen = readUInt();
		skipBytes(attrLen);
	}
}

uint8_t Parser::readUInt8()
{
	char c;
	f.read(&c, 1);
	return (uint8_t) c;
}

unsigned char Parser::readUChar()
{
	unsigned char c;
	readUChar(&c, 1);
	return c;
}

unsigned short Parser::readUShort()
{
	unsigned char twoChar[2];
	readUChar(twoChar, 2);
	return (((short) twoChar[0]) << 8) | twoChar[1];
}

int Parser::readInt()
{
	char fourChar[4];
	f.read(fourChar, 4);
	fourChar[0] = (int)fourChar[0] << 24;
	fourChar[1] = (int)fourChar[1] << 16;
	fourChar[2] = (int)fourChar[2] << 8;
	return fourChar[0] | fourChar[1] | fourChar[2] | fourChar[3];
	return 0;
}

unsigned int Parser::readUInt()
{
	return (unsigned int)readInt();
}

float Parser::readFloat()
{
	char fourChar[4];
	f.read(fourChar, 4);
	return 0.0f;
}

long Parser::readLong()
{
	char c[8];
	f.read(c, 8);
	long l = (long)c[0];
	for (int i = 7; i > 0; --i)
	{
		l |= c[8 - i] << (i * 8);
	}
	return l;
}

double Parser::readDouble()
{
	char eightChar[8];
	f.read(eightChar, 8);
	return 0.0;
}

void Parser::readUChar(unsigned char* dest, int size)
{
	char* temp = new char[size];
	f.read(temp, size);
	memcpy(dest, temp, size);
	delete temp;
}

void Parser::skipBytes(int size)
{
	char* c = new char[size];
	f.read(c, size);
	delete c;
}

void Parser::parseStrEntry()
{
	short strLen = readUShort();
	char* c = new char[strLen + 1];
	f.read(c, strLen);
	*(c + strLen) = '\0';
	constPool.push_back(new StrEntry(std::string(c)));
	delete c;
}

void Parser::parseIntEntry()
{
	constPool.push_back(new NumericalEntry(readInt()));
}

void Parser::parseFloatEntry()
{
	constPool.push_back(new NumericalEntry(readFloat()));
}

void Parser::parseLongEntry()
{
	constPool.push_back(new NumericalEntry(readLong()));
	constPool.push_back(new NullEntry());
}

void Parser::parseDoubleEntry()
{
	constPool.push_back(new NumericalEntry(readDouble()));
	constPool.push_back(new NullEntry());
}

void Parser::parseClassRefEntry()
{
	constPool.push_back(new NameRefEntry(readUShort()));
}

void Parser::parseStrRefEntry()
{
	constPool.push_back(new NameRefEntry(readUShort()));
}

void Parser::parseClassNameTypeRefEntry()
{
	unsigned short classRef = readUShort();
	unsigned short nameTypeRef = readUShort();
	constPool.push_back(new ClassNameTypeRefEntry(classRef, nameTypeRef));
}

void Parser::parseNameTypeDescriptorEntry()
{
	unsigned short nameIndex = readUShort();
	unsigned short descriptorIndex = readUShort();
	constPool.push_back(new NameTypeDescriptorEntry(nameIndex, descriptorIndex));
}

void Parser::print()
{
	for (int i = 0; i < magicNumSize; ++i)
	{
		printf("%X", magicNum[i]);
	}
	printf(" \n");
	printf("minor version: %i\n", minorVersion);
	printf("major version: %d\n", majorVersion);
	printf("const pool count: %d\n", constPoolCount);
	printf("access flags: %i\n", accessFlags);
	printf("this class: %s\n", constPool[constPool[thisClass]->getNameIndex()]->getStr().c_str());
	printf("super clas: %s\n", constPool[constPool[superClass]->getNameIndex()]->getStr().c_str());
	printf("interface count: %d\n", interfaceCount);
	printf("field count: %d\n", fieldCount);
	printf("method count: %d\n", methodCount);
	for (Method m : methods)
	{
		printf("\t%s\n", m.getName().c_str());
	}
}


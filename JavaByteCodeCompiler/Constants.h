
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>

//constant pool tag bytes
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
const int methodHandleTag = 15;
const int methodTypeTag = 16;
const int dynamicTag = 17;
const int invokeDynamicTag = 18;
const int moduleTag = 19;
const int packageTag = 20;


//bytecode instructions
//"_b" means byte, make sure not to conflict with function names
//store and load
const uint8_t iconst_m1_b = 0x02;
const uint8_t iconst_0_b = 0x03;
const uint8_t iconst_1_b = 0x04;
const uint8_t iconst_2_b = 0x05;
const uint8_t iconst_3_b = 0x06;	
const uint8_t iconst_4_b = 0x07;
const uint8_t iconst_5_b = 0x08;
const uint8_t iload_b = 0x15;
const uint8_t iload0_b = 0x1a;
const uint8_t iload1_b = 0x1b;
const uint8_t iload2_b = 0x1c;
const uint8_t iload3_b = 0x1d;
const uint8_t istore_b = 0x36;
const uint8_t istore_0_b = 0x3b;
const uint8_t istore_1_b = 0x3c;
const uint8_t istore_2_b = 0x3d;
const uint8_t istore_3_b = 0x3e;
//arithmetic
const uint8_t iadd_b = 0x60;
const uint8_t iinc_b = 0x84;
const uint8_t isub_b = 0x64;
const uint8_t imul_b = 0x68;
const uint8_t idiv_b = 0x6c;
const uint8_t ishl_b = 0x78;
const uint8_t ishr_b = 0x7a;
//conditionals
const uint8_t if_icmpne_b = 0xa0;
const uint8_t if_icmpeq_b = 0x9f;
const uint8_t if_icmpgt_b = 0xa3;
const uint8_t if_icmpge_b = 0xa2;
const uint8_t if_icmplt_b = 0xa1;
const uint8_t if_icmple_b = 0xa4;
const uint8_t ifeq_b = 0x99;
const uint8_t ifne_b = 0x9a;
const uint8_t ifgt_b = 0x9d;
const uint8_t ifge_b = 0x9c;
const uint8_t iflt_b = 0x9b;
const uint8_t ifle_b = 0x9e;
//other
const uint8_t goto_b = 0xa7;
const uint8_t bipush_b = 0x10;
const uint8_t invokestatic_b = 0xb8;
const uint8_t invokevirtual_b = 0xb6;
const uint8_t return_b = 0xb1;
const uint8_t ireturn_b = 0xac;
//skip over these
const uint8_t getstatic_b = 0xb2;


#endif

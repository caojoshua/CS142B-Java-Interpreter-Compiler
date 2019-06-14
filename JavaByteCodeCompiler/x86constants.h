#pragma once
#ifndef X86CONSTaNTS_H
#define X86CONSTANTS_H

//this allows uint8_t
#include <array>


const uint8_t REX_W = 0x48;

//regs
const uint8_t EAX = 0b000;
const uint8_t ECX = 0b001;
const uint8_t EDX = 0b010;
const uint8_t EBX = 0b011;
const uint8_t ESP = 0b100; //stack pointer
const uint8_t EBP = 0b101; //base pointer
const uint8_t ESI = 0b110;
const uint8_t EDI = 0b111;

const std::array<uint8_t, 6> intToReg =
{
	EAX,
	ECX,
	EDX,
	EBX,
	ESI,
	EDI
};

//order in which to store params, calling c++ function first looks at ECX
const std::array<uint8_t, 6> callerSavedRegs =
{
	ECX,
	EAX,
	EDX
};


//modrm
const uint8_t MOD_ADDR = 0b00;
const uint8_t MOD_REG_DISP8 = 0b01;
const uint8_t MOD_REG_DISP32 = 0b10;
const uint8_t MOD_REG = 0b11;


//opcodes
//we only care about 32s, e.g. in manual: MOV_r32_imm32
//RM = R/M = register/memory
const uint8_t ADD_RM_IMM = 0x81;	//add imm to rm
const uint8_t ADD_RM_R = 0x01;		//add r to rm
const uint8_t CALL_RM = 0xFF;		//call near absolute of address in r/m
const uint8_t CMP_RM_R = 0x39;		//compare RM against R
const uint8_t CMP_RM_IMM = 0x83;	//compare RM against IMM
const uint8_t MOV_R_IMM = 0xB8;		//mov IMM to R
const uint8_t MOV_R_RM = 0x8B;		//mov RM to R
const uint8_t MOV_RM_R = 0x89;		//mov R to RM
const uint8_t POP_RM = 0x8F;		//pop rm off of stack
const uint8_t PUSH_RM = 0xFF;		//push value of RM to stack
const uint8_t PUSH_IMM = 0x68;		//push IMM onto stack
const uint8_t RET86 = 0xc3;			//return to caller, 86 because naming conflict
const uint8_t SUB_RM_R = 0x29;		//sub r from rm
const uint8_t SUB_RM_IMM = 0x81;	//sub imm from rm

//jmp opcodes grouped together
//append "_32" because naming conflict, but it means addr displacement is 32 bits
const uint8_t JMP_32 = 0xE9;	//jump to 32 bit disp, jmp to r/m exists to but not necessary in this project
const uint8_t JCC_PRE = 0x0F;	//prefix to be emitted before opcode
const uint8_t JE_32 = 0x84;
const uint8_t JNE_32 = 0x85;
const uint8_t JL_32 = 0x8C;
const uint8_t JLE_32 = 0x8E;
const uint8_t JG_32 = 0x8F;
const uint8_t JGE_32 = 0x8D;

#endif
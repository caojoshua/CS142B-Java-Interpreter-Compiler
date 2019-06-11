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


//modrm
const uint8_t MOD_ADDR = 0b00;
const uint8_t MOD_REG_DISP8 = 0b01;
const uint8_t MOD_REG_DISP32 = 0b10;
const uint8_t MOD_REG = 0b11;


//opcodes
//we only care about 32s, e.g. in manual: MOV_r32_imm32
//RM = R/M = register/memory
const uint8_t MOV_R_IMM = 0xB8;		//mov IMM to R
const uint8_t MOV_R_RM = 0x8B;		//mov RM to R
const uint8_t MOV_RM_R = 0x89;		//mov R to RM
const uint8_t POP_RM = 0x8F;
const uint8_t PUSH_RM = 0xFF;		//push value of RM to stack
const uint8_t RET86 = 0xc3;			//return to caller, 86 because naming conflict


#endif
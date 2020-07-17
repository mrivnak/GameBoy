#include "instructions.hpp"

#include "registers.hpp"
#include "memory-bus.hpp"

#include <cstdio>

Instructions::Instructions() {
	initPrefixed();
	initNonPrefixed();
}

Instruction Instructions::fetchInstruction(const uint8_t instructionByte, bool prefixByte) {
	if (prefixByte) {
		return prefixed[instructionByte];
	}
	else {
		return nonPrefixed[instructionByte];
	}
}

Instructions::~Instructions() {

}

void Instructions::initNonPrefixed() {
	// NOP
	nonPrefixed[0x00] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_NOP(cycles, reg);
	};

	// LD BC,u16
	nonPrefixed[0x01] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD16_RN(reg.B, reg.C, cycles, reg, mem);
	};

	// LD (BC),A
	nonPrefixed[0x02] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_PR(getAddress(reg.B, reg.C), reg.A, cycles, reg, mem);
	};

	// INC BC
	nonPrefixed[0x03] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC16(reg.B, reg.C, cycles, reg);
	};

	// INC B
	nonPrefixed[0x04] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC(reg.B, cycles, reg);
	};

	// DEC B
	nonPrefixed[0x05] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DEC(reg.B, cycles, reg);
	};

	// LD B,u8
	nonPrefixed[0x06] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RN(reg.B, cycles, reg, mem);
	};

	// RLCA
	nonPrefixed[0x07] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RLCA(cycles, reg);
	};

	// LD (u16),SP
	nonPrefixed[0x08] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_SPW(cycles, reg, mem);
	};

	// ADD HL,BC
	nonPrefixed[0x09] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD16(reg.B, reg.C, cycles, reg);
	};

	// LD A,(BC)
	nonPrefixed[0x0A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RP(reg.A, getAddress(reg.B, reg.C), cycles, reg, mem);
	};

	// DEC BC
	nonPrefixed[0x0B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DEC16(reg.B, reg.C, cycles, reg);
	};

	// INC C
	nonPrefixed[0x0C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC(reg.C, cycles, reg);
	};

	// DEC C
	nonPrefixed[0x0D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DEC(reg.C, cycles, reg);
	};

	// LD C,u8
	nonPrefixed[0x0E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RN(reg.C, cycles, reg, mem);
	};

	// RRCA
	nonPrefixed[0x0F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RRCA(cycles, reg);
	};

	// STOP
	nonPrefixed[0x10] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_STOP(cycles, reg);
	};

	// LD DE,u16
	nonPrefixed[0x11] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD16_RN(reg.D, reg.E, cycles, reg, mem);
	};

	// LD (DE),A
	nonPrefixed[0x12] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_PR(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// INC DE
	nonPrefixed[0x13] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC16(reg.D, reg.E, cycles, reg);
	};

	// INC D
	nonPrefixed[0x14] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC(reg.D, cycles, reg);
	};

	// DEC D
	nonPrefixed[0x15] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DEC(reg.D, cycles, reg);
	};

	// LD D,u8
	nonPrefixed[0x16] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_PN(reg.D, cycles, reg, mem);
	};
	
	// RLA
	nonPrefixed[0x17] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RLA(cycles, reg);
	};

	// JR i8
	nonPrefixed[0x18] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(cycles, reg, mem);
	};

	// ADD HL,DE
	nonPrefixed[0x19] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD16(reg.D, reg.E, cycles, reg);
	};

	// LD A,(DE)
	nonPrefixed[0x1A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RP(reg.A, getAddress(reg.D, reg.E), cycles, reg, mem);
	};

	// DEC DE
	nonPrefixed[0x1B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DEC16(reg.D, reg.E, cycles, reg);
	};

	// INC E
	nonPrefixed[0x1C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC(reg.E, cycles, reg);
	};

	// DEC E
	nonPrefixed[0x1D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DEC(reg.E, cycles, reg);
	};

	// LD E,u8
	nonPrefixed[0x1E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RN(reg.E, cycles, reg, mem);
	};

	// RRA
	nonPrefixed[0x1F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RRA(cycles, reg);
	};

	// JR NZ,i8
	nonPrefixed[0x20] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(!reg.getZero(), cycles, reg, mem);
	};

	// LD HL,u16
	nonPrefixed[0x21] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD16_RN(reg.H, reg.L, cycles, reg, mem);
	};

	// LD (HL+),A
	nonPrefixed[0x22] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_IW(cycles, reg, mem);
	};

	// INC HL
	nonPrefixed[0x23] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC16(reg.H, reg.L, cycles, reg);
	};

	// INC H
	nonPrefixed[0x24] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC(reg.H, cycles, reg);
	};

	// DEC H
	nonPrefixed[0x25] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DEC(reg.H, cycles, reg);
	};

	// LD H,u8
	nonPrefixed[0x26] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RN(reg.H, cycles, reg, mem);
	};
	
	// DAA
	nonPrefixed[0x27] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DAA(cycles, reg);
	};

	// JR Z,i8
	nonPrefixed[0x28] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(reg.getZero(), cycles, reg, mem);
	};

	// ADD HL,HL
	nonPrefixed[0x29] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD16(reg.H, reg.L, cycles, reg);
	};

	// LD A,(HL+)
	nonPrefixed[0x2A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_IR(cycles, reg, mem);
	};

	// DEC HL
	nonPrefixed[0x2B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DEC16(reg.H, reg.L, cycles, reg);
	};

	// INC L
	nonPrefixed[0x2C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC(reg.L, cycles, reg);
	};

	// DEC L
	nonPrefixed[0x2D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DEC(reg.L, cycles, reg);
	};

	// LD L,u8
	nonPrefixed[0x2E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RN(reg.L, cycles, reg, mem);
	};

	// CPL
	nonPrefixed[0x2F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CPL(cycles, reg);
	};

	// JR NC,i8
	nonPrefixed[0x30] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(!reg.getCarry(), cycles, reg, mem);
	};

	// LD SP,u16
	nonPrefixed[0x31] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_SPR(cycles, reg, mem);
	};

	// LD (HL-),A
	nonPrefixed[0x32] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_DW(cycles, reg, mem);
	};

	// INC SP
	nonPrefixed[0x33] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC_SP(cycles, reg);
	};

	// INC (HL)
	nonPrefixed[0x34] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC(getAddress(reg.H, reg.L));
	};

	// DEC (HL)
	nonPrefixed[0x35] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		
	};

	// LD (HL),u8
	nonPrefixed[0x36] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_PN(getAddress(reg.H, reg.L), cycles, reg, mem);
	};
	
	// SCF
	nonPrefixed[0x37] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SCF(cycles, reg);
	};

	// JR C,i8
	nonPrefixed[0x38] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(reg.getCarry(), cycles, reg, mem);
	};

	// ADD HL,SP
	nonPrefixed[0x39] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD_HL_SP(cycles, reg, mem);
	};

	// LD A,(HL-)
	nonPrefixed[0x3A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_DR(cycles, reg, mem);
	};

	// DEC SP
	nonPrefixed[0x3B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DEC_SP(cycles, reg);
	};

	// INC A
	nonPrefixed[0x3C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC(reg.A, cycles, reg);
	};

	// DEC A
	nonPrefixed[0x3D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DEC(reg.A, cycles, reg);
	};

	// LD A,u8
	nonPrefixed[0x3E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RN(reg.A, cycles, reg, mem);
	};

	// CCF
	nonPrefixed[0x3F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CCF(cycles, reg);
	};

	// LD B,B
	nonPrefixed[0x40] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.B, reg.B, cycles, reg);
	};

	// LD B,C
	nonPrefixed[0x41] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.B, reg.C, cycles, reg);
	};

	// LD B,D
	nonPrefixed[0x42] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.B, reg.D, cycles, reg);
	};

	// LD B,E
	nonPrefixed[0x43] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.B, reg.E, cycles, reg);
	};

	// LD B,H
	nonPrefixed[0x44] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.B, reg.H, cycles, reg);
	};

	// LD B,L
	nonPrefixed[0x45] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.B, reg.L, cycles, reg);
	};

	// LD B,(HL)
	nonPrefixed[0x46] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RP(reg.B, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// LD B,A
	nonPrefixed[0x46] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.B, reg.A, cycles, reg);
	};

	// LD C,B
	nonPrefixed[0x48] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.C, reg.B, cycles, reg);
	};

	// LD C,C
	nonPrefixed[0x49] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.C, reg.C, cycles, reg);
	};

	// LD C,D
	nonPrefixed[0x4A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.C, reg.D, cycles, reg);
	};

	// LD C,E
	nonPrefixed[0x4B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.C, reg.E, cycles, reg);
	};

	// LD C,H
	nonPrefixed[0x4C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.C, reg.H, cycles, reg);
	};

	// LD C,L
	nonPrefixed[0x4D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.C, reg.L, cycles, reg);
	};

	// LD C,(HL)
	nonPrefixed[0x4E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RP(reg.C, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// LD C,A
	nonPrefixed[0x4F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.C, reg.A, cycles, reg);
	};

	// LD D,B
	nonPrefixed[0x50] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.D, reg.B, cycles, reg);
	};

	// LD D,C
	nonPrefixed[0x51] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.D, reg.C, cycles, reg);
	};

	// LD D,D
	nonPrefixed[0x52] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.D, reg.D, cycles, reg);
	};

	// LD D,E
	nonPrefixed[0x53] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.D, reg.E, cycles, reg);
	};

	// LD D,H
	nonPrefixed[0x54] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.B, reg.H, cycles, reg);
	};

	// LD D,L
	nonPrefixed[0x55] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.D, reg.L, cycles, reg);
	};

	// LD D,(HL)
	nonPrefixed[0x56] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RP(reg.D, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// LD D,A
	nonPrefixed[0x57] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.D, reg.A, cycles, reg);
	};

	// LD E,B
	nonPrefixed[0x58] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.E, reg.B, cycles, reg);
	};

	// LD E,C
	nonPrefixed[0x59] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.E, reg.C, cycles, reg);
	};

	// LD E,D
	nonPrefixed[0x5A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.E, reg.D, cycles, reg);
	};

	// LD E,E
	nonPrefixed[0x5B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.E, reg.E, cycles, reg);
	};

	// LD E,H
	nonPrefixed[0x5C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.E, reg.H, cycles, reg);
	};

	// LD E,L
	nonPrefixed[0x5D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.E, reg.L, cycles, reg);
	};

	// LD E,(HL)
	nonPrefixed[0x5E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RP(reg.E, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// LD E,A
	nonPrefixed[0x5F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.E, reg.A, cycles, reg);
	};

	// LD H,B
	nonPrefixed[0x60] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.H, reg.B, cycles, reg);
	};

	// LD H,C
	nonPrefixed[0x61] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.H, reg.C, cycles, reg);
	};

	// LD H,D
	nonPrefixed[0x62] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.H, reg.D, cycles, reg);
	};

	// LD H,E
	nonPrefixed[0x63] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.H, reg.E, cycles, reg);
	};

	// LD H,H
	nonPrefixed[0x64] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.H, reg.H, cycles, reg);
	};

	// LD H,L
	nonPrefixed[0x65] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.H, reg.L, cycles, reg);
	};

	// LD G,(HL)
	nonPrefixed[0x66] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RP(reg.H, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// LD H,A
	nonPrefixed[0x67] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.H, reg.A, cycles, reg);
	};

	// LD L,B
	nonPrefixed[0x68] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.L, reg.B, cycles, reg);
	};

	// LD L,C
	nonPrefixed[0x69] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.L, reg.C, cycles, reg);
	};

	// LD L,D
	nonPrefixed[0x6A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.L, reg.D, cycles, reg);
	};

	// LD L,E
	nonPrefixed[0x6B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.L, reg.E, cycles, reg);
	};

	// LD L,H
	nonPrefixed[0x6C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.L, reg.H, cycles, reg);
	};

	// LD L,L
	nonPrefixed[0x6D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.L, reg.L, cycles, reg);
	};

	// LD L,(HL)
	nonPrefixed[0x6E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RP(reg.L, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// LD L,A
	nonPrefixed[0x6F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.L, reg.A, cycles, reg);
	};

	// LD (HL),B
	nonPrefixed[0x70] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_PR(getAddress(reg.H, reg.L), reg.B, cycles, reg, mem);
	};

	// LD (HL),C
	nonPrefixed[0x71] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_PR(getAddress(reg.H, reg.L), reg.C, cycles, reg, mem);
	};

	// LD (HL),D
	nonPrefixed[0x72] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_PR(getAddress(reg.H, reg.L), reg.D, cycles, reg, mem);
	};

	// LD (HL),E
	nonPrefixed[0x73] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_PR(getAddress(reg.H, reg.L), reg.E, cycles, reg, mem);
	};

	// LD (HL),H
	nonPrefixed[0x74] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_PR(getAddress(reg.H, reg.L), reg.H, cycles, reg, mem);
	};

	// LD (HL),L
	nonPrefixed[0x75] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_PR(getAddress(reg.H, reg.L), reg.L, cycles, reg, mem);
	};

	// HALT
	nonPrefixed[0x76] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_HALT(cycles, reg);
	};

	// LD (HL),A
	nonPrefixed[0x77] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_PR(getAddress(reg.H, reg.L), reg.A, cycles, reg, mem);
	};

	// LD A,B
	nonPrefixed[0x78] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.A, reg.B, cycles, reg);
	};

	// LD A,C
	nonPrefixed[0x79] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.A, reg.C, cycles, reg);
	};

	// LD A,D
	nonPrefixed[0x7A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.A, reg.D, cycles, reg);
	};

	// LD A,E
	nonPrefixed[0x7B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.A, reg.E, cycles, reg);
	};

	// LD A,H
	nonPrefixed[0x7C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.A, reg.H, cycles, reg);
	};

	// LD A,L
	nonPrefixed[0x7D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.A, reg.L, cycles, reg);
	};

	// LD A,(HL)
	nonPrefixed[0x7E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RP(reg.A, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// LD A,A
	nonPrefixed[0x7F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_RR(reg.A, reg.A, cycles, reg);
	};

	// ADD A,B
	nonPrefixed[0x80] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD(reg.B, cycles, reg);
	};

	// ADD A,C
	nonPrefixed[0x81] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD(reg.C, cycles, reg);
	};

	// ADD A,D
	nonPrefixed[0x82] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD(reg.D, cycles, reg);
	};

	// ADD A,E
	nonPrefixed[0x83] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD(reg.E, cycles, reg);
	};

	// ADD A,H
	nonPrefixed[0x84] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD(reg.H, cycles, reg);
	};

	// ADD A,L
	nonPrefixed[0x85] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD(reg.L, cycles, reg);
	};

	// ADD A,(HL)
	nonPrefixed[0x86] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// ADD A,A
	nonPrefixed[0x87] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD(reg.A, cycles, reg);
	};

	// ADC A,B
	nonPrefixed[0x88] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADC(reg.B, cycles, reg);
	};

	// ADC A,C
	nonPrefixed[0x89] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADC(reg.C, cycles, reg);
	};

	// ADC A,D
	nonPrefixed[0x8A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADC(reg.D, cycles, reg);
	};

	// ADC A,E
	nonPrefixed[0x8B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADC(reg.E, cycles, reg);
	};

	// ADC A,H
	nonPrefixed[0x8C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADC(reg.H, cycles, reg);
	};

	// ADC A,L
	nonPrefixed[0x8D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADC(reg.L, cycles, reg);
	};

	// ADC A,(HL)
	nonPrefixed[0x8E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADC(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// ADC A,A
	nonPrefixed[0x8F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADC(reg.A, cycles, reg);
	};

	// SUB A,B
	nonPrefixed[0x90] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SUB(reg.B, cycles, reg);
	};

	// SUB A,C
	nonPrefixed[0x91] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SUB(reg.C, cycles, reg);
	};

	// SUB A,D
	nonPrefixed[0x92] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SUB(reg.D, cycles, reg);
	};

	// SUB A,E
	nonPrefixed[0x93] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SUB(reg.E, cycles, reg);
	};

	// SUB A,H
	nonPrefixed[0x94] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SUB(reg.H, cycles, reg);
	};

	// SUB A,L
	nonPrefixed[0x95] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SUB(reg.L, cycles, reg);
	};

	// SUB A,(HL)
	nonPrefixed[0x96] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SUB(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SUB A,A
	nonPrefixed[0x97] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SUB(reg.A, cycles, reg);
	};

	// SBC A,B
	nonPrefixed[0x98] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SBC(reg.B, cycles, reg);
	};

	// SBC A,C
	nonPrefixed[0x99] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SBC(reg.C, cycles, reg);
	};

	// SBC A,D
	nonPrefixed[0x9A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SBC(reg.D, cycles, reg);
	};

	// SBC A,E
	nonPrefixed[0x9B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SBC(reg.E, cycles, reg);
	};

	// SBC A,H
	nonPrefixed[0x9C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SBC(reg.H, cycles, reg);
	};

	// SBC A,L
	nonPrefixed[0x9D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SBC(reg.L, cycles, reg);
	};

	// SBC A,(HL)
	nonPrefixed[0x9E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SBC(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SBC A,A
	nonPrefixed[0x9F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SBC(reg.A, cycles, reg);
	};

	// AND A,B
	nonPrefixed[0xA0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_AND(reg.B, cycles, reg);
	};

	// AND A,C
	nonPrefixed[0xA1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_AND(reg.C, cycles, reg);
	};

	// AND A,D
	nonPrefixed[0xA2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_AND(reg.D, cycles, reg);
	};

	// AND A,E
	nonPrefixed[0xA3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_AND(reg.E, cycles, reg);
	};

	// AND A,H
	nonPrefixed[0xA4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_AND(reg.H, cycles, reg);
	};

	// AND A,L
	nonPrefixed[0xA5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_AND(reg.L, cycles, reg);
	};

	// AND A,(HL)
	nonPrefixed[0xA6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_AND(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// AND A,A
	nonPrefixed[0xA7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_AND(reg.A, cycles, reg);
	};

	// XOR A,B
	nonPrefixed[0xA8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_XOR(reg.B, cycles, reg);
	};

	// XOR A,C
	nonPrefixed[0xA9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_XOR(reg.C, cycles, reg);
	};

	// XOR A,D
	nonPrefixed[0xAA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_XOR(reg.D, cycles, reg);
	};

	// XOR A,E
	nonPrefixed[0xAB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_XOR(reg.E, cycles, reg);
	};

	// XOR A,H
	nonPrefixed[0xAC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_XOR(reg.H, cycles, reg);
	};

	// XOR A,L
	nonPrefixed[0xAD] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_XOR(reg.L, cycles, reg);
	};

	// XOR A,(HL)
	nonPrefixed[0xAE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_XOR(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// XOR A,A
	nonPrefixed[0xAF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_XOR(reg.A, cycles, reg);
	};

	// OR A,B
	nonPrefixed[0xB0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_OR(reg.B, cycles, reg);
	};

	// OR A,C
	nonPrefixed[0xB1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_OR(reg.C, cycles, reg);
	};

	// OR A,D
	nonPrefixed[0xB2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_OR(reg.D, cycles, reg);
	};

	// OR A,E
	nonPrefixed[0xB3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_OR(reg.E, cycles, reg);
	};

	// OR A,H
	nonPrefixed[0xB4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_OR(reg.H, cycles, reg);
	};

	// OR A,L
	nonPrefixed[0xB5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_OR(reg.L, cycles, reg);
	};

	// OR A,(HL)
	nonPrefixed[0xB6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_OR(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// OR A,A
	nonPrefixed[0xB7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_OR(reg.B, cycles, reg);
	};

	// CP A,B
	nonPrefixed[0xB8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CP(reg.B, cycles, reg);
	};

	// CP A,C
	nonPrefixed[0xB9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CP(reg.C, cycles, reg);
	};

	// CP A,D
	nonPrefixed[0xBA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CP(reg.D, cycles, reg);
	};

	// CP A,E
	nonPrefixed[0xBB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CP(reg.E, cycles, reg);
	};

	// CP A,H
	nonPrefixed[0xBC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CP(reg.H, cycles, reg);
	};

	// CP A,L
	nonPrefixed[0xBD] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CP(reg.L, cycles, reg);
	};

	// CP A,(HL)
	nonPrefixed[0xBE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CP(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// CP A,A
	nonPrefixed[0xBF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CP(reg.A, cycles, reg);
	};

	// RET NZ
	nonPrefixed[0xC0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RET(!reg.getZero(), cycles, reg, mem);
	};

	// POP BC
	nonPrefixed[0xC1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_POP(reg.B, reg.C, cycles, reg, mem);
	};

	// JP NZ,u16
	nonPrefixed[0xC2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(!reg.getZero(), cycles, reg, mem);
	};

	// JP u16
	nonPrefixed[0xC3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JP16(cycles, reg, mem);
	};

	// CALL NZ,u16
	nonPrefixed[0xC4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CALL(!reg.getZero(), cycles, reg, mem);
	};

	// PUSH BC
	nonPrefixed[0xC5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_PUSH(reg.B, reg.C, cycles, reg, mem);
	};

	// ADD A,u8
	nonPrefixed[0xC6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD(cycles, reg, mem);
	};

	// RST 0x00
	nonPrefixed[0xC7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0000, cycles, reg, mem);
	};

	// RET Z
	nonPrefixed[0xC8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RET(reg.getZero(), cycles, reg, mem);
	};

	// RET
	nonPrefixed[0xC9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RET(cycles, reg, mem);
	};

	// JP Z,u16
	nonPrefixed[0xCA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JP(reg.getZero(), cycles, reg, mem);
	};

	// 0xCB Prefix

	// CALL Z,u16
	nonPrefixed[0xCC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CALL(reg.getZero(), cycles, reg, mem);
	};

	// CALL u16
	nonPrefixed[0xCD] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CALL(cycles, reg, mem);
	};

	// ADC A,u8
	nonPrefixed[0xCE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADC(cycles, reg, mem);
	};

	// RST 0x08
	nonPrefixed[0xCF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0008, cycles, reg, mem);
	};

	// RET NC
	nonPrefixed[0xD0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RET(!reg.getCarry(), cycles, reg, mem);
	};

	// POP DE
	nonPrefixed[0xD1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_POP(reg.D, reg.E, cycles, reg, mem);
	};

	// JP NC,u16
	nonPrefixed[0xD2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JP(!get.getCarry(), cycles, reg, mem);
	};

	// 0xD3 Blank

	// CALL NC,u16
	nonPrefixed[0xD4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CALL(!reg.getCarry(), cycles, reg, mem);
	};

	// PUSH DE
	nonPrefixed[0xD5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_PUSH(reg.D, reg.E, cycles, reg, mem);
	};

	// SUB A,u8
	nonPrefixed[0xD6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SUB(cycles, reg, mem);
	};

	// RST 0x10
	nonPrefixed[0xD7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0010, cycles, reg, mem);
	};

	// RET C
	nonPrefixed[0xD8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RET(reg.getCarry(), cycles, reg, mem);
	};

	// RETI
	nonPrefixed[0xD9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RETI(cycles, reg, mem);
	};

	// JP C,u16
	nonPrefixed[0xDA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JP(reg.getCarry(), cycles, reg, mem);
	};

	// 0xDB Blank

	// CALL C,u16
	nonPrefixed[0xDC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CALL(reg.getCarry(), cycles, reg, mem);
	};

	// 0xDD Blank

	// SBC A,u8
	nonPrefixed[0xDE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SBC(cycles, reg, mem);
	};

	// RST 0x18
	nonPrefixed[0xDF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0018, cycles, reg, mem);
	};

	// LD (0xFF00+u8),A
	nonPrefixed[0xE0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LDH_PA(cycles, reg, mem);
	};

	// POP HL
	nonPrefixed[0xE1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_POP(reg.H, reg.L, cycles, reg, mem);
	};

	// LD (0xFF00+C),A
	nonPrefixed[0xE2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LDH_CA(cycles, reg, mem);
	};

	// 0xE3 Blank

	// 0xE4 Blank

	// PUSH HL
	nonPrefixed[0xE5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_PUSH(reg.H, reg.L, cycles, reg, mem);
	};

	//AND A,u8
	nonPrefixed[0xE6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_AND(cycles, reg, mem);
	};

	// RST 0x20
	nonPrefixed[0xE7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0020, cycles, reg, mem);
	};

	// ADD SP,i8
	nonPrefixed[0xE8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD_SP(cycles, reg, mem);
	};

	// JP HL
	nonPrefixed[0xE9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JP_HL(cycles, reg, mem);
	};

	// LD (u16),A
	nonPrefixed[0xEA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD16_NA(cycles, reg, mem);
	};

	// 0xEB Blank

	// 0xEC Blank

	// 0xED Blank

	// XOR A,u8
	nonPrefixed[0xEE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_XOR(cycles, reg, mem);
	};

	// RST 0x28
	nonPrefixed[0xEF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0028, cycles, reg, mem);
	};

	// LD A,(0xFF00+u8)
	nonPrefixed[0xF0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LDH_AP(cycles, reg, mem);
	};

	// POP AF
	nonPrefixed[0xF1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_POP_AF(cycles, reg, mem);
	};

	// LD A,(0xFF00+C)
	nonPrefixed[0xF2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LDH_AC(cycles, reg, mem);
	};

	// DI
	nonPrefixed[0xF3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DI(cycles, reg, mem);
	};

	// 0xF4 Blank

	// PUSH AF
	nonPrefixed[0xF5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_PUSH_AF(cycles, reg, mem);
	};

	//OR A,u8
	nonPrefixed[0xF6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_OR(cycles, reg, mem);
	};

	// RST 0x30
	nonPrefixed[0xF7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0030, cycles, reg, mem);
	};

	// LD HL,SP+i8 // TODO: Verify this
	nonPrefixed[0xF8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_HL_SP(cycles, reg, mem);
	};

	// LD SP,HL  // TODO: little endian?
	nonPrefixed[0xF9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_SP_HL(cycles, reg);
	};

	// LD A,(u16)
	nonPrefixed[0xFA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD16_AN(cycles, reg, mem);
	};

	// 0xFB EI
	nonPrefixed[0xFB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_EI(cycles, reg);
	};

	// 0xFC Blank

	// 0xFD Blank

	// CP A,u8
	nonPrefixed[0xFE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_CP(cycles, reg, mem);
	};

	// RST 0x38
	nonPrefixed[0xFF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0038, cycles, reg, mem);
	};
}


void Instructions::initPrefixed() {
	// RLC B
	prefixed[0x00] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RLC(reg.B, cycles, reg);
	};

	// RLC C
	prefixed[0x01] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RLC(reg.B, cycles, reg);
	};

	// RLC D
	prefixed[0x02] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RLC(reg.B, cycles, reg);
	};

	// RLC E
	prefixed[0x03] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RLC(reg.B, cycles, reg);
	};

	// RLC H
	prefixed[0x04] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RLC(reg.B, cycles, reg);
	};

	// RLC L
	prefixed[0x05] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RLC(reg.B, cycles, reg);
	};

	// RLC (HL)
	prefixed[0x06] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RLC(genAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RLC A
	prefixed[0x07] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RLC(reg.B, cycles, reg);
	};

	// RRC B
	prefixed[0x08] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RRC(reg.B, cycles, reg);
	};

	// RRC C
	prefixed[0x09] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RRC(reg.C, cycles, reg);
	};

	// RRC D
	prefixed[0x0A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RRC(reg.D, cycles, reg);
	};

	// RRC E
	prefixed[0x0B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RRC(reg.A, cycles, reg);
	};

	// RRC H
	prefixed[0x0C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RRC(reg.H, cycles, reg);
	};

	// RRC L
	prefixed[0x0D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RRC(reg.L, cycles, reg);
	};

	// RRC (HL)
	prefixed[0x0E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RRC(genAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RRC A
	prefixed[0x0F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RRC(reg.A, cycles, reg);
	};

	// RL B
	prefixed[0x10] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RL(reg.B, cycles, reg);
	};

	// RL C
	prefixed[0x11] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RL(reg.C, cycles, reg);
	};

	// RL D
	prefixed[0x12] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RL(reg.D, cycles, reg);
	};

	// RL E
	prefixed[0x13] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RL(reg.E, cycles, reg);
	};

	// RL H
	prefixed[0x14] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RL(reg.H, cycles, reg);
	};

	// RL L
	prefixed[0x15] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RL(reg.L, cycles, reg);
	};

	// RL (HL)
	prefixed[0x16] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RL(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RL A
	prefixed[0x17] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RL(reg.A, cycles, reg);
	};

	// RR B
	prefixed[0x18] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RR(reg.B, cycles, reg);
	};

	// RR C
	prefixed[0x19] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RR(reg.C, cycles, reg);
	};

	// RR D
	prefixed[0x1A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RR(reg.D, cycles, reg);
	};

	// RR E
	prefixed[0x1B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RR(reg.E, cycles, reg);
	};

	// RR H
	prefixed[0x1C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RR(reg.H, cycles, reg);
	};

	// RR L
	prefixed[0x1D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RR(reg.L, cycles, reg);
	};

	// RL (HL)
	prefixed[0x16] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RR(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RR A
	prefixed[0x1F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RR(reg.A, cycles, reg);
	};

	// SLA B
	prefixed[0x20] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SLA(reg.B, cycles, reg);
	};

	// SLA C
	prefixed[0x21] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SLA(reg.C, cycles, reg);
	};

	// SLA D
	prefixed[0x22] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SLA(reg.D, cycles, reg);
	};

	// SLA E
	prefixed[0x23] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SLA(reg.E, cycles, reg);
	};

	// SLA H
	prefixed[0x24] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SLA(reg.H, cycles, reg);
	};

	// SLA L
	prefixed[0x25] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SLA(reg.L, cycles, reg);
	};

	// SLA (HL)
	prefixed[0x26] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SLA(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SLA A
	prefixed[0x27] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SLA(reg.A, cycles, reg);
	};

	// SRA B
	prefixed[0x28] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRA(reg.B, cycles, reg);
	};

	// SRA C
	prefixed[0x29] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRA(reg.C, cycles, reg);
	};

	// SRA D
	prefixed[0x2A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRA(reg.D, cycles, reg);
	};

	// SRA E
	prefixed[0x2B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRA(reg.E, cycles, reg);
	};

	// SRA H
	prefixed[0x2C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRA(reg.H, cycles, reg);
	};

	// SRA L
	prefixed[0x2D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRA(reg.L, cycles, reg);
	};

	// SRA (HL)
	prefixed[0x2E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRA(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SRA A
	prefixed[0x2F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRA(reg.A, cycles, reg);
	};

	// SWAP B
	prefixed[0x30] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SWAP(reg.B, cycles, reg);
	};

	// SWAP C
	prefixed[0x31] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SWAP(reg.C, cycles, reg);
	};

	// SWAP D
	prefixed[0x32] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SWAP(reg.D, cycles, reg);
	};

	// SWAP E
	prefixed[0x33] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SWAP(reg.E, cycles, reg);
	};

	// SWAP H
	prefixed[0x34] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SWAP(reg.H, cycles, reg);
	};

	// SWAP L
	prefixed[0x35] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SWAP(reg.L, cycles, reg);
	};

	// SWAP (HL)
	prefixed[0x36] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SWAP(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SWAP A
	prefixed[0x37] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SWAP(reg.A, cycles, reg);
	};

	// SRL B
	prefixed[0x38] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRL(reg.B, cycles, reg);
	};

	// SRL C
	prefixed[0x39] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRL(reg.C, cycles, reg);
	};

	// SRL D
	prefixed[0x3A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRL(reg.D, cycles, reg);
	};

	// SRL E
	prefixed[0x3B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRL(reg.E, cycles, reg);
	};

	// SRL H
	prefixed[0x3C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRL(reg.H, cycles, reg);
	};

	// SRL L
	prefixed[0x3D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRL(reg.L, cycles, reg);
	};

	// SRL (HL)
	prefixed[0x3E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRL(getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SRL A
	prefixed[0x3F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SRL(reg.A, cycles, reg);
	};

	// BIT 0,B
	prefixed[0x40] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(0, reg.B, cycles, reg);
	};

	// BIT 0,C
	prefixed[0x41] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(0, reg.C, cycles, reg);
	};

	// BIT 0,D
	prefixed[0x42] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(0, reg.D, cycles, reg);
	};

	// BIT 0,E
	prefixed[0x43] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(0, reg.E, cycles, reg);
	};

	// BIT 0,H
	prefixed[0x44] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(0, reg.H, cycles, reg);
	};

	// BIT 0,L
	prefixed[0x45] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(0, reg.L, cycles, reg);
	};

	// BIT 0,(HL)
	prefixed[0x46] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(0, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// BIT 0,A
	prefixed[0x47] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(0, reg.A, cycles, reg);
	};

	// BIT 1,B
	prefixed[0x48] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(1, reg.B, cycles, reg);
	};

	// BIT 1,C
	prefixed[0x49] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(1, reg.C, cycles, reg);
	};

	// BIT 1,D
	prefixed[0x4A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(1, reg.D, cycles, reg);
	};

	// BIT 1,E
	prefixed[0x4B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(1, reg.E, cycles, reg);
	};

	// BIT 1,H
	prefixed[0x4C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(1, reg.H, cycles, reg);
	};

	// BIT 1,L
	prefixed[0x4D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(1, reg.L, cycles, reg);
	};

	// BIT 1,(HL)
	prefixed[0x4E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(1, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// BIT 1,A
	prefixed[0x4F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(1, reg.A, cycles, reg);
	};

	// BIT 2,B
	prefixed[0x50] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(2, reg.B, cycles, reg);
	};

	// BIT 2,C
	prefixed[0x51] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(2, reg.C, cycles, reg);
	};

	// BIT 2,D
	prefixed[0x52] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(2, reg.D, cycles, reg);
	};

	// BIT 2,E
	prefixed[0x53] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(2, reg.E, cycles, reg);
	};

	// BIT 2,H
	prefixed[0x54] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(2, reg.H, cycles, reg);
	};

	// BIT 2,L
	prefixed[0x55] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(2, reg.L, cycles, reg);
	};

	// BIT 2,(HL)
	prefixed[0x56] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(2, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// BIT 2,A
	prefixed[0x57] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(2, reg.A, cycles, reg);
	};

	// BIT 3,B
	prefixed[0x58] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(3, reg.B, cycles, reg);
	};

	// BIT 3,C
	prefixed[0x59] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(3, reg.C, cycles, reg);
	};

	// BIT 3,D
	prefixed[0x5A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(3, reg.D, cycles, reg);
	};

	// BIT 3,E
	prefixed[0x5B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(3, reg.E, cycles, reg);
	};

	// BIT 3,H
	prefixed[0x5C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(3, reg.H, cycles, reg);
	};

	// BIT 3,L
	prefixed[0x5D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(3, reg.L, cycles, reg);
	};

	// BIT 3,(HL)
	prefixed[0x5E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(3, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// BIT 3,A
	prefixed[0x5F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(3, reg.A, cycles, reg);
	};

	// BIT 4,B
	prefixed[0x60] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(4, reg.B, cycles, reg);
	};

	// BIT 4,C
	prefixed[0x61] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(4, reg.C, cycles, reg);
	};

	// BIT 4,D
	prefixed[0x62] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(4, reg.D, cycles, reg);
	};

	// BIT 4,E
	prefixed[0x63] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(4, reg.E, cycles, reg);
	};

	// BIT 4,H
	prefixed[0x64] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(4, reg.H, cycles, reg);
	};

	// BIT 4,L
	prefixed[0x65] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(4, reg.L, cycles, reg);
	};

	// BIT 4,(HL)
	prefixed[0x66] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(4, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// BIT 4,A
	prefixed[0x67] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(4, reg.A, cycles, reg);
	};

	// BIT 5,B
	prefixed[0x68] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(5, reg.B, cycles, reg);
	};

	// BIT 5,C
	prefixed[0x69] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(5, reg.C, cycles, reg);
	};

	// BIT 5,D
	prefixed[0x6A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(5, reg.D, cycles, reg);
	};

	// BIT 5,E
	prefixed[0x6B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(5, reg.E, cycles, reg);
	};

	// BIT 5,H
	prefixed[0x6C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(5, reg.H, cycles, reg);
	};

	// BIT 5,L
	prefixed[0x6D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(5, reg.L, cycles, reg);
	};

	// BIT 5,(HL)
	prefixed[0x6E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(5, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// BIT 5,A
	prefixed[0x6F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(5, reg.A, cycles, reg);
	};

	// BIT 6,B
	prefixed[0x70] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(6, reg.B, cycles, reg);
	};

	// BIT 6,C
	prefixed[0x71] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(6, reg.C, cycles, reg);
	};

	// BIT 6,D
	prefixed[0x72] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(6, reg.D, cycles, reg);
	};

	// BIT 6,E
	prefixed[0x73] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(6, reg.E, cycles, reg);
	};

	// BIT 6,H
	prefixed[0x74] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(6, reg.H, cycles, reg);
	};

	// BIT 6,L
	prefixed[0x75] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(6, reg.L, cycles, reg);
	};

	// BIT 6,(HL)
	prefixed[0x76] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(6, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// BIT 6,A
	prefixed[0x77] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(6, reg.A, cycles, reg);
	};

	// BIT 7,B
	prefixed[0x78] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(7, reg.B, cycles, reg);
	};

	// BIT 7,C
	prefixed[0x79] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(7, reg.C, cycles, reg);
	};

	// BIT 7,D
	prefixed[0x7A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(7, reg.D, cycles, reg);
	};

	// BIT 7,E
	prefixed[0x7B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(7, reg.E, cycles, reg);
	};

	// BIT 7,H
	prefixed[0x7C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(7, reg.H, cycles, reg);
	};

	// BIT 7,L
	prefixed[0x7D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(7, reg.L, cycles, reg);
	};

	// BIT 7,(HL)
	prefixed[0x7E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(7, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// BIT 7,A
	prefixed[0x7F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_BIT(7, reg.A, cycles, reg);
	};

	// RES 0,B
	prefixed[0x80] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(0, reg.B, cycles, reg);
	};

	// RES 0,C
	prefixed[0x81] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(0, reg.C, cycles, reg);
	};

	// RES 0,D
	prefixed[0x82] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(0, reg.D, cycles, reg);
	};

	// RES 0,E
	prefixed[0x83] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(0, reg.E, cycles, reg);
	};

	// RES 0,H
	prefixed[0x84] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(0, reg.H, cycles, reg);
	};

	// RES 0,L
	prefixed[0x85] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(0, reg.L, cycles, reg);
	};

	// RES 0,(HL)
	prefixed[0x86] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(0, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RES 0,A
	prefixed[0x87] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(0, reg.A, cycles, reg);
	};

	// RES 1,B
	prefixed[0x88] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(1, reg.B, cycles, reg);
	};

	// RES 1,C
	prefixed[0x89] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(1, reg.C, cycles, reg);
	};

	// RES 1,D
	prefixed[0x8A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(1, reg.D, cycles, reg);
	};

	// RES 1,E
	prefixed[0x8B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(1, reg.E, cycles, reg);
	};

	// RES 1,H
	prefixed[0x8C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(1, reg.H, cycles, reg);
	};

	// RES 1,L
	prefixed[0x8D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(1, reg.L, cycles, reg);
	};

	// RES 1,(HL)
	prefixed[0x8E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(1, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RES 1,A
	prefixed[0x8F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(1, reg.A, cycles, reg);
	};

	// RES 2,B
	prefixed[0x90] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(2, reg.B, cycles, reg);
	};

	// RES 2,C
	prefixed[0x91] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(2, reg.C, cycles, reg);
	};

	// RES 2,D
	prefixed[0x92] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(2, reg.D, cycles, reg);
	};

	// RES 2,E
	prefixed[0x93] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(2, reg.E, cycles, reg);
	};

	// RES 2,H
	prefixed[0x94] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(2, reg.H, cycles, reg);
	};

	// RES 2,L
	prefixed[0x95] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(2, reg.L, cycles, reg);
	};

	// RES 2,(HL)
	prefixed[0x96] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(2, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RES 2,A
	prefixed[0x97] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(2, reg.A, cycles, reg);
	};

	// RES 3,B
	prefixed[0x98] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(3, reg.B, cycles, reg);
	};

	// RES 3,C
	prefixed[0x99] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(3, reg.C, cycles, reg);
	};

	// RES 3,D
	prefixed[0x9A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(3, reg.D, cycles, reg);
	};

	// RES 3,E
	prefixed[0x9B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(3, reg.E, cycles, reg);
	};

	// RES 3,H
	prefixed[0x9C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(3, reg.H, cycles, reg);
	};

	// RES 3,L
	prefixed[0x9D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(3, reg.L, cycles, reg);
	};

	// RES 3,(HL)
	prefixed[0x9E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(3, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RES 3,A
	prefixed[0x9F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(3, reg.A, cycles, reg);
	};

	// RES 4,B
	prefixed[0xA0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(4, reg.B, cycles, reg);
	};

	// RES 4,C
	prefixed[0xA1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(4, reg.C, cycles, reg);
	};

	// RES 4,D
	prefixed[0xA2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(4, reg.D, cycles, reg);
	};

	// RES 4,E
	prefixed[0xA3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(4, reg.E, cycles, reg);
	};

	// RES 4,H
	prefixed[0xA4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(4, reg.H, cycles, reg);
	};

	// RES 4,L
	prefixed[0xA5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(4, reg.L, cycles, reg);
	};

	// RES 4,(HL)
	prefixed[0xA6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(4, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RES 4,A
	prefixed[0xA7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(4, reg.A, cycles, reg);
	};

	// RES 5,B
	prefixed[0xA8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(5, reg.B, cycles, reg);
	};

	// RES 5,C
	prefixed[0xA9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(5, reg.C, cycles, reg);
	};

	// RES 5,D
	prefixed[0xAA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(5, reg.D, cycles, reg);
	};

	// RES 5,E
	prefixed[0xAB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(5, reg.E, cycles, reg);
	};

	// RES 5,H
	prefixed[0xAC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(5, reg.H, cycles, reg);
	};

	// RES 5,L
	prefixed[0xAD] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(5, reg.L, cycles, reg);
	};

	// RES 5,(HL)
	prefixed[0xAE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(5, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RES 5,A
	prefixed[0xAF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(5, reg.A, cycles, reg);
	};

	// RES 6,B
	prefixed[0xB0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(6, reg.B, cycles, reg);
	};

	// RES 6,C
	prefixed[0xB1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(6, reg.C, cycles, reg);
	};

	// RES 6,D
	prefixed[0xB2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(6, reg.D, cycles, reg);
	};

	// RES 6,E
	prefixed[0xB3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(6, reg.E, cycles, reg);
	};

	// RES 6,H
	prefixed[0xB4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(6, reg.H, cycles, reg);
	};

	// RES 6,L
	prefixed[0xB5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(6, reg.L, cycles, reg);
	};

	// RES 6,(HL)
	prefixed[0xB6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(6, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RES 6,A
	prefixed[0xB7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(6, reg.A, cycles, reg);
	};

	// RES 7,B
	prefixed[0xB8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(7, reg.B, cycles, reg);
	};

	// RES 7,C
	prefixed[0xB9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(7, reg.C, cycles, reg);
	};

	// RES 7,D
	prefixed[0xBA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(7, reg.D, cycles, reg);
	};

	// RES 7,E
	prefixed[0xBB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(7, reg.E, cycles, reg);
	};

	// RES 7,H
	prefixed[0xBC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(7, reg.H, cycles, reg);
	};

	// RES 7,L
	prefixed[0xBD] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(7, reg.L, cycles, reg);
	};

	// RES 7,(HL)
	prefixed[0xBE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(7, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// RES 7,A
	prefixed[0xBF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RES(7, reg.A, cycles, reg);
	};

	// SET 0,B
	prefixed[0xC0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(0, reg.B, cycles, reg);
	};

	// SET 0,C
	prefixed[0xC1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(0, reg.C, cycles, reg);
	};

	// SET 0,D
	prefixed[0xC2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(0, reg.D, cycles, reg);
	};

	// SET 0,E
	prefixed[0xC3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(0, reg.E, cycles, reg);
	};

	// SET 0,H
	prefixed[0xC4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(0, reg.H, cycles, reg);
	};

	// SET 0,L
	prefixed[0xC5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(0, reg.L, cycles, reg);
	};

	// SET 0,(HL)
	prefixed[0xC6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(0, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SET 0,A
	prefixed[0xC7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(0, reg.A, cycles, reg);
	};

	// SET 1,B
	prefixed[0xC8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(1, reg.B, cycles, reg);
	};

	// SET 1,C
	prefixed[0xC9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(1, reg.C, cycles, reg);
	};

	// SET 1,D
	prefixed[0xCA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(1, reg.D, cycles, reg);
	};

	// SET 1,E
	prefixed[0xCB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(1, reg.E, cycles, reg);
	};

	// SET 1,H
	prefixed[0xCC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(1, reg.H, cycles, reg);
	};

	// SET 1,L
	prefixed[0xCD] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(1, reg.L, cycles, reg);
	};

	// SET 1,(HL)
	prefixed[0xCE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(1, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SET 1,A
	prefixed[0xCF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(1, reg.A, cycles, reg);
	};

	// SET 2,B
	prefixed[0xD0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(2, reg.B, cycles, reg);
	};

	// SET 2,C
	prefixed[0xD1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(2, reg.C, cycles, reg);
	};

	// SET 2,D
	prefixed[0xD2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(2, reg.D, cycles, reg);
	};

	// SET 2,E
	prefixed[0xD3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(2, reg.E, cycles, reg);
	};

	// SET 2,H
	prefixed[0xD4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(2, reg.H, cycles, reg);
	};

	// SET 2,L
	prefixed[0xD5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(2, reg.L, cycles, reg);
	};

	// SET 2,(HL)
	prefixed[0xD6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(2, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SET 2,A
	prefixed[0xD7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(2, reg.A, cycles, reg);
	};

	// SET 3,B
	prefixed[0xD8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(3, reg.B, cycles, reg);
	};

	// SET 3,C
	prefixed[0xD9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(3, reg.C, cycles, reg);
	};

	// SET 3,D
	prefixed[0xDA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(3, reg.D, cycles, reg);
	};

	// SET 3,E
	prefixed[0xDB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(3, reg.E, cycles, reg);
	};

	// SET 3,H
	prefixed[0xDC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(3, reg.H, cycles, reg);
	};

	// SET 3,L
	prefixed[0xDD] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(3, reg.L, cycles, reg);
	};

	// SET 3,(HL)
	prefixed[0xDE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(3, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SET 3,A
	prefixed[0xDF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(3, reg.A, cycles, reg);
	};

	// SET 4,B
	prefixed[0xE0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(4, reg.B, cycles, reg);
	};

	// SET 4,C
	prefixed[0xE1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(4, reg.C, cycles, reg);
	};

	// SET 4,D
	prefixed[0xE2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(4, reg.D, cycles, reg);
	};

	// SET 4,E
	prefixed[0xE3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(4, reg.E, cycles, reg);
	};

	// SET 4,H
	prefixed[0xE4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(4, reg.H, cycles, reg);
	};

	// SET 4,L
	prefixed[0xE5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(4, reg.L, cycles, reg);
	};

	// SET 4,(HL)
	prefixed[0xE6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(4, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SET 4,A
	prefixed[0xE7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(4, reg.A, cycles, reg);
	};

	// SET 5,B
	prefixed[0xE8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(5, reg.B, cycles, reg);
	};

	// SET 5,C
	prefixed[0xE9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(5, reg.C, cycles, reg);
	};

	// SET 5,D
	prefixed[0xEA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(5, reg.D, cycles, reg);
	};

	// SET 5,E
	prefixed[0xEB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(5, reg.E, cycles, reg);
	};

	// SET 5,H
	prefixed[0xEC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(5, reg.H, cycles, reg);
	};

	// SET 5,L
	prefixed[0xED] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(5, reg.L, cycles, reg);
	};

	// SET 5,(HL)
	prefixed[0xEE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(5, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SET 5,A
	prefixed[0xEF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(5, reg.A, cycles, reg);
	};

	// SET 6,B
	prefixed[0xF0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(6, reg.B, cycles, reg);
	};

	// SET 6,C
	prefixed[0xF1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(6, reg.C, cycles, reg);
	};

	// SET 6,D
	prefixed[0xF2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(6, reg.D, cycles, reg);
	};

	// SET 6,E
	prefixed[0xF3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(6, reg.E, cycles, reg);
	};

	// SET 6,H
	prefixed[0xF4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(6, reg.H, cycles, reg);
	};

	// SET 6,L
	prefixed[0xF5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(6, reg.L, cycles, reg);
	};

	// SET 6,(HL)
	prefixed[0xF6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(6, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SET 6,A
	prefixed[0xF7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(6, reg.A, cycles, reg);
	};

	// SET 7,B
	prefixed[0xF8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(7, reg.B, cycles, reg);
	};

	// SET 7,C
	prefixed[0xF9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(7, reg.C, cycles, reg);
	};

	// SET 7,D
	prefixed[0xFA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(7, reg.D, cycles, reg);
	};

	// SET 7,E
	prefixed[0xFB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(7, reg.E, cycles, reg);
	};

	// SET 7,H
	prefixed[0xFC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(7, reg.H, cycles, reg);
	};

	// SET 7,L
	prefixed[0xFD] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(7, reg.L, cycles, reg);
	};

	// SET 7,(HL)
	prefixed[0xFE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(7, getAddress(reg.H, reg.L), cycles, reg, mem);
	};

	// SET 7,A
	prefixed[0xFF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_SET(7, reg.A, cycles, reg);
	};
}

namespace {
	uint16_t getAddress(uint8_t a, uint8_t b) {
		return (uint16_t) a << 8 | (uint16_t) b;
	}

	// ## 8-bit arithmetic and logic instructions ##

	// ADC A,r8
	void op_ADC(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (data & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) data + (uint16_t) reg.getCarry();

		reg.A += data + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	}

	// ADC A,[HL]
	void op_ADC(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		
		uint8_t HL = (mem.read(address));
		reg.setHalfCarry(((reg.A & 0xF) + (HL & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) HL + (uint16_t) reg.getCarry();

		reg.A += HL + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	}

	// ADC A,n8
	void op_ADC(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.setHalfCarry(((reg.A & 0xF) + (mem.read(reg.PC + 1) & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) mem.read(reg.PC + 1) + (uint16_t) reg.getCarry();

		reg.A += mem.read(reg.PC + 1) + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 2;
	}

	// ADD A,r8
	void op_ADD(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (data & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) data;

		reg.A += data;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	}

	// ADD A,[HL]
	void op_ADD(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		
		uint8_t HL = (mem.read(address));
		reg.setHalfCarry(((reg.A & 0xF) + (HL & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) a + (uint16_t) HL;

		a += HL;

		reg.setZero(a == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	}
	
	// ADD A,n8
	void op_ADD(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.setHalfCarry(((reg.A & 0xF) + (mem.read(reg.PC + 1) & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) mem.read(reg.PC + 1);

		reg.A += mem.read(reg.PC + 1);

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 2;
	}

	// AND A,r8
	void op_AND(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.A &= data;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 1;
	}

	// AND A,[HL]
	void op_AND(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A &= mem.read(address);

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 1;
	}

	// AND A,n8
	void op_AND(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A &= mem.read(reg.PC + 1);

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 2;
	}

	// CP A,r8
	void op_CP(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 4;

		uint8_t result = reg.A - data;

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (data & 0xF)) & 0x10 == 0x10);
		reg.setCarry(data > reg.A);

		reg.PC += 1;
	}
	
	// CP A,[HL]
	void op_CP(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		uint8_t HL = (mem.read(address));
		uint8_t result = reg.A - HL;

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (HL & 0xF)) & 0x10 == 0x10);
		reg.setCarry(HL > reg.A);

		reg.PC += 1;
	}
	
	// CP A,n8
	void op_CP(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		uint8_t result = reg.A - mem.read(reg.PC + 1);

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (mem.read(reg.PC + 1) & 0xF)) & 0x10 == 0x10);
		reg.setCarry(mem.read(reg.PC + 1) > reg.A);

		reg.PC += 2;
	}

	// DEC r8
	void op_DEC(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.setHalfCarry(((data & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		data--;

		reg.setZero(data == 0);
		reg.setNegative(true);

		reg.PC += 1;
	}
	
	// DEC [HL]
	void op_DEC(uint16_t address, unsigned int& cycles, Registers& reg) {
		cycles = 12;

		uint8_t result = mem.read(address);

		reg.setHalfCarry(((result & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		result++;

		reg.setZero(result == 0);
		reg.setNegative(true);

		mem.write(address, result);

		reg.PC += 1;
	}

	// INC r8
	void op_INC(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.setHalfCarry(((data & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		data++;

		reg.setZero(data == 0);
		reg.setNegative(false);

		reg.PC += 1;
	}
	
	// INC [HL]
	void op_INC(uint16_t address, unsigned int& cycles, Registers& reg) {
		cycles = 12;

		uint8_t result = mem.read(address);

		reg.setHalfCarry(((result & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		result++;

		reg.setZero(result == 0);
		reg.setNegative(false);

		mem.write(address, result);

		reg.PC += 1;
	}

	// OR A,r8
	void op_OR(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.A |= data;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	}

	// OR A,[HL]
	void op_OR(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A |= mem.read(address);

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	}

	// OR A,n8
	void op_OR(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A ^= mem.read(reg.PC + 1);

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 2;
	}

	// SBC A,r8
	void op_SBC(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (data & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) data - (uint16_t) reg.getCarry();
		
		reg.setCarry(data + reg.getCarry() > reg.A);

		reg.A -= result;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	}

	// SBC A,[HL]
	void op_SBC(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		
		uint8_t HL = (mem.read(address));
		reg.setHalfCarry(((reg.A & 0xF) - (HL & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) HL - (uint16_t) reg.getCarry();
		
		reg.setCarry(HL + reg.getCarry() > reg.A);

		reg.A -= result;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	}

	// SBC A,n8
	void op_SBC(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.setHalfCarry(((reg.A & 0xF) - (mem.read(reg.PC + 1) & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) mem.read(reg.PC + 1) - (uint16_t) reg.getCarry();

		reg.setCarry(mem.read(reg.PC + 1) > reg.A);

		reg.A -= mem.read(reg.PC + 1) - reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 2;
	}

	// SUB A,r8
	void op_SUB(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (data & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) data;
		
		reg.setCarry(data > reg.A);

		reg.A -= data;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	}

	// SUB A,[HL]
	void op_SUB(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		
		uint8_t HL = (mem.read(address));
		reg.setHalfCarry(((reg.A & 0xF) - (HL & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) HL;
		
		reg.setCarry(HL > reg.A);

		reg.A -= HL;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	}
	
	// SUB A,n8
	void op_SUB(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.setHalfCarry(((reg.A & 0xF) - (mem.read(reg.PC + 1) & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) mem.read(reg.PC + 1);
		
		reg.setCarry(mem.read(reg.PC + 1) > reg.A);

		reg.A -= mem.read(reg.PC + 1);

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 2;
	}

	// XOR A,r8
	void op_XOR(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.A ^= data;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	}

	// XOR A,[HL]
	void op_XOR(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A ^= mem.read(address);

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	}

	// XOR A,n8
	void op_XOR(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A ^= mem.read(reg.PC + 1);

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 2;
	}

	// ## 16-bit arithmetic instructions ##

	// ADD HL,r16
	void op_ADD16(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		uint16_t HL = (uint16_t) reg.H << 8 | (uint16_t) reg.L;
		uint16_t AB = (uint16_t) a << 8 | (uint16_t) b;

		reg.setCarry((uint32_t) HL + (uint32_t) AB < 0xFFFF);
		reg.setHalfCarry(((HL & 0x0FFF) + ((AB) & 0x0FFF)) & 0x1000 == 0x1000);

		HL += AB;

		reg.setNegative(0);

		reg.H = (uint8_t) (HL >> 8) & 0xFF;
		reg.L = (uint8_t) HL & 0xFF;

		reg.PC += 1;
	}

	// DEC r16
	void op_DEC16(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		b++;
		if (b == 0xFF) {
			a--;
		}

		reg.PC += 1;
	}

	// INC r16
	void op_INC16(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg) {
		cycles = 8;
		
		b++;
		if (b == 0) {
			a++;
		}

		reg.PC += 1;
	}

	// ## Bit operation instructions ##

	// BIT u3,r8
	void op_BIT(int bit, uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		reg.setZero(!(data >> bit));
		reg.setNegative(false);
		reg.setHalfCarry(true);

		reg.PC += 2;
	}
	
	// BIT u3,[HL]
	void op_BIT(int bit, uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t data = mem.read(address);
		op_BIT(bit, data, cycles, reg);

		cycles = 16;
	}
	
	// RES u3,r8
	void op_RES(int bit, uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		data &= ~(1 << bit);

		reg.PC += 2;
	}
	
	// RES u3,[HL]
	void op_RES(int bit, uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t data = mem.read(address);
		op_RES(bit, data, cycles, reg);
		mem.write(address, data);

		cycles = 16;
	}
	
	// SET u3,r8
	void op_SET(int bit, uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		data |= 1 << bit;

		reg.PC += 2;
	}
	
	// SET u3,[HL]
	void op_SET(int bit, uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t data = mem.read(address);
		op_SET(bit, data, cycles, reg);
		mem.write(address, data);

		cycles = 16;
	}
	
	// SWAP r8
	void op_SWAP(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		uint8_t hi = data >> 4;
		uint8_t lo = data & 0xF;
		data = lo << 4 | hi;

		reg.setZero(data == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 2;
	}
	
	// SWAP [HL]
	void op_SWAP(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t data = mem.read(address);
		op_SWAP(data, cycles, reg);
		mem.write(address, data);

		cycles = 16;
	}

	// ## Bit shift instructions ##

	// RL r8
	void op_RL(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		bool tempCarry = reg.getCarry();
		bool newCarry = data >> 7;

		data = data << 1 | (short) tempCarry;

		reg.setCarry(newCarry);

		reg.setZero(data == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 2;
	}
	
	// RL [HL]
	void op_RL(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t data = mem.read(address);
		op_RL(data, cycles, reg);
		mem.write(address, data);
		
		cycles = 16;
	}
	
	// RLA
	void op_RLA(unsigned int& cycles, Registers& reg) {
		cycles = 4;

		bool tempCarry = reg.getCarry();
		bool newCarry = reg.A >> 7;
		reg.A = reg.A << 1 | tempCarry;
		reg.setCarry(newCarry);
		reg.setZero(false);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 1;
	}

	// RLC r8
	void op_RLC(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		reg.setCarry((data & 0x80) >> 7);
		data = (data << 1 | reg.getCarry());

		reg.setZero(data == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 2;
	}
	
	// RLC [HL]
	void op_RLC(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t data = mem.read(address);
		op_RLC(data, cycles, reg, mem);
		mem.write(address, data);
		
		cycles = 16;
	}
	
	// RLCA
	void op_RLCA(unsigned int& cycles, Registers& reg) {
		cycles = 4;

		bool tempCarry = reg.A >> 7;
		reg.setCarry(tempCarry);
		reg.A = reg.A << 1 | tempCarry;
		reg.setZero(false);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 1;
	}
	
	// RR r8
	void op_RR(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		bool tempCarry = reg.getCarry();

		reg.setCarry(data & 0x1);
		data = tempCarry << 7 | data >> 1;

		reg.setZero(data == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 2;
	}
	
	// RR [HL]
	void op_RR(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t data = mem.read(address);
		op_RR(data, cycles, reg);
		mem.write(address, data);
		
		cycles = 16;
	}
	
	// RRA
	void op_RRA(unsigned int& cycles, Registers& reg) {
		cycles = 4;

		bool tempCarry = reg.getCarry();
		bool newCarry = reg.A & 0x01;
		reg.setCarry(newCarry);
		reg.A = tempCarry << 7 | reg.A >> 1;
		reg.setZero(false);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 1;
	}
	
	// RRC r8
	void op_RRC(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		reg.setCarry(data & 0x1);
		data = (reg.getCarry() << 7 | data >> 1);

		reg.setZero(data == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 2;
	}
	
	// RRC [HL]
	void op_RRC(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t data = mem.read(address);
		op_RRC(data, cycles, reg, mem);
		mem.write(address, data);
		
		cycles = 16;
	}
	
	// RRCA
	void op_RRCA(unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.setCarry(reg.A & 0x1);
		reg.A = (reg.getCarry() << 7 | reg.A >> 1);

		reg.setZero(false);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 1;
	}
	
	// SLA r8
	void op_SLA(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		reg.setCarry(data & (1 << 7) >> 7);
		data <<= 1;

		reg.setZero(data == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 2;
	}
	
	// SLA [HL]
	void op_SLA(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t data = mem.read(address);
		op_SLA(data, cycles, reg);
		mem.write(address, data);

		cycles = 16;
	}
	
	// SRA r8
	void op_SRA(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		bool bit7 = data >> 7;

		reg.setCarry(data & 0x1);
		data = (short) bit7 << 7 | data >> 1;

		reg.setZero(data == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 2;
	}
	
	// SRA [HL]
	void op_SRA(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t data = mem.read(address);
		op_SRA(data, cycles, reg);
		mem.write(address, data);

		cycles = 16;
	}
	
	// SRL r8
	void op_SRL(uint8_t& data, unsigned int& cycles, Registers& reg) {
		cycles = 8;

		reg.setCarry(data & 0x1);
		
		data >>= 1;

		reg.setZero(data == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 2;
	}
	
	// SRL [HL]
	void op_SRL(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t data = mem.read(address);
		op_SRL(data, cycles, reg);
		mem.write(address, data);

		cycles = 16;
	}

	// ## Load instructions ##

	// PN = number -> pointer; RP = pointer -> register, etc.

	// LD r8,r8
	void op_LD_RR(uint8_t& dest, uint8_t& src, unsigned int& cycles, Registers& reg) {
		cycles = 4;

		dest = src;

		reg.PC += 1;
	}
	
	// LD r8,n8
	void op_LD_RN(uint8_t& dest, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		dest = mem.read(reg.PC + 1);

		reg.PC += 2;
	}
	
	// LD r16,n16
	void op_LD16_RN(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;
		a = mem.read(reg.PC + 2);
		b = mem.read(reg.PC + 1);
		reg.PC += 3;
	}
	
	// LD [HL],r8
	void op_LD_PR(uint16_t address, uint8_t data, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		mem.write(address, data);
		reg.PC += 1;
	}
	
	// LD [HL],n8
	void op_LD_PN(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		mem.write(address, mem.read(reg.PC + 1));

		reg.PC += 2;
	}

	// LD r8,[HL]
	void op_LD_RP(uint8_t& dest, uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		dest = mem.read(address);
		reg.PC += 1;
	}

	// LD [n16],A
	void op_LD16_NA(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		mem.write(getAddress(mem.read(reg.PC + 2), mem.read(reg.PC + 1)), reg.A);

		reg.PC += 3;
	}
	
	// LD A,[n16]
	void op_LD16_AN(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.A = mem.read((uint16_t) mem.read(reg.PC + 2) << 8 | (uint16_t) mem.read(reg.PC + 1));

		reg.PC += 3;
	}
	
	// LDH [n16],A
	void op_LDH_PA(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;
		mem.write(0xFF00 + (uint16_t) mem.read(reg.PC + 1), reg.A);
		reg.PC += 2;
	}
	
	// LDH [C],A
	void op_LDH_CA(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;
		mem.write(0xFF00 + (uint16_t) reg.C, reg.A);
		reg.PC += 2;
	}

	// LDH A,[n16]
	void op_LDH_AP(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.A = mem.read(0xFF00 + (uint16_t) mem.read(reg.PC + 1));

		reg.PC += 2;
	}
	
	// LDH A,[C]
	void op_LDH_AC(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.A = mem.read(0xFF00 + (uint16_t) reg.C);

		reg.PC += 2;
	}

	// LD [HLI],A
	void op_LD_IW(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(getAddress(reg.H, reg.L), reg.A);
		reg.L++;
		if (reg.L == 0) {
			reg.H++;
		}

		reg.PC += 1;
	}
	
	// LD [HLD],A
	void op_LD_DW(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(getAddress(reg.H, reg.L), reg.A);
		reg.L--;
		if (reg.L == 0xFF) {
			reg.H--;
		}

		reg.PC += 1;
	}
	
	// LD A,[HLI]
	void op_LD_IR(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A = mem.read(getAddress(reg.H, reg.L));
		reg.L++;
		if (reg.L == 0) {
			reg.H++;
		}

		reg.PC += 1;
	}
	
	// LD A,[HLD]
	void op_LD_DR(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A = mem.read(getAddress(reg.H, reg.L));
		reg.L--;
		if (reg.L == 0xFF) {
			reg.H--;
		}

		reg.PC += 1;
	}

	// ## Jumps and subroutines ##

	// CALL n16
	void op_CALL(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 24;

		uint8_t hi = (reg.PC + 2) >> 8;
		uint8_t lo = (reg.PC + 2) & 0xFF;

		reg.SP--;
		mem.write(reg.SP, hi);
		reg.SP--;
		mem.write(reg.SP, lo);
		reg.PC = ((uint16_t) mem.read(reg.PC + 2) << 8) | (uint16_t) mem.read(reg.PC + 1);
	}

	// CALL cc,n16
	void op_CALL(bool cond, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		if (cond) {
			cycles = 24;

			uint8_t hi = (reg.PC + 2) >> 8;
			uint8_t lo = (reg.PC + 2) & 0xFF;

			reg.SP--;
			mem.write(reg.SP, hi);
			reg.SP--;
			mem.write(reg.SP, lo);
			reg.PC = ((uint16_t) mem.read(reg.PC + 2) << 8) | (uint16_t) mem.read(reg.PC + 1);
		}
		else {
			reg.PC += 3;
		}
	}

	// JP HL
	void op_JP_HL(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.PC = ((uint16_t) mem.read(reg.H) << 8) | (uint16_t) reg.L;
	}

	// JP n16
	void op_JP(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.PC = ((uint16_t) mem.read(reg.PC + 2) << 8) | (uint16_t) mem.read(reg.PC + 1);
	}

	// JP cc,n16
	void op_JP(bool cond, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		if (cond) {
			reg.PC = ((uint16_t) mem.read(reg.PC + 2) << 8) | (uint16_t) mem.read(reg.PC + 1);
			cycles = 16;
		}
		else {
			reg.PC += 3;
		}
	}

	// JR e8
	void op_JR(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		int8_t offset = mem.read(reg.PC + 1) + 1;
		reg.PC += (uint16_t) offset;

		reg.PC += 2;
	}

	// JR cc,e8
	void op_JR(bool cond, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		if (cond) {
			cycles = 12;
			reg.PC += (int8_t) mem.read(reg.PC + 1);
		}

		reg.PC += 2;
	}

	// RET cc
	void op_RET(bool cond, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		if (cond) {
			reg.PC = ((uint16_t) mem.read(reg.SP + 1) << 8) | (uint16_t) mem.read(reg.SP);
			reg.SP += 2;
			cycles = 20;
		}
		else {
			reg.PC += 1;
		}
	}

	// RET
	void op_RET(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.PC = ((uint16_t) mem.read(reg.SP + 1) << 8) | (uint16_t) mem.read(reg.SP);
		reg.SP += 2;
	}
	
	// RETI
	void op_RETI(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.PC = ((uint16_t) mem.read(reg.SP + 1) << 8) | (uint16_t) mem.read(reg.SP);
		reg.SP += 2;

		mem.interruptsEnable = true;
	}

	// RST vec
	void op_RST(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		uint8_t hi = (reg.PC + 2) >> 8;
		uint8_t lo = (reg.PC + 2) & 0xFF;

		reg.SP--;
		mem.write(reg.SP, hi);
		reg.SP--;
		mem.write(reg.SP, lo);

		reg.PC = address;
	}

	// ## Stack operation instructions ##

	// ADD HL,SP
	void op_ADD_HL_SP(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		uint16_t HL = (uint16_t) reg.H << 8 | (uint16_t) reg.L;
		uint16_t SP = reg.SP;

		reg.setCarry((uint32_t) HL + (uint32_t) SP < 0xFFFF);
		reg.setHalfCarry(((HL & 0x0FFF) + ((SP) & 0x0FFF)) & 0x1000 == 0x1000);

		HL += SP;

		reg.setNegative(0);

		reg.H = (uint8_t) (HL >> 8) & 0xFF;
		reg.L = (uint8_t) HL & 0xFF;

		reg.PC += 1;
	}
	
	// ADD SP,e8  // TODO: Verify this
	void op_ADD_SP(unsigned int cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		int8_t i8 = mem.read(reg.PC + 1);
		reg.setHalfCarry(((reg.A & 0xF) + (((uint16_t) mem.read(reg.PC + 1)) & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) mem.read(reg.PC + 1);

		reg.A += (uint16_t) i8;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 2;
	}

	// DEC SP
	void op_DEC_SP(unsigned int& cycles, Registers& reg) {
		cycles = 8;

		reg.SP--;

		reg.PC += 1;
	}
	
	// INC SP
	void op_INC_SP(unsigned int& cycles, Registers& reg) {
		cycles = 8;
		
		reg.SP++;

		reg.PC += 1;
	}

	// LD SP,n16
	void op_LD_SPR(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.SP = (uint16_t) mem.read(reg.PC + 2) << 8 | (uint16_t) mem.read(reg.PC + 1);

		reg.PC += 3;
	}

	// LD [n16],SP
	void op_LD_SPW(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 20;

		mem.write(getAddress(mem.read(reg.PC + 2), mem.read(reg.PC + 1)), (uint8_t) reg.SP & 0xFF);
		mem.write(getAddress(mem.read(reg.PC + 2), mem.read(reg.PC + 1)) + 1, (uint8_t) reg.SP >> 8);

		reg.PC += 3;
	}
	
	// LD HL,SP+e8  // TODO: verify this
	void op_LD_HL_SP(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		int8_t i8 = mem.read(reg.PC + 1);
		uint16_t result = (uint16_t) reg.SP + (uint16_t) i8;

		reg.setHalfCarry(((reg.SP & 0xF) + ((uint16_t) i8 & 0xF)) >= 0x10);
		reg.setCarry(((uint16_t) i8 & 0xFF) + (reg.SP & 0xFF) > 0xFF);

		reg.H = (uint8_t) (result >> 8);
		reg.L = (uint8_t) (result & 0xFF);

		reg.setZero(false);
		reg.setNegative(false);

		reg.PC += 2;
	}

	// LD SP,HL
	void op_LD_SP_HL(unsigned int& cycles, Registers& reg) {
		cycles = 8;

		reg.SP = getAddress(reg.H, reg.L);

		reg.PC += 1;
	}
	
	// POP AF
	void op_POP_AF(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.SP++;
		reg.F = mem.read(reg.SP) & 0xF0;
		reg.SP++;
		reg.A = mem.read(reg.SP);

		reg.PC += 1;
	}

	// POP r16
	void op_POP(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		b = mem.read(reg.SP);
		reg.SP++;
		a = mem.read(reg.SP);
		reg.SP++;

		reg.PC += 1;
	}

	// PUSH AF
	void op_PUSH_AF(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.SP--;
		mem.write(reg.SP, reg.A);
		reg.SP--;
		mem.write(reg.SP, reg.F);

		reg.PC += 1;
	}

	// PUSH r16
	void op_PUSH(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.SP--;
		mem.write(reg.SP, a);
		reg.SP--;
		mem.write(reg.SP, b);

		reg.PC += 1;
	}

	// ## Miscellaneous instructons ##

	// CCF
	void op_CCF(unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(!reg.getCarry());

		reg.PC += 1;
	}
	
	// CPL
	void op_CPL(unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.A = ~reg.A;
		reg.setNegative(true);
		reg.setHalfCarry(true);

		reg.PC += 1;
	}

	// DAA
	void op_DAA(unsigned int& cycles, Registers& reg) {
		cycles = 4;

		if (reg.getNegative()) {
			if (reg.A < 0x99 || reg.getCarry()) {
				reg.A += 0x60;
				reg.setCarry(1);
			}
			if (reg.getHalfCarry() || reg.A & 0xF > 0x09) {
				reg.A += 0x06;
			}
		}
		else {
			if (reg.getCarry()) {
				reg.A -= 0x60;
			}
			if (reg.getHalfCarry()) {
				reg.A -= 0x06;
			}
		}

		if (reg.A == 0) {
			reg.setZero(true);
		}
		else {
			reg.setZero(false);
		}
		reg.setHalfCarry(false);

		reg.PC += 1;
	}
	
	// DI
	void op_DI(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		mem.interruptsEnable = false;  // TODO: what does this do

		reg.PC += 1;
	}

	// EI
	void op_EI(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		// TODO: implement interrupts
		std::cerr << "unsupported instruction 0xFB, exiting...";
		exit(5);

		reg.PC += 1;
	}

	// HALT
	void op_HALT(unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.HALT = true;  // TODO: implement HALT

		reg.PC += 1;
	}

	// NOP
	void op_NOP(unsigned int& cycles, Registers& reg) {
		cycles = 4;  // 4 ticks
		reg.PC += 1;  // 1 byte
	}
	
	// SCF
	void op_SCF(unsigned int& cycles, Registers& reg) {
		cycles = 4;

		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(true);

		reg.PC += 1;
	}

	// STOP
	void op_STOP(unsigned int& cycles, Registers& reg) {
		cycles = 4;
		reg.PC += 2;
	}

}

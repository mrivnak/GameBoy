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
		op_LD(reg.B, reg.C, cycles, reg, mem);
	};

	// LD (BC),A
	nonPrefixed[0x02] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD(getAddress(reg.B, reg.C), reg.A, cycles, reg, mem);
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
		op_LD(reg.B, cycles, reg, mem);
	};

	// RLCA
	nonPrefixed[0x07] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RLCA(cycles, reg);
	};

	// LD (u16),SP
	nonPrefixed[0x08] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD_SP(cycles, reg, mem);
	};

	// ADD HL,BC
	nonPrefixed[0x09] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_ADD16(reg.B, reg.C, cycles, reg);
	};

	// LD A,(BC)
	nonPrefixed[0x0A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD(reg.A, getAddress(reg.B, reg.C), cycles, reg, mem);
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
		op_LD(reg.C, cycles, reg, mem);
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
		op_LD(reg.D, reg.E, cycles, reg, mem);
	};

	// LD (DE),A
	nonPrefixed[0x12] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LD(getAddress(reg.H, reg.L), cycles, reg, mem);
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
		op_LD(reg.D, cycles, reg, mem);
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
		op_LD(reg.A, getAddress(reg.D, reg.E), cycles, reg, mem);
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
		op_LD(reg.E, cycles, reg, mem);
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
		op_LD(reg.H, reg.L, cycles, reg, mem);
	};

	// LD (HL+),A
	nonPrefixed[0x22] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_LDIW(cycles, reg, mem);
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
		op_LD(reg.H, cycles, reg, mem);
	};
	
	// DAA
	nonPrefixed[0x27] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_DAA(cycles, reg);
	};

	// JR Z,i8
	nonPrefixed[0x28] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		if (reg.getZero()) {
			cycles = 12;
			int8_t offset = mem.read(reg.PC + 1) + 1;
			reg.PC += (uint16_t) offset;
		}

		reg.PC += 2;
	};

	// ADD HL,HL
	nonPrefixed[0x29] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		uint16_t HL = (uint16_t) reg.H << 8 | (uint16_t) reg.L;

		reg.setCarry((uint32_t) HL + (uint32_t) HL < 0xFFFF);
		reg.setHalfCarry(((HL & 0x0FFF) + (HL & 0x0FFF)) & 0x1000 == 0x1000);

		HL += HL;

		reg.setNegative(0);

		reg.H = (uint8_t) (HL >> 8) & 0xFF;
		reg.L = (uint8_t) HL & 0xFF;

		reg.PC += 1;
	};

	// LD A,(HL+)
	nonPrefixed[0x2A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A = mem.read((uint16_t) reg.H << 8 | (uint16_t) reg.H);
		reg.L++;
		if (reg.L == 0) {
			reg.H++;
		}

		reg.PC += 1;
	};

	// DEC HL
	nonPrefixed[0x2B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.L--;
		if (reg.L == 0xFF) {
			reg.H--;
		}

		reg.PC += 1;
	};

	// INC L
	nonPrefixed[0x2C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.L & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		reg.L++;

		reg.setZero(reg.L == 0);
		reg.setNegative(false);

		reg.PC += 1;
	};

	// DEC L
	nonPrefixed[0x2D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.L & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		reg.L--;

		reg.setZero(reg.L == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// LD L,u8
	nonPrefixed[0x2E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.L = mem.read(reg.PC + 1);

		reg.PC += 2;
	};

	// CPL
	nonPrefixed[0x2F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A = ~reg.A;
		reg.setNegative(true);
		reg.setHalfCarry(true);

		reg.PC += 1;
	};

	// JR NC,i8
	nonPrefixed[0x30] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		if (!reg.getCarry()) {
			cycles = 12;
			int8_t offset = mem.read(reg.PC + 1) + 1;
			reg.PC += (uint16_t) offset;
		}

		reg.PC += 2;
	};

	// LD SP,u16
	nonPrefixed[0x31] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.SP = (uint16_t) mem.read(reg.PC + 2) << 8 | (uint16_t) mem.read(reg.PC + 1);

		reg.PC += 3;
	};

	// LD (HL-),A
	nonPrefixed[0x32] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(((uint16_t) reg.H << 8) | reg.L, reg.A);
		reg.L--;
		if (reg.L == 0xFF) {
			reg.H--;
		}

		// printf("HL: %X%X\n", reg.H, reg.L);

		reg.PC += 1;
	};

	// INC SP
	nonPrefixed[0x33] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		
		reg.SP++;

		reg.PC += 1;
	};

	// INC (HL)
	nonPrefixed[0x34] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		uint8_t result = mem.read(getAddress(reg.H, reg.L));

		reg.setHalfCarry(((result & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		result++;

		reg.setZero(reg.H == 0);
		reg.setNegative(false);

		mem.write(getAddress(reg.H, reg.L), result);

		reg.PC += 1;
	};

	// DEC (HL)
	nonPrefixed[0x35] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		uint8_t result = mem.read(getAddress(reg.H, reg.L));

		reg.setHalfCarry(((result & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		result++;

		reg.setZero(reg.H == 0);
		reg.setNegative(true);

		mem.write(getAddress(reg.H, reg.L), result);

		reg.PC += 1;
	};

	// LD (HL),u8
	nonPrefixed[0x36] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		mem.write(getAddress(reg.H, reg.L), mem.read(reg.PC + 1));

		reg.PC += 2;
	};
	
	// SCF
	nonPrefixed[0x37] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(true);

		reg.PC += 1;
	};

	// JR C,i8
	nonPrefixed[0x38] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		if (reg.getCarry()) {
			cycles = 12;
			int8_t offset = mem.read(reg.PC + 1) + 1;
			reg.PC += (uint16_t) offset;
		}

		reg.PC += 2;
	};

	// ADD HL,SP
	nonPrefixed[0x39] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
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
	};

	// LD A,(HL-)
	nonPrefixed[0x3A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A = mem.read(getAddress(reg.H, reg.L));
		reg.L--;
		if (reg.L == 0xFF) {
			reg.H--;
		}

		reg.PC += 1;
	};

	// DEC SP
	nonPrefixed[0x3B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.SP--;

		reg.PC += 1;
	};

	// INC A
	nonPrefixed[0x3C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		reg.A++;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);

		reg.PC += 1;
	};

	// DEC A
	nonPrefixed[0x3D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		reg.A--;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// LD A,u8
	nonPrefixed[0x3E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A = mem.read(reg.PC + 1);

		reg.PC += 2;
	};

	// CCF
	nonPrefixed[0x3F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(!reg.getCarry());

		reg.PC += 1;
	};

	// LD B,B
	nonPrefixed[0x40] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.B = reg.B;

		reg.PC += 1;
	};

	// LD B,C
	nonPrefixed[0x41] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.B = reg.C;

		reg.PC += 1;
	};

	// LD B,D
	nonPrefixed[0x42] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.B = reg.D;

		reg.PC += 1;
	};

	// LD B,E
	nonPrefixed[0x43] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.B = reg.E;

		reg.PC += 1;
	};

	// LD B,H
	nonPrefixed[0x44] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.B = reg.H;

		reg.PC += 1;
	};

	// LD B,L
	nonPrefixed[0x45] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.B = reg.L;

		reg.PC += 1;
	};

	// LD B,(HL)
	nonPrefixed[0x46] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.B = mem.read(getAddress(reg.H, reg.L));

		reg.PC += 1;
	};

	// LD B,A
	nonPrefixed[0x46] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.B = reg.A;

		reg.PC += 1;
	};

	// LD C,B
	nonPrefixed[0x48] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.C = reg.B;

		reg.PC += 1;
	};

	// LD C,C
	nonPrefixed[0x49] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.C = reg.C;

		reg.PC += 1;
	};

	// LD C,D
	nonPrefixed[0x4A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.C = reg.D;

		reg.PC += 1;
	};

	// LD C,E
	nonPrefixed[0x4B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.C = reg.E;

		reg.PC += 1;
	};

	// LD C,H
	nonPrefixed[0x4C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.C = reg.H;

		reg.PC += 1;
	};

	// LD C,L
	nonPrefixed[0x4D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.C = reg.L;

		reg.PC += 1;
	};

	// LD C,(HL)
	nonPrefixed[0x4E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.C = mem.read(getAddress(reg.H, reg.L));

		reg.PC += 1;
	};

	// LD C,A
	nonPrefixed[0x4F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.C = reg.A;

		reg.PC += 1;
	};

	// LD D,B
	nonPrefixed[0x50] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.D = reg.B;

		reg.PC += 1;
	};

	// LD D,C
	nonPrefixed[0x51] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.D = reg.C;

		reg.PC += 1;
	};

	// LD D,D
	nonPrefixed[0x52] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.D = reg.D;

		reg.PC += 1;
	};

	// LD D,E
	nonPrefixed[0x53] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.D = reg.E;

		reg.PC += 1;
	};

	// LD D,H
	nonPrefixed[0x54] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.B = reg.H;

		reg.PC += 1;
	};

	// LD D,L
	nonPrefixed[0x55] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.D = reg.L;

		reg.PC += 1;
	};

	// LD D,(HL)
	nonPrefixed[0x56] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.D = mem.read(getAddress(reg.H, reg.L));

		reg.PC += 1;
	};

	// LD D,A
	nonPrefixed[0x57] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.D = reg.A;

		reg.PC += 1;
	};

	// LD E,B
	nonPrefixed[0x58] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.E = reg.B;

		reg.PC += 1;
	};

	// LD E,C
	nonPrefixed[0x59] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.E = reg.C;

		reg.PC += 1;
	};

	// LD E,D
	nonPrefixed[0x5A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.E = reg.D;

		reg.PC += 1;
	};

	// LD E,E
	nonPrefixed[0x5B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.E = reg.E;

		reg.PC += 1;
	};

	// LD E,H
	nonPrefixed[0x5C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.E = reg.H;

		reg.PC += 1;
	};

	// LD E,L
	nonPrefixed[0x5D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.E = reg.L;

		reg.PC += 1;
	};

	// LD E,(HL)
	nonPrefixed[0x5E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.E = mem.read(getAddress(reg.H, reg.L));

		reg.PC += 1;
	};

	// LD E,A
	nonPrefixed[0x5F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.E = reg.A;

		reg.PC += 1;
	};

	// LD H,B
	nonPrefixed[0x60] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.H = reg.B;

		reg.PC += 1;
	};

	// LD H,C
	nonPrefixed[0x61] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.H = reg.C;

		reg.PC += 1;
	};

	// LD H,D
	nonPrefixed[0x62] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.H = reg.D;

		reg.PC += 1;
	};

	// LD H,E
	nonPrefixed[0x63] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.H = reg.E;

		reg.PC += 1;
	};

	// LD H,H
	nonPrefixed[0x64] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.H = reg.H;

		reg.PC += 1;
	};

	// LD H,L
	nonPrefixed[0x65] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.H = reg.L;

		reg.PC += 1;
	};

	// LD G,(HL)
	nonPrefixed[0x66] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.H = mem.read(getAddress(reg.H, reg.L));

		reg.PC += 1;
	};

	// LD H,A
	nonPrefixed[0x67] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.H = reg.A;

		reg.PC += 1;
	};

	// LD L,B
	nonPrefixed[0x68] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.L = reg.B;

		reg.PC += 1;
	};

	// LD L,C
	nonPrefixed[0x69] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.L = reg.C;

		reg.PC += 1;
	};

	// LD L,D
	nonPrefixed[0x6A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.L = reg.D;

		reg.PC += 1;
	};

	// LD L,E
	nonPrefixed[0x6B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.L = reg.E;

		reg.PC += 1;
	};

	// LD L,H
	nonPrefixed[0x6C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.L = reg.H;

		reg.PC += 1;
	};

	// LD L,L
	nonPrefixed[0x6D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.L = reg.L;

		reg.PC += 1;
	};

	// LD L,(HL)
	nonPrefixed[0x6E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.L = mem.read(getAddress(reg.H, reg.L));

		reg.PC += 1;
	};

	// LD L,A
	nonPrefixed[0x6F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.L = reg.A;

		reg.PC += 1;
	};

	// LD (HL),B
	nonPrefixed[0x70] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(getAddress(reg.H, reg.L), reg.B);

		reg.PC += 1;
	};

	// LD (HL),C
	nonPrefixed[0x71] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(getAddress(reg.H, reg.L), reg.C);

		reg.PC += 1;
	};

	// LD (HL),D
	nonPrefixed[0x72] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(getAddress(reg.H, reg.L), reg.D);

		reg.PC += 1;
	};

	// LD (HL),E
	nonPrefixed[0x73] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(getAddress(reg.H, reg.L), reg.E);

		reg.PC += 1;
	};

	// LD (HL),H
	nonPrefixed[0x74] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(getAddress(reg.H, reg.L), reg.H);

		reg.PC += 1;
	};

	// LD (HL),L
	nonPrefixed[0x75] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(getAddress(reg.H, reg.L), reg.L);

		reg.PC += 1;
	};

	// HALT
	nonPrefixed[0x76] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.HALT = true;  // implement HALT

		reg.PC += 1;
	};

	// LD (HL),A
	nonPrefixed[0x77] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(getAddress(reg.H, reg.L), reg.A);

		reg.PC += 1;
	};

	// LD A,B
	nonPrefixed[0x78] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A = reg.B;

		reg.PC += 1;
	};

	// LD A,C
	nonPrefixed[0x79] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A = reg.C;

		reg.PC += 1;
	};

	// LD A,D
	nonPrefixed[0x7A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A = reg.D;

		reg.PC += 1;
	};

	// LD A,E
	nonPrefixed[0x7B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A = reg.E;

		reg.PC += 1;
	};

	// LD A,H
	nonPrefixed[0x7C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A = reg.H;

		reg.PC += 1;
	};

	// LD A,L
	nonPrefixed[0x7D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A = reg.L;

		reg.PC += 1;
	};

	// LD A,(HL)
	nonPrefixed[0x7E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A = mem.read(getAddress(reg.H, reg.L));

		reg.PC += 1;
	};

	// LD A,A
	nonPrefixed[0x7F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A = reg.A;

		reg.PC += 1;
	};

	// ADD A,B
	nonPrefixed[0x80] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.B & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.B;

		reg.A += reg.B;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADD A,C
	nonPrefixed[0x81] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.C & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.C;

		reg.A += reg.C;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADD A,D
	nonPrefixed[0x82] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.D & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.D;

		reg.A += reg.D;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADD A,E
	nonPrefixed[0x83] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.E & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.E;

		reg.A += reg.E;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADD A,H
	nonPrefixed[0x84] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.H & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.H;

		reg.A += reg.H;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADD A,L
	nonPrefixed[0x85] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.L & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.L;

		reg.A += reg.L;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADD A,(HL)
	nonPrefixed[0x86] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		
		uint8_t HL = (mem.read(getAddress(reg.H, reg.L)));
		reg.setHalfCarry(((reg.A & 0xF) + (HL & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) HL;

		reg.A += HL;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADD A,A
	nonPrefixed[0x87] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.A & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.A;

		reg.A += reg.A;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADC A,B
	nonPrefixed[0x88] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.B & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.B + (uint16_t) reg.getCarry();

		reg.A += reg.B + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADC A,C
	nonPrefixed[0x89] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.C & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.C + (uint16_t) reg.getCarry();

		reg.A += reg.C + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADC A,D
	nonPrefixed[0x8A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.D & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.D + (uint16_t) reg.getCarry();

		reg.A += reg.D + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADC A,E
	nonPrefixed[0x8B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.E & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.E + (uint16_t) reg.getCarry();

		reg.A += reg.E + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADC A,H
	nonPrefixed[0x8C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.H & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.H + (uint16_t) reg.getCarry();

		reg.A += reg.H + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADC A,L
	nonPrefixed[0x8D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.L & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.L + (uint16_t) reg.getCarry();

		reg.A += reg.L + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADC A,(HL)
	nonPrefixed[0x8E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		
		uint8_t HL = (mem.read(getAddress(reg.H, reg.L)));
		reg.setHalfCarry(((reg.A & 0xF) + (HL & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) HL + (uint16_t) reg.getCarry();

		reg.A += HL + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// ADC A,A
	nonPrefixed[0x8F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.A & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.A + (uint16_t) reg.getCarry();

		reg.A += reg.A + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// SUB A,B
	nonPrefixed[0x90] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.B & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.B;
		
		reg.setCarry(reg.B > reg.A);

		reg.A -= reg.B;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SUB A,C
	nonPrefixed[0x91] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.C & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.C;
		
		reg.setCarry(reg.C > reg.A);

		reg.A -= reg.C;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SUB A,D
	nonPrefixed[0x92] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.D & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.D;
		
		reg.setCarry(reg.D > reg.A);

		reg.A -= reg.D;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SUB A,E
	nonPrefixed[0x93] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.E & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.E;
		
		reg.setCarry(reg.E > reg.A);

		reg.A -= reg.E;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SUB A,H
	nonPrefixed[0x94] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.H& 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.H;
		
		reg.setCarry(reg.H > reg.A);

		reg.A -= reg.H;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SUB A,L
	nonPrefixed[0x95] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.L & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.L;
		
		reg.setCarry(reg.L > reg.A);

		reg.A -= reg.L;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SUB A,(HL)
	nonPrefixed[0x96] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		
		uint8_t HL = (mem.read(getAddress(reg.H, reg.L)));
		reg.setHalfCarry(((reg.A & 0xF) - (HL & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) HL;
		
		reg.setCarry(HL > reg.A);

		reg.A -= HL;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SUB A,A
	nonPrefixed[0x97] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.A & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.A;
		
		reg.setCarry(reg.A > reg.A);

		reg.A -= reg.A;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SBC A,B
	nonPrefixed[0x98] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.B & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.B - (uint16_t) reg.getCarry();
		
		reg.setCarry(reg.B + reg.getCarry() > reg.A);

		reg.A -= result;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SBC A,C
	nonPrefixed[0x99] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.C & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.C - (uint16_t) reg.getCarry();
		
		reg.setCarry(reg.C + reg.getCarry() > reg.A);

		reg.A -= result;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SBC A,D
	nonPrefixed[0x9A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.D & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.D - (uint16_t) reg.getCarry();
		
		reg.setCarry(reg.D + reg.getCarry() > reg.A);

		reg.A -= result;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SBC A,E
	nonPrefixed[0x9B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.E & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.E - (uint16_t) reg.getCarry();
		
		reg.setCarry(reg.E + reg.getCarry() > reg.A);

		reg.A -= result;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SBC A,H
	nonPrefixed[0x9C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.H & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.H - (uint16_t) reg.getCarry();
		
		reg.setCarry(reg.H + reg.getCarry() > reg.A);

		reg.A -= result;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SBC A,L
	nonPrefixed[0x9D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.L & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.L - (uint16_t) reg.getCarry();
		
		reg.setCarry(reg.L + reg.getCarry() > reg.A);

		reg.A -= result;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SBC A,(HL)
	nonPrefixed[0x9E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		
		uint8_t HL = (mem.read(getAddress(reg.H, reg.L)));
		reg.setHalfCarry(((reg.A & 0xF) - (HL & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) HL - (uint16_t) reg.getCarry();
		
		reg.setCarry(HL + reg.getCarry() > reg.A);

		reg.A -= result;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// SBC A,A
	nonPrefixed[0x9F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) - (reg.A & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) reg.A - (uint16_t) reg.getCarry();
		
		reg.setCarry(reg.A + reg.getCarry() > reg.A);

		reg.A -= result;

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// AND A,B
	nonPrefixed[0xA0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A &= reg.B;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 1;
	};

	// AND A,C
	nonPrefixed[0xA1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A &= reg.C;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 1;
	};

	// AND A,D
	nonPrefixed[0xA2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A &= reg.D;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 1;
	};

	// AND A,E
	nonPrefixed[0xA3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A &= reg.E;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 1;
	};

	// AND A,H
	nonPrefixed[0xA4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A &= reg.H;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 1;
	};

	// AND A,L
	nonPrefixed[0xA5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A &= reg.L;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 1;
	};

	// AND A,(HL)
	nonPrefixed[0xA6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A &= mem.read(getAddress(reg.H, reg.L));

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 1;
	};

	// AND A,A
	nonPrefixed[0xA7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A &= reg.A;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 1;
	};

	// XOR A,B
	nonPrefixed[0xA8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A ^= reg.B;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// XOR A,C
	nonPrefixed[0xA9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A ^= reg.C;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// XOR A,D
	nonPrefixed[0xAA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A ^= reg.D;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// XOR A,E
	nonPrefixed[0xAB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A ^= reg.E;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// XOR A,H
	nonPrefixed[0xAC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A ^= reg.H;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// XOR A,L
	nonPrefixed[0xAD] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A ^= reg.L;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// XOR A,(HL)
	nonPrefixed[0xAE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A ^= mem.read(getAddress(reg.H, reg.L));

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// XOR A,A
	nonPrefixed[0xAF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A ^= reg.A;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);
		reg.PC += 1;
	};

	// OR A,B
	nonPrefixed[0xB0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A |= reg.B;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// OR A,C
	nonPrefixed[0xB1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A |= reg.C;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// OR A,D
	nonPrefixed[0xB2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A |= reg.D;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// OR A,E
	nonPrefixed[0xB3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A |= reg.E;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// OR A,H
	nonPrefixed[0xB4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A |= reg.H;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// OR A,L
	nonPrefixed[0xB5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A |= reg.L;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// OR A,(HL)
	nonPrefixed[0xB6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A |= mem.read(getAddress(reg.H, reg.L));

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 1;
	};

	// OR A,A
	nonPrefixed[0xB7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.A |= reg.A;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);
		reg.PC += 1;
	};

	// CP A,B
	nonPrefixed[0xB8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		uint8_t result = reg.A - reg.B;

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (reg.B & 0xF)) & 0x10 == 0x10);
		reg.setCarry(reg.B > reg.A);

		reg.PC += 1;
	};

	// CP A,C
	nonPrefixed[0xB9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		uint8_t result = reg.A - reg.C;

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (reg.C & 0xF)) & 0x10 == 0x10);
		reg.setCarry(reg.C > reg.A);

		reg.PC += 1;
	};

	// CP A,D
	nonPrefixed[0xBA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		uint8_t result = reg.A - reg.D;

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (reg.D & 0xF)) & 0x10 == 0x10);
		reg.setCarry(reg.D > reg.A);

		reg.PC += 1;
	};

	// CP A,E
	nonPrefixed[0xBB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		uint8_t result = reg.A - reg.E;

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (reg.E & 0xF)) & 0x10 == 0x10);
		reg.setCarry(reg.E > reg.A);

		reg.PC += 1;
	};

	// CP A,H
	nonPrefixed[0xBC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		uint8_t result = reg.A - reg.H;

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (reg.H & 0xF)) & 0x10 == 0x10);
		reg.setCarry(reg.H > reg.A);

		reg.PC += 1;
	};

	// CP A,L
	nonPrefixed[0xBD] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		uint8_t result = reg.A - reg.L;

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (reg.L & 0xF)) & 0x10 == 0x10);
		reg.setCarry(reg.L > reg.A);

		reg.PC += 1;
	};

	// CP A,(HL)
	nonPrefixed[0xBE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		uint8_t HL = (mem.read(getAddress(reg.H, reg.L)));
		uint8_t result = reg.A - HL;

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (HL & 0xF)) & 0x10 == 0x10);
		reg.setCarry(HL > reg.A);

		reg.PC += 1;
	};

	// CP A,A
	nonPrefixed[0xBF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		uint8_t result = reg.A - reg.A;

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (reg.A & 0xF)) & 0x10 == 0x10);
		reg.setCarry(reg.A > reg.A);

		reg.PC += 1;
	};

	// RET NZ
	nonPrefixed[0xC0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		if (!reg.getZero()) {
			reg.PC = ((uint16_t) mem.read(reg.SP + 1) << 8) | (uint16_t) mem.read(reg.SP);
			reg.SP += 2;
			cycles = 20;
		}
		else {
			reg.PC += 1;
		}
	};

	// POP BC
	nonPrefixed[0xC1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.C = mem.read(reg.SP);
		reg.SP++;
		reg.B = mem.read(reg.SP);
		reg.SP++;

		reg.PC += 1;
	};

	// JP NZ,u16
	nonPrefixed[0xC2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		if (!reg.getZero()) {
			reg.PC = ((uint16_t) mem.read(reg.PC + 2) << 8) | (uint16_t) mem.read(reg.PC + 1);
			cycles = 16;
		}
		else {
			reg.PC += 3;
		}
	};

	// JP u16
	nonPrefixed[0xC3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.PC = ((uint16_t) mem.read(reg.PC + 2) << 8) | (uint16_t) mem.read(reg.PC + 1);
	};

	// CALL NZ,u16
	nonPrefixed[0xC4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		if (!reg.getZero()) {
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
	};

	// PUSH BC
	nonPrefixed[0xC5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.SP--;
		mem.write(reg.SP, reg.B);
		reg.SP--;
		mem.write(reg.SP, reg.C);

		reg.PC += 1;
	};

	// ADD A,u8
	nonPrefixed[0xC6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.setHalfCarry(((reg.A & 0xF) + (mem.read(reg.PC + 1) & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) mem.read(reg.PC + 1);

		reg.A += mem.read(reg.PC + 1);

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 2;
	};

	// RST 0x00
	nonPrefixed[0xC7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0000, cycles, reg, mem);
	};

	// RET Z
	nonPrefixed[0xC8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		if (reg.getZero()) {
			reg.PC = ((uint16_t) mem.read(reg.SP + 1) << 8) | (uint16_t) mem.read(reg.SP);
			reg.SP += 2;
			cycles = 20;
		}
		else {
			reg.PC += 1;
		}
	};

	// RET
	nonPrefixed[0xC9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.PC = ((uint16_t) mem.read(reg.SP + 1) << 8) | (uint16_t) mem.read(reg.SP);
		reg.SP += 2;
	};

	// JP Z,u16
	nonPrefixed[0xCA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		if (reg.getZero()) {
			reg.PC = ((uint16_t) mem.read(reg.PC + 2) << 8) | (uint16_t) mem.read(reg.PC + 1);
			cycles = 16;
		}
		else {
			reg.PC += 3;
		}
	};

	// 0xCB Prefix

	// CALL Z,u16
	nonPrefixed[0xCC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		if (reg.getZero()) {
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
	};

	// CALL u16
	nonPrefixed[0xCD] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 24;

		uint8_t hi = (reg.PC + 2) >> 8;
		uint8_t lo = (reg.PC + 2) & 0xFF;

		reg.SP--;
		mem.write(reg.SP, hi);
		reg.SP--;
		mem.write(reg.SP, lo);
		reg.PC = ((uint16_t) mem.read(reg.PC + 2) << 8) | (uint16_t) mem.read(reg.PC + 1);
	};

	// ADC A,u8
	nonPrefixed[0xCE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.setHalfCarry(((reg.A & 0xF) + (mem.read(reg.PC + 1) & 0xF) + (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) mem.read(reg.PC + 1) + (uint16_t) reg.getCarry();

		reg.A += mem.read(reg.PC + 1) + reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 2;
	};

	// RST 0x08
	nonPrefixed[0xCF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0008, cycles, reg, mem);
	};

	// RET NC
	nonPrefixed[0xD0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		if (!reg.getCarry()) {
			reg.PC = ((uint16_t) mem.read(reg.SP + 1) << 8) | (uint16_t) mem.read(reg.SP);
			reg.SP += 2;
			cycles = 20;
		}
		else {
			reg.PC += 1;
		}
	};

	// POP DE
	nonPrefixed[0xD1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.E = mem.read(reg.SP);
		reg.SP++;
		reg.D = mem.read(reg.SP);
		reg.SP++;

		reg.PC += 1;
	};

	// JP NC,u16
	nonPrefixed[0xD2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		if (!reg.getCarry()) {
			reg.PC = ((uint16_t) mem.read(reg.PC + 2) << 8) | (uint16_t) mem.read(reg.PC + 1);
			cycles = 16;
		}
		else {
			reg.PC += 3;
		}
	};

	// 0xD3 Blank

	// CALL NC,u16
	nonPrefixed[0xD4] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		if (!reg.getCarry()) {
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
	};

	// PUSH DE
	nonPrefixed[0xD5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.SP--;
		mem.write(reg.SP, reg.D);
		reg.SP--;
		mem.write(reg.SP, reg.E);

		reg.PC += 1;
	};

	// SUB A,u8
	nonPrefixed[0xD6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.setHalfCarry(((reg.A & 0xF) - (mem.read(reg.PC + 1) & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) mem.read(reg.PC + 1);
		
		reg.setCarry(mem.read(reg.PC + 1) > reg.A);

		reg.A -= mem.read(reg.PC + 1);

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 2;
	};

	// RST 0x10
	nonPrefixed[0xD7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0010, cycles, reg, mem);
	};

	// RET C
	nonPrefixed[0xD8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		if (reg.getCarry()) {
			reg.PC = ((uint16_t) mem.read(reg.SP + 1) << 8) | (uint16_t) mem.read(reg.SP);
			reg.SP += 2;
			cycles = 20;
		}
		else {
			reg.PC += 1;
		}
	};

	// RETI
	nonPrefixed[0xD9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {

		reg.PC = ((uint16_t) mem.read(reg.SP + 1) << 8) | (uint16_t) mem.read(reg.SP);
		reg.SP += 2;

		mem.interruptsEnable = true;
	};

	// JP C,u16
	nonPrefixed[0xDA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		if (reg.getCarry()) {
			reg.PC = ((uint16_t) mem.read(reg.PC + 2) << 8) | (uint16_t) mem.read(reg.PC + 1);
			cycles = 16;
		}
		else {
			reg.PC += 3;
		}
	};

	// 0xDB Blank

	// CALL C,u16
	nonPrefixed[0xDC] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		if (reg.getCarry()) {
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
			reg.PC += 2;
		}
	};

	// 0xDD Blank

	// SBC A,u8
	nonPrefixed[0xDE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.setHalfCarry(((reg.A & 0xF) - (mem.read(reg.PC + 1) & 0xF) - (reg.getCarry() & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A - (uint16_t) mem.read(reg.PC + 1) - (uint16_t) reg.getCarry();

		reg.setCarry(mem.read(reg.PC + 1) > reg.A);

		reg.A -= mem.read(reg.PC + 1) - reg.getCarry();

		reg.setZero(reg.A == 0);
		reg.setNegative(true);

		reg.PC += 2;
	};

	// RST 0x18
	nonPrefixed[0xDF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0018, cycles, reg, mem);
	};

	// LD (0xFF00+u8),A
	nonPrefixed[0xE0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		mem.write(0xFF00 + (uint16_t) mem.read(reg.PC + 1), reg.A);

		reg.PC += 2;
	};

	// POP HL
	nonPrefixed[0xE1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.L = mem.read(reg.SP);
		reg.SP++;
		reg.H = mem.read(reg.SP);
		reg.SP++;

		reg.PC += 1;
	};

	// LD (0xFF00+C),A
	nonPrefixed[0xE2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(0xFF00 + (uint16_t) reg.C, reg.A);

		reg.PC += 1;
	};

	// 0xE3 Blank

	// 0xE4 Blank

	// PUSH HL
	nonPrefixed[0xE5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.SP--;
		mem.write(reg.SP, reg.H);
		reg.SP--;
		mem.write(reg.SP, reg.L);

		reg.PC += 1;
	};

	//AND A,u8
	nonPrefixed[0xE6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A &= mem.read(reg.PC + 1);

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(true);
		reg.setCarry(true);

		reg.PC += 2;
	};

	// RST 0x20
	nonPrefixed[0xE7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0020, cycles, reg, mem);
	};

	// ADD SP,i8 // TODO: Verify this
	nonPrefixed[0xE8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		int8_t i8 = mem.read(reg.PC + 1);
		reg.setHalfCarry(((reg.A & 0xF) + (((uint16_t) mem.read(reg.PC + 1)) & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) mem.read(reg.PC + 1);

		reg.A += (uint16_t) i8;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 2;
	};

	// JP HL
	nonPrefixed[0xE9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.PC = ((uint16_t) mem.read(reg.H) << 8) | (uint16_t) reg.L;
	};

	// LD (u16),A
	nonPrefixed[0xEA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		mem.write((uint16_t) mem.read(reg.PC + 2) << 8 | (uint16_t) mem.read(reg.PC + 1), reg.A);

		reg.PC += 3;
	};

	// 0xEB Blank

	// 0xEC Blank

	// 0xED Blank

	// XOR A,u8
	nonPrefixed[0xEE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A ^= mem.read(reg.PC + 1);

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 2;
	};

	// RST 0x28
	nonPrefixed[0xEF] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0028, cycles, reg, mem);
	};

	// LD A,(0xFF00+u8)
	nonPrefixed[0xF0] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.A = mem.read(0xFF00 + (uint16_t) mem.read(reg.PC + 1));

		reg.PC += 2;
	};

	// POP AF
	nonPrefixed[0xF1] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.F = mem.read(reg.SP) & 0xF0;
		reg.SP++;
		reg.A = mem.read(reg.SP);
		reg.SP++;

		reg.PC += 1;
	};

	// LD A,(0xFF00+C)
	nonPrefixed[0xF2] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A = mem.read(0xFF00 + (uint16_t) reg.C);

		reg.PC += 1;
	};

	// DI
	nonPrefixed[0xF3] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		mem.interruptsEnable = false;  // TODO: what does this do

		reg.PC += 1;
	};

	// 0xF4 Blank

	// PUSH AF
	nonPrefixed[0xF5] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.SP--;
		mem.write(reg.SP, reg.A);
		reg.SP--;
		mem.write(reg.SP, reg.F);

		reg.PC += 1;
	};

	//OR A,u8
	nonPrefixed[0xF6] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A |= mem.read(reg.PC + 1);

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setHalfCarry(false);
		reg.setCarry(false);

		reg.PC += 2;
	};

	// RST 0x30
	nonPrefixed[0xF7] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_RST(0x0030, cycles, reg, mem);
	};

	// LD HL,SP+i8 // TODO: Verify this
	nonPrefixed[0xF8] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
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
	};

	// LD SP,HL  // TODO: little endian?
	nonPrefixed[0xF9] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.SP = (uint16_t) reg.H << 8 | (uint16_t) reg.L;

		reg.PC += 1;
	};

	// LD A,(u16)
	nonPrefixed[0xFA] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 16;

		reg.A = mem.read((uint16_t) mem.read(reg.PC + 2) << 8 | (uint16_t) mem.read(reg.PC + 1));

		reg.PC += 3;
	};

	// 0xFB EI
	nonPrefixed[0xFB] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		// TODO: implement interrupts
		std::cerr << "unsupported instruction 0xFB, exiting...";
		exit(5);

		reg.PC += 1;
	};

	// 0xFC Blank

	// 0xFD Blank

	// CP A,u8
	nonPrefixed[0xFE] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		uint8_t result = reg.A - mem.read(reg.PC + 1);

		reg.setZero(result == 0);
		reg.setNegative(true);
		reg.setHalfCarry(((reg.A & 0xF) - (mem.read(reg.PC + 1) & 0xF)) & 0x10 == 0x10);
		reg.setCarry(mem.read(reg.PC + 1) > reg.A);

		reg.PC += 2;
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
	// ADC A,[HL]
	// ADC A,n8
	// ADD A,r8
	// ADD A,[HL]
	// ADD A,n8
	// AND A,r8
	// AND A,[HL]
	// AND A,n8
	// CP A,r8
	// CP A,[HL]
	// CP A,n8

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
	// OR A,r8
	// OR A,[HL]
	// OR A,n8
	// SBC A,r8
	// SBC A,[HL]
	// SBC A,n8
	// SUB A,r8
	// SUB A,[HL]
	// SUB A,n8
	// XOR A,r8
	// XOR A,[HL]
	// XOR A,n8

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

	// LD r8,r8
	
	// LD r8,n8
	void op_LD(uint8_t& dest, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		dest = mem.read(reg.PC + 1);

		reg.PC += 2;
	}
	
	// LD r16,n16
	void op_LD(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;
		a = mem.read(reg.PC + 2);
		b = mem.read(reg.PC + 1);
		reg.PC += 3;
	}
	
	// LD [HL],r8
	void op_LD(uint16_t address, uint8_t data, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		mem.write(address, data);
		reg.PC += 1;
	}
	
	// LD [HL],n8
	void op_LD(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(address, mem.read(reg.PC + 1));

		reg.PC += 1;
	}

	// LD r8,[HL]
	void op_LD(uint8_t& dest, uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		dest = mem.read(address);
		reg.PC += 1;
	}
	// LD [r16],A and LD [n16],A
	// LDH [n16],A
	// LDH [C],A
	// LD A,[r16]
	// LD A,[n16]
	// LDH A,[n16]
	// LDH A,[C]

	// LD [HLI],A
	void op_LDIW(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(getAddress(reg.H, reg.L), reg.A);
		reg.L++;
		if (reg.L == 0) {
			reg.H++;
		}

		reg.PC += 1;
	}
	
	// LD [HLD],A
	// LD A,[HLI]
	// LD A,[HLD]

	// ## Jumps and subroutines ##

	// CALL n16
	// CALL cc,n16
	// JP HL
	// JP n16
	// JP cc,n16

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
	// RET
	// RETI

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
	// ADD SP,e8
	// DEC SP
	// INC SP
	// LD SP,n16
	// LD [n16],SP

	// LD [n16],SP
	void op_LD_SP(unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 20;

		mem.write(getAddress(mem.read(reg.PC + 2), mem.read(reg.PC + 1)), (uint8_t) reg.SP & 0xFF);
		mem.write(getAddress(mem.read(reg.PC + 2), mem.read(reg.PC + 1)) + 1, (uint8_t) reg.SP >> 8);

		reg.PC += 3;
	}
	// LD HL,SP+e8
	// LD SP,HL
	// POP AF
	// POP r16
	// PUSH AF
	// PUSH r16

	// ## Miscellaneous instructons ##

	// CCF
	// CPL

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
	// EI
	// HALT

	// NOP
	void op_NOP(unsigned int& cycles, Registers& reg) {
		cycles = 4;  // 4 ticks
		reg.PC += 1;  // 1 byte
	}
	
	// SCF
	// STOP
	void op_STOP(unsigned int& cycles, Registers& reg) {
		cycles = 4;
		reg.PC += 2;
	}

}

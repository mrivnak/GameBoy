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
		cycles = 4;  // 4 ticks

		reg.PC += 1;  // 1 byte
	};

	// LD BC,u16
	nonPrefixed[0x01] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.B = mem.read(reg.PC + 1);
		reg.C = mem.read(reg.PC + 2);

		reg.PC += 3;
	};

	// LD (BC),A
	nonPrefixed[0x02] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(((uint16_t) reg.B << 8) | reg.C, reg.A);

		reg.PC += 1;
	};

	// INC BC
	nonPrefixed[0x03] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		
		reg.C++;
		if (reg.C == 0) {
			reg.B++;
		}

		reg.PC += 1;
	};

	// INC B
	nonPrefixed[0x04] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.B & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		reg.B++;

		reg.setZero(reg.B == 0);
		reg.setNegative(false);

		reg.PC += 1;
	};

	// DEC B
	nonPrefixed[0x05] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.B & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		reg.B--;

		reg.setZero(reg.B == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// LD B,u8
	nonPrefixed[0x06] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.B = mem.read(reg.PC + 1);

		reg.PC += 2;
	};

	// RLCA
	nonPrefixed[0x07] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		bool tempCarry = reg.A >> 7;
		reg.setCarry(tempCarry);
		reg.A = reg.A << 1 | tempCarry;
		reg.setZero(false);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 1;
	};

	// LD (u16),SP
	nonPrefixed[0x08] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 20;

		mem.write(((uint16_t) mem.read(reg.PC) << 8) | (uint16_t) mem.read(reg.PC + 1), (uint8_t) reg.SP & 0xFF);
		mem.write((((uint16_t) mem.read(reg.PC) << 8) | (uint16_t) mem.read(reg.PC + 1)) + 1, (uint8_t) reg.SP >> 8);

		reg.PC += 3;
	};

	// ADD HL,BC
	nonPrefixed[0x09] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		uint16_t HL = (uint16_t) reg.H << 8 | (uint16_t) reg.L;
		uint16_t BC = (uint16_t) reg.B << 8 | (uint16_t) reg.C;

		reg.setCarry((uint32_t) HL + (uint32_t) BC < 0xFFFF);
		reg.setHalfCarry(((HL & 0x0FFF) + ((BC) & 0x0FFF)) & 0x1000 == 0x1000);

		HL += BC;

		reg.setNegative(0);

		reg.H = (uint8_t) (HL >> 8) & 0xFF;
		reg.L = (uint8_t) HL & 0xFF;

		reg.PC += 1;
	};

	// LD A,(BC)
	nonPrefixed[0x0A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A = mem.read((uint16_t) reg.B << 8 | (uint16_t) reg.C);

		reg.PC += 1;
	};

	// DEC BC
	nonPrefixed[0x0B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.C--;
		if (reg.C == 0xFF) {
			reg.B--;
		}

		reg.PC += 1;
	};

	// INC C
	nonPrefixed[0x0C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.C & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		reg.C++;

		reg.setZero(reg.C == 0);
		reg.setNegative(false);

		reg.PC += 1;
	};

	// DEC C
	nonPrefixed[0x0D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.C & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		reg.C--;

		reg.setZero(reg.C == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// LD C,u8
	nonPrefixed[0x0E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.C = mem.read(reg.PC + 1);

		reg.PC += 2;
	};

	// RRCA
	nonPrefixed[0x0F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setCarry(reg.A & 0x1);
		reg.A = (reg.getCarry() << 7 | reg.A >> 1);

		reg.setZero(false);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 1;
	};

	// STOP
	nonPrefixed[0x10] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.PC += 2;
	};

	// LD DE,u16
	nonPrefixed[0x11] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.D = mem.read(reg.PC + 1);
		reg.E = mem.read(reg.PC + 2);

		reg.PC += 3;
	};

	// LD (DE),A
	nonPrefixed[0x12] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(((uint16_t) reg.D << 8) | reg.E, reg.A);

		reg.PC += 1;
	};

	// INC DE
	nonPrefixed[0x13] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		reg.E++;
		if (reg.E == 0) {
			reg.D++;
		}
		reg.PC += 1;
	};

	// INC D
	nonPrefixed[0x14] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.D & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		reg.D++;

		reg.setZero(reg.D == 0);
		reg.setNegative(false);

		reg.PC += 1;
	};

	// DEC D
	nonPrefixed[0x15] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.D & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		reg.D--;

		reg.setZero(reg.D == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// LD D,u8
	nonPrefixed[0x16] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.D = mem.read(reg.PC + 1);

		reg.PC += 2;
	};
	
	// RLA
	nonPrefixed[0x17] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		bool tempCarry = reg.getCarry();
		bool newCarry = reg.A >> 7;
		reg.A = reg.A << 1 | tempCarry;
		reg.setCarry(newCarry);
		reg.setZero(false);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 1;
	};

	// JR i8
	nonPrefixed[0x18] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.PC += (int8_t) mem.read(reg.PC);

		reg.PC += 2;
	};

	// ADD HL,DE
	nonPrefixed[0x19] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		uint16_t HL = (uint16_t) reg.H << 8 | (uint16_t) reg.L;
		uint16_t DE = (uint16_t) reg.D << 8 | (uint16_t) reg.E;

		reg.setCarry((uint32_t) HL + (uint32_t) DE < 0xFFFF);
		reg.setHalfCarry(((HL & 0x0FFF) + ((DE) & 0x0FFF)) & 0x1000 == 0x1000);

		HL += DE;

		reg.setNegative(0);

		reg.H = (uint8_t) (HL >> 8) & 0xFF;
		reg.L = (uint8_t) HL & 0xFF;

		reg.PC += 1;
	};

	// LD A,(DE)
	nonPrefixed[0x1A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A = mem.read((uint16_t) reg.D << 8 | (uint16_t) reg.E);

		reg.PC += 1;
	};

	// DEC DE
	nonPrefixed[0x1B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.E--;
		if (reg.E == 0xFF) {
			reg.D--;
		}

		reg.PC += 1;
	};

	// INC E
	nonPrefixed[0x1C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.E & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		reg.E++;

		reg.setZero(reg.E == 0);
		reg.setNegative(false);

		reg.PC += 1;
	};

	// DEC E
	nonPrefixed[0x1D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.E & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		reg.E--;

		reg.setZero(reg.E == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// LD E,u8
	nonPrefixed[0x1E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.E = mem.read(reg.PC + 1);

		reg.PC += 2;
	};

	// RRA
	nonPrefixed[0x1F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		bool tempCarry = reg.getCarry();
		bool newCarry = reg.A & 0x01;
		reg.setCarry(newCarry);
		reg.A = tempCarry << 7 | reg.A >> 1;
		reg.setZero(false);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 1;
	};

	// JR NZ,i8
	nonPrefixed[0x20] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		if (!reg.getZero()) {
			cycles = 12;
			reg.PC += (int8_t) mem.read(reg.PC);
		}

		reg.PC += 2;
	};

	// LD HL,u16
	nonPrefixed[0x21] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.H = mem.read(reg.PC + 1);
		reg.L = mem.read(reg.PC + 2);

		reg.PC += 3;
	};

	// LD (HL+),A
	nonPrefixed[0x22] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(((uint16_t) reg.H << 8) | reg.L, reg.A);
		reg.L++;
		if (reg.L == 0) {
			reg.H++;
		}

		reg.PC += 1;
	};

	// INC HL
	nonPrefixed[0x23] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		reg.L++;
		if (reg.L == 0) {
			reg.H++;
		}
		reg.PC += 1;
	};

	// INC H
	nonPrefixed[0x24] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.H & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		reg.H++;

		reg.setZero(reg.H == 0);
		reg.setNegative(false);

		reg.PC += 1;
	};

	// DEC H
	nonPrefixed[0x25] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.H & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		reg.H--;

		reg.setZero(reg.H == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};

	// LD H,u8
	nonPrefixed[0x26] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.H = mem.read(reg.PC + 1);

		reg.PC += 2;
	};
	
	// DAA
	nonPrefixed[0x27] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
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
	};

	// JR Z,i8
	nonPrefixed[0x28] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		if (reg.getZero()) {
			cycles = 12;
			reg.PC += (int8_t) mem.read(reg.PC);
		}

		reg.PC += 2;
	};

	// ADD HL,HL
	nonPrefixed[0x29] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		uint16_t HL = (uint16_t) reg.H << 8 | (uint16_t) reg.L;

		reg.setCarry((uint32_t) HL + (uint32_t) HL < 0xFFFF);
		reg.setHalfCarry(((HL & 0x0FFF) + ((HL) & 0x0FFF)) & 0x1000 == 0x1000);

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
			reg.PC += (int8_t) mem.read(reg.PC);
		}

		reg.PC += 2;
	};

	// LD SP,u16
	nonPrefixed[0x31] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.SP = (uint16_t) mem.read(reg.PC + 1) << 8 | (uint16_t) mem.read(reg.PC + 2);

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

		uint8_t result = mem.read((uint16_t) reg.H << 8 | (uint16_t) reg.L);

		reg.setHalfCarry(((result & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		result++;

		reg.setZero(reg.H == 0);
		reg.setNegative(false);

		mem.write((uint16_t) reg.H << 8 | (uint16_t) reg.L, result);

		reg.PC += 1;
	};

	// DEC (HL)
	nonPrefixed[0x35] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		uint8_t result = mem.read((uint16_t) reg.H << 8 | (uint16_t) reg.L);

		reg.setHalfCarry(((result & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		result++;

		reg.setZero(reg.H == 0);
		reg.setNegative(true);

		mem.write((uint16_t) reg.H << 8 | (uint16_t) reg.L, result);

		reg.PC += 1;
	};

	// LD (HL),u8
	nonPrefixed[0x36] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		mem.write((uint16_t) reg.H << 8 | (uint16_t) reg.L, mem.read(reg.PC + 1));

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
			reg.PC += (int8_t) mem.read(reg.PC);
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

		reg.A = mem.read((uint16_t) reg.H << 8 | (uint16_t) reg.H);
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

		reg.B = mem.read((uint16_t) reg.H << 8 | (uint16_t) reg.L);

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

		reg.C = mem.read((uint16_t) reg.H << 8 | (uint16_t) reg.L);

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

		reg.D = mem.read((uint16_t) reg.H << 8 | (uint16_t) reg.L);

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

		reg.E = mem.read((uint16_t) reg.H << 8 | (uint16_t) reg.L);

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

		reg.H = mem.read((uint16_t) reg.H << 8 | (uint16_t) reg.L);

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

		reg.L = mem.read((uint16_t) reg.H << 8 | (uint16_t) reg.L);

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

		mem.write(((uint16_t) reg.H << 8) | reg.L, reg.B);

		reg.PC += 1;
	};

	// LD (HL),C
	nonPrefixed[0x71] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(((uint16_t) reg.H << 8) | reg.L, reg.C);

		reg.PC += 1;
	};

	// LD (HL),D
	nonPrefixed[0x72] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(((uint16_t) reg.H << 8) | reg.L, reg.D);

		reg.PC += 1;
	};

	// LD (HL),E
	nonPrefixed[0x73] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(((uint16_t) reg.H << 8) | reg.L, reg.E);

		reg.PC += 1;
	};

	// LD (HL),H
	nonPrefixed[0x74] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(((uint16_t) reg.H << 8) | reg.L, reg.H);

		reg.PC += 1;
	};

	// LD (HL),L
	nonPrefixed[0x75] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(((uint16_t) reg.H << 8) | reg.L, reg.L);

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

		mem.write(((uint16_t) reg.H << 8) | reg.L, reg.A);

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

		reg.A = mem.read((uint16_t) reg.H << 8 | (uint16_t) reg.L);

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
	nonPrefixed[0x80] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.A & 0xF) + (reg.C & 0xF)) & 0x10 == 0x10);
		uint16_t result = (uint16_t) reg.A + (uint16_t) reg.C;

		reg.A += reg.C;

		reg.setZero(reg.A == 0);
		reg.setNegative(false);
		reg.setCarry(result > 0xFF);

		reg.PC += 1;
	};

	// TODO: implement the rest of the instructions
}


void Instructions::initPrefixed() {
	// TODO: implement prefixed instructions
}

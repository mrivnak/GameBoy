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

void Instructions::initPrefixed() {
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

		mem.write(((uint16_t) reg.D << 8) | reg.E, reg.A);
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

		if (!reg.getZero()) {
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

	// TODO: implement the rest of the instructions
}


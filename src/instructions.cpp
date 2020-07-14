#include "instructions.hpp"

#include "registers.hpp"
#include "memory-bus.hpp"

#include <cstdio>

namespace {
	using Instruction = Instructions::Instruction;

	void op_JR(unsigned int& cycles, bool cond, int8_t r8);
};

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
	nonPrefixed[0x00] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;  // 4 ticks

		reg.PC += 1;  // 1 byte
	};  // NOP

	nonPrefixed[0x01] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;

		reg.B = mem.read(reg.PC + 1);
		reg.C = mem.read(reg.PC + 2);

		reg.PC += 3;
	};  // LD BC,u16

	nonPrefixed[0x02] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		mem.write(((uint16_t) reg.B << 8) | reg.C, reg.A);

		reg.PC += 1;
	};  // LD (BC),a

	nonPrefixed[0x03] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		reg.C++;
		if (reg.C == 0) {
			reg.B++;
		}
		reg.PC += 1;
	};  // INC BC

	nonPrefixed[0x04] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.setHalfCarry(((reg.B & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		reg.B++;

		reg.setZero(reg.B == 0);
		reg.setNegative(false);

		reg.PC += 1;
	};  // INC B

	nonPrefixed[0x05] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.B & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		reg.B--;

		reg.setZero(reg.B == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};  // DEC B

	nonPrefixed[0x06] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.B = mem.read(reg.PC + 1);

		reg.PC += 2;
	};  // LD B,u8

	nonPrefixed[0x07] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		bool tempCarry = reg.A >> 7;
		reg.setCarry(tempCarry);
		reg.A = reg.A << 1 | tempCarry;
		reg.setZero(false);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 1;
	};  // RLCA

	nonPrefixed[0x08] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 20;

		mem.write(((uint16_t) mem.read(reg.PC) << 8) | (uint16_t) mem.read(reg.PC + 1), (uint8_t) reg.SP & 0xFF);
		mem.write((((uint16_t) mem.read(reg.PC) << 8) | (uint16_t) mem.read(reg.PC + 1)) + 1, (uint8_t) reg.SP >> 8);

		reg.PC += 3;
	};  // LD (u16),SP

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
	};  // ADD HL,BC

	nonPrefixed[0x0A] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.A = mem.read((uint16_t) reg.B << 8 | (uint16_t) reg.C);

		reg.PC += 1;
	};  // LD A,(BC)

	nonPrefixed[0x0B] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.C--;
		if (reg.C == 0xFF) {
			reg.B--;
		}

		reg.PC += 1;
	};  // DEC BC

	nonPrefixed[0x0C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.setHalfCarry(((reg.C & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		reg.C++;

		reg.setZero(reg.C == 0);
		reg.setNegative(false);

		reg.PC += 1;
	};  // INC C

	nonPrefixed[0x0D] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setHalfCarry(((reg.C & 0xF) - (1 & 0xF)) & 0x10 == 0x10);

		reg.C--;

		reg.setZero(reg.C == 0);
		reg.setNegative(true);

		reg.PC += 1;
	};  // DEC C

	nonPrefixed[0x0E] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.C = mem.read(reg.PC + 1);

		reg.PC += 2;
	};  // LD C,u8

	nonPrefixed[0x0F] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		reg.setCarry(reg.A & 0x1);
		reg.A = (reg.getCarry() << 7 | reg.A >> 1);

		reg.setZero(false);
		reg.setNegative(false);
		reg.setHalfCarry(false);

		reg.PC += 1;
	};  // RRCA

	nonPrefixed[0x10] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;

		

		reg.PC += 2;
	};  // STOP

	// TODO: implement the rest of the instructions
}

namespace {
	void op_JR(unsigned int& cycles, bool cond, int8_t r8, Registers& reg) {
		printf("JR %d\n", r8);

		if (cond) {
			cycles = 12;
			reg.PC += 2 + r8;
		}

		cycles = 8;
		reg.PC += 2;
	}
};

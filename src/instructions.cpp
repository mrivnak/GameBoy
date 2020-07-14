#include "instructions.hpp"

#include "registers.hpp"
#include "memory-bus.hpp"

#include <cstdio>

namespace {
	using Instruction = Instructions::Instruction;

	uint16_t op_JR(unsigned int& cycles, bool cond, int8_t r8);

	uint16_t op_INC8(unsigned int& cycles, uint8_t& r, Registers& rs);
	uint16_t op_INC16(unsigned int& cycles, uint8_t& rHigh, uint8_t& rLow);
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
		mem.write((((uint16_t) mem.read(reg.PC) << 8) | (uint16_t) mem.read(reg.PC + 1)) + 1, (uint8_t) reg.SP >> 0xFF);

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

	// TODO: implement the rest of the instructions

	/*
	nonPrefixed[0x13] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC16(pc, cycles, *reg.getD(), *reg.getE());
	}; // INC DE
	nonPrefixed[0x23] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC16(pc, cycles, *reg.getH(), *reg.getL());
	}; // INC HL
	nonPrefixed[0x33] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t hi = (reg.sp & 0xF0) >> 8;
		uint8_t lo = reg.sp & 0x0F;

		op_INC16(pc, cycles, hi, lo);
		reg.sp = (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo);

		pc;
	}; // INC SP

	// 8 bit increments
	nonPrefixed[0x04] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, reg.b, reg);
	}; // INC B
	nonPrefixed[0x14] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, reg.d, reg);
	}; // INC D
	nonPrefixed[0x24] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, reg.h, reg);
	}; // INC H
	nonPrefixed[0x3][0x4] = [](unsigned int& cycles, Registers& r, MemoryBus&) {
		op_INC8(pc, cycles, r.b);
	}; // INC (HL) // TODO: figure this out

	nonPrefixed[0x0C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.c, r);
	}; // INC C
	nonPrefixed[0x0C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.e, r);
	}; // INC E
	nonPrefixed[0x0C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.l, r);
	}; // INC L
	nonPrefixed[0x0C] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.a, r);
	}; // INC A
	
	// Jump Relative
	nonPrefixed[0x20] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(pc, cycles, !r.isZero(), static_cast<int8_t>(mem[pc + 1]));
	}; // JR NZ, r8

	nonPrefixed[0x30] = [](unsigned int& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(pc, cycles, !r.isCarry(), static_cast<int8_t>(mem[pc + 1]));
	}; // JR NC, r8
	*/
}

namespace {
	uint16_t op_JR(unsigned int& cycles, bool cond, int8_t r8, Registers& reg) {
		printf("JR %d\n", r8);

		if (cond) {
			cycles = 12;
			reg.PC += 2 + r8;
		}

		cycles = 8;
		reg.PC += 2;
	}
};

#include "instructions.hpp"

#include "registers.hpp"
#include "memory-bus.hpp"

#include <cstdio>

namespace {
	using Instruction = Instructions::Instruction;

	uint16_t op_JR(uint8_t& cycles, bool cond, int8_t r8);

	uint16_t op_INC8(uint8_t& cycles, uint8_t& r, Registers& rs);
	uint16_t op_INC16(uint8_t& cycles, uint8_t& rHigh, uint8_t& rLow);
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
	nonPrefixed[0x00] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;  // 4 ticks
		reg.PC += 1;  // 1 byte
	}; // NOP

	nonPrefixed[0x01] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;
		reg.B = mem.readByte(reg.PC + 1);
		reg.C = mem.readByte(reg.PC + 2);

		reg.PC += 3;
	}; // LD BC,u16

	nonPrefixed[0x02] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		//*reg.B = 
		// TODO: implement instructions with new memory map
		reg.PC += 1;
	}; // LD (BC),a

	nonPrefixed[0x03] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;
		reg.setBC(*reg.getBC() + 1);
		pc += 1;
	}; // INC BC

	nonPrefixed[0x04] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 8;

		reg.setHalfCarry(((*reg.getB() & 0xF) + (1 & 0xF)) & 0x10 == 0x10);

		reg.setB(*reg.getB() + 1);

		reg.setZero(*reg.getB() == 0);
		reg.setNegative(false);
		// TODO: verify that flags are set correctly

		pc += 1;
	}; // INC B

	/*
	nonPrefixed[0x13] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC16(pc, cycles, *reg.getD(), *reg.getE());
	}; // INC DE
	nonPrefixed[0x23] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC16(pc, cycles, *reg.getH(), *reg.getL());
	}; // INC HL
	nonPrefixed[0x33] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		uint8_t hi = (reg.sp & 0xF0) >> 8;
		uint8_t lo = reg.sp & 0x0F;

		op_INC16(pc, cycles, hi, lo);
		reg.sp = (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo);

		pc;
	}; // INC SP

	// 8 bit increments
	nonPrefixed[0x04] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, reg.b, reg);
	}; // INC B
	nonPrefixed[0x14] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, reg.d, reg);
	}; // INC D
	nonPrefixed[0x24] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, reg.h, reg);
	}; // INC H
	nonPrefixed[0x3][0x4] = [](uint8_t& cycles, Registers& r, MemoryBus&) {
		op_INC8(pc, cycles, r.b);
	}; // INC (HL) // TODO: figure this out

	nonPrefixed[0x0C] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.c, r);
	}; // INC C
	nonPrefixed[0x0C] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.e, r);
	}; // INC E
	nonPrefixed[0x0C] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.l, r);
	}; // INC L
	nonPrefixed[0x0C] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.a, r);
	}; // INC A
	
	// Jump Relative
	nonPrefixed[0x20] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(pc, cycles, !r.isZero(), static_cast<int8_t>(mem[pc + 1]));
	}; // JR NZ, r8

	nonPrefixed[0x30] = [](uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(pc, cycles, !r.isCarry(), static_cast<int8_t>(mem[pc + 1]));
	}; // JR NC, r8
	*/
}

namespace {
	uint16_t op_JR(uint8_t& cycles, bool cond, int8_t r8) {
		printf("JR %d\n", r8);

		if (cond) {
			cycles = 12;
			pc += 2 + r8;
		}

		cycles = 8;
		pc += 2;
	}
};

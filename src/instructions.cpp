#include "instructions.hpp"

#include "registers.hpp"
#include "memory-bus.hpp"

#include <cstdio>

namespace {
	using Instruction = Instructions::Instruction;

	uint16_t op_JR(uint16_t& pc, uint8_t& cycles, bool cond, int8_t r8);

	uint16_t op_INC8(uint16_t& pc, uint8_t& cycles, uint8_t& r, Registers& rs);
	uint16_t op_INC16(uint16_t& pc, uint8_t& cycles, uint8_t& rHigh, uint8_t& rLow);
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
	nonPrefixed[0x00] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;  // 4 ticks
		pc += 1;  // 1 byte
	}; // NOP
	nonPrefixed[0x01] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 12;
		reg.setBC = ((uint16_t) mem.getData(pc + 1) << 4) + mem.getData(pc + 2);
		pc += 3;
	}; // LD BC,u16

	nonPrefixed[0x10] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		cycles = 4;
	}; // STOP

	// 16 bit increments
	nonPrefixed[0x03] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC16(pc, cycles, r.b, r.c);
	}; // INC BC
	nonPrefixed[0x13] = [](uint16_t& pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		op_INC16(pc, cycles, r.d, r.e);
	}; // INC DE
	nonPrefixed[0x23] = [](uint16_t& pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		op_INC16(pc, cycles, r.h, r.l);
	}; // INC HL
	nonPrefixed[0x33] = [](uint16_t& pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		uint8_t hi = (r.sp & 0xF0) >> 8;
		uint8_t lo = r.sp & 0x0F;

		op_INC16(pc, cycles, hi, lo);
		r.sp = (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo);

		pc;
	}; // INC SP

	// 8 bit increments
	nonPrefixed[0x04] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.b, r);
	}; // INC B
	nonPrefixed[0x14] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.d, r);
	}; // INC D
	nonPrefixed[0x24] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.h, r);
	}; // INC H
	/*nonPrefixed[0x3][0x4] = [](uint16_t& pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		op_INC8(pc, cycles, r.b);
	}; // INC (HL) */ // TODO: figure this out

	nonPrefixed[0x0C] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.c, r);
	}; // INC C
	nonPrefixed[0x0C] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.e, r);
	}; // INC E
	nonPrefixed[0x0C] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.l, r);
	}; // INC L
	nonPrefixed[0x0C] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_INC8(pc, cycles, r.a, r);
	}; // INC A
	
	// Jump Relative
	nonPrefixed[0x20] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(pc, cycles, !r.isZero(), static_cast<int8_t>(mem[pc + 1]));
	}; // JR NZ, r8

	nonPrefixed[0x30] = [](uint16_t& pc, uint8_t& cycles, Registers& reg, MemoryBus& mem) {
		op_JR(pc, cycles, !r.isCarry(), static_cast<int8_t>(mem[pc + 1]));
	}; // JR NC, r8
}

namespace {
	uint16_t op_JR(uint16_t& pc, uint8_t& cycles, bool cond, int8_t r8) {
		printf("JR %d\n", r8);

		if (cond) {
			cycles = 12;
			pc += 2 + r8;
		}

		cycles = 8;
		pc += 2;
	}

	uint16_t op_INC8(uint16_t& pc, uint8_t& cycles, uint8_t& r, Registers& rs) {
		puts("INC8");

		rs.setHalfCarry(r == 0xFF);
		rs.setSubtract(false);
		rs.setZero(r == 0);

		++r;

		cycles = 4;
		pc++;
	}

	uint16_t op_INC16(uint16_t& pc, uint8_t& cycles, uint8_t& rHigh, uint8_t& rLow) {
		puts("INC16");

		uint16_t value = (static_cast<uint16_t>(rHigh) << 8) | rLow;
		
		++value;

		rHigh = static_cast<uint8_t>((value >> 8) & 0xFF);
		rLow = static_cast<uint8_t>(value & 0xFF);

		cycles = 8;
		pc++;
	}
};

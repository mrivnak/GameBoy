#include "instructions.hpp"

#include "registers.hpp"
#include "memory-bus.hpp"

#include <cstdio>

namespace {
	using Instruction = Instructions::Instruction;

	uint16_t op_JR(uint16_t pc, uint8_t& cycles, bool cond, int8_t r8);

	uint16_t op_INC8(uint16_t pc, uint8_t& cycles, uint8_t& r, Registers& rs);
	uint16_t op_INC16(uint16_t pc, uint8_t& cycles, uint8_t& rHigh, uint8_t& rLow);
};

Instructions::Instructions() {
	initPrefixed();
	initNonPrefixed();
}

Instruction Instructions::fetchInstruction(uint8_t ib, bool prefixByte) {
	const uint8_t high = (ib & 0xF0) >> 4;
	const uint8_t low = ib & 0x0F;

	if (prefixByte) {
		return prefixed[high][low];
	}

	return nonPrefixed[high][low];
}

Instructions::~Instructions() {
}

void Instructions::initPrefixed() {
}

void Instructions::initNonPrefixed() {
	nonPrefixed[0x0][0x0] = [](uint16_t pc, uint8_t& cycles, Registers&, MemoryBus&) {
		puts("NOP");
		cycles = 4;
		return pc + 1;
	}; // NOP

	nonPrefixed[0x1][0x0] = [](uint16_t pc, uint8_t& cycles, Registers&, MemoryBus&) {
		puts("STOP");
		cycles = 4;
		return pc + 1;
	}; // STOP

	// 16 bit increments
	nonPrefixed[0x0][0x3] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		return op_INC16(pc, cycles, r.b, r.c);
	}; // INC BC
	nonPrefixed[0x1][0x3] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		return op_INC16(pc, cycles, r.d, r.e);
	}; // INC DE
	nonPrefixed[0x2][0x3] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		return op_INC16(pc, cycles, r.h, r.l);
	}; // INC HL
	nonPrefixed[0x3][0x3] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		uint8_t hi = (r.sp & 0xF0) >> 8;
		uint8_t lo = r.sp & 0x0F;

		pc = op_INC16(pc, cycles, hi, lo);
		r.sp = (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo);

		return pc;
	}; // INC SP

	// 8 bit increments
	nonPrefixed[0x0][0x4] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		return op_INC8(pc, cycles, r.b, r);
	}; // INC B
	nonPrefixed[0x1][0x4] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		return op_INC8(pc, cycles, r.d, r);
	}; // INC D
	nonPrefixed[0x2][0x4] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		return op_INC8(pc, cycles, r.h, r);
	}; // INC H
	/*nonPrefixed[0x3][0x4] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		return op_INC8(pc, cycles, r.b);
	}; // INC (HL) */ // TODO: figure this out

	nonPrefixed[0x0][0xC] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		return op_INC8(pc, cycles, r.c, r);
	}; // INC C
	nonPrefixed[0x0][0xC] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		return op_INC8(pc, cycles, r.e, r);
	}; // INC E
	nonPrefixed[0x0][0xC] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		return op_INC8(pc, cycles, r.l, r);
	}; // INC L
	nonPrefixed[0x0][0xC] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus&) {
		return op_INC8(pc, cycles, r.a, r);
	}; // INC A
	
	// Jump Relative
	nonPrefixed[0x2][0x0] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus& mem) {
		return ::op_JR(pc, cycles, !r.isZero(), static_cast<int8_t>(mem[pc + 1]));
	}; // JR NZ, r8

	nonPrefixed[0x3][0x0] = [](uint16_t pc, uint8_t& cycles, Registers& r, MemoryBus& mem) {
		return ::op_JR(pc, cycles, !r.isCarry(), static_cast<int8_t>(mem[pc + 1]));
	}; // JR NC, r8
}

namespace {
	uint16_t op_JR(uint16_t pc, uint8_t& cycles, bool cond, int8_t r8) {
		printf("JR %d\n", r8);

		if (cond) {
			cycles = 12;
			return pc + 2 + r8;
		}

		cycles = 8;
		return pc + 2;
	}

	uint16_t op_INC8(uint16_t pc, uint8_t& cycles, uint8_t& r, Registers& rs) {
		puts("INC8");

		rs.setHalfCarry(r == 0xFF);
		rs.setSubtract(false);
		rs.setZero(r == 0);

		++r;

		cycles = 4;
		return pc + 1;
	}

	uint16_t op_INC16(uint16_t pc, uint8_t& cycles, uint8_t& rHigh, uint8_t& rLow) {
		puts("INC16");

		uint16_t value = (static_cast<uint16_t>(rHigh) << 8) | rLow;
		
		++value;

		rHigh = static_cast<uint8_t>((value >> 8) & 0xFF);
		rLow = static_cast<uint8_t>(value & 0xFF);

		cycles = 8;
		return pc + 1;
	}
};

#include "instructions.hpp"

#include "registers.hpp"
#include "memory-bus.hpp"

#include <cstdio>

namespace {
    using Instruction = Instructions::Instruction;

    uint16_t op_JR(uint16_t pc, bool cond, int8_t r8);
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
    nonPrefixed[0][0] = [](uint16_t pc, Registers&, MemoryBus&) { return pc + 1; }; // NOP
    nonPrefixed[0][1] = [](uint16_t pc, Registers&, MemoryBus&) {
        puts("STOP");
        return pc + 1;
    }; // STOP

    nonPrefixed[0][2] = [](uint16_t pc, Registers& r, MemoryBus& mem) {
        return ::op_JR(pc, !r.isZero(), static_cast<int8_t>(mem[pc + 1]));
    }; // JR NZ, r8
}

namespace {
    uint16_t op_JR(uint16_t pc, bool cond, int8_t r8) {
        return (cond ? (pc + r8) : (pc + 1)) + 1;
    }
};

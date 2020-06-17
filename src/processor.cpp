#include "processor.hpp"

#include "instructions.hpp"

#include <cstdio> // TODO: excise logging

Processor::Processor()
		: pc(0)
		, yieldCycles(0) {
}

void Processor::step() {
	if (yieldCycles == 0) {
		uint8_t ib = memory.readByte(pc);
		bool prefixByte = ib == Instructions::PREFIX_BYTE;

		if (prefixByte) {
			ib = memory.readByte(pc + 1);
		}

		if (auto instruction = Instructions::ref().fetchInstruction(ib, prefixByte)) {
			pc = instruction(pc, yieldCycles, registers, memory);
		}
		else {
			fprintf(stderr, "Invalid instruction: 0x%s%X\n",
					prefixByte ? "CB" : "", ib);
		}
	}

	--yieldCycles;
}

MemoryBus& Processor::getMemory() {
	return memory;
}

const MemoryBus& Processor::getMemory() const {
	return memory;
}

Registers& Processor::getRegisters() {
	return registers;
}

const Registers& Processor::getRegisters() const {
	return registers;
}

Processor::~Processor() {
}

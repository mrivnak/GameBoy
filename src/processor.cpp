#include "processor.hpp"

#include "instructions.hpp"

#include <cstdio> // TODO: excise logging

Processor::Processor()
		: pc(0) {
}

void Processor::step() {
	uint8_t ib = memory.readByte(pc);
	bool prefixByte = ib == Instructions::PREFIX_BYTE;

	if (prefixByte) {
		ib = memory.readByte(pc + 1);
	}

	if (auto instruction = Instructions::ref().fetchInstruction(ib, prefixByte)) {
		pc = instruction(pc, registers, memory);
	}
	else {
		fprintf(stderr, "Invalid instruction: 0x%s%X\n",
				prefixByte ? "CB" : "", ib);
	}
}

MemoryBus& Processor::getMemory() {
	return memory;
}

const MemoryBus& Processor::getMemory() const {
	return memory;
}

Processor::~Processor() {
}

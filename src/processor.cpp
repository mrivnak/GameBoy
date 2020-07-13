#include "processor.hpp"

#include "instructions.hpp"

#include <cstdio> // TODO: excise logging

Processor::Processor()
		: pc(0)
		, yieldCycles(0) {
}

void Processor::step() {
	if (yieldCycles == 0) {
		uint8_t instructionByte = memory.readByte(pc);
		bool prefixByte = instructionByte == Instructions::PREFIX_BYTE;

		if (prefixByte) {
			instructionByte = memory.readByte(pc + 1);
		}

		if (auto instruction = Instructions::ref().fetchInstruction(instructionByte, prefixByte)) {
			instruction(pc, yieldCycles, registers, memory);
		}
		else {
			fprintf(stderr, "Invalid instruction: 0x%s%X\n",
					prefixByte ? "CB" : "", instructionByte);
		}
	}

	yieldCycles--;
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

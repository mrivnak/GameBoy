#include "processor.hpp"

#include "instructions.hpp"

#include <cstdio> // TODO: excise logging

Processor::Processor() {
	cycles = 0;
}

void Processor::step() {
	if (cycles == 0) {
		uint8_t instructionByte = memory.readByte(pc);
		bool prefixByte = instructionByte == Instructions::PREFIX_BYTE;

		if (prefixByte) {
			instructionByte = memory.readByte(pc + 1);
		}

		if (auto instruction = Instructions::ref().fetchInstruction(instructionByte, prefixByte)) {
			instruction(cycles, registers, memory);
		}
		else {
			fprintf(stderr, "Invalid instruction: 0x%s%X\n",
					prefixByte ? "CB" : "", instructionByte);
		}
	}

	cycles--;
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

#include "processor.hpp"

#include "instructions.hpp"

#include <iostream>

Processor::Processor() {
	cycles = 0;
}

void Processor::step() {
	if (!registers.HALT) {
		if (cycles = 0) {
			uint8_t instructionByte = memory.read(registers.PC);
			bool prefixByte = instructionByte == Instructions::PREFIX_BYTE;

			if (prefixByte) {
				instructionByte = memory.read(registers.PC + 1);
			}

			if (auto instruction = Instructions::ref().fetchInstruction(instructionByte, prefixByte)) {
				instruction(cycles, registers, memory);
			}
			else {
				std::cerr << "Invalid instruction: 0x" << \
					(prefixByte ? "CB" : "") << std::hex << instructionByte << std::endl;
			}
		}
	} else {
		// TODO: exit halt
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

#include "processor.hpp"

#include "instructions.hpp"

#include <cstdio>

Processor::Processor() {
	cycles = 0;
	registers.PC = 0;
}

void Processor::step() {
	if (!registers.HALT) {
		if (cycles == 0) {
			uint8_t instructionByte = memory.read(registers.PC);
			bool prefixByte = instructionByte == Instructions::PREFIX_BYTE;

			if (prefixByte) {
				instructionByte = memory.read(registers.PC + 1);
			}

			if (auto instruction = Instructions::ref().fetchInstruction(instructionByte, prefixByte)) {
				printf("Instruction: 0x%s%X;\tPC: 0x%X\n", prefixByte ? "CB" : "", instructionByte, registers.PC);
				instruction(cycles, registers, memory);
			}
			else {
				fprintf(stderr, "Invalid Instruction: 0x%s%X\n", prefixByte ? "CB" : "", instructionByte);
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

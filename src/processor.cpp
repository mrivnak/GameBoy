#include "processor.hpp"

#include "instructions.hpp"

#include <cstdio>

Processor::Processor() {
	cycles = 0;
	registers.A = 0x0;
	registers.B = 0x0;
	registers.C = 0x0;
	registers.D = 0x0;
	registers.E = 0x0;
	registers.F = 0x0;
	registers.H = 0x0;
	registers.L = 0x0;
	registers.PC = 0x0;
	registers.SP = 0x0;
}

void Processor::step() {
	if (registers.PC == 0x100) {
		printf("Boot ROM done, exiting...\n");
		exit(0);
	}
	if (!registers.HALT) {
		if (cycles == 0) {
			uint8_t instructionByte = memory.read(registers.PC);
			bool prefixByte = instructionByte == Instructions::PREFIX_BYTE;

			if (prefixByte) {
				instructionByte = memory.read(registers.PC + 1);
			}

			if (auto instruction = Instructions::ref().fetchInstruction(instructionByte, prefixByte)) {
				// printf("Instruction: 0x%s%X;\tPC: 0x%X\n", prefixByte ? "CB" : "", instructionByte, registers.PC);
				printf("A:0x%02X B:0x%02X C:0x%02X D:0x%02X E:0x%02X F:0x%02X H:0x%02X L:0x%02X | PC:0x%04X | SP:0x%04X | OP:0x%s%X\u001b[91D",
					registers.A,
					registers.B,
					registers.C,
					registers.D,
					registers.E,
					registers.F,
					registers.H,
					registers.L,
					registers.PC,
					registers.SP,
					prefixByte ? "CB" : "--",
					instructionByte
				);
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

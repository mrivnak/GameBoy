#include "processor.hpp"

#include "instruction.hpp"

#include <cstdio> // TODO: excise logging

Processor::Processor() {
}

void Processor::step() {
	uint8_t ib = memory.readByte(pc);

	if (ib == Instruction::PREFIX_BYTE) {
		ib = memory.readByte(pc + 1);
	}

	if (auto* instruction = Instruction::fromByte(ib); instruction) {
		pc = execute(*instruction);
	}
	else {
		// TODO: error
	}
}

Processor::~Processor() {
}

uint16_t Processor::execute(const Instruction& instruction) {
	// TODO: execute instruction
}


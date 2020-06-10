#include "processor.hpp"

#include <cstdio> // TODO: excise logging
#include <array>

namespace {
	constexpr const uint8_t PREFIX_BYTE = 0xCB;

	using InstructionFactory = std::function<Processor::Instruction(uint8_t)>;

	const std::array<InstructionFactory, 16> prefixed {};
	const std::array<InstructionFactory, 16> nonPrefixed {
		[](uint8_t low) -> Processor::Instruction {
			printf("Got %d\n", low);

			switch (low) {
				case 0:
					return [](uint16_t pc, Registers&) {
						return pc + 1;
					};
				default:
					return Processor::Instruction();
			}
		}
	};
};

Processor::Processor()
		: pc(0) {
}

void Processor::step() {
	uint8_t ib = memory.readByte(pc);
	bool prefixByte = ib == ::PREFIX_BYTE;

	if (prefixByte) {
		ib = memory.readByte(pc + 1);
	}

	if (auto instruction = fetchInstruction(ib, prefixByte); instruction) {
		pc = instruction(pc, registers);
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

Processor::Instruction Processor::fetchInstruction(uint8_t ib, bool prefixByte) {
	const uint8_t high = (ib & 0xF0) >> 4;
	const uint8_t low = ib & 0x0F;

	auto& factory = prefixByte ? prefixed[high] : nonPrefixed[high];

	if (factory) {
		return factory(low);
	}

	// Empty std::function, resolvable to be not callable with operator bool
	return Processor::Instruction();
}

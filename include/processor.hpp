#pragma once

#include "registers.hpp"
#include "memory-bus.hpp"

class Instruction;

class Processor final {
	public:
		Processor();

		void step();

		MemoryBus& getMemory();
		const MemoryBus& getMemory() const;

		~Processor();
	private:
		Registers registers;

		uint16_t pc;

		MemoryBus memory;

		uint16_t execute(const Instruction&);

		// TODO: null copy and assign
};


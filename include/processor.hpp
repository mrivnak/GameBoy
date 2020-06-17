#pragma once

#include "registers.hpp"
#include "memory-bus.hpp"

class Processor final {
	public:
		Processor();

		void step();

		MemoryBus& getMemory();
		const MemoryBus& getMemory() const;

		Registers& getRegisters();
		const Registers& getRegisters() const;

		~Processor();
	private:
		Registers registers;

		uint16_t pc;
		uint8_t yieldCycles;

		MemoryBus memory;

		// TODO: null copy and assign
};


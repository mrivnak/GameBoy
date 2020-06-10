#pragma once

#include "registers.hpp"
#include "memory-bus.hpp"

#include <functional>

class Processor final {
	public:
		using Instruction = std::function<uint16_t(uint16_t, Registers&)>;

		Processor();

		void step();

		MemoryBus& getMemory();
		const MemoryBus& getMemory() const;

		~Processor();
	private:
		Registers registers;

		uint16_t pc;

		MemoryBus memory;

		static Instruction fetchInstruction(uint8_t ib, bool prefixByte);

		// TODO: null copy and assign
};


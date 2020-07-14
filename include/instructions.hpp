#pragma once

#include "singleton.hpp"

#include <cstdint>

#include <functional>

class Registers;
class MemoryBus;

// https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
// http://www.devrs.com/gb/files/opcodes.html
class Instructions final : public Singleton<Instructions> {
	public:
		static constexpr const uint8_t PREFIX_BYTE = 0xCB;

		using Instruction = std::function<void(unsigned int&, Registers&, MemoryBus&)>;

		Instructions();

		Instruction fetchInstruction(uint8_t instructionByte, bool prefixByte);

		~Instructions();
	private:
		Instruction prefixed[0xFF];
		Instruction nonPrefixed[0xFF];

		void initPrefixed();
		void initNonPrefixed();
};

#pragma once

#include <cstdint>

class Instruction {
	public:
		static constexpr const uint8_t PREFIX_BYTE = 0xCB;

		static Instruction* fromByte(uint8_t instructionByte);
	private:
};


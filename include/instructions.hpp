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

namespace {
	using Instruction = Instructions::Instruction;

	uint16_t getAddress(uint8_t a, uint8_t b);

	void op_RL(unsigned int& cycles, uint8_t& data, Registers& reg);
	void op_RL(unsigned int& cycles, uint16_t address, Registers& reg, MemoryBus& mem);

	void op_RR(unsigned int& cycles, uint8_t& data, Registers& reg);
	void op_RR(unsigned int& cycles, uint16_t address, Registers& reg, MemoryBus& mem);

	void op_SLA(unsigned int& cycles, uint8_t& data, Registers& reg);
	void op_SLA(unsigned int& cycles, uint16_t address, Registers& reg, MemoryBus& mem);

	void op_SRA(unsigned int& cycles, uint8_t& data, Registers& reg);
	void op_SRA(unsigned int& cycles, uint16_t address, Registers& reg, MemoryBus& mem);

	void op_SWAP(unsigned int& cycles, uint8_t& data, Registers& reg);
	void op_SWAP(unsigned int& cycles, uint16_t address, Registers& reg, MemoryBus& mem);

	void op_SRL(unsigned int& cycles, uint8_t& data, Registers& reg);
	void op_SRL(unsigned int& cycles, uint16_t address, Registers& reg, MemoryBus& mem);

	void op_BIT(unsigned int& cycles, int bit, uint8_t& data, Registers& reg);
	void op_BIT(unsigned int& cycles, int bit, uint16_t address, Registers& reg, MemoryBus& mem);

	void op_RES(unsigned int& cycles, int bit, uint8_t& data, Registers& reg);
	void op_RES(unsigned int& cycles, int bit, uint16_t address, Registers& reg, MemoryBus& mem);

	void op_SET(unsigned int& cycles, int bit, uint8_t& data, Registers& reg);
	void op_SET(unsigned int& cycles, int bit, uint16_t address, Registers& reg, MemoryBus& mem);
};
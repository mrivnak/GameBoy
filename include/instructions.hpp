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

	// ## 8-bit arithmetic and logic instructions ##

	// ADC A,r8
	// ADC A,[HL]
	// ADC A,n8
	// ADD A,r8
	// ADD A,[HL]
	// ADD A,n8
	// AND A,r8
	// AND A,[HL]
	// AND A,n8
	// CP A,r8
	// CP A,[HL]
	// CP A,n8
	void op_DEC(uint8_t& data, unsigned int& cycles, Registers& reg);  // DEC r8
	// DEC [HL]
	void op_INC(uint8_t& data, unsigned int& cycles, Registers& reg);  // INC r8
	// INC [HL]
	// OR A,r8
	// OR A,[HL]
	// OR A,n8
	// SBC A,r8
	// SBC A,[HL]
	// SBC A,n8
	// SUB A,r8
	// SUB A,[HL]
	// SUB A,n8
	// XOR A,r8
	// XOR A,[HL]
	// XOR A,n8

	// ## 16-bit arithmetic instructions ##

	void op_ADD(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg);  // ADD HL,r16
	void op_DEC(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg);  // DEC r16
	void op_INC(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg);  // INC r16

	// ## Bit operation instructions ##

	void op_BIT(int bit, uint8_t& data, unsigned int& cycles, Registers& reg);  // BIT u3,r8
	void op_BIT(int bit, uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // BIT u3,[HL]
	void op_RES(int bit, uint8_t& data, unsigned int& cycles, Registers& reg);  // RES u3,r8
	void op_RES(int bit, uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // RES u3,[HL]
	void op_SET(int bit, uint8_t& data, unsigned int& cycles, Registers& reg);  // SET u3,r8
	void op_SET(int bit, uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // SET u3,[HL]
	void op_SWAP(uint8_t& data, unsigned int& cycles, Registers& reg);  // SWAP r8
	void op_SWAP(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // SWAP [HL]

	// ## Bit shift instructions ##

	void op_RL(uint8_t& data, unsigned int& cycles, Registers& reg);  // RL r8
	void op_RL(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // RL [HL]
	void op_RLA(unsigned int& cycles, Registers& reg);  // RLA
	void op_RLC(uint8_t& data, unsigned int& cycles, Registers& reg);  // RLC r8
	void op_RLC(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // RLC [HL]
	void op_RLCA(unsigned int& cycles, Registers& reg); // RLCA
	void op_RR(uint8_t& data, unsigned int& cycles, Registers& reg);  // RR r8
	void op_RR(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // RR [HL]
	void op_RRA(unsigned int& cycles, Registers& reg);  // RRA
	void op_RRC(uint8_t& data, unsigned int& cycles, Registers& reg);  // RRC r8
	void op_RRC(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // RRC [HL]
	void op_RRCA(unsigned int& cycles, Registers& reg);  // RRCA
	void op_SLA(uint8_t& data, unsigned int& cycles, Registers& reg);  // SLA r8
	void op_SLA(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // SLA [HL]
	void op_SRA(uint8_t& data, unsigned int& cycles, Registers& reg);  // SRA r8
	void op_SRA(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // SRA [HL]
	void op_SRL(uint8_t& data, unsigned int& cycles, Registers& reg);  // SRL r8
	void op_SRL(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // SRL [HL]

	// ## Load instructions ##

	// LD r8,r8
	void op_LD(uint8_t& dest, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD r8,n8
	void op_LD(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD r16,n16
	void op_LD(uint16_t address, uint8_t data, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD [HL],r8
	void op_LD(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD [HL],n8
	void op_LD(uint8_t& dest, uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD r8,[HL]
	// LD [r16],A and LD [n16],A
	// LDH [n16],A
	// LDH [C],A
	// LD A,[r16]
	// LD A,[n16]
	// LDH A,[n16]
	// LDH A,[C]
	void op_LDIW(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD [HLI],A
	// LD [HLD],A
	// LD A,[HLI]
	// LD A,[HLD]

	// ## Jumps and subroutines ##

	// CALL n16
	// CALL cc,n16
	// JP HL
	// JP n16
	// JP cc,n16
	void op_JR(unsigned int& cycles, Registers& reg, MemoryBus& mem); // JR e8
	void op_JR(bool cond, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // JR cc,e8
	// RET cc
	// RET
	// RETI
	void op_RST(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // RST vec

	// ## Stack operation instructions ##

	// ADD HL,SP
	// ADD SP,e8
	// DEC SP
	// INC SP
	// LD SP,n16
	void op_LD_SP(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD [n16],SP
	// LD HL,SP+e8
	// LD SP,HL
	// POP AF
	// POP r16
	// PUSH AF
	// PUSH r16

	// ## Miscellaneous instructons ##

	// CCF
	// CPL
	void op_DAA(unsigned int& cycles, Registers& reg);  // DAA
	// DI
	// EI
	// HALT
	void op_NOP(unsigned int& cycles, Registers& reg);  // NOP
	// SCF
	void op_STOP(unsigned int& cycles, Registers& reg);  // STOP

};
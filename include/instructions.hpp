#pragma once

#include "singleton.hpp"

#include <cstdint>

#include <functional>

class Registers;
class MemoryBus;

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

	void op_ADC(uint8_t& data, unsigned int& cycles, Registers& reg);  // ADC A,r8
	void op_ADC(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // ADC A,[HL]
	void op_ADC(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // ADC A,n8
	void op_ADD(uint8_t& data, unsigned int& cycles, Registers& reg);  // ADD A,r8
	void op_ADD(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // ADD A,[HL]
	void op_ADD(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // ADD A,n8
	void op_AND(uint8_t& data, unsigned int& cycles, Registers& reg);  // AND A,r8
	void op_AND(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // AND A,[HL]
	void op_AND(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // AND A,n8
	void op_CP(uint8_t& data, unsigned int& cycles, Registers& reg);  // CP A,r8
	void op_CP(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem); // CP A,[HL]
	void op_CP(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // CP A,n8
	void op_DEC(uint8_t& data, unsigned int& cycles, Registers& reg);  // DEC r8
	void op_DEC(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // DEC [HL]
	void op_INC(uint8_t& data, unsigned int& cycles, Registers& reg);  // INC r8
	void op_INC(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // INC [HL]
	void op_OR(uint8_t& data, unsigned int& cycles, Registers& reg);  // OR A,r8
	void op_OR(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // OR A,[HL]
	void op_OR(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // OR A,n8
	void op_SBC(uint8_t& data, unsigned int& cycles, Registers& reg);  // SBC A,r8
	void op_SBC(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // SBC A,[HL]
	void op_SBC(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // SBC A,n8
	void op_SUB(uint8_t& data, unsigned int& cycles, Registers& reg);  // SUB A,r8
	void op_SUB(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // SUB A,[HL]
	void op_SUB(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // SUB A,n8
	void op_XOR(uint8_t& data, unsigned int& cycles, Registers& reg);  // XOR A,r8
	void op_XOR(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // XOR A,[HL]
	void op_XOR(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // XOR A,n8

	// ## 16-bit arithmetic instructions ##

	void op_ADD16(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg);  // ADD HL,r16
	void op_DEC16(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg);  // DEC r16
	void op_INC16(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg);  // INC r16

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

	void op_LD_RR(uint8_t& dest, uint8_t& src, unsigned int& cycles, Registers& reg);  // LD r8,r8
	void op_LD_RN(uint8_t& dest, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD r8,n8
	void op_LD16_RN(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD r16,n16
	void op_LD_PR(uint16_t address, uint8_t data, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD [HL],r8
	void op_LD_PN(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD [HL],n8
	void op_LD_RP(uint8_t& dest, uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD r8,[HL]
	void op_LD16_NA(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD [n16],A
	void op_LD16_AN(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD A,[n16]
	void op_LDH_PA(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LDH [n16],A
	void op_LDH_AP(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LDH A,[n16]
	void op_LDH_CA(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LDH [C],A
	void op_LDH_AC(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LDH A,[C]
	void op_LD_IW(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD [HLI],A
	void op_LD_DW(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD [HLD],A
	void op_LD_IR(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD A,[HLI]
	void op_LD_DR(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD A,[HLD]

	// ## Jumps and subroutines ##

	void op_CALL(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // CALL n16
	void op_CALL(bool cond, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // CALL cc,n16
	void op_JP_HL(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // JP HL
	void op_JP(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // JP n16
	void op_JP(bool cond, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // JP cc,n16
	void op_JR(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // JR e8
	void op_JR(bool cond, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // JR cc,e8
	void op_RET(bool cond, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // RET cc
	void op_RET(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // RET
	void op_RETI(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // RETI
	void op_RST(uint16_t address, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // RST vec

	// ## Stack operation instructions ##

	void op_ADD_HL_SP(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // ADD HL,SP
	void op_ADD_SP(unsigned int cycles, Registers& reg, MemoryBus& mem);  // ADD SP,e8
	void op_DEC_SP(unsigned int& cycles, Registers& reg);  // DEC SP
	void op_INC_SP(unsigned int& cycles, Registers& reg);  // INC SP
	void op_LD_SPR(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD SP,n16
	void op_LD_SPW(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD [n16],SP
	void op_LD_HL_SP(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // LD HL,SP+e8
	void op_LD_SP_HL(unsigned int& cycles, Registers& reg);  // LD SP,HL
	void op_POP_AF(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // POP AF
	void op_POP(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // POP r16
	void op_PUSH_AF(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // PUSH AF
	void op_PUSH(uint8_t& a, uint8_t& b, unsigned int& cycles, Registers& reg, MemoryBus& mem);  // PUSH r16

	// ## Miscellaneous instructons ##

	void op_CCF(unsigned int& cycles, Registers& reg);  // CCF
	void op_CPL(unsigned int& cycles, Registers& reg);  // CPL
	void op_DAA(unsigned int& cycles, Registers& reg);  // DAA
	void op_DI(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // DI
	void op_EI(unsigned int& cycles, Registers& reg, MemoryBus& mem);  // EI
	void op_HALT(unsigned int& cycles, Registers& reg);  // HALT
	void op_NOP(unsigned int& cycles, Registers& reg);  // NOP
	void op_SCF(unsigned int& cycles, Registers& reg);  // SCF
	void op_STOP(unsigned int& cycles, Registers& reg);  // STOP

};
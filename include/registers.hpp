#pragma once

#include <cstdint>

struct Flags {
	enum {
		FLAG_ZERO		= 0b10000000,
		FLAG_NEGATIVE	= 0b01000000,
		FLAG_HALF_CARRY	= 0b00100000,
		FLAG_CARRY		= 0b00010000
	};
};

// TODO: check if this needs a constructor to zero the registers
struct Registers {
	uint8_t
		A,
		B,
		C,
		D,
		E,
		F,  // Flags register
		H,
		L
	;

	uint16_t PC;
	uint16_t SP;

};
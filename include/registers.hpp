#pragma once

#include <cstdint>

struct Flags {
	enum {
		FLAG_ZERO		= 128,
		FLAG_SUBTRACT	= 64,
		FLAG_HALF_CARRY	= 32,
		FLAG_CARRY		= 16
	};
};

// TODO: check if this needs a constructor to zero the registers
struct Registers {
	void setAF(uint16_t value);
	void setBC(uint16_t value);
	void setDE(uint16_t value);
	void setHL(uint16_t value);

	void setZero(bool zero);
	void setSubtract(bool subtract);
	void setHalfCarry(bool halfCarry);
	void setCarry(bool carry);

	void setFlag(uint8_t flag, bool set);

	uint16_t getAF() const;
	uint16_t getBC() const;
	uint16_t getDE() const;
	uint16_t getHL() const;

	bool isZero() const;
	bool isSubtract() const;
	bool isHalfCarry() const;
	bool isCarry() const;

	// TODO: see if these should be zeroed 
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t f;
	uint8_t h;
	uint8_t l;

	uint8_t flags = 0;

	uint16_t sp = 0; // TODO: see if its right to zero it
};


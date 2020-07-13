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

	void setAF8(uint8_t value);
	void setBC8(uint8_t value);
	void setDE8(uint8_t value);
	void setHL8(uint8_t value);

	void setA(uint8_t value);
	void setF(uint8_t value);
	void setB(uint8_t value);
	void setC(uint8_t value);
	void setD(uint8_t value);
	void setE(uint8_t value);
	void setH(uint8_t value);
	void setL(uint8_t value);

	void setZero(bool zero);
	void setSubtract(bool subtract);
	void setHalfCarry(bool halfCarry);
	void setCarry(bool carry);

	void setFlag(uint8_t flag, bool set);

	uint16_t * getAF();
	uint16_t * getBC();
	uint16_t * getDE();
	uint16_t * getHL();

	uint8_t * getA();
	uint8_t * getF();
	uint8_t * getB();
	uint8_t * getC();
	uint8_t * getD();
	uint8_t * getE();
	uint8_t * getH();
	uint8_t * getL();

	bool isZero() const;
	bool isSubtract() const;
	bool isHalfCarry() const;
	bool isCarry() const;

	uint8_t regs[8]; // a, f, b, c, d, e, h, l

	uint8_t flags = 0;

	uint16_t sp = 0; // TODO: see if its right to zero it
};


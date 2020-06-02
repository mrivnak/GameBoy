#pragma once

#include <cstdint>

struct Registers {
	void setAF(uint16_t value);
	void setBC(uint16_t value);
	void setDE(uint16_t value);
	void setHL(uint16_t value);

	uint16_t getAF() const;
	uint16_t getBC() const;
	uint16_t getDE() const;
	uint16_t getHL() const;

	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t f;
	uint8_t h;
	uint8_t l;
};


#include "registers.hpp"

void Registers::setAF(uint16_t value) {
	a = static_cast<uint8_t>((value & 0xFF00) >> 8);
	f = static_cast<uint8_t>(value & 0xFF);
}

void Registers::setBC(uint16_t value) {
	b = static_cast<uint8_t>((value & 0xFF00) >> 8);
	c = static_cast<uint8_t>(value & 0xFF);
}

void Registers::setDE(uint16_t value) {
	d = static_cast<uint8_t>((value & 0xFF00) >> 8);
	e = static_cast<uint8_t>(value & 0xFF);
}

void Registers::setHL(uint16_t value) {
	h = static_cast<uint8_t>((value & 0xFF00) >> 8);
	l = static_cast<uint8_t>(value & 0xFF);
}


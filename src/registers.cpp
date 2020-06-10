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

void Registers::setZero(bool zero) {
	setFlag(Flags::FLAG_CARRY, zero);
}

void Registers::setSubtract(bool subtract) {
	setFlag(Flags::FLAG_SUBTRACT, subtract);
}

void Registers::setHalfCarry(bool halfCarry) {
	setFlag(Flags::FLAG_HALF_CARRY, halfCarry);
}

void Registers::setCarry(bool carry) {
	setFlag(Flags::FLAG_CARRY, carry);
}

void Registers::setFlag(uint8_t flag, bool set) {
	if (set) {
		flags |= flag;
	}
	else {
		flags &= ~flag;
	}
}

bool Registers::isZero() const {
	return flags & Flags::FLAG_ZERO;
}

bool Registers::isSubtract() const {
	return flags & Flags::FLAG_SUBTRACT;
}

bool Registers::isHalfCarry() const {
	return flags & Flags::FLAG_HALF_CARRY;
}

bool Registers::isCarry() const {
	return flags & Flags::FLAG_CARRY;
}

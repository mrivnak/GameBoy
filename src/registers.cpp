#include "registers.hpp"

void Registers::setZero(bool zero) {
	setFlag(Flags::FLAG_CARRY, zero);
}

void Registers::setNegative(bool negative) {
	setFlag(Flags::FLAG_NEGATIVE, negative);
}

void Registers::setHalfCarry(bool halfCarry) {
	setFlag(Flags::FLAG_HALF_CARRY, halfCarry);
}

void Registers::setCarry(bool carry) {
	setFlag(Flags::FLAG_CARRY, carry);
}

void Registers::setFlag(uint8_t flag, bool set) {
	if (set) {
		F |= flag;
	}
	else {
		F &= ~flag;
	}
}

bool Registers::getZero() const {
	return F & Flags::FLAG_ZERO;
}

bool Registers::getNegative() const {
	return F & Flags::FLAG_NEGATIVE;
}

bool Registers::getHalfCarry() const {
	return F & Flags::FLAG_HALF_CARRY;
}

bool Registers::getCarry() const {
	return F & Flags::FLAG_CARRY;
}

#include "registers.hpp"

void Registers::setAF(uint16_t value) {
	regs[0] = (uint8_t) (value >> 8);
	regs[1] = (uint8_t) value;
}

void Registers::setBC(uint16_t value) {
	regs[2] = (uint8_t) (value >> 8);
	regs[3] = (uint8_t) value;
}

void Registers::setDE(uint16_t value) {
	regs[4] = (uint8_t) (value >> 8);
	regs[5] = (uint8_t) value;
}

void Registers::setHL(uint16_t value) {
	regs[6] = (uint8_t) (value >> 8);
	regs[7] = (uint8_t) value;
}

void Registers::setAF8(uint8_t value) {
	regs[0] = value;
}

void Registers::setBC8(uint8_t value) {
	regs[2] = value;
}

void Registers::setDE8(uint8_t value) {
	regs[4] = value;
}

void Registers::setHL8(uint8_t value) {
	regs[6] = value;
}

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
		flags |= flag;
	}
	else {
		flags &= ~flag;
	}
}

bool Registers::isZero() const {
	return flags & Flags::FLAG_ZERO;
}

bool Registers::isNegative() const {
	return flags & Flags::FLAG_NEGATIVE;
}

bool Registers::isHalfCarry() const {
	return flags & Flags::FLAG_HALF_CARRY;
}

bool Registers::isCarry() const {
	return flags & Flags::FLAG_CARRY;
}

uint16_t * Registers::getAF() {
	return (uint16_t*) &regs;
}

uint16_t * Registers::getBC() {
	return (uint16_t*) (&regs + 2);
}

uint16_t * Registers::getDE() {
	return (uint16_t*) (&regs + 2);
}

uint16_t * Registers::getHL() {
	return (uint16_t*) (&regs + 2);
}

uint8_t * Registers::getA() {
	return regs[0];
}

uint8_t * Registers::getF() {
	return regs[1];
}

uint8_t * Registers::getB() {
	return regs[2];
}

uint8_t * Registers::getC() {
	return regs[3];
}

uint8_t * Registers::getD() {
	return regs[4];
}

uint8_t * Registers::getE() {
	return regs[5];
}

uint8_t * Registers::getH() {
	return regs[6];
}

uint8_t * Registers::getL() {
	return regs[7];
}

#pragma once

#include <cstdint>
#include <iostream>

#include "cartridge.hpp"
#include "display.hpp"

struct MemoryBus {

	uint8_t read(const uint16_t address) const;
	void write(const uint16_t address, const uint8_t byte);

	uint8_t readIO(const uint16_t address) const;
	void writeIO(const uint16_t address, const uint8_t byte);

	uint8_t operator[](const uint16_t address) const;

	void loadCartridge(Cartridge * cartridge);
	void loadDisplay(Display * display);

	std::array<uint8_t, 0x2000> WRAM;
	std::array<uint8_t, 0x80> HRAM;
	bool interruptsEnable;

	Cartridge * cartridge;
	Display * display;

	bool ERAMEnable;
	uint8_t bankingMode;
};

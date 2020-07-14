#include "memory-bus.hpp"

#include <cstdlib>

uint8_t MemoryBus::read(const uint16_t address) const {
	switch (address) {
		case 0x0000 ... 0x3FFF:
			return cartridge->ROM[address];
		case 0x4000 ... 0x7FFF:
			return cartridge->rombank.bank[cartridge->rombank.bankPtr][address - 0x4000];
		case 0x8000 ... 0x9FFF:
			return display->VRAMEnable ? display->VRAM[address - 0x8000] : 0;
		case 0xA000 ... 0xBFFF:
			switch (cartridge->header.type) {
				case 0:
					return cartridge->rambank.bank[cartridge->rambank.bankPtr][address - 0xA000];
				case 1 ... 3:
					return ERAMEnable ? cartridge->rambank.bank[cartridge->rambank.bankPtr][address - 0xA000] : 0;
				default:
					return 0;
			}
		case 0xC000 ... 0xDFFF:
			return WRAM[address - 0xC000];
		case 0xE000 ... 0xFDFF:
			return WRAM[address - 0xE000];  // Echo RAM, a copy of the bank of RAM below it. Works both ways
		// TODO: continute implementing memory map
	}
}

void MemoryBus::write(const uint16_t address, const uint8_t byte) {

}

void MemoryBus::loadCartridge(Cartridge * cartridge) {
	this->cartridge = cartridge;
}

void MemoryBus::loadDisplay(Display * display) {
	this->display = display;
}

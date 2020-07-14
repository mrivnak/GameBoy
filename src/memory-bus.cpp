#include "memory-bus.hpp"

#include <cstdlib>

uint8_t MemoryBus::read(const uint16_t address) const {
	switch (address) {
		case 0x0000 ... 0x3FFF:  // 16K ROM bank 0, from cartridge, usually a fixed bank
			return cartridge->ROM[address];
		case 0x4000 ... 0x7FFF:  // 16K ROM bank 1-N, from cartridge, switchable bank via MB (if any)
			return cartridge->rombank.bank[cartridge->rombank.bankPtr][address - 0x4000];
		case 0x8000 ... 0x9FFF:  // 8K Video RAM, only bank 0 switchable in non-CGB mode, 0/1 in CGB mode
			return display->VRAMEnable ? display->VRAM[address - 0x8000] : 0;
		case 0xA000 ... 0xBFFF:  // 8K External RAM, in cartridge, switchable bank if any
			switch (cartridge->header.type) {
				case 0:
					return cartridge->rambank.bank[cartridge->rambank.bankPtr][address - 0xA000];
				case 1 ... 3:
					return ERAMEnable ? cartridge->rambank.bank[cartridge->rambank.bankPtr][address - 0xA000] : 0;
				default:
					return 0;
			}
		case 0xC000 ... 0xDFFF:  // 4K Work RAM bank 0-1, banks 2-7 switchable in CGB mode
			return WRAM[address - 0xC000];
		case 0xE000 ... 0xFDFF:  // Echo RAM, a copy of the bank of RAM below it. Works both ways
			return WRAM[address - 0xE000];
		case 0xFE00 ... 0xFE9F:  // Sprite Attribute Table (OAM)
			return display->OAM[address - 0xFE00];
		case 0xFF00 ... 0xFF7F:  // IO Registers
			return readIO(address);
		case 0xFF80 ... 0xFFFE:  // High RAM
			return HRAM[address - 0xFF80];
		case 0xFFFF:  // Interrupts Enable Register
			return interruptsEnable;
		default:
			// 0xFEA0 ... 0xFEFF Not usable
			std::cerr << "ERROR: Attempt to read non-readable memory address: " << address << std::endl;
			exit(1);
			// TODO: handle this better
	}
}

void MemoryBus::write(const uint16_t address, const uint8_t byte) {
	switch (address) {
		case 0x0000 ... 0x1FFF:  // 16K ROM bank 0, from cartridge, usually a fixed bank
			switch (cartridge->header.type) {
				case 0x01 ... 0x03:
					ERAMEnable = (byte & 0xF == 0xA);
					break;
			}
			break;
		case 0x2000 ... 0x3FFF:  // 16K ROM bank 0, from cartridge, usually a fixed bank
			switch (cartridge->header.type) {
				case 0x01:
					if (cartridge->header.ROMSize > 0x04) {
						if (byte == 0x00) {
							cartridge->rombank.bankPtr = 0x01;
						}
						else {
							cartridge->rombank.bankPtr = (cartridge->rambank.bankPtr << 5 | (byte & 0x1F));
						}
					}
					else {
						if (byte == 0x00) {
							cartridge->rombank.bankPtr = 0x01;
						}
						else {
							cartridge->rombank.bankPtr = byte & 0x1F;
						}
					}
			}
		case 0x4000 ... 0x5FFF:  // 16K ROM bank 1-N, from cartridge, switchable bank via MB (if any)
			switch (cartridge->header.type) {
				case 0x01 ... 0x03:
					if (cartridge->header.RAMSize == 0x03 && ERAMEnable) {
						cartridge->rambank.bankPtr = byte & 0x03;
					}
			}
		case 0x6000 ... 0x7FFF:  // 16K ROM bank 1-N, from cartridge, switchable bank via MB (if any)
			if (cartridge->header.type == 0x01) {
				bankingMode = byte & 0x01;
			}
		case 0x8000 ... 0x9FFF:  // 8K Video RAM, only bank 0 switchable in non-CGB mode, 0/1 in CGB mode
			display->VRAM[address - 0x8000] = byte;
		case 0xA000 ... 0xBFFF:  // 8K External RAM, in cartridge, switchable bank if any
			switch (cartridge->header.type) {
				case 0x00:
					cartridge->rambank.bank[cartridge->rambank.bankPtr][address - 0xA000] = byte;
				case 0x01 ... 0x03:
					if (ERAMEnable) {
						cartridge->rambank.bank[cartridge->rambank.bankPtr][address - 0xA000] = byte;
					}
			}
		case 0xC000 ... 0xDFFF:  // 4K Work RAM bank 0-1, banks 2-7 switchable in CGB mode
			WRAM[address - 0xC000] = byte;
		case 0xE000 ... 0xFDFF:  // Echo RAM, a copy of the bank of RAM below it. Works both ways
			WRAM[address - 0xE000] = byte;
		case 0xFE00 ... 0xFE9F:  // Sprite Attribute Table (OAM)
			display->OAM[address - 0xFE00] = byte;
		case 0xFF00 ... 0xFF7F:  // IO Registers
			writeIO(address, byte);
		case 0xFF80 ... 0xFFFE:  // High RAM
			HRAM[address - 0xFF80] = byte;
		case 0xFFFF:  // Interrupts Enable Register
			interruptsEnable = byte;
		default:
			// 0xFEA0 ... 0xFEFF Not usable
			std::cerr << "ERROR: Attempt to write non-writeable memory address: " << address << std::endl;
			exit(1);
			// TODO: handle this better
	}
}

void MemoryBus::loadCartridge(Cartridge * cart) {
	this->cartridge = cart;
}

void MemoryBus::loadDisplay(Display * display) {
	this->display = display;
}

uint8_t MemoryBus::readIO(const uint16_t address) const {
	// TODO: implement read I/O
	return 0;
}

void MemoryBus::writeIO(const uint16_t address, const uint8_t byte) {
	// TODO: implement write I/O
}
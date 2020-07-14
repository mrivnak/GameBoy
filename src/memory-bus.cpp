#include "memory-bus.hpp"

#include <cstdlib>

MemoryBus::MemoryBus()
	: memory(static_cast<uint8_t*>(std::malloc(MEMORY_SIZE))) {}

uint8_t MemoryBus::read(const uint16_t address) const {
	switch (address) {
		case 0x0000 ... 0x3FFF:
			return cartridge.ROM[address];
			break;
		// TODO: continute implementing memory map
	}
}

void MemoryBus::write(const uint16_t address, const uint8_t byte) {

}

uint8_t MemoryBus::operator[](uint16_t address) const {
	return memory[address];
}

uint8_t* MemoryBus::getData() {
	return memory;
}

const uint8_t* MemoryBus::getData() const {
	return memory;
}

MemoryBus::~MemoryBus() {
	std::free(memory);
}


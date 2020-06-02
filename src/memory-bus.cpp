#include "memory-bus.hpp"

#include <cstdlib>

MemoryBus::MemoryBus()
	: memory(static_cast<uint8_t*>(std::malloc(MEMORY_SIZE))) {}

uint8_t MemoryBus::readByte(uint16_t address) const {
	return memory[address];
}

MemoryBus::~MemoryBus() {
	std::free(memory);
}


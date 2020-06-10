#include "memory-bus.hpp"

#include <cstdlib>

MemoryBus::MemoryBus()
	: memory(static_cast<uint8_t*>(std::malloc(MEMORY_SIZE))) {}

uint8_t MemoryBus::readByte(uint16_t address) const {
	return memory[address];
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


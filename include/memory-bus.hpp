#pragma once

#include <cstdint>

class MemoryBus final {
	public:
		static constexpr const uintptr_t MEMORY_SIZE = 0xFFFF;

		MemoryBus();

		uint8_t readByte(uint16_t address) const;

		uint8_t operator[](uint16_t address) const;

		uint8_t* getData();
		const uint8_t* getData() const;

		~MemoryBus();
	private:
		uint8_t* memory;

		// TODO: null copy and assign
};


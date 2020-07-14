#pragma once

#include <cstdint>

#include "cartridge.hpp"
#include "display.hpp"

class MemoryBus final {
	public:
		static constexpr const uintptr_t MEMORY_SIZE = 0x10000;

		MemoryBus();

		uint8_t read(const uint16_t address) const;
		void write(const uint16_t address, const uint8_t byte);

		uint8_t operator[](uint16_t address) const;

		void loadCartridge(Cartridge * cartridge);
		void loadDisplay(Display * display);

		~MemoryBus();
	private:
		std::array<uint8_t, 0x2000> WRAM;
		std::array<uint8_t, 0x80> HRAM;

		Cartridge * cartridge;
		Display * display;

		bool ERAMEnable;

		// TODO: null copy and assign
};

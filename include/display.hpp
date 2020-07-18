#pragma once

#include <array>
#include <cstdint>

class Display {
	public:
		Display();
		~Display();

		void clock();

		std::array<std::array<uint8_t, 160>, 144> frameBuffer;
		std::array<uint8_t, 0x2000> VRAM;
		std::array<uint8_t, 0xA0> OAM;

		uint8_t
			LCDC,
			LCDCSTAT,
			SCX,
			SCY,
			LY,
			LYC,
			LX,
			WY,
			WX,
			BGP,
			OBP0,
			OBP1,
			DMA
		;

		unsigned int cycles;
		bool OAMAccess;

		std::array<int, 10> visibleSprites;

		bool VRAMEnable;
	private:
		// TODO: null copy and assign
};

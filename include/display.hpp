#pragma once

#include <array>
#include <cstdint>

class Display {
	public:
		Display();
		~Display();

		void clock();
		void spriteSelect();
		void drawScanline();

		void drawSpriteLine(uint8_t attr, uint8_t left, uint8_t right);
		void drawBackgroundLine(uint8_t left, uint8_t right);

		// LCDC stuff
		uint16_t getBackgroundStartAddress();
		uint16_t getBackgroundMapAddress();
		uint16_t getWindowMapAddress();
		void setLYCInterrupt();
		void setOAMInterrupt();
		void setVBlankInterrupt();
		void setHBlankInterrupt();

		void dmaTransfer();
		uint8_t spritePalette(uint8_t attr);
		bool windowEnable();

		std::array<std::array<uint8_t, 160>, 144> frameBuffer;
		std::array<uint8_t, 0x2000> VRAM;
		std::array<uint8_t, 0xA0> OAM;

		std::array<std::array<uint8_t, 160>, 144> getFrameBuffer();

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
};

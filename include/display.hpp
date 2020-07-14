#pragma once

#include <cstdint>

class MemoryBus;

// TODO: move out of display.hpp
/*
Sprite block:
Byte 0: Y position
Byte 1: X position
Byte 2: Flags:
	Bit 7: Priority
		if this bit is set to 0, sprite is displayed on top of background & window
		if this bit is set to 1, then sprite will be hidden behind colors 1, 2, and 3 of the background & window
		(sprite only prevails over color 0 of BG & win)
	Bit 6: Y flip
		Sprite pattern is flipped vertically if this bit is set to 1
	Bit 5: X flip
		Sprite pattern is flipped horizontally if this bit is set to 1
	Bit 4: Palette number
		Sprite colors are taken from OBJ1PAL if this bit isset to 1
		and from OBJ0PAL otherwise
*/
struct SpriteBlock {
	enum SpriteFlags {
		FLAG_PRIORITY = 128,
		FLAG_Y_FLIP = 64,
		FLAG_X_FLIP = 32,
		FLAG_PALETTE = 16
	};

	uint8_t y;
	uint8_t x;
	uint8_t flags;
};

/*
Registers of interest (located within program memory)
- FF47 (BGP): BG & Window palette data
	- Bit 7-6: data for Dot Data 11
	- Bit 5-4: data for Dot Data 10
	- Bit 3-2: data for Dot Data 01
	- Bit 1-0: Data for Dot Data 00
- FF48 (OBP0): Object Palette 0 Data (R/W)
	This selects the colors for sprite palette 0.
	It works exactly as BGP ($FF47) except each value of 0 is
	transparent.
- FF49 (OBP1): Object Palette 1 Data (R/W)
	This Selects the colors for sprite palette 1. It works exactly as
	OBP0. See BGP for details.
- FF4A (WY): Window Y Position (R/W)
	0 <= WY <= 143
	WY must be within above parameters for window to be visible
- FF4B (WX): Window X Position (R/W)
	0 <= WX <= 166
	WX must be within above parameters for window to be visible
	WX is offset from absolute screen coordinates by 7. Setting the window to
	WX=7, WY=0 will put the upper left corner of the window at absolute screen coordinates
	0, 0.

	OBJ Characters (SpriteS) can still enter the window. None of the window
	colors are transparent so any background tiles under the window are hidden.
*/

// size: 160x144
// TODO: figure out where registers SCROLLX and SCROLLY are
class Display final {
	public:
		Display(MemoryBus& memory);

		~Display();
	private:
		MemoryBus* memory;
		// TODO: null copy and assign
};

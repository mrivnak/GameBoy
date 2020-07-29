# GB Docs

## Components

### Cartridge [catridge.hpp]

Structure that contains the different parts of the "cartridge" so that the binary can be loaded and accessed easily.

- Header
- ROM Bank
- RAM Bank

### Display [display.hpp]

(not functional at the moment)

Class that defines the Pixel Processing Unit (PPU) of the Game Boy. Sprites are selected from the Object Attribute Memory (OAM) and drawn on top of the background. The Frame is drawn past the screen borders and the window X and Y are used to select a 160x144 frame from that.

### Instructions

Container class for the CPU instruction set, a set of hundreds of functions used by the CPU to actually do things. These "things" can be arithmetic, comparison, load, or control flow operations that are executed from the ROM.

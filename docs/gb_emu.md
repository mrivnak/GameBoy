# GB Docs

## Components [/src/ and /include/]

### Cartridge [catridge.hpp]

Structure that contains the different parts of the "cartridge" so that the binary can be loaded and accessed easily.

- Header
- ROM Bank
- RAM Bank

### Display [display.hpp]

(not functional at the moment)

Class that defines the Pixel Processing Unit (PPU) of the Game Boy. Sprites are selected from the Object Attribute Memory (OAM) and drawn on top of the background. The Frame is drawn past the screen borders and the window X and Y are used to select a 160x144 frame from that.

### Instructions [instructions.hpp]

(further testing required)

Container class for the CPU instruction set, a set of hundreds of functions used by the CPU to actually do things. These "things" can be arithmetic, comparison, load, or control flow operations that are executed from the ROM.

### Memory Bus [memory-bus.hpp]

Structure used to handle all reading and writing to the memory, as well and the display and I/O. the read() and write() functions distributes data to the correct memory structures. Since the emulator is not a simple memory map like actual hardware would be, we use many separate containers and the memory bus is capable of accessing these containers depending on the memory address being accessed. This also controls the memory banking that is implemented by certain cartridge designs.

### Motherboard [motherboard.hpp]

Utility class to interface between the various other conponents in the system, such as the memory bus, cartridge, processor, etc. This handles loading the boot and game ROMs into the system memory, as well as passing the memory bus between other components. It also provides some abstraction towards the main loop where calling Motherboard::clock() will call all the necessary functions that are associated with that without revealing everything to the main function.

### Processor [processor.hpp]

The processor class is fairly simple as it's only real function is to read instructions and execute them. Most of the real CPU work has been delegated to the Instructions class listed earlier. Apart from that it is tasked with counting cycles so that the CPU timing is correct. These cycles are defined and reset with each instruction.

### Registers [registers.hpp]

The Registers structure is a fairly simple piece that takes care of accessing registers. For simplicity, at the moment the registers are all public since it's a struct. This allows the instruction set to be a bit more streamlined. In addition to this it also has some functions to handle the flags register which has 4 flags in an 8-bit register.

## Tests [/gb-test-roms/]

gb-test-roms submodule included in the top level directory, supplied set of roms used to verify the instruction set, memory management, etc.

# GameBoy

A Nintendo Game Boy emulator written in modern C++
by Michael Rivnak and Leonard Litvak

## Final Version

As the semester has come to a close, this project will likely not see any further development. The current version has a CPU that has been partially tested, further testing requires the presence of a PPU which has not been fully implemented and does not work in the current form. Up to that section of the boot ROM, though, the CPU does execute instructions correctly although there may be some issues with registers being written correctly. The most obvious indicator that the game has been loaded into memory correctly is the game title that is displayed in the window title. This is read from the memory after the cartridge is loaded as the title is present on the rom. Other than the CPU and a partial PPU, there is also a partial audio implementation but this is very much a luxury for this assignment and also completely untested.

Overall this assignment was too ambitious for a 3 month project for a group who was previously completely new to emulation in general. Despite this, the project has a respectable amount of functionality and is able to demonstrate our understanding of the system. The code is also structured in a way that it is relatively simple to understand based on the component abstraction.

## Documentation

see [gb_emu.md](https://github.com/mrivnak/GameBoy/blob/master/docs/gb_emu.md)

## References

[GameBoy CPU Manual - GBCPUman.pdf](http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf)

[Introduction - DMG-01: How to Emulate a Game Boy](https://blog.ryanlevick.com/DMG-01/public/book/introduction.html)

[\[GB\] APU / Sound Emulation : EmuDev](https://www.reddit.com/r/EmuDev/comments/5gkwi5/gb_apu_sound_emulation/)

[Pan Docs - GbdevWiki](https://gbdev.gg8.se/wiki/articles/Pan_Docs)

[gbops - The Game Boy opcode table](https://izik1.github.io/gbops/index.html)

[gbdev/awesome-gbdev: A curated list of Game Boy development resources such as tools, docs, emulators, related projects and open-source ROMs.](https://github.com/gbdev/awesome-gbdev#opcodes)

[Rednex GB](https://rednex.github.io/rgbds)

[A Visual Guide to the Gameboy's Half-Carry Flag · Robdor](https://robdor.com/2016/08/10/gameboy-emulator-half-carry-flag/)
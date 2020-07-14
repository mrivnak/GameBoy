#pragma once

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "cartridge.hpp"
#include "display.hpp"
#include "memory-bus.hpp"
#include "processor.hpp"
#include "registers.hpp"

const int BOOT_ROM_SIZE = 0xFF;

class Motherboard {
	public:
		Motherboard();
		~Motherboard();

		void clock();

		void loadBootROM();
		void loadCartridge(std::string filename);
		void loadMemory();

		std::string getTitle();
	private:
		Processor processor;
		MemoryBus * memoryBus;
		Cartridge cartridge;
		Display display;

		std::vector<uint8_t> readFile(std::string filename);
};
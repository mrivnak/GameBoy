#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

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

		void loadFile(std::string filename);
		void loadData(void * data, int size);
	private:
		Processor processor;
};
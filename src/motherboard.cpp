#include "motherboard.hpp"

Motherboard::Motherboard() {

}

Motherboard::~Motherboard() {

}

void Motherboard::clock() {
	processor.step();
}

// TODO: IMPORTANT Add proper handling for cartridge and boot roms with new structures
void Motherboard::loadFile(std::string filename) {
	std::FILE* file = std::fopen(filename.c_str(), "r");

	std::fseek(file, 0, SEEK_END);
	size_t size = std::ftell(file);
	std::fseek(file, 0, SEEK_SET); 

	if (!file) {
		std::cerr << "Failed to open ROM file!" << std::endl;
		exit(1);
	}

	if (std::fread(processor.getMemory().getData(), 1, size, file) != size) {
		std::cerr << "Failed to read ROM file!" << std::endl;
		std::fclose(file);
		exit(1);
	}
	else {
		std::fclose(file);
	}
}

void Motherboard::loadData(void * data, int size) {
	std::memcpy(processor.getMemory().getData(), data, size);
}

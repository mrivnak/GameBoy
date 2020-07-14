#include "motherboard.hpp"

Motherboard::Motherboard() {
	loadMemory();
}

Motherboard::~Motherboard() {

}

void Motherboard::clock() {
	processor.step();
}

// TODO: IMPORTANT Add proper handling for cartridge and boot roms with new structures
void Motherboard::loadBootROM() {
	std::vector<uint8_t> * data = new std::vector<uint8_t>;

	*data = readFile("../res/DMG_ROM.bin");

	for (int i = 0; i < 0xFF; i++) {
		cartridge.ROM[i] = (*data)[i];
		cartridge.rombank.bank[0][i] = (*data)[i];
	}

	

	delete data;
}

void Motherboard::loadCartridge(std::string filename) {
	std::vector<uint8_t> * data = new std::vector<uint8_t>;

	*data = readFile(filename);

	// Cartridge metadata
	for (int i = 0; i < 16; i++) {
		cartridge.header.title[i] = (*data)[0x0134+i];  // Read game title bytes
	}
	cartridge.header.type = (*data)[0x0147];
	cartridge.header.ROMSize = (*data)[0x0148];
	cartridge.header.RAMSize = (*data)[0x0149];
	cartridge.header.CGB = (*data)[0x0143] == 0x80;  // Check for Game Boy Color cartridge

	for (int i = 0; i < 0x3FFF; i++) {
		cartridge.ROM[i] = (*data)[i];
		cartridge.rombank.bank[0][i] = (*data)[i];
	}
	for (int i = 1; i < 256; i++) {
		for (int j = 0; j < 0x3FFF; j++) {
			if ((i * 0x4000) + j < data->size()) {
				cartridge.rombank.bank[i][j] = (*data)[(i * 0x4000) + j];
			}
		}
	}

	delete data;
}

void Motherboard::loadMemory() {
	memoryBus = &processor.getMemory();
	memoryBus->loadCartridge(&cartridge);  // Pass cartridge to memory bus
	memoryBus->loadDisplay(&display);
}

std::vector<uint8_t> Motherboard::readFile(std::string filename)
{
    // open the file:
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<uint8_t> vec;

    // read the data:
	char b;
    if (file) {
		for (int i = 0; i < fileSize; i++) {
			file.read(&b, 1);
			vec.push_back((uint8_t) b);
		}
	}

    return vec;
}

std::string Motherboard::getTitle() {
	std::string str(std::begin(cartridge.header.title), std::end(cartridge.header.title));
	return str;
}

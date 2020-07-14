#include "motherboard.hpp"

Motherboard::Motherboard() {

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

	for (int i = 0; i < data->size(); i++) {
		cartridge.ROM[i] = (*data)[i];
		cartridge.rombank.bank[0][i] = (*data)[i];
	}

	delete data;
}

void Motherboard::loadCartridge(std::string filename) {
	
}

std::vector<uint8_t> readFile(std::string filename)
{
    // open the file:
    std::ifstream file(filename.c_str(), std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<uint8_t> vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<uint8_t>(file),
               std::istream_iterator<uint8_t>());

    return vec;
}

#include "bitmap.hpp"

#include <cstdlib>
#include <cstring>

Bitmap::Bitmap(int width, int height, int numChannels,
			const uint8_t* dataIn)
		: width(width)
		, height(height)
		, numChannels(numChannels)
		, data(nullptr) {
	data = static_cast<uint8_t*>(std::malloc(calcDataSize()));

	if (dataIn) {
		std::memcpy(data, dataIn, calcDataSize());
	}
}

int Bitmap::getWidth() const {
	return width;
}

int Bitmap::getHeight() const {
	return height;
}

int Bitmap::getNumChannels() const {
	return numChannels;
}

uint8_t* Bitmap::getData() {
	return data;
}

const uint8_t* Bitmap::getData() const {
	return data;
}

Bitmap::~Bitmap() {
	std::free(data);
}

uintptr_t Bitmap::calcDataSize() const {
	return width * height * numChannels * sizeof(uint8_t);
}

#pragma once

#include <cstdint>

class Bitmap final {
	public:
		Bitmap(int width = 1, int height = 1,
				int numChannels = 3, const uint8_t* data = nullptr);

		int getWidth() const;
		int getHeight() const;

		int getNumChannels() const;

		uint8_t* getData();
		const uint8_t* getData() const;

		~Bitmap();
	private:
		int width;
		int height;
		int numChannels;

		uint8_t* data;

		uintptr_t calcDataSize() const;
};

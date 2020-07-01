#pragma once

#include <cstdint>

class Bitmap;

class Texture final {
	public:
		Texture(uint32_t width, uint32_t height, uint32_t internalPixelFormat,
				const void* data, uint32_t pixelFormat, uint32_t dataType);

		Texture(const Bitmap& bitmap);

		uint32_t getID();

		uint32_t getWidth() const;
		uint32_t getHeight() const;

		~Texture();
	private:
		uint32_t textureID;

		uint32_t width;
		uint32_t height;
};

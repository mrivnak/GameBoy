#include "texture.hpp"

#include "bitmap.hpp"

#include <GL/gl.h>

Texture::Texture(uint32_t width, uint32_t height, uint32_t internalFormat,
			const void* data, uint32_t pixelFormat, uint32_t dataType)
		: textureID(0)
		, width(width)
		, height(height) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height,
			0, pixelFormat, dataType, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
}

Texture::Texture(const Bitmap& bitmap)
		: Texture(bitmap.getWidth(), bitmap.getHeight(), GL_RGB,
				bitmap.getData(), GL_RGB, GL_UNSIGNED_BYTE) {}

uint32_t Texture::getID() {
	return textureID;
}

uint32_t Texture::getWidth() const {
	return width;
}

uint32_t Texture::getHeight() const {
	return height;
}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);
}

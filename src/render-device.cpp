#include "render-device.hpp"

#include "window.hpp"

#include "texture.hpp"

#include <GL/gl.h>

RenderDevice::RenderDevice(Window& window) {
	context = SDL_GL_CreateContext(window.getHandle());

	glEnable(GL_TEXTURE_2D);
}

void RenderDevice::clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderDevice::drawTexturedQuad(Texture& texture) {
	glBindTexture(GL_TEXTURE_2D, texture.getID());

	glBegin(GL_QUADS);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);

	glEnd();
}

RenderDevice::~RenderDevice() {
	SDL_GL_DeleteContext(context);
}

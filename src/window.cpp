#include "window.hpp"

#include "render-device.hpp"

Window::Window(const std::string& title, int width, int height)
		: window(nullptr)
		, renderDevice(nullptr)
		, width(width)
		, height(height) {
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height, SDL_WINDOW_OPENGL);

	renderDevice = new RenderDevice(*this);

	SDL_GL_SetSwapInterval(1);
}

void Window::swapBuffers() {
	SDL_GL_SwapWindow(window);
}

SDL_Window* Window::getHandle() {
	return window;
}

RenderDevice& Window::getRenderDevice() {
	return *renderDevice;
}

int Window::getWidth() const {
	return width;
}

int Window::getHeight() const {
	return height;
}

Window::~Window() {
	delete renderDevice;
	SDL_DestroyWindow(window);
}

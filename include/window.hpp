#pragma once

#include <string_view>
#include <SDL2/SDL.h>

class Window final {
	public:
		Window(const std::string_view& title, int width, int height);

		void pollEvents();
		void swapBuffers();

		bool isCloseRequested() const;

		~Window();
	private:
		SDL_Window* window;
		SDL_GLContext context;

		int width;
		int height;

		bool shouldClose;

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
};


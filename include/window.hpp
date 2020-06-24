#pragma once

#include <string_view>

struct SDL_Window;
class RenderDevice;

class Window final {
	public:
		void swapBuffers();

		SDL_Window* getHandle();
		RenderDevice& getRenderDevice();

		int getWidth() const;
		int getHeight() const;

		~Window();
	protected:
		Window(const std::string_view& title, int width, int height);

		friend class Application;
	private:
		SDL_Window* window;
		RenderDevice* renderDevice;

		int width;
		int height;

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
};


#include "application.hpp"

#include "window.hpp"

#include <SDL2/SDL.h>

Application::Application()
		: running(true) {
	SDL_Init(SDL_INIT_EVERYTHING);
}

Window& Application::createWindow(const std::string& title,
		int width, int height) {
	auto* win = new Window(title, width, height);
	windows.push_back(win);

	return *win;
}

void Application::pollEvents() {
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				running = false;
				break;
		}
	}
}

bool Application::isRunning() const {
	return running;
}

Application::~Application() {
	for (auto* win  : windows) {
		delete win;
	}

	SDL_Quit();
}
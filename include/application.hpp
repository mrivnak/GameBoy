#pragma once

#include <string>
#include <vector>

class Window;

class Application final {
	public:
		Application();

		Window& createWindow(const std::string& title,
				int width, int height);

		void pollEvents();

		bool isRunning() const;

		~Application();
	private:
		std::vector<Window*> windows;

		bool running;

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
};

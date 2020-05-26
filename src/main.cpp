#include "window.hpp"

#ifdef main
#undef main
#endif
int main(int argc, char** argv) {
	Window window("My Window", 800, 600);

	while (!window.isCloseRequested()) {
		window.pollEvents();
		window.swapBuffers();
	}

	return 0;
}


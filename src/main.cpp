#include "window.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <GL/gl.h>

#ifdef main
#undef main
#endif
int main(int argc, char** argv) {
	Window window("My Window", 800, 600);

	std::FILE* file = std::fopen("../res/DMG_ROM.bin", "r");

	if (!file) {
		std::puts("Failed to open boot ROM file");
		return 1;
	}

	uint8_t* mem = static_cast<uint8_t*>(std::malloc(0xFFFF));

	if (std::fread(mem, 1, 0xFF, file) != 0xFF) {
		std::puts("Failed to write boot ROM!");
		std::fclose(file);
		std::free(mem);
		return 1;
	}

	std::fclose(file);
	std::free(mem);

	while (!window.isCloseRequested()) {
		window.pollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f(-1.f, -1.f);
		glVertex2f(0.f, 1.f);
		glVertex2f(1.f, -1.f);
		glEnd();

		window.swapBuffers();
	}

	return 0;
}


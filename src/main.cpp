#include "window.hpp"

#include <GL/gl.h>

#ifdef main
#undef main
#endif
int main(int argc, char** argv) {
	Window window("My Window", 800, 600);

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


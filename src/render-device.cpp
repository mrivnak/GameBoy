#include "render-device.hpp"

#include "window.hpp"

#include <GL/gl.h>

RenderDevice::RenderDevice(Window& window) {
    context = SDL_GL_CreateContext(window.getHandle());
}

void RenderDevice::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

RenderDevice::~RenderDevice() {
    SDL_GL_DeleteContext(context);
}

#pragma once

#include <SDL2/SDL.h>

class Window;

// TODO: probably expand this to also have RenderContext which we draw from
class RenderDevice final {
    public:
        RenderDevice(Window& window);

        void clear();

        // TODO: render quad

        ~RenderDevice();
    private:
        SDL_GLContext context;

        RenderDevice(const RenderDevice&) = delete;
        RenderDevice(RenderDevice&&) = delete;
        RenderDevice& operator=(const RenderDevice&) = delete;
};

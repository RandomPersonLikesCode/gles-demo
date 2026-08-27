// SPDX-License-Identifier: MIT

#pragma once

#include <SDL3/SDL.h>

namespace Core {
    struct Gui {
        float scaling = 1.0f;

        void create(SDL_Window *window, SDL_GLContext ctx);
        ~Gui(void);
    };
}

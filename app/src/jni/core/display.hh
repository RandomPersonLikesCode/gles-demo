// SPDX-License-Identifier: MIT

#pragma once

#include <SDL3/SDL.h>

namespace Core {
  struct Display {
    SDL_Window *window;
    SDL_GLContext renderer_ctx;

    const char *title;
    int width, height;

    Display(const char *title, int width, int height);
    ~Display();

    bool init();
  };
}

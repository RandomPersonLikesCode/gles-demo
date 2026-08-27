// SPDX-License-Identifier: MIT

#pragma once

#include <SDL3/SDL.h>

namespace Core {
  struct Display {
    SDL_Window* window = nullptr;
    SDL_GLContext renderer_ctx = nullptr;

    const char* title = "OpenGL ES Demo";
    int width = 800;
    int height = 600;

    bool create(void);
    ~Display(void);
  };
}  // namespace Core

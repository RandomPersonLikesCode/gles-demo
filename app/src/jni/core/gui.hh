// SPDX-License-Identifier: MIT

#pragma once

#include <SDL3/SDL.h>

namespace Core {
  struct Gui {
    float finger_x = 0.0f;
    float finger_y = 0.0f;

    void create(SDL_Window *window, SDL_GLContext ctx);
    ~Gui(void);
  };
}  // namespace Core

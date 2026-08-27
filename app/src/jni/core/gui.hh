// SPDX-License-Identifier: MIT

#pragma once

#include <SDL3/SDL.h>

namespace Core {
  struct Gui {
    void create(SDL_Window* window, SDL_GLContext ctx);
    ~Gui(void);
  };
}  // namespace Core

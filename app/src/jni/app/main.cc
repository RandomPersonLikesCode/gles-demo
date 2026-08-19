// SPDX-License-Identifier: MIT

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <GLES3/gl32.h>

#include "../core/display.hh"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  SDL_Event events = {};
  bool is_running = true;

  Core::Display display = {
    nullptr,
    nullptr,
    "SDL Demo",
    800,
    600
  }; if(!display.create()) {
    return 1;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  while (is_running) {
    while (SDL_PollEvent(&events)) {
      switch (events.type) {
        case SDL_EVENT_QUIT:
          is_running = false;
          break;
      }
    }
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(display.window);
  }

  return 0;
}

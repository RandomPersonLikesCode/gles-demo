// SPDX-License-Identifier: MIT

#include <GLES3/gl32.h>

#include <SDL3/SDL.h>

#include "../utils/logging.hh"
#include "./display.hh"

bool Core::Display::create(void) {
  SDL_SetHint(SDL_HINT_ANDROID_TRAP_BACK_BUTTON, "1");
  SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    LOG_ERR("SDL error: %s", SDL_GetError());
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_ES);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  this->window =
      SDL_CreateWindow(this->title, this->width, this->height,
                       SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);

  if (!this->window) {
    LOG_ERR("SDL error: %s", SDL_GetError());
    return false;
  }

  SDL_GetWindowSizeInPixels(this->window, &this->width, &this->height);

  this->renderer_ctx = SDL_GL_CreateContext(this->window);

  if (!this->renderer_ctx) {
    LOG_ERR("SDL error: %s", SDL_GetError());
    return false;
  }

  SDL_GL_SetSwapInterval(1);
  SDL_GL_MakeCurrent(this->window, this->renderer_ctx);

  glViewport(0, 0, this->width, this->height);
  glEnable(GL_DEPTH_TEST);

  return true;
}

Core::Display::~Display(void) {
  SDL_GL_MakeCurrent(nullptr, nullptr);

  SDL_GL_DestroyContext(this->renderer_ctx);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

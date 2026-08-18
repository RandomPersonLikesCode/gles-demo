// SPDX-License-Identifier: MIT

#include <SDL3/SDL.h>

#include "./display.hh"

Core::Display::Display(const char *title, int width, int height)
  : window(nullptr), renderer_ctx(nullptr), title(title),
    width(width), height(height) {}

Core::Display::~Display() {
  SDL_GL_MakeCurrent(nullptr, nullptr);

  SDL_GL_DestroyContext(this->renderer_ctx);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

bool Core::Display::init() {
  SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return false;
  }

  SDL_GL_SetAttribute(
    SDL_GL_CONTEXT_PROFILE_MASK,
    SDL_GL_CONTEXT_PROFILE_ES
  );

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  this->window = SDL_CreateWindow(
    this->title,
    this->width,
    this->height,
    SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
  );

  if (!this->window) {
    return false;
  }

  this->renderer_ctx = SDL_GL_CreateContext(this->window);

  if (!this->renderer_ctx) {
    return false;
  }

  SDL_GL_MakeCurrent(this->window, this->renderer_ctx);

  return true;
}

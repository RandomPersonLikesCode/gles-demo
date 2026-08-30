// SPDX-License-Identifier: MIT

#include <SDL3/SDL.h>

#include "./model.hh"

void Core::Model::create(const char *path) {
  this->data = static_cast<unsigned char *>(SDL_LoadFile(path, nullptr));
  this->model = m3d_load(data, nullptr, nullptr, nullptr);
}

Core::Model::~Model(void) {
  m3d_free(this->model);
  SDL_free(this->data);
}

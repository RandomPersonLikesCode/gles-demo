// SPDX-License-Identifier: MIT

#include <SDL3/SDL.h>

#include <GLES3/gl32.h>

#include "stb_image.h"

#include "../utils/logging.hh"
#include "./texture.hh"

void Core::Texture::create(const char *path) {
  size_t size = 0;
  stbi_uc *data = static_cast<stbi_uc *>(SDL_LoadFile(path, &size));

  if (!data) {
    LOG_ERR("SDL error: %s", SDL_GetError());
  }

  int width = 0;
  int height = 0;
  int channels = 0;

  stbi_uc *tex =
      stbi_load_from_memory(data, size, &width, &height, &channels, 4);

  if (!tex) {
    LOG_ERR("stbi error: %s", stbi_failure_reason());
  }

  glGenTextures(1, &this->handle);
  glBindTexture(GL_TEXTURE_2D, this->handle);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, tex);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(tex);
  SDL_free(data);
}

Core::Texture::~Texture(void) {
  glDeleteTextures(1, &this->handle);
}

// SPDX-License-Identifier: MIT

#pragma once

#include <GLES3/gl32.h>

namespace Core {
  struct Texture {
    GLuint handle = 0;

    void create(const char *path);
    ~Texture(void);
  };
}  // namespace Core

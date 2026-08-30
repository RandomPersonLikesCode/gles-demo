// SPDX-License-Identifier: MIT

#pragma once

#include "m3d.h"

namespace Core {
  struct Model {
    m3d_t *model = nullptr;
    unsigned char *data = nullptr;

    void create(const char *path);
    ~Model(void);
  };
}  // namespace Core

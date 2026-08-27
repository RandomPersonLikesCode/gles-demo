// SPDX-License-Identifier: MIT

#pragma once

#include <GLES3/gl32.h>

namespace Core {
  void load_shader(const char* path, GLenum type, GLuint& out);

  struct Program {
    GLuint handle;

    struct Uniforms {
      GLuint base_tex;
      GLuint model;
      GLuint view;
      GLuint projection;
    } uniforms;

    void create(void);
    ~Program(void);

    GLuint get_uniform(const char* name);
  };
}  // namespace Core

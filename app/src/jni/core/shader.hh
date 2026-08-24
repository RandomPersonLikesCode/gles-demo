// SPDX-License-Identifier: MIT

#pragma once

#include <GLES3/gl32.h>

namespace Core {
    bool load_shader(const char *path, GLenum type, GLuint &out);

    struct Program {
        GLuint handle = 0;

        bool create(void);
        ~Program(void);
    };
}

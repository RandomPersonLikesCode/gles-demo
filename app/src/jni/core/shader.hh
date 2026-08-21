// SPDX-License-Identifier: MIT

#pragma once

#include <GLES3/gl32.h>

namespace Core {
    struct Program {
        GLuint handle = 0;

        bool create(void);
        ~Program(void);

        static bool load_shader(const char *path, GLenum type, GLuint *out);
    };
}

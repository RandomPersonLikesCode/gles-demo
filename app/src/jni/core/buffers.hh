// SPDX-License-Identifier: MIT

#pragma once

#include <GLES3/gl32.h>

namespace Core {
    struct Buffers {
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;

        GLfloat vertices[12] = {
            -1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f
        };

        GLuint indices[6] = {
            0, 1, 3,
            2, 3, 1
        };

        void create(void);
        ~Buffers(void);
    };
}

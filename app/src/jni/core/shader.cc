// SPDX-License-Identifier: MIT

#include <SDL3/SDL.h>

#include <GLES3/gl32.h>

#include "./shader.hh"
#include "../utils/logging.hh"

bool Core::load_shader(
    const char *path,
    GLenum type,
    GLuint &out
) {
    char *src = static_cast<char*>(SDL_LoadFile(path, nullptr));

    if (!src) {
        LOG_ERR("SDL error: %s", SDL_GetError());
        return false;
    }

    out = glCreateShader(type);
    glShaderSource(out, 1, &src, nullptr);
    glCompileShader(out);

    GLint is_ok = 0;
    glGetShaderiv(out, GL_COMPILE_STATUS, &is_ok);
    if (is_ok != GL_TRUE) {
        char msg_buff[512] = "";
        glGetShaderInfoLog(out, sizeof(msg_buff), nullptr, msg_buff);
        LOG_ERR("GL shader error: %s", msg_buff);
        return false;
    }

    SDL_free(static_cast<void*>(src));
    return true;
}

bool Core::Program::create(void) {
    // Ignored return value to make sure it does not immediately exit
    GLuint vert_shader = 0;
    load_shader("shaders/vert_shader.glsl", GL_VERTEX_SHADER, vert_shader);

    GLuint frag_shader = 0;
    load_shader(
        "shaders/frag_shader.glsl",
        GL_FRAGMENT_SHADER,
        frag_shader
    );

    this->handle = glCreateProgram();
    glAttachShader(this->handle, vert_shader);
    glAttachShader(this->handle, frag_shader);
    glLinkProgram(this->handle);

    GLint is_ok = 0;
    glGetProgramiv(this->handle, GL_LINK_STATUS, &is_ok);
    if (is_ok != GL_TRUE) {
        char msg_buff[512];
        glGetProgramInfoLog(
            this->handle,
            sizeof(msg_buff),
            nullptr,
            msg_buff
        );
        LOG_ERR("GL program error: %s", msg_buff);
        return false;
    }

    glDeleteShader(frag_shader);
    glDeleteShader(vert_shader);
    return true;
}

Core::Program::~Program(void) {
    glDeleteProgram(this->handle);
}

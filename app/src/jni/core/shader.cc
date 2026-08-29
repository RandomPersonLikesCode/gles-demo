// SPDX-License-Identifier: MIT

#include <SDL3/SDL.h>

#include <GLES3/gl32.h>

#include "../utils/logging.hh"
#include "./shader.hh"

void Core::load_shader(const char *path, GLenum type, GLuint &out) {
  char *src = static_cast<char *>(SDL_LoadFile(path, nullptr));

  if (!src) {
    LOG_ERR("SDL error: %s", SDL_GetError());
    return;
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
  }

  SDL_free(src);
}

void Core::Program::create(void) {
  GLuint vert_shader = 0;
  load_shader("shaders/vert_shader.glsl", GL_VERTEX_SHADER, vert_shader);

  GLuint frag_shader = 0;
  load_shader("shaders/frag_shader.glsl", GL_FRAGMENT_SHADER, frag_shader);

  this->handle = glCreateProgram();
  glAttachShader(this->handle, vert_shader);
  glAttachShader(this->handle, frag_shader);
  glLinkProgram(this->handle);

  GLint is_ok = 0;
  glGetProgramiv(this->handle, GL_LINK_STATUS, &is_ok);
  if (is_ok != GL_TRUE) {
    char msg_buff[512];
    glGetProgramInfoLog(this->handle, sizeof(msg_buff), nullptr, msg_buff);
    LOG_ERR("GL program error: %s", msg_buff);
  }

  this->uniforms.base_tex = this->get_uniform("base_tex");
  this->uniforms.model = this->get_uniform("model");
  this->uniforms.view = this->get_uniform("view");
  this->uniforms.projection = this->get_uniform("projection");

  glDeleteShader(frag_shader);
  glDeleteShader(vert_shader);
}

Core::Program::~Program(void) {
  glDeleteProgram(this->handle);
}

GLuint Core::Program::get_uniform(const char *name) {
  return glGetUniformLocation(this->handle, name);
}

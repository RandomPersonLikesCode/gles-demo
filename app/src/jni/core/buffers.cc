// SPDX-License-Identifier: MIT

#include <GLES3/gl32.h>

#include "./buffers.hh"

void Core::Buffers::create(void) {
  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);

  glGenBuffers(1, &this->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (void *)0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (void *)(3 * sizeof(GLfloat)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

Core::Buffers::~Buffers(void) {
  glDeleteBuffers(1, &this->vbo);
  glDeleteVertexArrays(1, &this->vao);
}

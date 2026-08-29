// SPDX-License-Identifier: MIT

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "./mvp.hh"

void Core::MVP::create(float aspect_ratio) {
  this->view = glm::lookAt(cam_pos, cam_pos + cam_front,
                           glm::vec3(0.0f, 1.0f, 0.0f));

  this->projection =
      glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
}

// SPDX-License-Identifier: MIT

#pragma once

#include "glm/glm.hpp"

namespace Core {
  struct MVP {
    glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 cam_front = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    float cam_sensitivity = 0.35f;
    float cam_speed = 2.5f;
    float cam_pitch = 0.0f;
    float cam_yaw = -90.0f;
    float last_pos_x = 0.0f;
    float last_pos_y = 0.0f;

    bool is_cam_move = false;
    bool is_first_click = true;

    void create(float aspect_ratio);
  };
}  // namespace Core

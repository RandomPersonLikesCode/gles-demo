// SPDX-License-Identifier: MIT

#define SDL_MAIN_USE_CALLBACKS
#define STB_IMAGE_IMPLEMENTATION
#define M3D_IMPLEMENTATION

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <GLES3/gl32.h>

#include <cmath>
#include <new>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl3.h"

#include "stb_image.h"

#include "./app_state.hh"

#include "./core/buffers.hh"
#include "./core/display.hh"
#include "./core/gui.hh"
#include "./core/model.hh"
#include "./core/mvp.hh"
#include "./core/shader.hh"
#include "./core/texture.hh"

Uint64 last = 0;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  App::AppState *state = new (std::nothrow) App::AppState();
  stbi_set_flip_vertically_on_load(true);

  if (!state) {
    return SDL_APP_FAILURE;
  }

  if (!state->dp.create()) {
    return SDL_APP_FAILURE;
  }

  state->gui.create(state->dp.window, state->dp.renderer_ctx);
  state->rect.create();
  state->prog.create();
  state->mvp.create(state->dp.aspect_ratio);
  state->tex.create(App::metal);
  state->teapot.create(App::teapot);

  last = SDL_GetTicksNS();

  *appstate = state;
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  App::AppState *state = static_cast<App::AppState *>(appstate);

  Uint64 current = SDL_GetTicksNS();

  double dt = static_cast<double>((current - last)) / SDL_NS_PER_SECOND;

  last = current;
  float cam_speed = state->mvp.cam_speed * dt;

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Model Properties");

  ImGui::Text("Path: %s", App::teapot);
  ImGui::Text("Name: %s", state->teapot.model->name);
  ImGui::Text("Author: %s", state->teapot.model->author);
  ImGui::Text("Description: %s", state->teapot.model->desc);
  ImGui::Text("License: %s", state->teapot.model->license);

  ImGui::NewLine();

  ImGui::Text("Vertex count: %u", state->teapot.model->numvertex);

  ImGui::End();

  if (state->show_exit_panel) {
    ImGui::Begin("Exit?");
    if (ImGui::Button("Yes")) {
      state->show_exit_panel = false;
      return SDL_APP_SUCCESS;
    }

    ImGui::SameLine();

    if (ImGui::Button("No")) {
      state->show_exit_panel = false;
    }

    ImGui::End();
  }

  if (state->mvp.is_cam_move) {
    state->mvp.cam_pos += cam_speed * state->mvp.cam_front;
  }

  state->mvp.view = glm::lookAt(state->mvp.cam_pos,
                                state->mvp.cam_pos + state->mvp.cam_front,
                                glm::vec3(0.0f, 1.0f, 0.0f));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(state->prog.handle);
  glBindVertexArray(state->rect.vao);

  glUniform1i(state->prog.uniforms.base_tex, 0);
  glUniformMatrix4fv(state->prog.uniforms.model, 1, GL_FALSE,
                     glm::value_ptr(state->mvp.model));

  glUniformMatrix4fv(state->prog.uniforms.view, 1, GL_FALSE,
                     glm::value_ptr(state->mvp.view));

  glUniformMatrix4fv(state->prog.uniforms.projection, 1, GL_FALSE,
                     glm::value_ptr(state->mvp.projection));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, state->tex.handle);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(state->dp.window);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  App::AppState *state = static_cast<App::AppState *>(appstate);
  ImGuiIO &io = ImGui::GetIO();

  ImGui_ImplSDL3_ProcessEvent(event);
  if (io.WantCaptureMouse || io.WantTextInput) {
    return SDL_APP_CONTINUE;
  }

  switch (event->type) {
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;

      break;
    case SDL_EVENT_KEY_UP:
      if (event->key.scancode == SDL_SCANCODE_AC_BACK) {
        state->show_exit_panel = true;
      }

      break;
    case SDL_EVENT_FINGER_DOWN:
      if (event->tfinger.x < 0.5f) {
        state->mvp.is_cam_move = true;
      }

      break;
    case SDL_EVENT_FINGER_UP:
      state->mvp.is_cam_move = false;
      state->mvp.is_first_click = true;

      break;
    case SDL_EVENT_FINGER_MOTION: {
      float pos_x = event->tfinger.x * state->dp.width;
      float pos_y = event->tfinger.y * state->dp.height;

      if (event->tfinger.x > 0.5f) {
        if (state->mvp.is_first_click) {
          state->mvp.last_pos_x = pos_x;
          state->mvp.last_pos_y = pos_y;

          state->mvp.is_first_click = false;
        }

        float offset_x = pos_x - state->mvp.last_pos_x;
        float offset_y = state->mvp.last_pos_y - pos_y;
        state->mvp.last_pos_x = pos_x;
        state->mvp.last_pos_y = pos_y;

        offset_x *= state->mvp.cam_sensitivity;
        offset_y *= state->mvp.cam_sensitivity;

        state->mvp.cam_yaw += offset_x;
        state->mvp.cam_pitch += offset_y;

        state->mvp.cam_pitch =
            glm::clamp(state->mvp.cam_pitch, -89.0f, 89.0f);

        glm::vec3 dir;
        dir.x = std::cos(glm::radians(state->mvp.cam_yaw)) *
                std::cos(glm::radians(state->mvp.cam_pitch));

        dir.y = std::sin(glm::radians(state->mvp.cam_pitch));

        dir.z = std::sin(glm::radians(state->mvp.cam_yaw)) *
                std::cos(glm::radians(state->mvp.cam_pitch));

        state->mvp.cam_front = glm::normalize(dir);
      }
    }

    break;
  }

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  if (appstate) {
    App::AppState *state = static_cast<App::AppState *>(appstate);

    delete state;
  }
}

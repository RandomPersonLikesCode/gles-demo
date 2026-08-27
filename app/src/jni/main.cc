// SPDX-License-Identifier: MIT

#define SDL_MAIN_USE_CALLBACKS
#define STB_IMAGE_IMPLEMENTATION

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <GLES3/gl32.h>

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
#include "./core/shader.hh"
#include "./core/texture.hh"

float rot_speed = 100.0f;
float fov = 45.0f;
float t = 1.0f;

float aspect = 0.0f;

Uint64 last = 0;

glm::mat4 model(1.0f);
glm::mat4 view(1.0f);
glm::mat4 projection(1.0f);

const float cam_speed = 100.0f;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  App::AppState* state = new (std::nothrow) App::AppState();
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
  state->tex.create(App::metal);

  aspect = static_cast<float>(state->dp.width) / state->dp.height;

  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  last = SDL_GetTicksNS();

  *appstate = state;
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  App::AppState* state = static_cast<App::AppState*>(appstate);

  Uint64 current = SDL_GetTicksNS();

  double dt = static_cast<double>((current - last)) / SDL_NS_PER_SECOND;

  last = current;

  t += rot_speed * dt;

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Properties");

  ImGui::SliderFloat("Rotation speed", &rot_speed, 0.0f, 1000.0f);
  ImGui::SliderFloat("FOV", &fov, 0.0f, 180.0f);

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

  projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
  model = glm::rotate(glm::mat4(1.0f), glm::radians(t),
                      glm::vec3(1.0f, 0.5f, 0.3f));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(state->prog.handle);
  glBindVertexArray(state->rect.vao);

  glUniform1i(state->prog.uniforms.base_tex, 0);
  glUniformMatrix4fv(state->prog.uniforms.model, 1, GL_FALSE,
                     glm::value_ptr(model));

  glUniformMatrix4fv(state->prog.uniforms.view, 1, GL_FALSE,
                     glm::value_ptr(view));

  glUniformMatrix4fv(state->prog.uniforms.projection, 1, GL_FALSE,
                     glm::value_ptr(projection));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, state->tex.handle);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(state->dp.window);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  App::AppState* state = static_cast<App::AppState*>(appstate);
  ImGui_ImplSDL3_ProcessEvent(event);

  switch (event->type) {
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;

      break;
    case SDL_EVENT_KEY_UP:
      if (event->key.scancode == SDL_SCANCODE_AC_BACK) {
        state->show_exit_panel = true;
      }

      break;
  }

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  if (appstate) {
    App::AppState* state = static_cast<App::AppState*>(appstate);

    delete state;
  }
}

// SPDX-License-Identifier: MIT

#include <SDL3/SDL.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl3.h"

#include "./gui.hh"

constexpr const char* glsl_ver = "#version 300 es";

void Core::Gui::create(SDL_Window* window, SDL_GLContext ctx) {
  float scaling =
      SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay()) - 0.5f;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForOpenGL(window, ctx);
  ImGui_ImplOpenGL3_Init(glsl_ver);

  ImGuiStyle& style = ImGui::GetStyle();
  style.ScaleAllSizes(scaling);
  style.FontScaleDpi = scaling;
}

Core::Gui::~Gui(void) {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}

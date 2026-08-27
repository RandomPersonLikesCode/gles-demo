// SPDX-License-Identifier: MIT

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_opengl3.h"

#include "./gui.hh"

constexpr const char *glsl_ver = "#version 300 es";

void Core::Gui::create(SDL_Window *window, SDL_GLContext ctx) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(window, ctx);
    ImGui_ImplOpenGL3_Init(glsl_ver);

    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(this->scaling);
    style.FontScaleDpi = this->scaling;
}

Core::Gui::~Gui(void) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

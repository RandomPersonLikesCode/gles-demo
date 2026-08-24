// SPDX-License-Identifier: MIT

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <GLES3/gl32.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_opengl3.h"

#include "./core/buffers.hh"
#include "./core/display.hh"
#include "./core/gui.hh"
#include "./core/shader.hh"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    Core::Display display = {
        .title = "SDL Demo",
    };

    if (!display.create()) {
        return 1;
    }

    Core::GUI gui = {
        .scaling = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay()) - 0.5f
    };

    gui.create(display.window, display.renderer_ctx);

    Core::Buffers rectangle = {};
    rectangle.create();

    Core::Program prog = {};
    prog.create();

    const GLuint tri_color = glGetUniformLocation(prog.handle, "tri_col");

    float color[] = {1.0f, 1.0f, 1.0f};
    bool is_running = true;
    while (is_running) {
        SDL_Event events = {};

        while (SDL_PollEvent(&events)) {
            ImGui_ImplSDL3_ProcessEvent(&events);

            switch (events.type) {
                case SDL_EVENT_QUIT:
                    is_running = false;
                    break;
            }
        }
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // TODO: expand more as needed
        ImGui::Begin("Properties");
        ImGui::ColorPicker3("Color", color, ImGuiColorEditFlags_NoInputs);
        ImGui::End();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(prog.handle);
        glBindVertexArray(rectangle.vao);

        glUniform3fv(tri_color, 1, color);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(display.window);
    }

    return 0;
}

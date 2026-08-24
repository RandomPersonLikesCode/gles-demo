// SPDX-License-Identifier: MIT

#define STB_IMAGE_IMPLEMENTATION

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <GLES3/gl32.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_opengl3.h"

#include "stb_image.h"

#include "./core/buffers.hh"
#include "./core/display.hh"
#include "./core/gui.hh"
#include "./core/shader.hh"
#include "./core/texture.hh"

constexpr char const *tex_path = "textures/rusty_metal/base.png";

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

    Core::Texture tex = {};
    tex.create(tex_path);

    const GLuint bright_fac = glGetUniformLocation(prog.handle, "bright_fac");

    float bright = 1.0f;
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

        ImGui::SliderFloat("Brightness", &bright, 0.0f, 1.0f, "%.2f");

        ImGui::Text("Texture");
        ImGui::Text("path: %s", tex_path);

        ImGui::End();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(prog.handle);
        glBindTexture(GL_TEXTURE_2D, tex.handle);
        glBindVertexArray(rectangle.vao);
        
        glUniform1f(bright_fac, bright);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(display.window);
    }

    return 0;
}

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

constexpr const char *metal = "textures/rusty_metal/base.png";
constexpr const char *brick = "textures/brick_all/base.png";

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

    Core::Texture rusty_metal = {};
    rusty_metal.create(metal);

    Core::Texture brick_wall = {};
    brick_wall.create(brick);

    const GLuint bright_fac = glGetUniformLocation(
        prog.handle,
        "bright_fac"
    );
    const GLuint base_tex = glGetUniformLocation(
        prog.handle,
        "base_tex"
    );

    float bright = 1.0f;
    bool is_running = true;

    const char *textures[] = {metal, brick};
    const GLuint tex_handles[] = {rusty_metal.handle, brick_wall.handle};
    const char *display_items[] = {"Rusty metal", "Brick wall"};
    int selected = 0;

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

        ImGui::Combo(
            "Texture selection",
            &selected,
            display_items,
            IM_ARRAYSIZE(display_items)
        );

        ImGui::SliderFloat("Brightness", &bright, 0.0f, 1.0f, "%.2f");

        ImGui::Text("Texture");

        ImGui::Text("path: %s", textures[selected]);

        ImGui::End();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(prog.handle);
        glBindVertexArray(rectangle.vao);
        
        glUniform1f(bright_fac, bright);
        glUniform1i(base_tex, 0);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, tex_handles[selected]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(display.window);
    }

    return 0;
}

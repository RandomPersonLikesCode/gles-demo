// SPDX-License-Identifier: MIT

#define SDL_MAIN_USE_CALLBACKS
#define STB_IMAGE_IMPLEMENTATION

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <GLES3/gl32.h>

#include <new>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_opengl3.h"

#include "stb_image.h"

#include "./app_state.hh"

#include "./core/buffers.hh"
#include "./core/display.hh"
#include "./core/gui.hh"
#include "./core/shader.hh"
#include "./core/texture.hh"

GLuint bright_fac = 0;
GLuint base_tex = 0;
float brightness = 1.0f;
bool show_exit = false;

long long unsigned int counter = 0;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    App::AppState *state = new (std::nothrow) App::AppState();

    if (!state) {
        return SDL_APP_FAILURE;
    }

    state->dp.title = "OpenGL ES Demo";
    if (!state->dp.create()) {
        return SDL_APP_FAILURE;
    }

    state->gui.scaling = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    state->gui.create(state->dp.window, state->dp.renderer_ctx);

    state->rect.create();
    state->prog.create();

    stbi_set_flip_vertically_on_load(true);
    state->test.create(App::test);

    bright_fac = state->prog.get_uniform("bright_fac");
    base_tex = state->prog.get_uniform("base_tex");

    *appstate = state;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    App::AppState *state = static_cast<App::AppState*>(appstate);

    counter++;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Properties");

    ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f, "%.2f");
    ImGui::Text("Counter: %llu", counter);

    ImGui::End();

    if (show_exit) {
        ImGui::Begin("Exit?");
        if (ImGui::Button("Yes")) {
            show_exit = false;
            return SDL_APP_SUCCESS;
        }

        ImGui::SameLine();

        if (ImGui::Button("No")) {
            show_exit = false;
        }

        ImGui::End();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(state->prog.handle);
    glBindVertexArray(state->rect.vao);

    glUniform1f(bright_fac, brightness);
    glUniform1i(base_tex, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state->test.handle);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(state->dp.window);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    ImGui_ImplSDL3_ProcessEvent(event);

    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;

            break;
        case SDL_EVENT_KEY_UP:
            if (event->key.scancode == SDL_SCANCODE_AC_BACK) {
                show_exit = true;
            }

            break;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (appstate) {
        App::AppState *state = static_cast<App::AppState*>(appstate);

        delete state;
    }
}

// SPDX-License-Identifier: MIT

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <GLES3/gl32.h>

#include "./core/display.hh"
#include "./core/shader.hh"

GLfloat verts[] = {
    0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    Core::Display display = {
        .title = "SDL Demo",
    };

    if (!display.create()) {
        return 1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    Core::Program prog = {};

    // Also ignored here
    prog.create();

    bool is_running = true;
    while (is_running) {
        SDL_Event events = {};

        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                case SDL_EVENT_QUIT:
                    is_running = false;
                    break;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog.handle);
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(display.window);
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return 0;
}

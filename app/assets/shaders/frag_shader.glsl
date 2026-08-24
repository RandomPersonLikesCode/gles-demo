#version 320 es
// SPDX-License-Identifier: MIT
precision mediump float;

uniform float bright_fac;
uniform sampler2D tex;

in vec2 tex_coord;
out vec4 final_col;

void main(void) {
    final_col = texture(tex, tex_coord) * bright_fac;
}

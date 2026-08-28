#version 320 es
// SPDX-License-Identifier: MIT
precision mediump float;

uniform float brightness;
uniform sampler2D base_tex;

in vec2 tex_coord;
out vec4 final_col;

void main(void) {
    final_col = texture(base_tex, tex_coord) * brightness;
}

#version 320 es
// SPDX-License-Identifier: MIT
precision mediump float;

uniform vec3 tri_col;

out vec4 final_col;

void main(void) {
    final_col = vec4(tri_col, 1.0);
}

#version 320 es
// SPDX-License-Identifier: MIT
precision mediump float;

in vec3 frag_col;
out vec4 final_col;

void main(void) {
    final_col = vec4(frag_col, 1.0);
}

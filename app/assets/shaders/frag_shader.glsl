#version 320 es
// SPDX-License-Identifier: MIT
precision mediump float;

uniform vec3 col_pick;

in vec3 frag_col;
out vec4 final_col;

void main(void) {
    final_col = vec4(col_pick, 1.0);
}

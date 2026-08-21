#version 320 es
// SPDX-License-Identifier: MIT

precision highp float;

layout (location = 0) in vec3 org_pos;
layout (location = 1) in vec3 org_col;

out vec3 frag_col;

void main(void) {
    gl_Position = vec4(org_pos, 1.0);
    frag_col = org_col;
}

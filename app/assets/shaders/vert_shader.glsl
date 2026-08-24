#version 320 es
// SPDX-License-Identifier: MIT

precision highp float;

layout (location = 0) in vec3 org_pos;
layout (location = 1) in vec2 org_tex_coord;

out vec2 tex_coord;

void main(void) {
    gl_Position = vec4(org_pos * 0.5, 1.0);
    tex_coord = org_tex_coord;
}

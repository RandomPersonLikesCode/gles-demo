#version 320 es
// SPDX-License-Identifier: MIT

precision highp float;

layout (location = 0) in vec3 org_pos;
layout (location = 1) in vec2 org_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 tex_coord;

void main(void) {
    gl_Position = projection * view * model * vec4(org_pos, 1.0);
    tex_coord = org_tex_coord;
}

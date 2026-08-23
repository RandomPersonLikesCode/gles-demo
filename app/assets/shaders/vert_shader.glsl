#version 320 es
// SPDX-License-Identifier: MIT

precision highp float;

layout (location = 0) in vec3 org_pos;

void main(void) {
    gl_Position = vec4(org_pos * 0.5, 1.0);
}

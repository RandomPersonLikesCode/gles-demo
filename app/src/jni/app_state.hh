// SPDX-License-Identifier: `MIT

#pragma once

#include "./core/buffers.hh"
#include "./core/display.hh"
#include "./core/gui.hh"
#include "./core/shader.hh"
#include "./core/texture.hh"

namespace App {
    struct AppState {
        Core::Display dp;
        Core::Gui gui;
        Core::Buffers rect;
        Core::Program prog;
        Core::Texture metal;
        Core::Texture wall;
        Core::Texture test;
    };

    constexpr const char *metal = "textures/rusty_metal/base.png";
    constexpr const char *wall = "textures/brick_wall/base.png";
    constexpr const char *test = "textures/test.png";
}

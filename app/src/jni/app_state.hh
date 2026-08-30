// SPDX-License-Identifier: `MIT

#pragma once

#include "./core/buffers.hh"
#include "./core/display.hh"
#include "./core/gui.hh"
#include "./core/model.hh"
#include "./core/mvp.hh"
#include "./core/shader.hh"
#include "./core/texture.hh"

namespace App {
  struct AppState {
    Core::Display dp;
    Core::Gui gui;
    Core::Buffers rect;
    Core::Program prog;
    Core::MVP mvp;
    Core::Texture tex;
    Core::Model teapot;

    bool show_exit_panel;
  };

  constexpr const char *metal = "textures/metal.png";
  constexpr const char *teapot = "teapot.m3d";
}  // namespace App

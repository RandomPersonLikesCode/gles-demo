// SPDX-License-Identifier: MIT

package com.rplc.sdl_demo;

import org.libsdl.app.SDLActivity;

public class MainActivity extends SDLActivity {
  @Override
  protected String[] getLibraries() {
    return new String[] {
      "SDL3",
      "main"
    };
  }
}

// SPDX-License-Identifier: MIT

#pragma once

#define LOG_ERR(msg, ...)                                        \
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, msg, __VA_ARGS__)

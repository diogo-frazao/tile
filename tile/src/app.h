#pragma once
#include <stdint.h>

inline constexpr uint16_t s_screenWidth = 320;
inline constexpr uint16_t s_screenHeight = 180;
inline struct SDL_Window* s_window = nullptr;
inline struct SDL_Renderer* s_renderer = nullptr;

void runApp();
void _initWindow();
void _update();
void _render();
void _killWindow();
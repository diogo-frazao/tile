#pragma once
#include <stdint.h>
#include "../screens.h"

inline constexpr uint16_t s_screenWidth = 320;
inline constexpr uint16_t s_screenHeight = 180;
inline struct SDL_Window* s_window = nullptr;
inline struct SDL_Renderer* s_renderer = nullptr;

class App
{
public:
	void runApp();
private:
	void initWindow();
	void update();
	void render();
	void killWindow();

	MainScreen _mainScreen;
};

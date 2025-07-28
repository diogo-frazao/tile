#pragma once
#include <stdint.h>
#include "../screens.h"

inline bool s_debugCollidersEnabled = false;
inline bool s_useCustomMouse = false;

// Base game res
inline constexpr uint16_t k_screenWidth = 320;
inline constexpr uint16_t k_screenHeight = 180;

// This defines the resolution of the ui texture (used to draw all the ui)
inline constexpr uint16_t k_uiResolutionWidth = 1920;
inline constexpr uint16_t k_uiResolutionHeight = 1080;

inline struct SDL_Window* s_window = nullptr;
inline struct SDL_Renderer* s_renderer = nullptr;

class App
{
public:
	void runApp();
private:
	void initWindow();
	void start();
	void update();
	void render();
	void killWindow();

	PanelScreen _panelScreen;
	SettingsScreen _settingsScreen;
	AddSpritesScreen _addSpritesScreen;
	MainScreen _mainScreen;
	LayerInspectorScreen _layerInspectorScreen;
};

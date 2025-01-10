#pragma once
#include "core/spriteHandler.h"
#include "core/lib.h"
#include "core/ui.h"
#include "core/widgetLink.h"
#include <vector>
#include <stdint.h>
#include <array>

struct SDL_Texture;

class MainScreen
{
public:
	void start();
	void update();
	void render();
};

class SettingsScreen
{
public:
	void start();
	void update();
	void render();
	void destroy();
	inline static bool s_active = false;
	void onSettingsSaved(const std::vector<int8_t>& results);
private:
	SDL_Texture* _uiTexture;
	Text _title;
	Text _subtitle;
	WidgetLink _widgetLink;
	const std::array<const char*, 5> _resolutionOptions{
		"<  320x180  >",
		"<  960x540  >",
		"< 1280x720  >",
		"< 1920x1080 >",
		"< 2560x1440 >"
	};
};

class PanelScreen
{
public:
	void render();
	inline static bool s_isPanelActive = false;
};

class MouseScreen
{
public:
	void start();
	void update();
	void render();
private:
	Sprite _mouseSprite;
	IVec2 _lastMousePosition;
};
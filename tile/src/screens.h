#pragma once
#include "core/spriteHandler.h"
#include "core/lib.h"
#include "core/ui.h"
#include <vector>
#include <memory>

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
private:
	SDL_Texture* _uiTexture;
	Text _title;
	Text _subtitle;
	CheckBox _checkbox;
	OptionSelector _optionSelector;
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
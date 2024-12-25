#pragma once
#include "core/spriteHandler.h"
#include "core/lib.h"
#include "core/ui.h"

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
	inline static bool active = false;
private:
	Text title;
	Text subtitle;
};

class PanelScreen
{
public:
	void render();
	inline static bool isPanelActive = false;
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
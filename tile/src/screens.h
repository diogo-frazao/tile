#pragma once
#include "core/spriteHandler.h"
#include "core/lib.h"

class MainScreen
{
public:
	void start();
	void update();
	void render();
private:
	Sprite _mouseSprite;
	IVec2 _lastMousePosition;
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
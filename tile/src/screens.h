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
	// TODO port to input system
	IVec2 _lastMousePosition;
};
#pragma once
#include <stdint.h>
#include "core/spriteHandler.h"

class MainScreen
{
public:
	void start();
	void update();
	void render();
private:
	Sprite _mouseSprite;
	// TODO port to input system
	int32_t lastMouseX = 0;
	int32_t lastMouseY = 0;
};
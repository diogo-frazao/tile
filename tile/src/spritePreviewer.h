#pragma once
#include "core/spriteHandler.h"
#include <vector>

class SpritePreviewer
{
public:
	void start();
	void render();
	bool s_isActive = false;
	std::vector<Sprite> _spritesToPreview;
private:
	Sprite _backgroundSprite;
};
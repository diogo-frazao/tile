#pragma once
#include "core/spriteHandler.h"
#include "core/lib.h"
#include <vector>

class SpritePreviewer
{
public:
	void start(const Vec2& startLocation = Vec2());
	void render();
	bool s_isActive = false;
	std::vector<Sprite> _spritesToPreview;
private:
	Sprite _backgroundSprite;
	Vec2 _locationToStartGrid;
};
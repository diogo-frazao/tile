#pragma once
#include "core/spriteHandler.h"
#include "core/lib.h"
#include <vector>

class SpritePreviewer
{
public:
	SpritePreviewer(const Vec2& startLocation = Vec2());
	void render();
	bool _isVisible = false;
	std::vector<Sprite> _spritesToPreview;

	bool operator==(const SpritePreviewer& other) const
	{
		return this == &other;
	}

private:
	Sprite _backgroundSprite;
	Vec2 _locationToStartGrid;
};
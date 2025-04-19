#pragma once
#include "core/spriteHandler.h"
#include "core/lib.h"
#include <vector>

class SpritePreviewer
{
public:
	SpritePreviewer(const Vec2& startLocation = Vec2(), LayerType layer = EMPTY);
	void render();
	bool _isVisible = false;
	std::vector<Sprite> _spritesToPreview;

	bool operator==(const SpritePreviewer& other) const
	{
		return this == &other;
	}

private:
	LayerType _layer;
	Sprite _backgroundSprite;
	Vec2 _locationToStartGrid;
};
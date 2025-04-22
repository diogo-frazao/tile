#pragma once
#include "core/spriteHandler.h"
#include "core/lib.h"
#include <vector>

struct SpriteWithExternalRect
{
	Sprite sprite;
	RectCollider rect;

	SpriteWithExternalRect() = default;
	SpriteWithExternalRect(const Sprite& sprite) : sprite(sprite) {};
};

class SpritePreviewer
{
public:
	SpritePreviewer(const Vec2& startLocation = Vec2(), LayerType layer = EMPTY);
	void render();
	bool _isVisible = false;
	std::vector<SpriteWithExternalRect> _spritesToPreview;
	LayerType _layer;

	bool operator==(const SpritePreviewer& other) const
	{
		return this == &other;
	}

private:
	Sprite _backgroundSprite;
	Vec2 _locationToStartGrid;
};
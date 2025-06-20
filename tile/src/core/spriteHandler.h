#pragma once
#include <stdint.h>
#include "lib.h"
#include "input.h"

struct SDL_Texture;
struct SDL_Rect;
struct SDL_FRect;
inline SDL_Texture* s_atlasTexture;

enum SpriteType
{
	CURSOR,
	CURSOR_DRAGGING,
	MOCKUP,
	BUTTON,
	PREVIEWER_BG,
	UNDO_BUTTON,
	UNDO_BUTTON_HOVERED,
	SPRITES_MAX,
};

struct Sprite
{
	Vec2 position;
	IVec2 size;
	IVec2 offset;

	bool isValid()
	{
		return size.x > 0 && size.y > 0;
	}

	void invalidate()
	{
		position = Vec2();
		size = IVec2();
		offset = IVec2();
	}

	Sprite() : position(0.f, 0.f), size(0, 0), offset(0, 0) {};
	Sprite(const IVec2& size, const IVec2& offset) : size(size), offset(offset) {};
};

namespace textures
{
	SDL_Texture* loadAtlas();
	Sprite getSprite(SpriteType type);
}

void renderSprite(const Sprite& sprite, bool flipHorizontal = false);
void renderSpriteInsideRect(const Sprite& sprite, const Vec2& targetRect);
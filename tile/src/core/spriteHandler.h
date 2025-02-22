#pragma once
#include <stdint.h>
#include "lib.h"

struct SDL_Texture;
struct SDL_Rect;
struct SDL_FRect;
inline SDL_Texture* s_atlasTexture;

enum SpriteType
{
	CURSOR,
	MOCKUP,
	BUTTON,
	PREVIEWER_BG,
	SPRITES_MAX,
};

struct Sprite
{
	Vec2 position;
	IVec2 size;
	IVec2 offset;

	Sprite() = default;
	Sprite(const IVec2& size, const IVec2& offset) : size(size), offset(offset) {};
};

namespace textures
{
	SDL_Texture* loadAtlas();
	Sprite getSprite(SpriteType type);
}

void renderSprite(const Sprite& sprite);
void renderSpriteInsideRect(const Sprite& sprite, const Vec2& targetRect);
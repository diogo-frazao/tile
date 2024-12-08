#pragma once
#include <stdint.h>
#include "lib.h"

struct SDL_Texture;
inline SDL_Texture* s_atlasTexture;

enum spriteType
{
	CURSOR,
	SPRITES_MAX,
};

struct Sprite
{
	Vec2 position;
	IVec2 size;
	IVec2 offset;
};


namespace textures
{
	SDL_Texture* loadAtlas();
	Sprite getSprite(spriteType type);
}
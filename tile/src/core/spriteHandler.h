#pragma once
#include <stdint.h>
#include "lib.h"
#include "input.h"
#include "log.h"

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
	SPRITES_MAX
};

struct Sprite
{
	Vec2 position;
	IVec2 size;
	IVec2 offset;
	
	bool isTile;
	IVec2 startingTileOffset;

	bool isValid()
	{
		return size.x > 0 && size.y > 0;
	}

	void invalidate()
	{
		position = Vec2();
		size = IVec2();
		offset = IVec2();
		isTile = false;
	}

	void setTileOffsetFromMask(uint8_t neighborMask)
	{
		D_ASSERT(isTile, "Trying to assign a tile offset for a sprite that's not a tile");

		uint8_t row = neighborMask / 4;
		uint8_t col = neighborMask % 4;

		offset.x = startingTileOffset.x + col * size.x;
		offset.y = startingTileOffset.y + row * size.y;
	}

	void handleTileCornerFromEmptyNeighborIndex(uint8_t emptyNeighborIndex)
	{
		D_ASSERT(isTile, "Trying to assign a tile offset for a sprite that's not a tile");

		uint8_t row = 4;
		uint8_t col = (emptyNeighborIndex - 4);

		offset.x = startingTileOffset.x + col * size.x;
		offset.y = startingTileOffset.y + row * size.y;
	}

	Sprite() : position(0.f, 0.f), size(0, 0), offset(0, 0), isTile(false) {};
	Sprite(const IVec2& size, const IVec2& offset, bool isTile = false, const IVec2& startingTileOffset = IVec2()) : 
	size(size), offset(offset), isTile(isTile), startingTileOffset(startingTileOffset) {};
};

namespace textures
{
	SDL_Texture* loadAtlas();
	Sprite getSprite(SpriteType type);
}

void renderSprite(const Sprite& sprite, bool flipHorizontal = false);
void renderSpriteInsideRect(const Sprite& sprite, const Vec2& targetRect);
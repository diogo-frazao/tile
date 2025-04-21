#include "spriteHandler.h"

#include "SDL_image.h"
#include <string>
#include "app.h"
#include "log.h"

namespace textures
{
	SDL_Texture* loadAtlas()
	{
		static const char* path = "tile/graphics/atlas.png";
		SDL_Texture* const texture = IMG_LoadTexture(s_renderer, path);
		if (texture == nullptr)
		{
			D_ASSERT(false, "Failed to load atlas texture. Error %s", SDL_GetError());
			return nullptr;
		}

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		s_atlasTexture = texture;
		return s_atlasTexture;
	}

	Sprite getSprite(SpriteType type)
	{
		Sprite sprite;

		switch (type)
		{
			case CURSOR:
				sprite.offset = { 0 ,0 };
				sprite.size = { 7, 10 };
				break;
			case CURSOR_DRAGGING:
				sprite.offset = { 7, 0 };
				sprite.size = { 11, 9 };
				break;
			case MOCKUP:
				sprite.offset = { 18, 0 };
				sprite.size = { 320, 180 };
				break;
			case BUTTON:
				sprite.offset = { 328, 0 };
				sprite.size = { 10, 9 };
				break;
			case PREVIEWER_BG:
				sprite.offset = { 338, 0 };
				sprite.size = { 9,9 };
				break;
			default:
				D_ASSERT(false, "Invalid texture to load");
		}

		return sprite;
	}
}

void renderSprite(const Sprite& sprite)
{
	s_src.x = sprite.offset.x;
	s_src.y = sprite.offset.y;
	s_src.w = sprite.size.x;
	s_src.h = sprite.size.y;

	s_dest.x = sprite.position.x;
	s_dest.y = sprite.position.y;
	s_dest.w = (float)sprite.size.x;
	s_dest.h = (float)sprite.size.y;

	SDL_RenderCopyExF(s_renderer, s_atlasTexture, &s_src, &s_dest, 0, nullptr, SDL_FLIP_NONE);
}

void renderSpriteInsideRect(const Sprite& sprite, const Vec2& targetRect)
{
	bool shouldScaleSprite = ((float)sprite.size.x > targetRect.x) || ((float)sprite.size.y > targetRect.y);

	s_src.x = sprite.offset.x;
	s_src.y = sprite.offset.y;
	s_src.w = sprite.size.x;
	s_src.h = sprite.size.y;

	s_dest.x = sprite.position.x;
	s_dest.y = sprite.position.y;
	s_dest.w = shouldScaleSprite ? targetRect.x : (float)sprite.size.x;
	s_dest.h = shouldScaleSprite ? targetRect.y : (float)sprite.size.y;

	SDL_RenderCopyExF(s_renderer, s_atlasTexture, &s_src, &s_dest, 0, nullptr, SDL_FLIP_NONE);
}
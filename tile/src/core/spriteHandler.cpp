#include "spriteHandler.h"

#include "SDL_image.h"
#include <string>
#include "app.h"
#include "log.h"

namespace textures
{
	SDL_Texture* loadAtlas()
	{
		static const char* path = "graphics/atlas.png";
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
			case MOCKUP:
				sprite.offset = { 8, 0 };
				sprite.size = { 320, 180 };
				break;
			case BUTTON:
				sprite.offset = { 328, 0 };
				sprite.size = { 10, 9 };
				break;
			default:
				D_ASSERT(false, "Invalid texture to load");
		}

		return sprite;
	}
}

void renderSprite(const Sprite& sprite, SDL_Rect& src, SDL_FRect& dest)
{
	src.x = sprite.offset.x;
	src.y = sprite.offset.y;
	src.w = sprite.size.x;
	src.h = sprite.size.y;

	dest.x = sprite.position.x;
	dest.y = sprite.position.y;
	dest.w = (float)sprite.size.x;
	dest.h = (float)sprite.size.y;

	SDL_RenderCopyExF(s_renderer, s_atlasTexture, &src, &dest, 0, nullptr, SDL_FLIP_NONE);
}

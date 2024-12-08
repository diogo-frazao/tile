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
			D_ASSERT(false, "Texture is null. Error %s", SDL_GetError());
			return nullptr;
		}

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		s_atlasTexture = texture;
		return s_atlasTexture;
	}

	Sprite getSprite(spriteType type)
	{
		Sprite sprite;

		switch (type)
		{
			case CURSOR:
			{
				sprite.offset = { 0 ,0 };
				sprite.size = { 7, 10 };
				break;
			}
			default:
			{
				D_ASSERT(false, "Invalid texture to load");
			}
		}

		return sprite;
	}
}
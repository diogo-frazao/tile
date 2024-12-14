#pragma once
#include "lib.h"
#include <stdint.h>

struct SDL_Texture;
struct SDL_Color;

namespace ui
{
	struct Text
	{
		Vec2 position;
		SDL_Texture* texture;
		Vec2 bounds;

		Text(const Vec2& pos, const Vec2& bounds, SDL_Texture* text) : position(pos), texture(text), bounds(bounds) {};
	};

	Text createText(const char* text, const Vec2& position, uint8_t size, const SDL_Color& color);
}
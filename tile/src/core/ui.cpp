#include "ui.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "log.h"
#include "app.h"

namespace ui
{
	static Text s_invalidText{Vec2(0, 0), Vec2(0, 0), nullptr};

	Text createText(const char* text, const Vec2& position, uint8_t size, const SDL_Color& color)
	{
		static const char* fontPath = "font/Renogare.ttf";

		TTF_Font* const font = TTF_OpenFont(fontPath, size);
		if (font == nullptr)
		{
			D_ASSERT(false, "Failed to load font %s. Error: %s", fontPath, TTF_GetError());
			return s_invalidText;
		}

		SDL_Surface* const textSurface = TTF_RenderUTF8_Solid(font, text, color);
		SDL_Texture* const textTexture = SDL_CreateTextureFromSurface(s_renderer, textSurface);
		if (textTexture == nullptr)
		{
			D_ASSERT(false, "Failed to load font %s. Error: %s", fontPath, TTF_GetError());
			return s_invalidText;
		}

		SDL_FRect newRect { position.x, position.y, (float)textSurface->w, (float)textSurface->h };
		SDL_FreeSurface(textSurface);

		return Text(position, Vec2(newRect.w, newRect.h), textTexture);
	}
}
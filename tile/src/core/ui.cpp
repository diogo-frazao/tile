#include "ui.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "log.h"
#include "app.h"

static Text s_invalidText{Vec2(0, 0), Vec2(0, 0), nullptr};

static SDL_Color s_selectedUIColor{ 234, 140, 70 };

Text createText(const char* text, const Vec2& position, uint8_t size, const SDL_Color& color, const DrawMode drawmode)
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
	TTF_CloseFont(font);

	return Text(position, Vec2(newRect.w, newRect.h), textTexture, drawmode);
}

void renderText(SDL_Texture* targetTexture, SDL_FRect& dest, const Text& text)
{
	SDL_SetRenderTarget(s_renderer, targetTexture);

	switch (text.drawMode)
	{
		case TOP_LEFT:
		{
			dest.x = text.position.x;
			dest.y = text.position.y;
			break;
		}
		case TOP_CENTER:
		{
			dest.x = text.position.x - (text.bounds.x / 2);
			dest.y = text.position.y;
			break;
		}
		case TOP_RIGHT:
		{
			dest.x = text.position.x - text.bounds.x;
			dest.y = text.position.y;
			break;
		}
	}

	dest.w = text.bounds.x;
	dest.h = text.bounds.y;

	SDL_RenderCopyExF(s_renderer, text.texture, nullptr, &dest, 0, nullptr, SDL_FLIP_NONE);
	SDL_SetRenderTarget(s_renderer, nullptr);
	SDL_RenderCopy(s_renderer, targetTexture, nullptr, nullptr);
}

void destroyText(const Text& text)
{
	if (text.texture == nullptr)
	{
		D_ASSERT(false, "Text has invalid texture");
		return;
	}

	SDL_DestroyTexture(text.texture);
}

void onUISelected(const Text& text)
{
	if (!text.texture || SDL_SetTextureColorMod(text.texture, s_selectedUIColor.r, s_selectedUIColor.g, s_selectedUIColor.b) < 0)
	{
		D_ASSERT(false, "Invalid texture blend");
		return;
	}
}

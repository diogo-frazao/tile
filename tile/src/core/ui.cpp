#include "ui.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "log.h"
#include "app.h"

static Text s_invalidText{ Vec2(0, 0), Vec2(0, 0), nullptr, {255, 255, 255}, 150 };

static SDL_Color s_selectedUIColor{ 234, 140, 70 };
static const char* s_selectedCharacter = "x";
static const char* s_notSelectedCharacter = "_";

Text createText(const char* text, const Vec2& position, uint16_t size, const SDL_Color& color, const DrawMode drawmode)
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

	return Text(position, Vec2(newRect.w, newRect.h), textTexture, color, size, drawmode);
}

CheckBox createCheckbox(bool startEnabled, const Vec2& position, uint16_t size, const SDL_Color& color, const DrawMode drawMode)
{
	CheckBox checkbox;
	checkbox.isSelected = startEnabled;
	const char* newCheckboxText = startEnabled ? s_selectedCharacter : s_notSelectedCharacter;
	checkbox.text = createText(newCheckboxText, position, size, color, drawMode);
	return checkbox;
}

void Text::render(SDL_Texture* targetTexture, SDL_FRect& dest)
{
	SDL_SetRenderTarget(s_renderer, targetTexture);

	switch (drawMode)
	{
		case TOP_LEFT:
		{
			dest.x = position.x;
			dest.y = position.y;
			break;
		}
		case TOP_CENTER:
		{
			dest.x = position.x - (bounds.x / 2);
			dest.y = position.y;
			break;
		}
		case TOP_RIGHT:
		{
			dest.x = position.x - bounds.x;
			dest.y = position.y;
			break;
		}
	}

	dest.w = bounds.x;
	dest.h = bounds.y;

	// Prevent artifacts/leftovers on this new texture. This will not clear the whole renderer, just the new target it points to (aka target texture)
	// This way we can have a "new" draw per frame like we have with game sprites
	SDL_RenderClear(s_renderer);

	SDL_RenderCopyExF(s_renderer, texture, nullptr, &dest, 0, nullptr, SDL_FLIP_NONE);
	SDL_SetRenderTarget(s_renderer, nullptr);
	SDL_RenderCopy(s_renderer, targetTexture, nullptr, nullptr);
}

void Text::destroy()
{
	if (texture == nullptr)
	{
		D_ASSERT(false, "Text has invalid texture");
		return;
	}

	SDL_DestroyTexture(texture);
}

void Text::onHovered()
{
	if (!texture || SDL_SetTextureColorMod(texture, s_selectedUIColor.r, s_selectedUIColor.g, s_selectedUIColor.b) < 0)
	{
		D_ASSERT(false, "Invalid texture blend");
		return;
	}
}

void CheckBox::render(SDL_Texture* targetTexture, SDL_FRect& dest)
{
	text.render(targetTexture, dest);
}

void CheckBox::destroy()
{
	text.destroy();
}

void CheckBox::onHovered()
{
	text.onHovered();
}

void CheckBox::onSelected()
{
	isSelected = !isSelected;
	const char* newCheckboxText = isSelected ? s_selectedCharacter : s_notSelectedCharacter;
	Text newText = createText(newCheckboxText, text.position, text.size, text.color, text.drawMode);
	text.destroy();
	text = newText;
}

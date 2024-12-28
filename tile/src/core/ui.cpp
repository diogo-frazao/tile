#include "ui.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "log.h"
#include "app.h"

static const char* k_selectedCharacter = "x";
static const char* k_notSelectedCharacter = "_";

// Local helper functions

static SDL_Texture* createText(Vec2& outBounds, const char* text, const Vec2& position, uint16_t size, const SDL_Color& color)
{
	static const char* fontPath = "font/Renogare.ttf";

	TTF_Font* const font = TTF_OpenFont(fontPath, size);
	if (font == nullptr)
	{
		D_ASSERT(false, "Failed to load font %s. Error: %s", fontPath, TTF_GetError());
		return nullptr;
	}

	SDL_Surface* const textSurface = TTF_RenderUTF8_Solid(font, text, color);
	SDL_Texture* const textTexture = SDL_CreateTextureFromSurface(s_renderer, textSurface);
	if (textTexture == nullptr)
	{
		D_ASSERT(false, "Failed to load font %s. Error: %s", fontPath, TTF_GetError());
		return nullptr;
	}

	outBounds = Vec2((float)textSurface->w, (float)textSurface->h);

	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);

	return textTexture;
}

static void recreateText(Text& text, const char* newText)
{
	SDL_Texture* oldTexture = text._texture;
	Vec2 newBounds;
	SDL_Texture* newTexture = createText(newBounds, newText, text._position, text._size, text._color);
	text._bounds = newBounds;
	text._texture = newTexture;

	D_ASSERT(oldTexture, "Invalid texture to swap");
	SDL_DestroyTexture(oldTexture);
}

//
// Option Selector

OptionSelector::OptionSelector(const std::vector<const char*>& options, const Vec2& position, uint16_t size,
	const SDL_Color& color, const DrawMode drawMode, bool allowWrap)
{
	D_ASSERT((options.size() > 0) && (options.size() < 250), "Invalid options array");

	Vec2 textBounds;
	SDL_Texture* textTexture = createText(textBounds, options[0], position, size, color);

	this->_position = position;
	this->_bounds = textBounds;
	this->_texture = textTexture;
	this->_color = color;
	this->_size = size;
	this->_drawMode = drawMode;
	this->_isHovered = false;
	this->_allowWrap = allowWrap;
	this->_options = options;
	this->_selectedIndex = 0;
}

void OptionSelector::onRightPressed()
{
	if ((_selectedIndex == (_options.size() - 1)) && _allowWrap)
	{
		_selectedIndex = 0;
	}
	else
	{
		_selectedIndex = min(++_selectedIndex, static_cast<int>(_options.size() - 1));
	}

	SDL_Texture* oldTexture = _texture;
	Vec2 newBounds;
	SDL_Texture* newTexture = createText(newBounds, _options[_selectedIndex], _position, _size, _color);
	_bounds = newBounds;
	_texture = newTexture;

	D_ASSERT(oldTexture, "Invalid texture to swap");
	SDL_DestroyTexture(oldTexture);
}

void OptionSelector::onLeftPressed()
{
	if (_selectedIndex == 0 && _allowWrap)
	{
		_selectedIndex = static_cast<int>(_options.size() - 1);
	}
	else
	{
		_selectedIndex = max(--_selectedIndex, 0);
	}

	SDL_Texture* oldTexture = _texture;
	Vec2 newBounds;
	SDL_Texture* newTexture = createText(newBounds, _options[_selectedIndex], _position, _size, _color);
	_bounds = newBounds;
	_texture = newTexture;

	D_ASSERT(oldTexture, "Invalid texture to swap");
	SDL_DestroyTexture(oldTexture);
}

//
// Checkbox

CheckBox::CheckBox(bool startEnabled, const Vec2& position, uint16_t size, const SDL_Color& color, const DrawMode drawMode)
{
	const char* text = startEnabled ? k_selectedCharacter : k_notSelectedCharacter;
	Vec2 textBounds;
	SDL_Texture* textTexture = createText(textBounds, text, position, size, color);

	this->_isSelected = startEnabled;
	this->_position = position;
	this->_bounds = textBounds;
	this->_texture = textTexture;
	this->_color = color;
	this->_size = size;
	this->_drawMode = drawMode;
	this->_isHovered = false;
}

void CheckBox::onSelected()
{
	_isSelected = !_isSelected;
	const char* newCheckboxText = _isSelected ? k_selectedCharacter : k_notSelectedCharacter;

	SDL_Texture* oldTexture = _texture;
	Vec2 newBounds;
	SDL_Texture* newTexture = createText(newBounds, newCheckboxText, _position, _size, _color);
	_bounds = newBounds;
	_texture = newTexture;

	D_ASSERT(oldTexture, "Invalid texture to swap");
	SDL_DestroyTexture(oldTexture);
}

//
// Text

Text::Text(const char* text, const Vec2& position, uint16_t size, const SDL_Color& color, const DrawMode drawMode)
{
	Vec2 textBounds;
	SDL_Texture* textTexture = createText(textBounds, text, position, size, color);

	this->_position = position;
	this->_bounds = textBounds;
	this->_texture = textTexture;
	this->_color = color;
	this->_size = size;
	this->_drawMode = drawMode;
	this->_isHovered = false;
}

void Text::update()
{
	// TODO implement auto hover and click and release and etc
}

void Text::render(SDL_Texture* targetTexture, SDL_FRect& dest)
{
	SDL_SetRenderTarget(s_renderer, targetTexture);

	switch (_drawMode)
	{
		case TOP_LEFT:
		{
			dest.x = _position.x;
			dest.y = _position.y;
			break;
		}
		case TOP_CENTER:
		{
			dest.x = _position.x - (_bounds.x / 2);
			dest.y = _position.y;
			break;
		}
		case TOP_RIGHT:
		{
			dest.x = _position.x - _bounds.x;
			dest.y = _position.y;
			break;
		}
	}

	dest.w = _bounds.x;
	dest.h = _bounds.y;

	// Prevent artifacts/leftovers on this new texture. This will not clear the whole renderer, just the new target it points to (aka target texture)
	// This way we can have a "new" draw per frame like we have with game sprites
	SDL_RenderClear(s_renderer);

	SDL_RenderCopyExF(s_renderer, _texture, nullptr, &dest, 0, nullptr, SDL_FLIP_NONE);
	SDL_SetRenderTarget(s_renderer, nullptr);
	SDL_RenderCopy(s_renderer, targetTexture, nullptr, nullptr);
}

void Text::onHovered()
{
	if (!_texture || SDL_SetTextureColorMod(_texture, k_orange.r, k_orange.g, k_orange.b) < 0)
	{
		D_ASSERT(false, "Invalid texture blend");
		return;
	}
}

//
// Global helper functions

void destroyWidget(const Text& widget)
{
	if (widget._texture != nullptr)
	{
		SDL_DestroyTexture(widget._texture);
	}
}

void destroyWidget(Text* widget)
{
	if (!widget)
	{
		return;
	}

	if (widget->_texture != nullptr)
	{
		SDL_DestroyTexture(widget->_texture);
	}

	delete widget;
	widget = nullptr;
}

//

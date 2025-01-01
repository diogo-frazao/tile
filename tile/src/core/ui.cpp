#include "ui.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "log.h"
#include "app.h"
#include "input.h"

static const char* k_selectedCharacter = "x";
static const char* k_notSelectedCharacter = "_";

// Local helper functions

static Vec2 worldToScreen(const Vec2& pos)
{
	IVec2 uiFactor = IVec2(k_uiResolutionWidth / k_screenWidth, k_uiResolutionHeight / k_screenHeight);
	return Vec2(pos.x * uiFactor.x, pos.y * uiFactor.y);
}

static Vec2 screenToWorld(const Vec2& screenPos)
{
	IVec2 uiFactor = IVec2(k_uiResolutionWidth / k_screenWidth, k_uiResolutionHeight / k_screenHeight);
	return Vec2(screenPos.x / uiFactor.x, screenPos.y / uiFactor.y);
}

static SDL_Texture* createText(Vec2& outWorldBounds, const char* text, uint16_t size, const SDL_Color& color)
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

	Vec2 screenBounds = Vec2((float)textSurface->w, (float)textSurface->h);
	outWorldBounds = screenToWorld(screenBounds);

	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);

	return textTexture;
}

static void recreateText(Text& text, const char* newText)
{
	SDL_Texture* oldTexture = text._texture;
	Vec2 newBounds;
	SDL_Texture* newTexture = createText(newBounds, newText, text._size, text._color);
	text._worldBounds = newBounds;
	text._texture = newTexture;

	D_ASSERT(oldTexture, "Invalid texture to swap");
	SDL_DestroyTexture(oldTexture);
}

//
// Option Selector

OptionSelector::OptionSelector(const std::vector<const char*>& options, const Vec2& position, uint16_t size,
	const SDL_Color& color, const bool justify, bool allowWrap)
{
	D_ASSERT((options.size() > 0) && (options.size() < 250), "Invalid options array");
	Vec2 textBounds;
	SDL_Texture* textTexture = createText(textBounds, options[0], size, color);

	this->_worldPosition = position;
	this->_worldBounds = textBounds;
	this->_texture = textTexture;
	this->_color = color;
	this->_size = size;
	this->_isHovered = false;
	this->_allowWrap = allowWrap;
	this->_options = options;
	this->_selectedIndex = 0;
	this->_isHovered = false;
	this->_mainCollider = RectCollider(position, textBounds);

	if (justify)
	{
		_worldPosition.x -= _worldBounds.x / 2;
	}
}

void OptionSelector::update()
{
	Text::update();

	IVec2 mousePos = getMousePosition();
	if (wasMouseButtonPressedThisFrame(SDL_BUTTON_LEFT) && _isHovered)
	{
		float mouseXPerc = mousePos.x / (_worldPosition.x + _worldBounds.x);
		if (mouseXPerc >= 0.8f)
		{
			onRightPressed();
		}
		else
		{
			onLeftPressed();
		}
	}
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
	SDL_Texture* newTexture = createText(newBounds, _options[_selectedIndex], _size, _color);
	_worldBounds = newBounds;
	_texture = newTexture;
	onHovered(_isHovered);

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
	SDL_Texture* newTexture = createText(newBounds, _options[_selectedIndex], _size, _color);
	_worldBounds = newBounds;
	_texture = newTexture;
	onHovered(_isHovered);

	D_ASSERT(oldTexture, "Invalid texture to swap");
	SDL_DestroyTexture(oldTexture);
}

//
// Checkbox

CheckBox::CheckBox(bool startEnabled, const Vec2& position, uint16_t size, const SDL_Color& color, const bool justify)
{
	const char* text = startEnabled ? k_selectedCharacter : k_notSelectedCharacter;
	Vec2 textBounds;
	SDL_Texture* textTexture = createText(textBounds, text, size, color);

	this->_isSelected = startEnabled;
	this->_worldPosition = position;
	this->_worldBounds = textBounds;
	this->_texture = textTexture;
	this->_color = color;
	this->_size = size;
	this->_isHovered = false;
	this->_mainCollider = RectCollider(position, textBounds);

	if (justify)
	{
		_worldPosition.x -= _worldBounds.x / 2;
	}
}

void CheckBox::update()
{
	Text::update();

	if (wasMouseButtonPressedThisFrame(SDL_BUTTON_LEFT) && _isHovered)
	{
		onSelected();
	}
}

void CheckBox::onSelected()
{
	_isSelected = !_isSelected;
	const char* newCheckboxText = _isSelected ? k_selectedCharacter : k_notSelectedCharacter;

	SDL_Texture* oldTexture = _texture;
	Vec2 newBounds;
	SDL_Texture* newTexture = createText(newBounds, newCheckboxText, _size, _color);
	_worldBounds = newBounds;
	_texture = newTexture;
	onHovered(_isHovered);

	D_ASSERT(oldTexture, "Invalid texture to swap");
	SDL_DestroyTexture(oldTexture);
}

//
// Text

Text::Text(const char* text, const Vec2& position, uint16_t size, const SDL_Color& color, const bool justify)
{
	Vec2 textBounds;
	SDL_Texture* textTexture = createText(textBounds, text, size, color);

	this->_worldPosition = position;
	this->_worldBounds = textBounds;
	this->_texture = textTexture;
	this->_color = color;
	this->_size = size;
	this->_isHovered = false;
	this->_mainCollider = RectCollider(position, textBounds);

	if (justify)
	{
		_worldPosition.x -= _worldBounds.x / 2;
	}
}

void Text::update()
{
	_mainCollider.centerPoint = _worldPosition;
	_mainCollider.size = _worldBounds;

	IVec2 mousePos = getMousePosition();

	bool isHovering = pointInRect(mousePos, _mainCollider);
	if (isHovering != _isHovered)
	{
		onHovered(isHovering);
		_isHovered = isHovering;
	}
}

void Text::render(SDL_Texture* targetTexture, SDL_FRect& dest)
{
	SDL_SetRenderTarget(s_renderer, targetTexture);

	// Convert world position to screen position
	Vec2 screenPosition = worldToScreen(_worldPosition);
	Vec2 screenBounds = worldToScreen(_worldBounds);

	dest.x = screenPosition.x;
	dest.y = screenPosition.y;
	dest.w = screenBounds.x;
	dest.h = screenBounds.y;

	// Prevent artifacts/leftovers on this new texture. This will not clear the whole renderer, just the new target it points to (aka target texture)
	// This way we can have a "new" draw per frame like we have with game sprites
	SDL_RenderClear(s_renderer);

	SDL_RenderCopyExF(s_renderer, _texture, nullptr, &dest, 0, nullptr, SDL_FLIP_NONE);
	SDL_SetRenderTarget(s_renderer, nullptr);
	SDL_RenderCopy(s_renderer, targetTexture, nullptr, nullptr);

	// Draw debug collider
	if (s_debugCollidersEnabled)
	{
		SDL_SetRenderDrawColor(s_renderer, _mainCollider.debugColor.r, _mainCollider.debugColor.g, _mainCollider.debugColor.b, _mainCollider.debugColor.a);
		SDL_FRect debugRect{ _mainCollider.centerPoint.x, _mainCollider.centerPoint.y, _mainCollider.size.x, _mainCollider.size.y };
		SDL_RenderDrawRectF(s_renderer, &debugRect);
		SDL_SetRenderDrawColor(s_renderer, 0,0,0,1);
	}
}

void Text::onHovered(bool isHovered)
{
	SDL_Color targetColor = isHovered ? k_orange : k_white;
	if (!_texture || SDL_SetTextureColorMod(_texture, targetColor.r, targetColor.g, targetColor.b) < 0)
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

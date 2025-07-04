#pragma once
#include "lib.h"
#include <stdint.h>
#include <SDL_pixels.h>
#include <vector>
#include "spriteHandler.h"
#include <string>

struct SDL_Texture;
struct SDL_FRect;

inline const SDL_Color k_white{ 255, 255, 255, 255};
inline const SDL_Color k_gray{ 137, 128, 128, 255};
inline const SDL_Color k_orange{ 234, 140, 70, 255};
inline const SDL_Color k_red{ 255, 59, 93, 255 };

enum WidgetType
{
	TEXT,
	CHECKBOX,
	OPTIONSELECTOR,
	INPUT_WIDGET,
	INVALID
};

enum DrawMode
{
	LEFT,
	CENTER,
	RIGHT
};

class InteractableText
{
public:
	Vec2 _worldPosition{0,0};
	SDL_Texture* _texture = nullptr;
	Vec2 _worldBounds{0,0};
	uint16_t _size = 0;
	SDL_Color _color{0,0,0};
	RectCollider _mainCollider;
	bool _isHovered = false;
	DrawMode _drawMode = DrawMode::LEFT;
	WidgetType _widgetType = WidgetType::INVALID;

	void setupDrawMode(DrawMode drawMode);
	void tryHover();
	void update();
	void render(SDL_Texture* targetTexture);
	void onHovered(bool isHovered);

	InteractableText() = default;
	InteractableText(const char* text, const Vec2& worldPosition, uint16_t size, const SDL_Color& color, const DrawMode drawMode = LEFT);
};


class CheckBox : public InteractableText
{
public:
	int8_t _isSelected = false;
	void trySelect();
	void onSelected();
	CheckBox() = default;
	CheckBox(bool startEnabled, const Vec2& worldPosition, uint16_t size, const SDL_Color& color, const DrawMode drawMode = LEFT);
};

class OptionSelector : public InteractableText
{
public:
	std::vector<const char*> _options;
	int8_t _selectedIndex = 0;
	bool _allowWrap = true;

	void selectOption(int8_t index);
	void trySwapOption();
	void onRightPressed();
	void onLeftPressed();
	OptionSelector() = default;
	OptionSelector(const std::vector<const char*>& options, const Vec2& worldPosition, uint16_t size, const SDL_Color& color,
		const DrawMode drawMode = LEFT, bool allowWrap = true, int8_t selectedIndex = 0);
};

class InputWidget : public InteractableText
{
public:
	bool _isEditingText = false;
	std::string _currentText;

	void tryEditText();
	InputWidget() = default;
	InputWidget(const Vec2& worldPosition, uint16_t size, const SDL_Color& color, const DrawMode drawMode = LEFT);
};

class Button
{
public:
	Sprite _sprite;
	InteractableText _text;

	void render(SDL_Texture* targetTexture);
	bool tryPress();

	Button() = default;
	Button(const char* buttonText, const SpriteType buttonSprite, const Vec2& position, uint8_t buttonSize = 40);
};

class SpriteButton
{
public:
	SpriteButton() = default;
	SpriteButton(const Sprite&& normalSprite, const Sprite&& hoveredSprite);
	void update();
	void render(bool flipHorizontal = false);
	bool tryHover();
	bool tryPress();
	Sprite _normalSprite;
	Sprite _hoveredSprite;
	Sprite _sprite;
};

void destroyWidget(const InteractableText& widget);
void destroyWidget(InteractableText* widget);
SDL_Texture* createUITexture();
#pragma once
#include "lib.h"
#include <stdint.h>
#include <SDL_pixels.h>
#include <vector>

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
	INVALID
};

enum DrawMode
{
	LEFT,
	CENTER,
	RIGHT
};

class Text
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
	virtual void update();
	virtual void render(SDL_Texture* targetTexture, SDL_FRect& dest);
	virtual void onHovered(bool isHovered);

	Text() = default;
	Text(const char* text, const Vec2& worldPosition, uint16_t size, const SDL_Color& color, const DrawMode drawMode = LEFT);
};


class CheckBox : public Text
{
public:
	bool _isSelected = false;
	void trySelect();
	void onSelected();
	CheckBox() = default;
	CheckBox(bool startEnabled, const Vec2& worldPosition, uint16_t size, const SDL_Color& color, const DrawMode drawMode = LEFT);
};

class OptionSelector : public Text
{
public:
	std::vector<const char*> _options;
	int8_t _selectedIndex = 0;
	bool _allowWrap = true;

	void trySwapOption();
	void onRightPressed();
	void onLeftPressed();
	OptionSelector() = default;
	OptionSelector(const std::vector<const char*>& options, const Vec2& worldPosition, uint16_t size, const SDL_Color& color,
		const DrawMode drawMode = LEFT, bool allowWrap = true);
};


void destroyWidget(const Text& widget);
void destroyWidget(Text* widget);
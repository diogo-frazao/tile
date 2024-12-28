#pragma once
#include "lib.h"
#include <stdint.h>
#include <SDL_pixels.h>
#include <vector>

struct SDL_Texture;
struct SDL_FRect;

inline const SDL_Color k_white{ 255, 255, 255 };
inline const SDL_Color k_gray{ 137, 128, 128 };
inline const SDL_Color k_orange{ 234, 140, 70 };

enum DrawMode
{
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT,
};

class Text
{
public:
	Vec2 _position{0,0};
	SDL_Texture* _texture = nullptr;
	Vec2 _bounds{0,0};
	uint16_t _size = 0;
	SDL_Color _color{0,0,0};
	DrawMode _drawMode = TOP_CENTER;
	bool _isHovered = false;

	virtual void update();
	virtual void render(SDL_Texture* targetTexture, SDL_FRect& dest);
	virtual void onHovered();

	Text() = default;
	Text(const char* text, const Vec2& position, uint16_t size, const SDL_Color& color, const DrawMode drawMode = TOP_CENTER);
};


class CheckBox : public Text
{
public:
	bool _isSelected = false;
	void onSelected();
	CheckBox() = default;
	CheckBox(bool startEnabled, const Vec2& position, uint16_t size, const SDL_Color& color, const DrawMode drawMode = TOP_CENTER);
};

class OptionSelector : public Text
{
public:
	std::vector<const char*> _options;
	int8_t _selectedIndex = 0;
	bool _allowWrap = true;

	void onRightPressed();
	void onLeftPressed();
	OptionSelector() = default;
	OptionSelector(const std::vector<const char*>& options, const Vec2& position, uint16_t size, const SDL_Color& color,
		const DrawMode drawMode = TOP_CENTER, bool allowWrap = true);
};

void destroyWidget(const Text& widget);
void destroyWidget(Text* widget);
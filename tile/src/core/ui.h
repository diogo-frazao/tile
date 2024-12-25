#pragma once
#include "lib.h"
#include <stdint.h>
#include <SDL_pixels.h>
#include <vector>

struct SDL_Texture;
struct SDL_FRect;

inline SDL_Color s_white{ 255, 255, 255 };
inline SDL_Color s_gray{ 137, 128, 128 };
inline SDL_Color s_orange{ 234, 140, 70 };

enum DrawMode
{
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT,
};

class Text
{
public:
	Vec2 position{0,0};
	SDL_Texture* texture = nullptr;
	Vec2 bounds{0,0};
	uint16_t size = 0;
	SDL_Color color{0,0,0};
	DrawMode drawMode = TOP_CENTER;
	bool isHovered = false;

	void render(SDL_Texture* targetTexture, SDL_FRect& dest);
	void destroy();
	void onHovered();

	Text() = default;
	Text(const Vec2& pos, const Vec2& bounds, SDL_Texture* text, const SDL_Color& color, uint16_t size, DrawMode drawMode = TOP_CENTER) :
		position(pos), texture(text), bounds(bounds), color(color), size(size), drawMode(drawMode), isHovered(false) {};
};

class CheckBox
{
public:
	Text text;
	bool isSelected = false;

	void render(SDL_Texture* targetTexture, SDL_FRect& dest);
	void destroy();
	void onHovered();
	void onSelected();
	CheckBox() = default;
};

class OptionSelector
{
public:
	Text text;
	int8_t selectedIndex = 0;
	std::vector<const char*> options;
	bool allowWrap = true;

	void render(SDL_Texture* targetTexture, SDL_FRect& dest);
	void destroy();
	void onHovered();
	void onRightPressed();
	void onLeftPressed();
	OptionSelector() = default;
};

Text createText(const char* text, const Vec2& position, uint16_t size, const SDL_Color& color, const DrawMode drawMode = TOP_CENTER);
CheckBox createCheckbox(bool startEnabled, const Vec2& position, uint16_t size, const SDL_Color& color, const DrawMode drawMode = TOP_CENTER);
OptionSelector createOptionSelector(const std::vector<const char*>& options, const Vec2& position, uint16_t size, const SDL_Color& color, 
	const DrawMode drawMode = TOP_CENTER, bool allowWrap = true);
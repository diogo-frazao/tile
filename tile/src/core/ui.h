#pragma once
#include "lib.h"
#include <stdint.h>
#include <SDL_pixels.h>

struct SDL_Texture;
struct SDL_FRect;

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

Text createText(const char* text, const Vec2& position, uint16_t size, const SDL_Color& color, const DrawMode drawMode = TOP_CENTER);
CheckBox createCheckbox(bool startEnabled, const Vec2& position, uint16_t size, const SDL_Color& color, const DrawMode drawMode = TOP_CENTER);
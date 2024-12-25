#pragma once
#include "lib.h"
#include <stdint.h>

struct SDL_Texture;
struct SDL_Color;
struct SDL_FRect;

enum DrawMode
{
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT,
};

struct Text
{
	Vec2 position;
	SDL_Texture* texture;
	Vec2 bounds;
	DrawMode drawMode;
	bool isSelected;

	Text() = default;
	Text(const Vec2& pos, const Vec2& bounds, SDL_Texture* text, DrawMode drawMode = TOP_CENTER) :
		position(pos), texture(text), bounds(bounds), drawMode(drawMode) {};
};

Text createText(const char* text, const Vec2& position, uint8_t size, const SDL_Color& color, const DrawMode drawMode = TOP_CENTER);
void renderText(SDL_Texture* targetTexture, SDL_FRect& dest, const Text& text);
void destroyText(const Text& text);
// TODO change to support diferent UI types
void onUISelected(const Text& text);

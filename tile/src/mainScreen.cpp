#include "screens.h"
#include "core/spriteHandler.h"
#include <SDL.h>
#include "core/app.h"
#include "core/lib.h"

void MainScreen::start()
{
	SDL_ShowCursor(SDL_DISABLE);
	_mouseSprite = textures::getSprite(CURSOR);
}

void MainScreen::update()
{
	// TODO port to input system
	int32_t w = 0;
	SDL_GetWindowSize(s_window, &w, nullptr);
	uint8_t screenfactor = w / s_screenWidth;

	int32_t mx = 0;
	int32_t my = 0;
	SDL_GetMouseState(&mx, &my);

	int32_t mouseX = mx / screenfactor;
	int32_t mouseY = my / screenfactor;

	_mouseSprite.position.x = lerp(lastMouseX, mouseX, 0.6f);
	_mouseSprite.position.y = lerp(lastMouseY, mouseY, 0.6f);

	lastMouseX = mouseX;
	lastMouseY = mouseY;
}

void MainScreen::render()
{
	SDL_Rect src{};
	SDL_FRect dest{};

	src.x = _mouseSprite.offset.x;
	src.y = _mouseSprite.offset.y;
	src.w = _mouseSprite.size.x;
	src.h = _mouseSprite.size.y;

	dest.x = _mouseSprite.position.x;
	dest.y = _mouseSprite.position.y;
	dest.w = (float)_mouseSprite.size.x;
	dest.h = (float)_mouseSprite.size.y;

	SDL_RenderCopyExF(s_renderer, s_atlasTexture, &src, &dest, 0, nullptr, SDL_FLIP_NONE);
}
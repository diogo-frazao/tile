#include "screens.h"
#include <SDL.h>
#include "core/input.h"
#include "core/app.h"
#include "core/log.h"

void PanelScreen::render()
{
	if (s_isPanelActive)
	{
		SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 200);
		SDL_RenderFillRect(s_renderer, nullptr);
		SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 1);
	}
}

void MouseScreen::start()
{
	SDL_ShowCursor(0);
	_mouseSprite = textures::getSprite(CURSOR);
}

void MouseScreen::update()
{
	IVec2 mousePos = getMousePosition();
	_mouseSprite.position.x = lerp(_lastMousePosition.x, mousePos.x, 0.6f);
	_mouseSprite.position.y = lerp(_lastMousePosition.y, mousePos.y, 0.6f);

	_lastMousePosition.x = mousePos.x;
	_lastMousePosition.y = mousePos.y;
}

void MouseScreen::render()
{
	static SDL_Rect src{};
	static SDL_FRect dest{};

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


#include "screens.h"
#include <SDL.h>
#include "core/input.h"
#include "core/app.h"
#include "core/log.h"
#include "core/debugUtils.h"

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
	setMouseState(MouseSpriteState::NORMAL);
}

void MouseScreen::update()
{
	if (!s_useCustomMouse)
	{
		SDL_ShowCursor(1);
		return;
	}

	// Needed to override imgui and hide the mouse every frame
	SDL_ShowCursor(0);

	_mouseSprite.position.x = lerp(_lastMousePosition.x, s_mousePositionThisFrame.x, 0.6f);
	_mouseSprite.position.y = lerp(_lastMousePosition.y, s_mousePositionThisFrame.y, 0.6f);

	_lastMousePosition.x = s_mousePositionThisFrame.x;
	_lastMousePosition.y = s_mousePositionThisFrame.y;
}

void MouseScreen::render()
{
	if (!s_useCustomMouse)
	{
		return;
	}
	
	debugDrawPoint(s_mousePositionThisFrame);
	renderSprite(_mouseSprite);
}

void MouseScreen::setMouseState(MouseSpriteState state)
{
	if (_mouseSpriteState == state)
	{
		return;
	}

	_mouseSpriteState = state;
	Vec2 previousPos = _mouseSprite.position;
	switch (state)
	{
		case MouseSpriteState::NORMAL:
			_mouseSprite = textures::getSprite(CURSOR);
			break;
		case MouseSpriteState::DRAGGING:
			_mouseSprite = textures::getSprite(CURSOR_DRAGGING);
			break;
		case MouseSpriteState::INVALID:
			D_ASSERT(false, "Invalid mouse state");
			break;
	}

	_mouseSprite.position = previousPos;
}


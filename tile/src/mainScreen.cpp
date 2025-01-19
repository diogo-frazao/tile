#include "screens.h"
#include "core/spriteHandler.h"
#include <SDL.h>
#include "core/app.h"
#include "core/lib.h"
#include "core/input.h"
#include "core/log.h"
#include "core/ui.h"

void MainScreen::start()
{
	_uiTexture = createUITexture();
	_addSpriteButton = Button("+", BUTTON, Vec2(0, 5), 62);
	_backgroundButton = Button("0", BUTTON, Vec2(0, _addSpriteButton._sprite.position.y + _addSpriteButton._sprite.size.y + 1.f));
	_middlegroundButton = Button("1", BUTTON, Vec2(0, _backgroundButton._sprite.position.y + _backgroundButton._sprite.size.y + 1.f));
	_foregroundButton = Button("2", BUTTON, Vec2(0, _middlegroundButton._sprite.position.y + _middlegroundButton._sprite.size.y + 1.f));
}

void MainScreen::update()
{
	if (wasKeyPressedThisFrame(SDL_SCANCODE_ESCAPE) && !AddSpritesScreen::s_active)
	{
		SettingsScreen::s_active = !SettingsScreen::s_active;
		s_active = !SettingsScreen::s_active;
		PanelScreen::s_isPanelActive = SettingsScreen::s_active;
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_P) && !SettingsScreen::s_active)
	{
		AddSpritesScreen::s_active = !AddSpritesScreen::s_active;
		s_active = !AddSpritesScreen::s_active;
		PanelScreen::s_isPanelActive = AddSpritesScreen::s_active;
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_Q))
	{
		s_debugCollidersEnabled = !s_debugCollidersEnabled;
	}

	if (s_active)
	{
		_addSpriteButton._text.tryHover();
		_backgroundButton._text.tryHover();
		_middlegroundButton._text.tryHover();
		_foregroundButton._text.tryHover();
	}
}

void MainScreen::render()
{
	static SDL_Rect src{};
	static SDL_FRect dest{};

	// TODO remove later
	static Sprite mockup = textures::getSprite(MOCKUP);
	mockup.position = { 0,0 };
	renderSprite(mockup, src, dest);

	_addSpriteButton.render(_uiTexture, src, dest);
	_backgroundButton.render(_uiTexture, src, dest);
	_middlegroundButton.render(_uiTexture, src, dest);
	_foregroundButton.render(_uiTexture, src, dest);
}

void MainScreen::destroy()
{
	SDL_DestroyTexture(_uiTexture);
}

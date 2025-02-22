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
	s_active = true;
	_uiTexture = createUITexture();
	_addSpriteButton = Button("+", BUTTON, Vec2(0, 5), 62);
	_backgroundButton = Button("0", BUTTON, Vec2(0, _addSpriteButton._sprite.position.y + _addSpriteButton._sprite.size.y + 1.f));
	_middlegroundButton = Button("1", BUTTON, Vec2(0, _backgroundButton._sprite.position.y + _backgroundButton._sprite.size.y + 1.f));
	_foregroundButton = Button("2", BUTTON, Vec2(0, _middlegroundButton._sprite.position.y + _middlegroundButton._sprite.size.y + 1.f));

	_backgroundSpritePreviewer.start();
	_middlegroundSpritePreviewer.start();
	_foregroundSpritePreviewer.start();
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

	if (wasKeyPressedThisFrame(SDL_SCANCODE_X))
	{
		static Sprite testSprite = textures::getSprite(PREVIEWER_BG);
		_backgroundSpritePreviewer._spritesToPreview.push_back(testSprite);
	}

	if (s_active)
	{
		_addSpriteButton._text.tryHover();
		_backgroundButton._text.tryHover();
		_middlegroundButton._text.tryHover();
		_foregroundButton._text.tryHover();

		if (_backgroundButton.tryPress())
		{
			_backgroundSpritePreviewer.s_isActive = !_backgroundSpritePreviewer.s_isActive;
		}
	}
}

void MainScreen::render()
{
	// TODO remove later
	static Sprite mockup = textures::getSprite(MOCKUP);
	mockup.position = { 0,0 };
	renderSprite(mockup);

	_addSpriteButton.render(_uiTexture);
	_backgroundButton.render(_uiTexture);
	_middlegroundButton.render(_uiTexture);
	_foregroundButton.render(_uiTexture);

	_backgroundSpritePreviewer.render();
	_middlegroundSpritePreviewer.render();
	_foregroundSpritePreviewer.render();
}

void MainScreen::destroy()
{
	SDL_DestroyTexture(_uiTexture);
}

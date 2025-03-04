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

	_spritePreviewers[0] = SpritePreviewer(_backgroundButton._sprite.position);
	_spritePreviewers[1] = SpritePreviewer(_middlegroundButton._sprite.position);
	_spritePreviewers[2] = SpritePreviewer(_foregroundButton._sprite.position);
}

void MainScreen::update()
{
	if (wasKeyPressedThisFrame(SDL_SCANCODE_ESCAPE))
	{
		SettingsScreen::s_active = false;
		AddSpritesScreen::s_active = false;
		PanelScreen::s_isPanelActive = false;
		s_active = true;
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_TAB) && !AddSpritesScreen::s_active)
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

	// debug add sprites previewer
	if (wasKeyPressedThisFrame(SDL_SCANCODE_1))
	{
		Sprite sign{{10,16}, {328, 9}};
		_spritePreviewers[0]._spritesToPreview.push_back(sign);
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_2))
	{
		Sprite rock{{8,5}, {347, 0}};
		_spritePreviewers[0]._spritesToPreview.push_back(rock);
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_3))
	{
		Sprite bush{{24,13}, {338, 9}};
		_spritePreviewers[0]._spritesToPreview.push_back(bush);
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_4))
	{
		Sprite tree{{35,48}, {328, 25}};
		_spritePreviewers[0]._spritesToPreview.push_back(tree);
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_5))
	{
		Sprite miniRock{{7,3}, {355, 0}};
		_spritePreviewers[0]._spritesToPreview.push_back(miniRock);
	}

	if (s_active)
	{
		_addSpriteButton._text.tryHover();
		
		if (!_spritePreviewers[0].s_isActive)
		{
			_backgroundButton._text.tryHover();
		}

		if (!_spritePreviewers[1].s_isActive)
		{
			_middlegroundButton._text.tryHover();
		}

		if (!_spritePreviewers[2].s_isActive)
		{
			_foregroundButton._text.tryHover();
		}

		if (_addSpriteButton.tryPress())
		{
			AddSpritesScreen::s_active = true;
			PanelScreen::s_isPanelActive = true;
			s_active = false;
		}

		if (_backgroundButton.tryPress())
		{
			_spritePreviewers[0].s_isActive = !_spritePreviewers[0].s_isActive;
			_backgroundButton._text.onHovered(true);
		}

		if (_middlegroundButton.tryPress())
		{
			_spritePreviewers[1].s_isActive = !_spritePreviewers[1].s_isActive;
			_middlegroundButton._text.onHovered(true);
		}

		if (_foregroundButton.tryPress())
		{
			_spritePreviewers[2].s_isActive = !_spritePreviewers[2].s_isActive;
			_foregroundButton._text.onHovered(true);
		}
	}
}

void MainScreen::render()
{
	// TODO remove later
	static Sprite mockup = textures::getSprite(MOCKUP);
	mockup.position = { 0,0 };
	renderSprite(mockup);

	_spritePreviewers[0].render();
	_spritePreviewers[1].render();
	_spritePreviewers[2].render();

	_addSpriteButton.render(_uiTexture);
	_backgroundButton.render(_uiTexture);
	_middlegroundButton.render(_uiTexture);
	_foregroundButton.render(_uiTexture);
}

void MainScreen::destroy()
{
	SDL_DestroyTexture(_uiTexture);
}

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

	
	// TODO: remove debug add sprites previewer
	static std::array<Sprite, 5> debugSprites = {
		Sprite({10,16}, {328, 9}), // sign
		Sprite({8,5}, {347, 0}), // rock
		Sprite({24,13}, {338, 9}), // bush
		Sprite({35,48}, {328, 25}), // tree
		Sprite({7,3}, {355, 0}) // mini rock
	};

	if (wasKeyPressedThisFrame(SDL_SCANCODE_1))
	{
		int randomSpriteIndex = rand() % 5;
		Sprite spriteToAdd = debugSprites[randomSpriteIndex];
		_spritePreviewers[0]._spritesToPreview.push_back(spriteToAdd);
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_2))
	{
		int randomSpriteIndex = rand() % 5;
		Sprite spriteToAdd = debugSprites[randomSpriteIndex];
		_spritePreviewers[1]._spritesToPreview.push_back(spriteToAdd);
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_3))
	{
		int randomSpriteIndex = rand() % 5;
		Sprite spriteToAdd = debugSprites[randomSpriteIndex];
		_spritePreviewers[2]._spritesToPreview.push_back(spriteToAdd);
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
			toggleSpritePreviewerAtIndex(0);
			_backgroundButton._text.onHovered(true);
		}

		if (_middlegroundButton.tryPress())
		{
			toggleSpritePreviewerAtIndex(1);
			_middlegroundButton._text.onHovered(true);
		}

		if (_foregroundButton.tryPress())
		{
			toggleSpritePreviewerAtIndex(2);
			_foregroundButton._text.onHovered(true);
		}

		if (_spriteInHand.isValid())
		{
			_spriteInHand.position = getMousePosition();
		}
	}
}

void MainScreen::toggleSpritePreviewerAtIndex(uint8_t spritePreviewerIndex)
{
	for (uint8_t i = 0; i < _spritePreviewers.size(); ++i)
	{
		if (i == spritePreviewerIndex)
		{
			_spritePreviewers[i].s_isActive = !_spritePreviewers[i].s_isActive;
		}
		else
		{
			_spritePreviewers[i].s_isActive = false;
		}
	}
}

void MainScreen::render()
{
	// TODO remove later
	static Sprite mockup = textures::getSprite(MOCKUP);
	mockup.position = { 0,0 };
	renderSprite(mockup);

	if (_spriteInHand.isValid())
	{
		renderSprite(_spriteInHand);
	}

	for (SpritePreviewer& previewer : _spritePreviewers)
	{
		previewer.render();
	}
	
	_addSpriteButton.render(_uiTexture);
	_backgroundButton.render(_uiTexture);
	_middlegroundButton.render(_uiTexture);
	_foregroundButton.render(_uiTexture);
}

void MainScreen::destroy()
{
	SDL_DestroyTexture(_uiTexture);
}

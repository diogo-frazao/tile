#include "screens.h"
#include "core/spriteHandler.h"
#include <SDL.h>
#include "core/app.h"
#include "core/lib.h"
#include "core/input.h"
#include "core/log.h"
#include "core/ui.h"
#include <cmath>

void MainScreen::start()
{
	s_active = true;
	_uiTexture = createUITexture();
	_addSpriteButton = Button("+", BUTTON, Vec2(0, 5), 62);

	Button backgroundButton {"0", BUTTON, Vec2(0, _addSpriteButton._sprite.position.y + _addSpriteButton._sprite.size.y + 1.f)};
	Button middlegroundButton {"1", BUTTON, Vec2(0, backgroundButton._sprite.position.y + backgroundButton._sprite.size.y + 1.f)};
	Button foregroundButton {"2", BUTTON, Vec2(0, middlegroundButton._sprite.position.y + middlegroundButton._sprite.size.y + 1.f)};

	SpritePreviewer backgroundLayerSpritePreviewer {backgroundButton._sprite.position};
	SpritePreviewer middleLayerSpritePreviewer {middlegroundButton._sprite.position};
	SpritePreviewer foregroundLayerSpritePreviewer {foregroundButton._sprite.position};

	_spritePreviewerButtons = {
		std::pair{std::move(backgroundLayerSpritePreviewer), std::move(backgroundButton)},
		std::pair{std::move(middleLayerSpritePreviewer), std::move(middlegroundButton)},
		std::pair{std::move(foregroundLayerSpritePreviewer), std::move(foregroundButton)}
	};
}

void closeOpenScreenAndEnableMainScreen()
{
	SettingsScreen::s_active = false;
	AddSpritesScreen::s_active = false;
	PanelScreen::s_isPanelActive = false;
	MainScreen::s_active = true;
}

void toggleSettingsScreen()
{
	SettingsScreen::s_active = !SettingsScreen::s_active;
	MainScreen::s_active = !SettingsScreen::s_active;
	PanelScreen::s_isPanelActive = SettingsScreen::s_active;
}

void toggleAddSpritesScreen()
{
	AddSpritesScreen::s_active = !AddSpritesScreen::s_active;
	MainScreen::s_active = !AddSpritesScreen::s_active;
	PanelScreen::s_isPanelActive = AddSpritesScreen::s_active;
}

void MainScreen::handleAddSpritesToLayersDebug()
{
#if DEBUG_ENABLED == 1
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
		_spritePreviewerButtons[BACKGROUND].first._spritesToPreview.emplace_back(spriteToAdd);
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_2))
	{
		int randomSpriteIndex = rand() % 5;
		Sprite spriteToAdd = debugSprites[randomSpriteIndex];
		_spritePreviewerButtons[MIDDLEGROUND].first._spritesToPreview.emplace_back(spriteToAdd);
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_3))
	{
		int randomSpriteIndex = rand() % 5;
		Sprite spriteToAdd = debugSprites[randomSpriteIndex];
		_spritePreviewerButtons[FOREGROUND].first._spritesToPreview.emplace_back(spriteToAdd);
	}
#endif
}

void MainScreen::update()
{
	if (wasKeyPressedThisFrame(SDL_SCANCODE_ESCAPE))
	{
		closeOpenScreenAndEnableMainScreen();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_TAB) && !AddSpritesScreen::s_active)
	{
		toggleSettingsScreen();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_P) && !SettingsScreen::s_active)
	{
		toggleAddSpritesScreen();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_Q))
	{
		s_debugCollidersEnabled = !s_debugCollidersEnabled;
	}

	if (!s_active)
	{
		return;
	}

	handleAddSpritesToLayersDebug();

	for (SpritePreviewerButtonPair& pair : _spritePreviewerButtons)
	{
		// Only allow hovering text if the spritePreviewer isn't expanded/visible
		// When exapended, it's always hovered
		if (!pair.first._isVisible)
		{
			pair.second._text.tryHover();
		}

		if (pair.second.tryPress())
		{
			toggleSpritePreviewerAndDisableOthers(pair.first);
			pair.second._text.onHovered(true);
		}
	}

	_addSpriteButton._text.tryHover();
	if (_addSpriteButton.tryPress())
	{
		toggleAddSpritesScreen();
	}

	if (_spriteInHand.isValid())
	{
		MouseScreen::instance().setMouseState(MouseScreen::MouseSpriteState::DRAGGING);
		_spriteInHand.position = s_mousePositionThisFrame;

		if (shouldReleaseSpriteInHand())
		{
			// TODO add sprite to corresponding layer
			_spriteInHand.invalidate();
			MouseScreen::instance().setMouseState(MouseScreen::MouseSpriteState::NORMAL);
		}
	}
}

bool MainScreen::shouldReleaseSpriteInHand()
{
	bool wantsToChangeOrCollapseLayer = false;
	for (const SpritePreviewerButtonPair& pair : _spritePreviewerButtons)
	{
		if (pointInRect(s_mousePositionThisFrame, pair.second._text._mainCollider))
		{
			wantsToChangeOrCollapseLayer = true;
		}
	}

	return wasMouseButtonPressedThisFrame(SDL_BUTTON_LEFT) && !wantsToChangeOrCollapseLayer;
}

void MainScreen::toggleSpritePreviewerAndDisableOthers(SpritePreviewer& spritePreviewer)
{
	for (uint8_t i = 0; i < _spritePreviewerButtons.size(); ++i)
	{
		if (_spritePreviewerButtons[i].first == spritePreviewer)
		{
			_spritePreviewerButtons[i].first._isVisible = !_spritePreviewerButtons[i].first._isVisible;
		}
		else
		{
			_spritePreviewerButtons[i].first._isVisible = false;
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

	if (_spriteInHand.isValid())
	{
		renderSprite(_spriteInHand);
	}

	for (SpritePreviewerButtonPair& pair : _spritePreviewerButtons)
	{
		pair.first.render();
		pair.second.render(_uiTexture);
	}
}

void MainScreen::destroy()
{
	SDL_DestroyTexture(_uiTexture);
}
